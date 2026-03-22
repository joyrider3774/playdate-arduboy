/*
 * Player.cpp : プレイヤ
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Shot.h"


/*
 * 変数の定義
 */

// 状態
static struct Player player;

// パターン
static const int8_t playerPatterns[] PROGMEM = {
  0x80, 0xa0, -0x08, -0x06, 0x10, 0x90, 0xb0, -0x08,  0x02, 0x10, 
  0x82, 0xa2, -0x08, -0x06, 0x10, 0x92, 0xb2, -0x08,  0x02, 0x10, 
  0x84, 0xa4, -0x08, -0x06, 0x10, 0x94, 0xb4, -0x08,  0x02, 0x10, 
  0x86, 0xa6, -0x08, -0x06, 0x10, 0x96, 0xb6, -0x08,  0x02, 0x10, 
  0x88, 0xa8, -0x08, -0x06, 0x10, 0x98, 0xb8, -0x08,  0x02, 0x10, 
  0x00, 0x00,  0x00,  0x00, 0x01, 0x00, 0x00,  0x00,  0x00, 0x01, 
};


/*
 * 内部関数の宣言
 */
static void PlayerStart(void);
static void PlayerPlay(void);
static void PlayerClear(void);
static void PlayerEnd(void);
static void PlayerBomb(void);
static void PlayerFire(void);


/*
 * プレイヤを初期化する
 */
void PlayerInitialize(void)
{
  // フォームの設定
  player.form = PLAYER_FORM_FIGHTER;
  
  // 耐久力の設定
  player.life = PLAYER_LIFE;

  // ノーダメージの設定
  player.nodamage = 0x00;

  // 状態の設定
  player.state = PLAYER_STATE_NULL;
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
    break;

  // スタート
  case PLAYER_STATE_START:
    PlayerStart();
    break;

  // プレイ
  case PLAYER_STATE_PLAY:
    PlayerPlay();
    break;

  // クリア
  case PLAYER_STATE_CLEAR:
    PlayerClear();
    break;

  // 終了
  case PLAYER_STATE_END:
    PlayerEnd();
    break;

  // 爆発
  case PLAYER_STATE_BOMB:
    PlayerBomb();
    break;

  // その他
  default:
    break;
  }

  // ノーダメージの更新
  if (player.nodamage > 0x00) {
    --player.nodamage;
  }
}

/*
 * プレイヤを描画する
 */
