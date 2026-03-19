#include <Arduboy2.h>
#include <EEPROM.h>
#include "TinyerFont/Tinyerfont.h"
#include <Sprites.h>

#include "state.h"
#include "draw.h"
#include "images.h"

//#define DRAW_GRID 1
//#define DRAW_CPU 1
//#define DRAW_DEBUG 1

const uint8_t MAX_RINGS = 50;
const uint8_t RING_RADIUS = 100;
const float NEAR_CLIP_PLANE = 1.2f;
const uint8_t MAX_LAPS = 10;
const uint8_t FRAME_RATE = 40;
const uint8_t START_BOOST = 50;
const uint8_t  DEFAULT_LAPS = 6;
const uint8_t NUM_RINGS = 50;
const uint8_t LEVEL_NUM = 20;

const uint16_t EEPROM_OFFSET = 564;


#define MS_TO_FRAMES(ms) ((ms)/(1000/FRAME_RATE))
#define FRAMES_TO_MS(frame) ((frame)*(1000/FRAME_RATE))


Arduboy2 arduboy;
Tinyerfont tinyfont = Tinyerfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());


bool save_laps;
float position = 0;
float speed=0;
float rot=0;
float boost=START_BOOST;
uint16_t crash_sequence = 0;
int8_t x_shake;

uint8_t difficulty = 1;
uint8_t level = 0;
uint8_t num_laps = DEFAULT_LAPS;
uint8_t lap = 1;
long lap_start_millis = 0;
long race_start_millis = 0;
long lap_times[MAX_LAPS];

float engine_force = 0;
float mass = 500;
float drag = 100;
float friction = .1;
float braking_friction = 0;

float turn_speed = 0;
float turn_friction = .15;
float turn_static_friction = .005;

// level seeds
int levels[] = {
  6,4,7,2,9,1,8, 10,11,12,13,14,15,16,17,18,19,20,21,22
};


enum State {
  MENU,
  START_RACE,
  RACE,
  FINISH,
  FINISH_MENU
};


StateController<State, FRAMES_TO_MS(1)> state;


/**
 * @brief Save laps to eeprom
 *
 * Packs a time for each level into 12bits (* 20 levels) -> 30 bytes + 1 byte xor'ed hash
 * The default save option is determined based on if the hash matches or not.
 */
class SaveData {
  public:
    static const uint8_t BYTES = (LEVEL_NUM * 12+4)/8;
    struct {
      byte best_laps[BYTES];
      uint8_t hash;
    } data;

  void load() {
    EEPROM.get(EEPROM_OFFSET, data);
  }

  void clear() {
    for (uint8_t i=0; i<BYTES; i++) {
      data.best_laps[i] = 0xff;
    }
    data.hash = get_hash();
  }

  uint8_t get_hash() {
    uint8_t check = 0x7A;
    for (uint8_t i=0; i<BYTES; i++) {
      check ^= data.best_laps[i];
    }
    return check;
  }

  boolean valid() {
    return get_hash() == data.hash;
  }

  long get_level(uint8_t id) {
    uint8_t index = id * 12 /8;
    uint16_t result = (((uint16_t)data.best_laps[index])<<8) | data.best_laps[index+1];
    if ((id % 2) == 0) {
      result >>= 4;
    } else {
      result = result & 0x0FFF;
    }

    return result*10;
  }

  void set_level(uint8_t id, long value) {
    uint16_t write = min(value/10, (long)0xffff);
    uint8_t index = id*12/8;
    if ((id & 0x01) == 0) {
      data.best_laps[index] = (uint8_t)(write>>4);
      data.best_laps[index+1] = (((uint8_t)(write)<<4) & 0xF0) | (data.best_laps[index+1] & 0x0F);
    } else {
      data.best_laps[index] = (data.best_laps[index] & 0xF0) | (((uint8_t)(write>>8)) & 0x0F);
      data.best_laps[index+1] = (uint8_t)(write);
    }
    data.hash = get_hash();

  }

  void save() {
    EEPROM.put(EEPROM_OFFSET, data);
  }

};


SaveData save_data;


enum RingType {
  START_LINE=0,
  OPEN=1,
  ONE_THIRD=2,
  MIDDLE=3,
  BOOSTER=4
};
/**
 * @brief One ring of the level.
 *
 * The level has MAX_RINGS rings each being one of 5 types (Start line, empty, bar, half circle, boost).
 * The rotation specifies the rotation of each obstacle (360 degrees being a value of 256)
  */
