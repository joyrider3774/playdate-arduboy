/*
 * Player.cpp : プレイヤ
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"


/*
 * 変数の定義
 */

// 状態
static struct Player player;

// ショット
static const uint8_t playerShotReloads[] PROGMEM = {
  0x0c, 0x18, 0x03, 
};
static const uint8_t playerShotFires[] PROGMEM = {
  0x06, 0x0c, 0x02, 
};
static const uint8_t playerShotPowers[] PROGMEM = {
  0x03, 0x0a, 0x01, 
};
static const uint8_t playerShotPatterns[] PROGMEM = {
  0x80, 0x08, 0x81, 0x08, 0x8c, 0x08, 0x00, 0x00, 
  0x82, 0x10, 0x84, 0x10, 0x8d, 0x08, 0x00, 0x00, 
  0x86, 0x18, 0x89, 0x18, 0x8e, 0x10, 0x00, 0x00, 
};

// サウンド
static const char playerSoundShotPistol[] PROGMEM = "T1O5C1";
static const char playerSoundShotRifle[] PROGMEM = "T1O4C2";
static const char playerSoundShotMachineGun[] PROGMEM = "T1O2C1";
static const char playerSoundShotNoBullet[] PROGMEM = "T1O2A1";



/*
 * 内部関数の宣言
 */
static void PlayerStay(void);
static void PlayerPlay(void);


/*
 * プレイヤを初期化する
 */
void PlayerInitialize(void)
{
  // 位置の設定
  player.positionX = 0x0080 << 8;
  player.positionY = 0x0080 << 8;

  // 向きの設定
  player.angle = 0xc0;

  // ショットの設定
  player.shot = PLAYER_SHOT_PISTOL;
  player.shotReload = 0x00;
  player.shotChange = 0x00;
  player.shotHit = 0x00;
  player.shotFire = 0x00;

  // 時間の設定
  player.time = PLAYER_TIME;

  // 弾の設定
  player.bullet = PLAYER_BULLET;

  // 体力の設定
  player.life = PLAYER_LIFE;

  // ダメージの設定
  player.damage = 0x00;
  player.damageX = 0x00;
  player.damageY = 0x00;

  // 状態の設定
  player.state = PLAYER_STATE_STAY;
}

/*
 * プレイヤを更新する
 */
void PlayerUpdate(void)
{
  // 状態別の処理
  switch (player.state & 0xf0) {

  // 何もしない
  case PLAYER_STATE_NULL:
    break;

  // 待機
  case PLAYER_STATE_STAY:
    PlayerStay();
    break;

  // プレイ
  case PLAYER_STATE_PLAY:
    PlayerPlay();
    break;

  // その他
  default:
    break;
  }
}

/*
 * プレイヤを描画する
 */
