#include "game.hpp"

// warning messy code below

#ifdef ARDUINO
#pragma GCC optimize("no-move-loop-invariants")
#pragma GCC optimize("no-inline-small-functions")
#endif

#if ARDUGOLF_FX

#include "fx_courses.hpp"

#ifndef ARDUINO
#include <FX_DATA.hpp>
#endif

void fx_read_data_bytes(uint24_t addr, uint8_t* dst, size_t n)
{
#ifdef ARDUINO
    FX::readDataBytes(addr, dst, n);
#else
    memcpy(dst, FX_DATA + addr, n);
#endif
}

static array<uint8_t, 19> fx_header;
uint8_t fx_course;

static constexpr uint8_t NUM_COURSES =
    uint8_t(sizeof(ALL_COURSE_ADDRS) / sizeof(uint24_t));
#define NUM_LEVELS fx_header[0]

#else

#include "levels/levels_default.hpp"

static auto const* const LEVELS = LEVELS_DEFAULT;
static constexpr uint8_t NUM_LEVELS =
    sizeof(LEVELS_DEFAULT) / sizeof(LEVELS_DEFAULT[0]);

level_info const* current_level;

#endif

uint8_t leveli;
level_info_ext levelext;

#define DEBUG_TITLE_CAM 0
#define TITLE_GRAPHICS 1
static constexpr uint8_t TITLE_LEVEL = 0;

static constexpr uint8_t STARTING_LEVEL = 0;

// alternative yaw for non camera uses
// (like aiming, tracking ball velocity)
uint16_t yaw_aim;

uint8_t power_aim;
static constexpr uint8_t MIN_POWER = 2;
static constexpr uint8_t MAX_POWER = 128;

static constexpr int16_t DEFAULT_PITCH_AIM = 4096;
static constexpr uint16_t DIST_AIM = 256 * 6;
static constexpr uint16_t DIST_ROLL = 256 * 14;

static dvec3 prev_ball;

static uint8_t practice;
static uint8_t ab_btn_wait;

static int16_t pitch_aim;

static uint8_t yaw_speed;

static uint8_t title_menu_index = 0;

st state;
uint8_t nframe;
static uint16_t yaw_level;

static uint8_t info_offset;
static constexpr uint8_t INFO_OFFSET_MAX = 32;
static uint8_t power_offset;
static constexpr uint8_t POWER_OFFSET_MAX = 32;

static uint8_t menui;
static uint8_t menu_offset;
static constexpr uint8_t MENU_OFFSET_MAX = 42;

static uint8_t prev_btns;

array<uint8_t, 18> shots;

#if ARDUGOLF_FX
static uint24_t get_course_fx_addr()
{
    uint8_t const* pcourse = (uint8_t*)&ALL_COURSE_ADDRS[fx_course];
    uint24_t course_addr =
        ((uint24_t)pgm_read_byte(pcourse + 0) <<  0) |
        ((uint24_t)pgm_read_byte(pcourse + 1) <<  8) |
        ((uint24_t)pgm_read_byte(pcourse + 2) << 16);
    return course_addr;
}

uint24_t get_hole_fx_addr(uint8_t i)
{
    return get_course_fx_addr() + i * 1024 + 256; // +256 to skip header
}

static void set_course(uint8_t course)
{
    fx_course = course;
    fx_read_data_bytes(get_course_fx_addr(), (uint8_t*)&fx_header[0], 19);
    leveli = 0;
}
#endif

static uint8_t get_par(uint8_t i)
{
#if ARDUGOLF_FX
    return fx_header[i + 1];
#else
    return pgm_read_byte((uint8_t*)&LEVELS[i] + offsetof(level_info, ext.par));
#endif
}