struct Ring {
  uint8_t rotation;
  RingType type;
} ;
Ring rings[MAX_RINGS];


struct Pointf
{
  float x;
  float y;
  Pointf() = default;
  constexpr Pointf(float x, float y)
    : x(x), y(y)
  {
  }
};


float adj_dist(float x, float center, float dist) {
  return (x-center) * (dist*dist)/80 + center;
}

/**
 * @brief get x,y units for rotating 0-255
 */
Pointf get_rot(uint8_t rotate, uint8_t r) {
  if (r < 3) {
    uint8_t rot =  -rotate - 16;
    if (rot < 32)
      return Pointf(.707f, .707f);
    if (rot < 64)
      return Pointf(0, 1);
    if (rot < 96)
      return Pointf(-.707f, .707f);
    if (rot < 128)
      return Pointf(-1, 0);
    if (rot < 160)
      return Pointf(-.707f, -.707f);
    if (rot < 192)
      return Pointf(-1, 0);
    if (rot < 224)
      return Pointf(.707f, -.707f);
    return Pointf(1,0);
  } else {
    float adjr = -rotate*M_PI*2/256;
    return Pointf(cos(adjr), sin(adjr));
  }
}


Pointf get_circ_pt(Pointf center, float r, Pointf rotate) {
  return Pointf(center.x+r*rotate.x, center.y+r*rotate.y);
}


float y_adj_center(float dist_to_cam) {
  return max(-128.0f, -80.0f/(dist_to_cam));
}


float size_adj(float dist_to_cam) {
  return min(128.0f, 2.0f*64.0f/(dist_to_cam));
}


/**
 * @brief Helper to blink something based on frame
 */
bool blink(uint16_t on_frames, uint16_t total_frames, uint16_t offset_frames=0) {
  return ((arduboy.frameCount - offset_frames) % total_frames) < on_frames;
}


/**
 * @brief draw a booster arrow
 */
void draw_arrow(float x, float d2cam1, Pointf rot1, Pointf rot2, Pointf rot3) {
  float pt_dist = 1.0f;
  Pointf xy = Pointf(x, 32+y_adj_center(d2cam1));
  Pointf xy2 = Pointf(x, 32+y_adj_center(d2cam1+pt_dist));
  float r1 = size_adj(d2cam1);
  float r2 = size_adj(d2cam1 + pt_dist);

  Pointf p1 = get_circ_pt(xy, r1, rot1);
  Pointf p2 = get_circ_pt(xy2, r2, rot2);
  Pointf p3 = get_circ_pt(xy, r1, rot3);
  arduboy.drawLine(p1.x,p1.y,p2.x,p2.y, WHITE);
  arduboy.drawLine(p3.x,p3.y,p2.x,p2.y, WHITE);
}


/**
 * @brief Draw a level ring
 */
