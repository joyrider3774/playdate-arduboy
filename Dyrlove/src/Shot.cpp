/*
 * Shot.cpp : 自弾
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Shot.h"


/*
 * 変数の定義
 */

// 自弾
static struct Shot shots[SHOT_N];


/*
 * 内部関数の宣言
 */


/*
 * 自弾を初期化する
 */
void ShotInitialize(void)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の初期化
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 状態の設定
    shot->state = SHOT_STATE_NULL;

    // 次へ
    ++shot;
  }
}

/*
 * 自弾を更新する
 */
void ShotUpdate(void)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の更新
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 状態別の処理
    switch (shot->state & 0xf0) {

    // ←移動
    case SHOT_STATE_MOVE_LEFT:
      {
        shot->positionX -= SHOT_SPEED;
        if (shot->positionX <= (GAME_VIEW_LEFT - 0x04)) {
          shot->state = SHOT_STATE_NULL;
        }
      }
      break;

    // →移動
    case SHOT_STATE_MOVE_RIGHT:
      {
        shot->positionX += SHOT_SPEED;
        if (shot->positionX >= (GAME_VIEW_RIGHT + 0x04)) {
          shot->state = SHOT_STATE_NULL;
        }
      }
      break;

    // ヒット
    case SHOT_STATE_HIT:
      {
        if ((--shot->state & 0x0f) == 0x00) {
          shot->state = SHOT_STATE_NULL;
        }
      }
      break;

    // その他　
    default:
      break;
    }

    // 次へ
    ++shot;
  }
}

/*
 * 自弾を描画する
 */
void ShotRender(void)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の描画
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 状態別の処理
    switch (shot->state & 0xf0) {

    // ←移動
    case SHOT_STATE_MOVE_LEFT:
      {
        AppClipPattern(
          0x0004, 
          0x0005, 
          shot->positionX - 0x06 + GAME_VIEW_OX, 
          shot->positionY - 0x04 + GAME_VIEW_OY, 
          0x08, 
          SYSTEM_VIDEO_FLIP_NORMAL
        );
      }
      break;

    // →移動
    case SHOT_STATE_MOVE_RIGHT:
      {
        AppClipPattern(
          0x0004, 
          0x0005, 
          shot->positionX - 0x01 + GAME_VIEW_OX, 
          shot->positionY - 0x04 + GAME_VIEW_OY, 
          0x08, 
          SYSTEM_VIDEO_FLIP_REVERSE
        );
      }
      break;
    
    // ヒット
    case SHOT_STATE_HIT:
      {
        AppDrawPattern(
          0x0003, 
          shot->positionX - 0x04 + GAME_VIEW_OX, 
          shot->positionY - 0x04 + GAME_VIEW_OY, 
          0x08, 
          SYSTEM_VIDEO_FLIP_NORMAL
        );
      }
      break;

    // その他
    default:
      break;
    }

    // 次へ
    ++shot;
  }
}

/*
 * 自弾を登録する
 */
void ShotEntry(int8_t x, int8_t y, int8_t direction)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の走査
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 空の自弾
    if (shot->state == SHOT_STATE_NULL) {

      // 自弾の登録
      shot->state = direction == SHOT_DIRECTION_LEFT ? SHOT_STATE_MOVE_LEFT : SHOT_STATE_MOVE_RIGHT;
      shot->positionX = x;
      shot->positionY = y;
      break;
    }

    // 次へ
    ++shot;
  }
}

/*
 * 自弾の存在を取得する
 */
bool ShotIsLive(int8_t index)
{
  uint8_t state = shots[index].state & 0xf0;
  return state == SHOT_STATE_MOVE_LEFT || state == SHOT_STATE_MOVE_RIGHT ? true : false;
}

/*
 * 自弾がヒットする
 */
void ShotHit(int8_t index)
{
  shots[index].state = SHOT_STATE_HIT + 0x04;
}
  
/*
 * 自弾の位置を取得する
 */
bool ShotGetPosition(int8_t index, int8_t *x, int8_t *y)
{
  bool result = false;
  if (ShotIsLive(index)) {
    *x = shots[index].positionX;
    *y = shots[index].positionY;
    result = true;
  }
  return result;
}