static void draw_nframe_progress(uint8_t oy, uint8_t n)
{
    constexpr uint8_t R = 9 * FB_FRAC_COEF;
    int8_t fs = 0;
    int8_t fc = 127;
    dvec2 vc;
    vc.x = 117 * FB_FRAC_COEF;
    vc.y = oy * FB_FRAC_COEF;
    dvec2 v0;
    v0.x = vc.x + R;
    v0.y = vc.y;
    draw_ball_filled(vc, R + FB_FRAC_COEF, 0xffff);
    draw_ball_filled(vc, R - FB_FRAC_COEF, 0x0000);
    for(uint8_t j = 1; j <= n; ++j)
    {
        dvec2 v1;
        v1.x = vc.x + int8_t((uint16_t)fmuls(fcos(j * 8), R) >> 8);
        v1.y = vc.y + int8_t((uint16_t)fmuls(fsin(j * 8), R) >> 8);
        draw_tri(vc, v0, v1, 4);
        v0 = v1;
    }
}

static void reset_ball()
{
    ball = levelext.ball_pos;
    ball_vel = {};
    ball_vel_ang = {};
}

void load_level_from_prog()
{
#if !ARDUGOLF_FX
    memcpy_P(&levelext, &current_level->ext, sizeof(levelext));
    {
        int8_t* pvy  = &buf_vy [0];
        int8_t* pvxz = &buf_vxz[0];
        int8_t const* pv = pgmptr(&current_level->verts);
        for(uint8_t i = 0; i < levelext.num_verts; ++i)
        {
            *pvxz++ = pgm_read_byte(pv++);
            *pvy++  = pgm_read_byte(pv++);
            *pvxz++ = pgm_read_byte(pv++);
        }
    }
    memcpy_P(
        &buf_faces[0],
        pgmptr(&current_level->faces),
        levelext.num_faces * 3);
    memcpy_P(
        &buf_boxes[0],
        pgmptr(&current_level->boxes),
        sizeof(phys_box) * levelext.num_boxes);
#endif
}

void set_level(uint8_t index)
{
    leveli = index;
#if ARDUGOLF_FX

#else
    current_level = &LEVELS[index];
#endif
    reset_ball();
    nframe = 0;
    yaw_level = 0;
    yaw_aim = 0;
    power_aim = 32;
    pitch_aim = DEFAULT_PITCH_AIM;
    state = st::LEVEL;
}

static void reset_to_title()
{
#if ARDUGOLF_FX
    set_course(0);
#endif
    set_level(0);
    for(auto& s : shots) s = 0;
    state = st::TITLE;
    cam = { 3331, 1664, -3451 };
    yaw = 57344;
    pitch = 3840;
    practice &= 1;
    info_offset = INFO_OFFSET_MAX;
    power_offset = POWER_OFFSET_MAX;
    menu_offset = MENU_OFFSET_MAX;
    ab_btn_wait = 0;
    update_camera_reset_velocities();
}

void game_setup()
{
#if ARDUGOLF_FX
    set_course(LEVELS_DEFAULT);
#endif
    set_level(0);
    reset_to_title();

#if DEBUG_SERIAL && defined(ARDUINO)
    Serial.begin(9600);
#endif
}

void move_forward(int16_t amount)
{
    int16_t sinA = mul_f15_s16(amount, fsin16(yaw));
    int16_t cosA = mul_f15_s16(amount, fcos16(yaw));
    cam.x += sinA;
    cam.z -= cosA;
}

void move_right(int16_t amount)
{
    int16_t sinA = mul_f15_s16(amount, fsin16(yaw));
    int16_t cosA = mul_f15_s16(amount, fcos16(yaw));
    cam.x += cosA;
    cam.z += sinA;
}
void move_up(int16_t amount)
{
    cam.y += amount;
}

void look_up(int16_t amount)
{
    pitch -= amount;
    pitch = tclamp<int16_t>(pitch, -64 * 256, 64 * 256);
}
void look_right(int16_t amount)
{
    yaw += amount;
}