void draw_ring(float dist_to_cam, uint8_t offset) {
  float y = 32+y_adj_center(dist_to_cam);
  float r = size_adj(dist_to_cam);
  float x = 64+x_shake;
  uint8_t dither = uint8_t(dist_to_cam);

  uint8_t angle = (uint8_t)(rot + rings[offset].rotation);
  switch(rings[offset].type) {
    case 0: // Start
      drawArc(x,y,r, WHITE, 0, 0);
      drawArc(x,y,r+2, WHITE, 0, 0, dither);
      drawArc(x,y,r+4, WHITE, 0, 0, dither);
      break;
    case 1: // open
      drawArc(x, y, r, WHITE, 0, 0, dither);
      break;
    case 2: // 1/3
      {
        drawArc(x,y,r, WHITE, 96+angle, angle, dither);
        Pointf rot1 = get_rot(angle, r);
        Pointf rot2 = get_rot(96+angle, r);
        Pointf start = get_circ_pt(Pointf(x,y), r, rot1);
        Pointf stop = get_circ_pt(Pointf(x,y), r, rot2);
        arduboy.drawLine(start.x, start.y, stop.x, stop.y, WHITE);
        drawArc(x,y,r, WHITE, angle, 96+angle);
      }
      break;
    case 3: // middle
      {
        uint8_t w=7;
        drawArc(x,y,r, WHITE, w+angle, 128-w+angle, dither);
        drawArc(x,y,r, WHITE, 128-w+angle, 128+w+angle);
        drawArc(x,y,r, WHITE, 128+w+angle, -w+angle, dither);
        drawArc(x,y,r, WHITE, -w+angle, w+angle);
        Pointf p1 = get_circ_pt(Pointf(x,y), r, get_rot(w+angle, r));
        Pointf p2 = get_circ_pt(Pointf(x,y), r, get_rot(128-w+angle, r));
        arduboy.drawLine(p1.x,p1.y,p2.x,p2.y, WHITE);

        p1 = get_circ_pt(Pointf(x,y), r, get_rot(128+w+angle, r));
        p2 = get_circ_pt(Pointf(x,y), r, get_rot(-w+angle, r));
        arduboy.drawLine(p1.x,p1.y,p2.x,p2.y, WHITE);
      }
      break;
    case 4: // boost
      {
        drawArc(x,y,r, WHITE, 0,0, dither);
        const uint8_t w = 10;
        Pointf rot1 = get_rot(w+angle, r);
        Pointf rot2 = get_rot(angle, r);
        Pointf rot3 = get_rot(-w+angle, r);
        const uint16_t total_ms=800;
        const uint16_t st = total_ms>>3;
        if (blink(MS_TO_FRAMES(3*st), MS_TO_FRAMES(total_ms), MS_TO_FRAMES(0*st))) {
          draw_arrow(x, dist_to_cam, rot1, rot2, rot3);
        }
        if (dist_to_cam < 12) {
          if (blink(MS_TO_FRAMES(3*st), MS_TO_FRAMES(total_ms), MS_TO_FRAMES(1*st))) {
            draw_arrow(x, dist_to_cam+1, rot1, rot2, rot3);
          }
          if (blink(MS_TO_FRAMES(3*st), MS_TO_FRAMES(total_ms), MS_TO_FRAMES(2*st))) {
            draw_arrow(x, dist_to_cam+2, rot1, rot2, rot3);
          }
          if (blink(MS_TO_FRAMES(3*st), MS_TO_FRAMES(total_ms), MS_TO_FRAMES(3*st))) {
            draw_arrow(x, dist_to_cam+3, rot1, rot2, rot3);
          }
        }
      }

      break;
  };
}


/**
 * @brief You have crashed. Darn.
 */
void crash() {
  crash_sequence = max(min((float)(40*(speed*20)),(float)(FRAME_RATE*3/2)), 10.0f);
  speed = 0;
}


/**
 * @brief Checks for collisions the first frame after passing an offset.
 */
void check_collision(uint8_t offset) {
  uint8_t angle = (uint8_t)(192u - (uint8_t)rot - rings[offset].rotation);
  uint8_t ship_size=5;
  switch(rings[offset].type) {
    case RingType::START_LINE:
      {
        lap_times[lap-1] = millis() - lap_start_millis;
        lap_start_millis = millis();
        lap++;
      }
      break;
    case RingType::OPEN:
      break;
    case RingType::ONE_THIRD: // (0,96)
      if (angle < 96+ship_size || angle > 256-ship_size) {
        crash();
      }
      break;
    case RingType::MIDDLE: // (+-7)
      if (angle < ship_size+7 || angle > 256-ship_size-7) {
        crash();
      }
      if (angle < 128+ship_size+7 && angle > 128-ship_size-7) {
        crash();
      }
      break;
    case RingType::BOOSTER: // (+-10)
      if (angle < ship_size+10 || angle > 256-ship_size-10) {
        boost = min(200.0f, boost+20.0f);
      }
      break;
  }
}


/**
 * @brief Draw a generic in-between ring.
 */
void draw_generic_ring(float dist_to_cam) {
  drawArc(64+x_shake,32+y_adj_center(dist_to_cam),size_adj(dist_to_cam), WHITE, 0, 0, uint8_t(dist_to_cam*3/5));
}


void draw_level() {
  uint8_t drawn_rings=36;
  uint8_t ring_mult = 8;
  float draw_pos = position-0.1;

  uint16_t current_position = (uint16_t)(draw_pos*ring_mult);
  float distance_to_cam = ((int)(draw_pos*ring_mult+1) - draw_pos*ring_mult);

  for(uint8_t ring=0; ring<drawn_rings; ring++) {
    if(distance_to_cam > NEAR_CLIP_PLANE) {
      if(current_position % ring_mult == 0) {
        draw_ring(distance_to_cam, current_position/ring_mult);
      } else {
        draw_generic_ring(distance_to_cam);
      }
    }

    distance_to_cam += 1;
    current_position += 1;
    if (current_position >= NUM_RINGS*ring_mult) {
      current_position -= NUM_RINGS*ring_mult;
    }
  }
}