void PlayerRender(void)
{
  // ショットの描画
  {
    // ヒットマークの描画
    if (player.shotHit > 0x00) {
      uint8_t distance = FieldGetViewDistance(0x40);
      if (distance < FIELD_DISTANCE_FAR) {
        uint8_t pattern = distance > (FIELD_DISTANCE_FAR / 0x02) ? 0x10 : 0x12;
        AppClipPattern(pattern + 0x01, pattern + 0x00, 0x3c, 0x1e, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }

    // 銃の描画
    {
      uint8_t *p = playerShotPatterns + (player.shot << 3);
      uint8_t y = 0x30 + ((((player.shotChange & 0x80) != 0x00 ? PLAYER_SHOT_CHANGE - (player.shotChange & 0x7f) : player.shotChange)) << 1);
      if (player.shotFire > 0x00) {
        uint8_t pattern = pgm_read_byte(p + 0x04);
        uint8_t length = pgm_read_byte(p + 0x05);
        uint8_t x = (WIDTH - length) >> 1;
        AppClipPattern(pattern + 0x00, pattern + 0x20, x, y - 0x0c + 0x00, length, SYSTEM_VIDEO_FLIP_NORMAL);
        AppClipPattern(pattern + 0x10, pattern + 0x30, x, y - 0x0c + 0x08, length, SYSTEM_VIDEO_FLIP_NORMAL);
        p += 0x02;
      }
      {
        uint8_t pattern = pgm_read_byte(p + 0x00);
        uint8_t length = pgm_read_byte(p + 0x01);
        uint8_t x = (WIDTH - length) >> 1;
        AppClipPattern(pattern + 0x00, pattern + 0x20, x, y + 0x00, length, SYSTEM_VIDEO_FLIP_NORMAL);
        AppClipPattern(pattern + 0x10, pattern + 0x30, x, y + 0x08, length, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }
  }

  // 時間の描画
  {
    uint16_t time = (player.time + 29) / 30;
    uint8_t minute = time / 60;
    uint8_t second = time % 60;
    AppDrawPattern(0x14, 0x60 + player.damageX, 0x39 + player.damageY, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawChar('\'', 0x70 + player.damageX, 0x3a + player.damageY);
    AppDrawChar('\"', 0x7c + player.damageX, 0x3a + player.damageY);
    AppDrawNumber(minute, 0x68 + player.damageX, 0x3a + player.damageY, 0x02);
    AppDrawNumber(second, 0x74 + player.damageX, 0x3a + player.damageY, 0x02);
  }

  // 体力の描画
  {
    AppDrawPattern(0x15, 0x00 + player.damageX, 0x00 + player.damageY, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawNumber(player.life, 0x08 + player.damageX, 0x01 + player.damageY, 0x03);
  }

  // 弾の描画
  {
    uint8_t mask = 0xff << (0x07 * player.shotReload / pgm_read_byte(playerShotReloads + player.shot));
    AppMaskPattern(0x16, mask, 0x18 + player.damageX, 0x00 + player.damageY, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawNumber(player.bullet, 0x20 + player.damageX, 0x01 + player.damageY, 0x03);
  }

  // 残りの数の描画
  {
    AppDrawPattern(0x17, 0x5c + player.damageX, 0x00 + player.damageY, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawChar('/', 0x70 + player.damageX, 0x01 + player.damageY);
    AppDrawNumber(EnemyGetLive(), 0x64 + player.damageX, 0x01 + player.damageY, 0x03);
    AppDrawNumber(ENEMY_TOTAL, 0x74 + player.damageX, 0x01 + player.damageY, 0x03);
  }
}

/*
 * プレイヤを待機する
 */
static void PlayerStay(void)
{
  // カメラの設定
  FieldSetCamera(player.positionX >> 8, player.positionY >> 8, player.angle);
}

/*
 * プレイヤをプレイする
 */
static void PlayerPlay(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // 状態の更新
    ++player.state;
  }

  // 時間の更新
  if (player.time >= 0x00) {
    --player.time;
  }

  // ダメージの更新
  if (player.damage == 0x00) {
    if (EnemyIsNear(player.positionX >> 8, player.positionY >> 8)) {
      if (player.life > 0x00) {
        --player.life;
        player.damage = PLAYER_DAMAGE;
      }
    }
  }
  if (player.damage > 0x00 && PlayerIsLive()) {
    --player.damage;
    player.damageX = (SystemGetRandom() & 0x04) - 0x02;
    player.damageY = (SystemGetRandom() & 0x04) - 0x02;
  } else {
    player.damageX = 0x00;
    player.damageY = 0x00;
  }
  
  // ショットの更新
  {
    // ヒット判定
    if (player.shotHit > 0x00) {
      if (player.shotHit == PLAYER_SHOT_HIT) {
        EnemyHit(FieldGetViewMap(0x40), pgm_read_byte(playerShotPowers + player.shot));
      }
      --player.shotHit;
    }

    // リロードの更新
    if (player.shotReload > 0x00) {
      --player.shotReload;
    }

    // 持ち替えの更新
    if (player.shotChange != 0x00) {
      --player.shotChange;
      if (player.shotChange == 0x80) {
        if (++player.shot > PLAYER_SHOT_MACHINEGUN) {
          player.shot = PLAYER_SHOT_PISTOL;
        }
        player.shotChange = PLAYER_SHOT_CHANGE;
      }
    }

    // ファイアの更新
    if (player.shotFire > 0x00) {
      --player.shotFire;
    }

    // ショットの操作
    if (PlayerIsLive()) {
      
      // 発射
      if (player.shotReload == 0x00 && SystemIsInputPush(B_BUTTON)) {
        uint8_t power = pgm_read_byte(playerShotPowers + player.shot);
        if (player.bullet >= power) {
          player.shotReload = pgm_read_byte(playerShotReloads + player.shot);
          player.shotHit = PLAYER_SHOT_HIT;
          player.shotFire = pgm_read_byte(playerShotFires + player.shot);
          player.bullet -= power;
          SystemRequestSound(
            player.shot == PLAYER_SHOT_PISTOL ? playerSoundShotPistol : (player.shot == PLAYER_SHOT_RIFLE ? playerSoundShotRifle : playerSoundShotMachineGun), 
            false
          );
        } else {
          SystemRequestSound(playerSoundShotNoBullet, false);
        }
        
      // 銃の持ち替え
      } else if (player.shotChange == 0x00 && SystemIsInputPush(A_BUTTON) && SystemIsInputPush(DOWN_BUTTON)) {
        player.shotChange = 0x80 | PLAYER_SHOT_CHANGE;
      }
    }
  }

  // 移動
  if (PlayerIsLive()) {
    int16_t mx = 0x0000;
    int16_t my = 0x0000;
    if (SystemIsInputPush(A_BUTTON)) {
      if (SystemIsInputPush(LEFT_BUTTON)) {
        player.angle -= PLAYER_SPEED_TURN;
      } else if (SystemIsInputPush(RIGHT_BUTTON)) {
        player.angle += PLAYER_SPEED_TURN;
      }
      if (SystemIsInputPush(UP_BUTTON)) {
        mx += AppGetCos(player.angle);
        my += AppGetSin(player.angle);
      } else if (SystemIsInputPush(DOWN_BUTTON)) {
        mx += AppGetCos(player.angle + 0x80);
        my += AppGetSin(player.angle + 0x80);
      }
    } else {
      if (SystemIsInputPush(LEFT_BUTTON)) {
        mx += AppGetCos(player.angle - 0x40);
        my += AppGetSin(player.angle - 0x40);
      } else if (SystemIsInputPush(RIGHT_BUTTON)) {
        mx += AppGetCos(player.angle + 0x40);
        my += AppGetSin(player.angle + 0x40);
      }
      if (SystemIsInputPush(UP_BUTTON)) {
        mx += AppGetCos(player.angle);
        my += AppGetSin(player.angle);
      } else if (SystemIsInputPush(DOWN_BUTTON)) {
        mx += AppGetCos(player.angle + 0x80);
        my += AppGetSin(player.angle + 0x80);
      }
    }
    mx *= PLAYER_SPEED_MOVE;
    my *= PLAYER_SPEED_MOVE;
    if (mx < 0x0000) {
      player.positionX += mx;
      uint8_t x = (player.positionX >> 8) - PLAYER_COLLISION_SIZE;
      uint8_t y = player.positionY >> 8;
      if (FieldGetMap(x, y - PLAYER_COLLISION_SIZE) != 0x00 || FieldGetMap(x, y + PLAYER_COLLISION_SIZE) != 0) {
        player.positionX = ((x & 0xf0) + 0x10 + PLAYER_COLLISION_SIZE) << 8;
      }
    } else if (mx > 0x0000) {
      player.positionX += mx;
      uint8_t x = (player.positionX >> 8) + PLAYER_COLLISION_SIZE;
      uint8_t y = player.positionY >> 8;
      if (FieldGetMap(x, y - PLAYER_COLLISION_SIZE) != 0x00 || FieldGetMap(x, y + PLAYER_COLLISION_SIZE) != 0) {
        player.positionX = ((x & 0xf0) - 0x01 - PLAYER_COLLISION_SIZE) << 8;
      }
    }
    if (my < 0x0000) {
      player.positionY += my;
      uint8_t x = player.positionX >> 8;
      uint8_t y = (player.positionY >> 8) - PLAYER_COLLISION_SIZE;
      if (FieldGetMap(x - PLAYER_COLLISION_SIZE, y) != 0x00 || FieldGetMap(x + PLAYER_COLLISION_SIZE, y) != 0) {
        player.positionY = ((y & 0xf0) + 0x10 + PLAYER_COLLISION_SIZE) << 8;
      }
    } else if (my > 0x0000) {
      player.positionY += my;
      uint8_t x = player.positionX >> 8;
      uint8_t y = (player.positionY >> 8) + PLAYER_COLLISION_SIZE;
      if (FieldGetMap(x - PLAYER_COLLISION_SIZE, y) != 0x00 || FieldGetMap(x + PLAYER_COLLISION_SIZE, y) != 0) {
        player.positionY = ((y & 0xf0) - 0x01 - PLAYER_COLLISION_SIZE) << 8;
      }
    }
  }

  // カメラの設定
  FieldSetCamera(player.positionX >> 8, player.positionY >> 8, player.angle);
}

/*
 * プレイヤの状態を判定する
 */
bool PlayerIsState(uint8_t state)
{
  return (player.state & 0xf0) == state;
}

/*
 * プレイヤの状態を設定する
 */
void PlayerSetState(uint8_t state)
{
  player.state = state;
}

/*
 * プレイヤが生存しているかどうかを取得する
 */
bool PlayerIsLive(void)
{
  return player.time > 0x0000 && player.life > 0x00 && player.bullet > 0x0000 ? true : false;
}

/*
 * プレイヤの位置を取得する
 */
bool PlayerGetPosition(uint8_t *x, uint8_t *y)
{
  bool result = false;
  if ((player.state & 0xf0) != PLAYER_STATE_NULL) {
    *x = player.positionX >> 8;
    *y = player.positionY >> 8;
    result = true;
  }
  return result;
}

/*
 * プレイヤの向きを取得する
 */
uint8_t PlayerGetAngle(void)
{
  return player.angle;
}