bool ball_in_hole()
{
    dvec3 flag = levelext.flag_pos;
    flag.x = tabs(flag.x - ball.x);
    flag.y = tabs(flag.y - ball.y);
    flag.z = tabs(flag.z - ball.z);
    return flag.x < 256 && flag.y < 256 && flag.z < 256;
}

static void draw_scorecard(uint8_t r, uint8_t i, uint8_t OX = 0)
{
    if(i >= NUM_LEVELS) return;
    uint8_t ni = tmin<uint8_t>(9, NUM_LEVELS - i);
    uint8_t y = r * 8;
    uint8_t bx = ni * 10 + 14;
    for(uint8_t x = 0; x <= bx; x += 2)
    {
        set_pixel(x + OX, y + 9);
        set_pixel(x + OX, y + 17);
        set_pixel(x + OX, y + 25);
    }
    uint8_t* b = &buf[0] + (r + 1) * FBW + OX;
    bx -= 14;
    for(uint8_t x = 0; x <= bx; x += 10)
        b[x] = b[x + FBW] = 0xaa;
    b[bx + 14] = b[bx + 14 + FBW] = 0xaa;
    uint8_t tpar = 0;
    uint16_t tshot = 0;
    for(uint8_t n = 1, x = OX + 2; n <= ni; ++n, x += 10)
    {
        set_number2(n + i, r, x);
        uint8_t t = shots[n + i - 1];
        tshot += t;
        if(t != 0)
        {
            set_number2(t, r + 2, x);
            t = get_par(n + i - 1);
            tpar += t;
            set_number2(t, r + 1, x);
        }
    }
    if(tpar != 0)
    {
        set_number3(tpar, r + 1, OX + bx + 2);
        set_number3(tshot, r + 2, OX + bx + 2);
    }
}

static void draw_hole_in_one_overlay()
{
    constexpr uint8_t r = 5;
    constexpr uint8_t c = 14;

    static uint16_t const PATS[8] PROGMEM =
    {
        0xaaff, 0xaa55, 0x0011, 0x0000, 0x0011, 0xaa55, 0xaaff, 0xffff,
    };
    uint16_t pat = pgm_read_word(&PATS[tmin<uint8_t>(nframe & 15, 7)]);

    uint8_t* b = &buf[0] + (r * FBW + c);
    uint8_t const* p = GFX_HIO;
    uint16_t t[3];
    t[0] = 0;
    t[1] = pgm_read_byte(p);
    t[1] |= ((uint16_t)pgm_read_byte(p + 101) << 8);
    ++p;
    for(uint8_t i = 0; i < 100; ++i)
    {
        t[2] = pgm_read_byte(p);
        t[2] |= ((uint16_t)pgm_read_byte(p + 101) << 8);
        ++p;
        uint16_t clear = t[0] | t[1] | t[2];
        clear |= (clear << 1);
        clear |= (clear >> 1);
        b[0] &= ~uint8_t(clear);
        b[0] |= uint8_t(t[1]) & uint8_t(pat);
        b[FBW] &= ~uint8_t(clear >> 8);
        b[FBW] |= uint8_t(t[1] >> 8) & uint8_t(pat);
        ++b;
        t[0] = t[1];
        t[1] = t[2];
        pat = (pat << 8) | (pat >> 8);
    }
}