void PlayerRender(void)
{
  //プレイヤの存在
  if (player.state != PLAYER_STATE_NULL) {

    // 自機の描画
    if (player.form < PLAYER_FORM_BOMB) {
      if ((player.nodamage & 0x02) == 0x00) {
        int8_t *p = playerPatterns + (player.form * PLAYER_PATTERN_N * PLAYER_PATTERN_SIZE);
        for (int8_t i = 0x00; i < PLAYER_PATTERN_N; i++) {
          AppClipPattern(
            pgm_read_byte(p + PLAYER_PATTERN_NORMAL), 
            pgm_read_byte(p + PLAYER_PATTERN_CLIP), 
            player.positionX + pgm_read_byte(p + PLAYER_PATTERN_X) + GAME_VIEW_OX, 
            player.positionY + pgm_read_byte(p + PLAYER_PATTERN_Y) + GAME_VIEW_OY, 
            pgm_read_byte(p + PLAYER_PATTERN_WIDTH), 
            player.direction
          );
          p += PLAYER_PATTERN_SIZE;
        }
      }

    // 爆発の描画
    } else {
      AppDrawPattern(
        0x0010 + (player.param >> 3), 
        player.positionX - 0x08 + GAME_VIEW_OX, 
        player.positionY - 0x08 + GAME_VIEW_OY, 
        0x10, 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
      AppDrawPattern(
        0x0012 + (player.param >> 3), 
        player.positionX - 0x08 + GAME_VIEW_OX, 
        player.positionY + 0x00 + GAME_VIEW_OY, 
        0x10, 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
    }
  }
}

/*
 * プレイヤをスタートする
 */
static void PlayerStart(void)
{
  // 状態別の処理
  switch (player.state & 0x0f) {

  // 0x00: 初期化
  case 0x00:
    {
      // フォームの設定
      player.form = PLAYER_FORM_FIGHTER;
      
      // 位置の設定
      player.positionX = GAME_VIEW_LEFT - 0x08;
      player.positionY = 0x00;
  
      // 向きの設定
      player.direction = PLAYER_DIRECTION_RIGHT;

      // 状態の更新
      ++player.state;
    }
    break;
    
  // 0x01: ファイターで登場
  case 0x01:
    {
      player.positionX += PLAYER_SPEED;
      if (player.positionX >= 0x00) {
        player.param = 0x00;
        ++player.form;
        ++player.state;
      }
    }
    break;

  // 0x02: ガウォークに変形しながら下がる
  case 0x02:
    {
      ++player.param;
      if (player.param >= 0x08) {
        player.form = PLAYER_FORM_GERWALK;
      }
      if ((player.param & 0x01) == 0x00) {
        player.positionX -= PLAYER_SPEED;
      }
      if (player.positionX <= GAME_VIEW_LEFT / 2) {
        player.param = 0x00;
        ++player.form;
        ++player.state;
      }
    }
    break;

  // 0x03: バトロイドに変形
  case 0x03:
    {
      ++player.param;
      if (player.param >= 0x08) {
        player.form = PLAYER_FORM_BATTROID;
        player.state = PLAYER_STATE_STAY;
      }
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * プレイヤをプレイする
 */
static void PlayerPlay(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // パラメータの設定
    player.param = 0x00;

    // 状態の更新
    ++player.state;
  }

  // ←→の移動
  if (SystemIsInputPush(LEFT_BUTTON)) {
    player.positionX -= PLAYER_SPEED;
    if (player.positionX < (GAME_VIEW_LEFT + 0x04)) {
      player.positionX = (GAME_VIEW_LEFT + 0x04);
    }
  } else if (SystemIsInputPush(RIGHT_BUTTON)) {
    player.positionX += PLAYER_SPEED;
    if (player.positionX > (GAME_VIEW_RIGHT - 0x03)) {
      player.positionX = (GAME_VIEW_RIGHT - 0x03);
    }
  }

  // ↑↓の移動
  if (SystemIsInputPush(UP_BUTTON)) {
    player.positionY -= PLAYER_SPEED;
    if (player.positionY < (GAME_VIEW_TOP + GAME_STATUS_HEIGHT + 0x04)) {
      player.positionY = (GAME_VIEW_TOP + GAME_STATUS_HEIGHT + 0x04);
    }
  } else if (SystemIsInputPush(DOWN_BUTTON)) {
    player.positionY += PLAYER_SPEED;
    if (player.positionY > (GAME_VIEW_BOTTOM - 0x03)) {
      player.positionY = (GAME_VIEW_BOTTOM - 0x03);
    }
  }

  // 発射
  if (SystemIsInputPush(B_BUTTON)) {
    player.direction = PLAYER_DIRECTION_RIGHT;
    if (SystemIsInputEdge(B_BUTTON)) {
      player.param = 0x00;
    }
    if (player.param == 0x00) {
      PlayerFire();
    }
    if (++player.param >= PLAYER_FIRE_INTERVAL) {
      player.param = 0x00;
    }
  } else if (SystemIsInputPush(A_BUTTON)) {
    player.direction = PLAYER_DIRECTION_LEFT;
    if (SystemIsInputEdge(A_BUTTON)) {
      player.param = 0x00;
    }
    if (player.param == 0x00) {
      PlayerFire();
    }
    if (++player.param >= PLAYER_FIRE_INTERVAL) {
      player.param = 0x00;
    }
  }
}

/*
 * プレイヤをクリアする
 */
static void PlayerClear(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // パラメータの設定
    player.param = 0x00;

    // 状態の更新
    ++player.state;
  }

  // パラメータの更新
  if ((++player.param & 0x01) == 0x00) {
    
    // →移動
    if (player.positionX < 0x00) {
      ++player.positionX;

    // ←移動
    } else if (player.positionX > 0x00) {
      --player.positionX;

    // ↓移動
    } else if (player.positionY < 0x00) {
      ++player.positionY;
      
    // ↑移動
    } else if (player.positionY > 0x00) {
      --player.positionY;

    // 移動の完了　
    } else {
      player.state = PLAYER_STATE_STAY;
    }
  }
}
    
/*
 * プレイヤを終了する
 */
static void PlayerEnd(void)
{
  // 状態別の処理
  switch (player.state & 0x0f) {

  // 0x00: 初期化
  case 0x00:
    {
      player.param = 0x00;
      --player.form;
      ++player.state;
    }
    break;
    
  // 0x01: ガウォークに変形しながら下がる
  case 0x01:
    {
      ++player.param;
      if (player.param >= 0x08) {
        player.form = PLAYER_FORM_GERWALK;
      }
      if ((player.param & 0x02) == 0x00) {
        player.positionX -= PLAYER_SPEED;
      }
      if (player.positionX <= GAME_VIEW_LEFT * 3 / 4) {
        player.param = 0x00;
        --player.form;
        ++player.state;
      }
    }
    break;

  // 0x02: ファイターで退場
  case 0x02:
    {
      ++player.param;
      if (player.param >= 0x08) {
        player.form = PLAYER_FORM_FIGHTER;
      }
      player.positionX += 0x02 * PLAYER_SPEED;
      if (player.positionX >= GAME_VIEW_RIGHT + 0x08) {
        player.state = PLAYER_STATE_STAY;
      }
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * プレイヤが爆発する
 */
static void PlayerBomb(void)
{
  // パラメータの更新
  if (--player.param == 0x00) {

    // プレイヤの削除
    player.state = PLAYER_STATE_NULL;
  }
}

/*
 * 自弾を発射する
 */
static void PlayerFire(void)
{
  ShotEntry(player.positionX, player.positionY, player.direction);
}

/*
 * プレイヤの存在を取得する
 */
bool PlayerIsLive(void)
{
  return (player.state & 0xf0) != PLAYER_STATE_NULL && player.life > 0x00 ? true : false;
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
 * プレイヤの位置を取得する
 */
bool PlayerGetPosition(int8_t *x, int8_t *y)
{
  bool result = false;
  if ((player.state & 0xf0) == PLAYER_STATE_PLAY) {
    *x = player.positionX;
    *y = player.positionY;
    result = true;
  }
  return result;
}

/*
 * プレイヤの耐久力を取得する
 */
uint8_t PlayerGetLife(void)
{
  return player.life;
}

/*
 * プレイヤの耐久力を設定する
 */
void PlayerSetLife(uint8_t life)
{
  if (life < PLAYER_LIFE) {
    player.life = life;
  }
}

/*
 * プレイヤがダメージを受ける
 */
void PlayerDamage(void)
{
  if (player.nodamage == 0x00 && player.life > 0x00) {
    if (--player.life == 0x00) {
      player.form = PLAYER_FORM_BOMB;
      player.state = PLAYER_STATE_BOMB;
      player.param = 0x18;
    }
    player.nodamage = 0x30;
  }
}