void gen_ring(uint8_t num) {
  rings[num].type = (RingType)(random(4)+1);
  rings[num].rotation = random(16)*16;
}


/**
 * @brief Generate the current level
 */
void load_level() {
  randomSeed(levels[level]);
  for(uint8_t i=1; i<NUM_RINGS; i++) {
    gen_ring(i);
  }

  for(uint8_t i=1; i<NUM_RINGS; i++) {
    if (difficulty==0 && i%2==0) {
      rings[i].type = RingType::OPEN;
    }
    if (difficulty<2) {
      if (random(10) < 4) {
        rings[i].type = RingType::OPEN;
      }
    }

  }
  rings[0].type = RingType::START_LINE;
}


void draw_cpu() {
  tinyfont.setCursor(2,30);
  static int cpu = 0;
  if (arduboy.everyXFrames(20)) {
    cpu = arduboy.cpuLoad();
  } else {
    cpu = max(cpu, arduboy.cpuLoad());
  }

  tinyfont.print(cpu);
  tinyfont.print('%');
}


void draw_debug() {
  tinyfont.setCursor(90,1);
  tinyfont.print(position, 2);
  tinyfont.setCursor(90,6);
  tinyfont.print(speed*100, 2);
  tinyfont.setCursor(90,12);
  tinyfont.print(boost);
}


void physics() {

  if (crash_sequence) {
    crash_sequence--;
    x_shake = (random(25)) * crash_sequence / 100;
    if (crash_sequence & 0x1) {
      x_shake = -x_shake;
    }
    if (!crash_sequence) {
      x_shake =0;
      position += .2;
    }
    return;
  }

  uint8_t max_engine_force = (difficulty==2)?9:7;
  if(arduboy.buttonsState() & (UP_BUTTON | A_BUTTON | B_BUTTON)) {
    engine_force+=.25;
    if (engine_force > max_engine_force)
      engine_force = max_engine_force;
  } else {
    engine_force = 0;
  }
  if(arduboy.pressed(B_BUTTON)) {
    if (boost && arduboy.pressed(B_BUTTON)) {
      engine_force *= 3;
      if (arduboy.everyXFrames(2) || difficulty != 2) {
        boost --;
      }
    }
  }

  if(arduboy.pressed(DOWN_BUTTON)) {
    braking_friction += 2;
    if (braking_friction > 30) {
      braking_friction=30;
    }
  } else {
    braking_friction -= 5;
    if (braking_friction < 0) {
      braking_friction=0;
    }
  }

  if(arduboy.pressed(RIGHT_BUTTON)) {
    turn_speed -= 1 + speed * 30;
  }
  if(arduboy.pressed(LEFT_BUTTON)) {
    turn_speed += 1 + speed * 30;
  }

  rot += turn_speed*.1;
  if (rot < 0) {
    rot += 256;
  }
  if (rot >= 256) {
    rot -= 256;
  }
  if (turn_speed > 0) {
    turn_speed = turn_speed - turn_friction * turn_speed;
  } else {
    turn_speed = turn_speed - turn_friction * turn_speed;
  }

  float force = engine_force - (drag*speed) - friction - braking_friction;
  float accel = force/mass;
  speed += accel / FRAME_RATE;


  if(speed <0) {
    speed = 0;
  }

  float last_position = position;
  position += speed;
  if (position > NUM_RINGS) {
    position -= NUM_RINGS;
  }

  if((uint8_t)last_position != (uint8_t)position) {
    check_collision((uint8_t)position);
  }
}


void draw_guages(uint8_t value1, uint8_t value2) {

  const uint8_t x=16, y=62, xmax=126, num=20, h1=2, h2=10;
  const uint8_t w=(xmax-x-64)/num;
  for (uint8_t i=0; i<num; i++) {
    uint8_t h = h1+i*(h2-h1)/num;
    arduboy.fillRect(64-x-(i*w), y-h, w-1, h, (value1>i*100/num)?WHITE:BLACK);
    arduboy.fillRect(64+x+(i*w), y-h, w-1, h, (value2>i*100/num)?WHITE:BLACK);
  }
  if (value1 == 100) {
    arduboy.fillRect(64-x-(num*w)-3, y-h2, w-1+3, h2, WHITE);
  }
  if (value2 == 100) {
    arduboy.fillRect(64+x+(num*w), y-h2, w-1+3, h2, WHITE);
  }
}