static void render_in_game_scene_and_graphics()
{
    render_scene();

    if(state != st::MENU && menu_offset < MENU_OFFSET_MAX)
        menu_offset += 3;
    if(state != st::AIM && state != st::LEVEL)
    {
        if(info_offset < INFO_OFFSET_MAX)
            ++info_offset;
        if(power_offset < POWER_OFFSET_MAX)
            ++power_offset;
    }

    if(practice)
        draw_graphic(GFX_MENU, 0, -menu_offset, 4, 42, GRAPHIC_OVERWRITE);
    else
        draw_graphic(GFX_MENU + 42, 0, -menu_offset, 3, 42, GRAPHIC_OVERWRITE);
    draw_graphic(GFX_ARROW, menui, 36 - menu_offset, 1, 3, GRAPHIC_SET);

    draw_graphic(GFX_INFO_BAR, FBR - 3, -info_offset, 3, 28, GRAPHIC_OVERWRITE);
    {
        uint8_t nx = 18 - info_offset;
        set_number2(leveli + 1, FBR - 3, nx);
        set_number2(get_par(leveli), FBR - 2, nx);
        if(state == st::LEVEL)
        {
            draw_graphic(GFX_BES, FBR - 1, -info_offset, 1, 11, GRAPHIC_OVERWRITE);
            load();
            uint8_t best = savedata.best_holes[leveli];
            if(best < 100)
                set_number2(best, FBR - 1, nx);
        }
        else
            set_number2(shots[leveli] + 1, FBR - 1, nx);
    }

    if(power_offset < 5)
    {
        uint8_t* b = &buf[0] + (FBW - 6) + power_offset;
        uint8_t t = 6 - power_offset;
        for(uint16_t y = 0; y < FBW * 8; y += FBW)
        {
            for(uint8_t x = 0; x < t; ++x)
                b[y + x] = 0;
            b[y + 1] = 0xff;
        }
        t = 125 + power_offset;
        for(uint8_t y = 64 - (power_aim >> 1); y < 64; ++y)
            for(uint8_t x = t; x < 128; ++x)
                set_pixel(x, y);
    }
    draw_graphic(GFX_POWER, FBR - 1, 100 + power_offset, 1, 24, GRAPHIC_OVERWRITE);
}

static void update_camera_behind_ball()
{
    dvec3 above_ball = ball;
    above_ball.y += (256 * 2);
    update_camera_look_at_fastangle(
        above_ball, yaw_aim, pitch_aim, DIST_AIM, 64, 64);
}

/**************************************************************************
STATE LOGIC
**************************************************************************/

static void state_title(uint8_t btns, uint8_t pressed)
{
#if DEBUG_TITLE_CAM
    // enable this block to adjust camera view at title screen

    reset_ball();

    if(btns & BTN_A)
    {
        // look
        if(btns & BTN_UP) look_up(256);
        if(btns & BTN_DOWN) look_up(-256);
        if(btns & BTN_LEFT) look_right(-256);
        if(btns & BTN_RIGHT) look_right(256);

        pitch = tclamp<int16_t>(pitch, -64 * 256, 64 * 256);
    }
    else if(btns & BTN_B)
    {
        // rise and fall
        if(btns & BTN_UP) move_up(64);
        if(btns & BTN_DOWN) move_up(-64);
    }
    else
    {
        // move and strafe
        int8_t sinA = fsin(yaw) / 4;
        int8_t cosA = fcos(yaw) / 4;
        if(btns & BTN_UP) move_forward(64);
        if(btns & BTN_DOWN) move_forward(-64);
        if(btns & BTN_LEFT) move_right(-64);
        if(btns & BTN_RIGHT) move_right(64);
    }
#else
    if(ab_btn_wait < 8)
        ++ab_btn_wait;
    else if(pressed & BTN_A)
    {
        if(title_menu_index == 2)
        {
            state = st::HISCORES;
        }
        else
        {
            practice = title_menu_index;
#if ARDUGOLF_FX
            nframe = 0;
            state = st::FX_COURSE;
#else
            ab_btn_wait = 0;
            set_level(0);
#endif
            save_audio_on_off();
        }
    }
    else if(pressed & BTN_B)
        toggle_audio();
    else if(pressed & BTN_UP)
        title_menu_index = (title_menu_index == 0 ? 2 : title_menu_index - 1);
    else if(pressed & BTN_DOWN)
        title_menu_index = (title_menu_index == 2 ? 0 : title_menu_index + 1);
#endif
    render_scene();

#if TITLE_GRAPHICS
    draw_graphic(GFX_TITLE, 1, 50, 2, 77, GRAPHIC_OVERWRITE);
    draw_graphic(GFX_TITLE_MENU, 3, 66, 4, 45, GRAPHIC_OVERWRITE);

    uint8_t x0 = 76, x1 = 100;
    if(title_menu_index == 1)
        x0 = 65, x1 = 111;
    if(title_menu_index == 2)
        x0 = 70, x1 = 106;
    uint8_t y1 = title_menu_index * 10 + 36;
        
    while(x0 <= x1)
    {
        for(uint8_t y = y1 - 8; y <= y1; ++y)
            inv_pixel(x0, y);
        ++x0;
    }
    constexpr uint8_t SHORTEN = SHORTENED_AUDIO_GRAPHIC ? 20 : 0;
    draw_graphic(
        GFX_AUDIO, FBR - 1, FBW - 29 + SHORTEN, 1,
        (audio_enabled() ? 29 : 24) - SHORTEN,
        GRAPHIC_OVERWRITE);
#endif
}

static void state_overview(uint8_t btns, uint8_t pressed)
{
    update_camera_look_at_fastangle(
        { 0, 0, 0 }, yaw_level, 6000, 256 * 28, 64, 64);
    yaw_level += 256;

    if(nframe == 255 || (pressed & BTN_B))
        state = st::AIM, ab_btn_wait = 0;

    render_in_game_scene_and_graphics();
}

static void state_level(uint8_t btns, uint8_t pressed)
{
    reset_ball();
    update_camera_look_at_fastangle(
        { 0, 0, 0 }, yaw_level, 6000, 256 * 28, 64, 64);
    yaw_level += 256;
    if(info_offset > 0)
        --info_offset;
#if !ARDUGOLF_FX
    if(practice & 1)
    {
        if(pressed & BTN_LEFT)
            leveli = (leveli == 0 ? NUM_LEVELS - 1 : leveli - 1);
        if(pressed & BTN_RIGHT)
            leveli = (leveli == NUM_LEVELS - 1 ? 0 : leveli + 1);
        if(ab_btn_wait < 8)
            ++ab_btn_wait;
        else if(pressed & BTN_A)
            state = st::AIM, practice = 2, ab_btn_wait = 0;
        if(pressed & BTN_B)
            reset_to_title();
        current_level = &LEVELS[leveli];
        reset_ball();
        render_in_game_scene_and_graphics();
        return;
    }
    else
#endif
    if(nframe == 255 || (pressed & BTN_B))
        state = st::AIM, ab_btn_wait = 0;

    render_in_game_scene_and_graphics();
}

static void state_aim(uint8_t btns, uint8_t pressed)
{
    update_camera_behind_ball();

    {
        uint16_t ys = yaw_speed;
        if(btns & (BTN_LEFT | BTN_RIGHT))
            ys += ys / 4 + 2;
        else
            ys -= (ys / 4 + 1);
        yaw_speed = (uint8_t)tclamp<uint16_t>(ys, 1, 255);
    }

    if(btns & BTN_LEFT) yaw_aim -= yaw_speed;
    if(btns & BTN_RIGHT) yaw_aim += yaw_speed;

    if(btns & BTN_UP) power_aim += 2;
    if(btns & BTN_DOWN) power_aim -= 2;
    power_aim = tclamp(power_aim, MIN_POWER, MAX_POWER);

    if(info_offset > 0)
        --info_offset;
    if(power_offset > 0)
        --power_offset;

    if(ab_btn_wait < 8)
        ++ab_btn_wait;
    else if(pressed & BTN_A)
    {
        int16_t ys = fsin16(yaw_aim);
        int16_t yc = -fcos16(yaw_aim);
        prev_ball = ball;
        ball_vel.x = mul_f8_s16(ys, power_aim);
        ball_vel.z = mul_f8_s16(yc, power_aim);
        state = st::ROLLING;
        play_tone(180, 100);
    }

    if(pressed & BTN_B)
        state = st::MENU, menui = 0;

    render_in_game_scene_and_graphics();
}