void print_time(long elapsed) {
  uint8_t minutes = min((long)(elapsed/60000), (long)99);
  long ms_elapsed=elapsed%60000;
  uint8_t sec = ms_elapsed/1000;
  uint8_t ms = (ms_elapsed%1000) / 10;
  char buff[11];
  sprintf(buff, "%3d:%02d.%02d", minutes, sec, ms);
  tinyfont.print(buff);
}


void draw_lap() {
  tinyfont.setCursor(2,2);
  tinyfont.print(lap);
  tinyfont.print('/');
  tinyfont.print(num_laps);
  long elapsed = millis() - race_start_millis;

  tinyfont.setCursor(52, 2);
  print_time(elapsed);

  arduboy.drawLine(126,10,126,40);
  arduboy.drawRect(125,10+position*30/NUM_RINGS, 3, 3);
}


void draw_ship() {
  Sprites::drawExternalMask(56-(turn_speed*.3+.5)+x_shake,45,ship, shipMask, 0,0);
}


void start_race() {
  lap=1;
  load_level();
  speed = 0;
  rot = 0;
  turn_speed = 0;
  boost = START_BOOST;
}


void menu() {
  static uint8_t menu_option = 0;
  arduboy.pollButtons();
  char buff[5];

  Sprites::drawOverwrite(5,5,logo, 0);

  tinyfont.setTextColor(WHITE);
  if (menu_option == 0) {
    tinyfont.setTextColor(BLACK);
    arduboy.fillRoundRect(74,27,20,6,2, WHITE);
    if (arduboy.justPressed(LEFT_BUTTON) && level > 0) {
      level--;
    }
    if (arduboy.justPressed(RIGHT_BUTTON) && level < LEVEL_NUM-1) {
      level++;
    }
  }
  tinyfont.setCursor(75,28);
  tinyfont.print("Level");
  Sprites::drawOverwrite(95,26, menuIcon, 0);
  Sprites::drawOverwrite(120,26, menuIcon, 1);
  tinyfont.setCursor(107,28);
  tinyfont.setTextColor(WHITE);
  sprintf(buff, "%02d", level+1);
  tinyfont.print(buff);

  if (menu_option == 1) {
    tinyfont.setTextColor(BLACK);
    arduboy.fillRoundRect(76,37,17,6,2, WHITE);
    if (arduboy.justPressed(LEFT_BUTTON) && num_laps > 1) {
      num_laps--;
    }
    if (arduboy.justPressed(RIGHT_BUTTON) && num_laps < MAX_LAPS) {
      num_laps++;
    }
  }
  tinyfont.setCursor(77,38);
  tinyfont.print("Laps");
  Sprites::drawOverwrite(95,36, menuIcon, 0);
  Sprites::drawOverwrite(120,36, menuIcon, 1);
  tinyfont.setCursor(107,38);
  tinyfont.setTextColor(WHITE);
  sprintf(buff, "%02d", num_laps);
  tinyfont.print(buff);

  if (menu_option == 2) {
    tinyfont.setTextColor(BLACK);
    arduboy.fillRoundRect(57,47,37,6,2, WHITE);
    if (arduboy.justPressed(LEFT_BUTTON) && difficulty > 0) {
      difficulty--;
    }
    if (arduboy.justPressed(RIGHT_BUTTON) && difficulty < 2) {
      difficulty++;
    }
  }
  tinyfont.setCursor(58,48);
  tinyfont.print("Difficulty");
  Sprites::drawOverwrite(95,46, menuIcon, 0);
  Sprites::drawOverwrite(120,46, menuIcon, 1);
  tinyfont.setCursor(104,48);
  tinyfont.setTextColor(WHITE);
  switch(difficulty) {
    case 0:
      tinyfont.print("easy");
      break;
    case 1:
      tinyfont.print("norm");
      break;
    case 2:
      tinyfont.print("hard");
      break;
  }


  tinyfont.setCursor(51,58);
  if (menu_option == 3) {
    tinyfont.setTextColor(BLACK);
    arduboy.fillRoundRect(50,57,51,6,2, WHITE);
    if (arduboy.justPressed(LEFT_BUTTON)) {
      save_laps = false;
    }
    if (arduboy.justPressed(RIGHT_BUTTON)) {
      save_laps = true;
    }
  }
  tinyfont.print("Save Lap Times");
  if(save_laps) {
    Sprites::drawOverwrite(103,56, menuIcon, 4);
    Sprites::drawOverwrite(111,56, menuIcon, 5);
  } else {
    Sprites::drawOverwrite(103,56, menuIcon, 2);
    Sprites::drawOverwrite(111,56, menuIcon, 3);
  }
  tinyfont.setTextColor(WHITE);

  if (arduboy.justPressed(UP_BUTTON)) {
    menu_option = (menu_option + 3) % 4;
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    menu_option = (menu_option + 1) % 4;
  }
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
    state.go(State::START_RACE);
  }
}