static void state_rolling(uint8_t btns, uint8_t pressed)
{
    if(physics_step())
    {
        yaw_aim = yaw_to_flag();
        shots[leveli] += 1;
        state = st::AIM;
    }
    else if(ball.y < (256 * -20))
    {
        ball = prev_ball;
        ball_vel = {};
        ball_vel_ang = {};
        shots[leveli] += 2; // penalty
        state = st::AIM;
        play_tone(300, 100, 180, 100, 120, 100);
    }
    else if(ball_in_hole())
    {
        shots[leveli] += 1;
        state = st::HOLE;
        yaw_aim = yaw;
        nframe = 0;
        play_tone(180, 100, 300, 100);
    }
    shots[leveli] = tmin<uint8_t>(shots[leveli], 99);
    update_camera_follow_ball(DIST_ROLL, 64, 16);

    render_in_game_scene_and_graphics();
}

static void state_hole(uint8_t btns, uint8_t pressed)
{
    dvec3 flag = levelext.flag_pos;
    update_camera_look_at_fastangle(flag, yaw_aim, 6000, 256 * 20, 64, 64);
    yaw_aim += 256;
    if(nframe == 255 || (pressed & BTN_B))
        state = st::SCORE, ab_btn_wait = 0;

    render_in_game_scene_and_graphics();

    if(shots[leveli] == 1)
        draw_hole_in_one_overlay();
}

static void state_score(uint8_t btns, uint8_t pressed)
{
#if 1
    // update save file
    // save after first rendered frame so any delay isn't noticeable
    if(!practice && ab_btn_wait == 1)
    {
        bool need_save = false;
        load();
        if(shots[leveli] < savedata.best_holes[leveli])
            need_save = true, savedata.best_holes[leveli] = shots[leveli];
        if(leveli == NUM_LEVELS - 1)
        {
            need_save = true;
            savedata.num_played += 1;
            uint16_t ta = 0, tb = 0;
            for(uint8_t i = 0; i < NUM_LEVELS; ++i)
                ta += shots[i], tb += savedata.best_game[i];
            if(ta < tb)
            {
                for(uint8_t i = 0; i < NUM_LEVELS; ++i)
                    savedata.best_game[i] = shots[i];
                savedata.best_game = shots;
            }
        }
        if(need_save)
        {
            savedata.checksum = checksum();
            save();
        }
    }
#endif

    // need to clear here because buffer gets filled by displayPrefetch
    clear_buf();
    draw_scorecard(0, 0);
    draw_scorecard(4, 9);
    if(ab_btn_wait < 16)
        ++ab_btn_wait, nframe = 0;
    else if(btns & (BTN_A | BTN_B))
    {
        if(nframe == 32)
        {
            if(practice || (btns & BTN_B) || leveli == NUM_LEVELS - 1)
                practice >>= 1, reset_to_title();
            else
                set_level(leveli + 1);
        }
    }
    else
        nframe = 0;
    uint8_t proga = 0, progb = 0;
    if(btns & BTN_A) proga = nframe;
    else if(btns & BTN_B) progb = nframe;
    if(!practice)
    {
        draw_nframe_progress(21, proga);
        draw_graphic(GFX_NEXT, 2, 110, 1, 15, GRAPHIC_SET);
    }
    draw_nframe_progress(21 + 24, progb);
    draw_graphic(GFX_QUIT, FBR - 3, 110, 1, 15, GRAPHIC_SET);
}

static void state_menu(uint8_t btns, uint8_t pressed)
{
    update_camera_behind_ball();

    uint8_t n = practice ? 3 : 2;
    if(menu_offset > 0)
        menu_offset -= 3;
    if(pressed & BTN_B)
        state = st::AIM;
    if(pressed & BTN_UP)
        menui = (menui == 0 ? n : menui - 1);
    if(pressed & BTN_DOWN)
        menui = (menui == n ? 0 : menui + 1);
    if(pressed & BTN_A)
    {
        uint8_t m = menui;
        if(practice)
        {
            if(m == 0)
            {
                set_level(leveli);
                state = st::AIM;
                ab_btn_wait = 0;
            }
            --m;
        }
        if(m == 0)
            yaw_level = yaw_aim, nframe = 0, state = st::OVERVIEW;
        if(m == 1)
            state = st::PITCH;
        if(m == 2)
            reset_to_title();
    }

    render_in_game_scene_and_graphics();
}

static void state_pitch(uint8_t btns, uint8_t pressed)
{
    update_camera_behind_ball();

    if(btns & BTN_UP) pitch_aim -= 64;
    if(btns & BTN_DOWN) pitch_aim += 64;
    pitch_aim = tclamp<int16_t>(pitch_aim, 0, 256 * 32);
    if(pressed & BTN_B)
        state = st::AIM;

    render_in_game_scene_and_graphics();
}

static void state_hiscores(uint8_t btns, uint8_t pressed)
{
#if ARDUGOLF_FX
    if(pressed & (BTN_UP | BTN_LEFT))
        set_course(fx_course == 0 ? NUM_COURSES - 1 : fx_course - 1);
    if(pressed & (BTN_DOWN | BTN_RIGHT))
        set_course(fx_course == NUM_COURSES - 1 ? 0 : fx_course + 1);
#endif

    load();
    for(uint8_t i = 0; i < 18; ++i)
    {
        uint8_t n = savedata.best_game[i];
        if(n >= 100) n = 0;
        shots[i] = n;
    }

    clear_buf();
#if !ARDUGOLF_FX
    draw_scorecard(0, 0, 12);
    draw_scorecard(4, 0, 12);
#endif

#if ARDUGOLF_FX
    // shift stuff down to make room for other UI elements
    draw_scorecard(4, 9, 12);
    for(uint8_t i = 7; i >= 4; --i)
    {
        for(uint8_t j = 0; j < FBW; ++j)
        {
            uint8_t* d = &buf[i * FBW + j];
            *d = (*d << 6) | (d[-FBW] >> 2);
        }
    }
    for(uint8_t j = 0; j < FBW; ++j)
    {
        buf[FBW * 0 + j] |= 0x40;
        buf[FBW * 4 + j] &= 0x80;
    }
    draw_scorecard(1, 0, 12);

    fx_read_data_bytes(
            get_course_fx_addr(),
            (uint8_t*)&fxcourseinfo,
            sizeof(fx_level_header));
    uint8_t w = text_width_nonprog(fxcourseinfo.name.data()) >> 1;
    draw_text_nonprog(64 - w, 0, fxcourseinfo.name.data());

    draw_char(64 - w - 8, 0, '{');
    draw_char(64 + w + 6, 0, '}');
#endif

    if(pressed & BTN_B)
        reset_to_title();
}