void draw_finish() {
  arduboy.pollButtons();
  arduboy.setCursor(20,20);
  arduboy.print("FINISH!");

  long total = 0;
  long best = 999999999;
  uint8_t best_index = 0;
  for(uint8_t i=0;i<num_laps;i++) {
    tinyfont.setCursor(87, 3+i*5);
    tinyfont.print(i+1);
    tinyfont.setCursor(98, 3+i*5);
    print_time(lap_times[i]);
    total += lap_times[i];
    if (lap_times[i] < best) {
      best = lap_times[i];
      best_index = i;
    }
  }

  long best_save = save_data.get_level(level);

  tinyfont.setCursor(48,43);
  tinyfont.print("Best:");
  tinyfont.setCursor(45,49);


  if (best < best_save) {
    print_time(best);
    tinyfont.setCursor(19,32);
    tinyfont.print("New Best Lap");
  } else {
    print_time(best_save);
  }

  arduboy.drawCircle(84, 4+best_index*5, 1, WHITE);

  tinyfont.setCursor(5,43);
  tinyfont.print("Race Time:");
  tinyfont.setCursor(9,49);
  print_time(total);

  tinyfont.setCursor(31,3);
  tinyfont.print("level:");
  tinyfont.setCursor(54,3);
  tinyfont.print(level+1);

  if (state.entered(State::FINISH)) {
    state.go(State::FINISH_MENU, 1500);
  }

  if (state.in(State::FINISH_MENU)) {
    Sprites::drawOverwrite(5,55, menuIcon, 0);
    tinyfont.setCursor(16,57);
    tinyfont.print("Menu");
    Sprites::drawOverwrite(115,55, menuIcon, 1);
    tinyfont.setCursor(75,57);
    tinyfont.print("Next Level");

    if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(A_BUTTON)) {
      state.go(State::MENU);
    }
    if (arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(B_BUTTON)) {
      level = ((level + 1) % LEVEL_NUM);
      state.go(State::START_RACE);
    }
    if (arduboy.buttonsState() & (LEFT_BUTTON | RIGHT_BUTTON | A_BUTTON | B_BUTTON)) {
      if (best < best_save) {
        save_data.set_level(level, best);
      }
      if(save_laps) {
        save_data.save();
      }
    }
  }
}


void setup() {
  arduboy.begin();

  save_data.load();
  save_laps = save_data.valid();
  if (!save_laps) {
    save_data.clear();
  }

  arduboy.clear();
  arduboy.setFrameRate(FRAME_RATE);

  state.go(State::MENU);
}


void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }

  state.frame();

  if (state.in(State::MENU)) {
    menu();
  }

  if (state.entered(State::START_RACE)) {
    start_race();
    state.go(State::RACE, 1500);
  }

  if (lap > num_laps) {
    lap = 1;
    state.go(State::FINISH);
  }

  if (state.in(State::FINISH, State::FINISH_MENU)) {
    draw_finish();
  }

  if (state.in(State::RACE, State::START_RACE)) {
    draw_level();
    draw_ship();
  }

  if (state.in(State::RACE)) {
    physics();
    draw_lap();
    draw_guages(boost/2, min(speed*800.0f, 100.0f));
  }

  if (state.in(State::START_RACE)) {
    lap_start_millis = millis();
    race_start_millis = lap_start_millis;

    arduboy.setCursor(60, 26);
    arduboy.print(state.next_time/500 + 1);
  }

  arduboy.display(CLEAR_BUFFER);
}