#if ARDUGOLF_FX
static void state_fx_course(uint8_t btns, uint8_t pressed)
{
    reset_ball();
    update_camera_look_at_fastangle(
        { 0, -12 * 256, 0 }, yaw_level, 6000, 256 * 40, 64, 64);
    yaw_level += 256;

    render_scene();

    draw_graphic(GFX_INFO_BAR, 1, 0, 3, 28, GRAPHIC_OVERWRITE);
    draw_graphic(GFX_BES, 3, 0, 1, 11, GRAPHIC_OVERWRITE);
    set_number2(leveli + 1, 1, 18);
    set_number2(get_par(leveli), 2, 18);
    {
        load();
        uint8_t best = savedata.best_holes[leveli];
        if(best < 100)
            set_number2(best, 3, 18);
    }
    if(practice)
        draw_graphic(GFX_ARROWS_H, 3, 26, 1, 17, GRAPHIC_OVERWRITE);

#if 1
    {
        load();
        uint8_t best = savedata.best_holes[leveli];
        if(best < 100)
        {
            constexpr uint8_t X = 100;
            buf[FBW * 3 + X + 0] = 0x00;
            buf[FBW * 3 + X + 1] = 0xfe;
            for(uint8_t i = X + 2; i < 128; ++i)
                buf[FBW * 3 + i] = 0x02;
            draw_text(X + 3, 8 * 3 + 3, PSTR("Best:"));
            set_number2(best, 3, X + 21);
        }
    }
#endif

    // clear bottom half of buffer
    for(uint16_t i = 0; i < FBW; ++i)
        buf[FBW * (FBR - 4) + i] = 0x02;
    for(uint16_t i = 0; i < FBW * 3; ++i)
        buf[FBW * (FBR - 3) + i] = 0;

    fx_read_data_bytes(
        get_course_fx_addr(),
        (uint8_t*)&fxcourseinfo,
        sizeof(fx_level_header));
    draw_text_nonprog(0, 36, fxcourseinfo.name.data());
    uint8_t w = text_width_nonprog(fxcourseinfo.author.data());
    draw_text_nonprog(128 - w, 36, fxcourseinfo.author.data());

    uint8_t y = 47;
    uint8_t x = 8;
    uint8_t i = 0;
    for(;;)
    {
        char c;
        uint8_t j = i;
        w = 0;
        while((c = fxcourseinfo.desc[j]) > ' ')
            w += char_width(c) + 1, ++j;
        if(x + w > FBW)
        {
            y += 6, x = 8;
            if(y > 59) break;
        }
        while(i < j)
            x += draw_char(x, y, fxcourseinfo.desc[i++]) + 1;
        ++i;
        x += 2;
        if(c == '\0') break;
    }

    draw_graphic(GFX_ARROWS_V, 6, 0, 2, 5, GRAPHIC_OVERWRITE);

    if(pressed & BTN_UP)
        set_course(fx_course == 0 ? NUM_COURSES - 1 : fx_course - 1);
    if(pressed & BTN_DOWN)
        set_course(fx_course == NUM_COURSES - 1 ? 0 : fx_course + 1);
    if(practice && (pressed & BTN_LEFT))
        leveli = (leveli == 0 ? NUM_LEVELS - 1 : leveli - 1);
    if(practice && (pressed & BTN_RIGHT) || !practice && (nframe & 0x3f) == 0x3f)
        leveli = (leveli == NUM_LEVELS - 1 ? 0 : leveli + 1);

    if(ab_btn_wait < 8)
        ++ab_btn_wait;
    else if(pressed & BTN_A)
    {
        if(practice)
            state = st::AIM, practice = 2, ab_btn_wait = 0;
        else
            set_level(0);
    }
    else if(pressed & BTN_B)
        reset_to_title();
}
#endif

using state_func = void(*)(uint8_t, uint8_t);

static state_func const STATE_FUNCS[] PROGMEM =
{
    state_title,
    state_level,
    state_aim,
    state_rolling,
    state_hole,
    state_score,
    state_menu,
    state_overview,
    state_pitch,
    state_hiscores,
#if ARDUGOLF_FX
    state_fx_course,
#endif
};

void game_loop()
{
    uint8_t btns = poll_btns();
    uint8_t pressed = btns & ~prev_btns;
    prev_btns = btns;

    ++nframe;

#if !ARDUGOLF_FX
    load_level_from_prog();
#endif

    pgmptr(&STATE_FUNCS[(uint8_t)state])(btns, pressed);
}

void reset_forder()
{
    for(uint8_t i = 0; i < MAX_FACES; ++i)
        forder[i] = i;
}
