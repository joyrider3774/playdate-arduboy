/*
 * Bullet.cpp : 敵弾
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// 敵弾
static struct Bullet bullets[BULLET_N];

// 三角関数
static const uint8_t bulletSins[] PROGMEM = {
  0x00, 0x06, 0x0c, 0x12, 0x19, 0x1f, 0x25, 0x2b, 0x31, 0x38, 0x3e, 0x44, 0x4a, 0x50, 0x56, 0x5c, 
  0x61, 0x67, 0x6d, 0x73, 0x78, 0x7e, 0x83, 0x88, 0x8e, 0x93, 0x98, 0x9d, 0xa2, 0xa7, 0xab, 0xb0, 
  0xb5, 0xb9, 0xbd, 0xc1, 0xc5, 0xc9, 0xcd, 0xd1, 0xd4, 0xd8, 0xdb, 0xde, 0xe1, 0xe4, 0xe7, 0xea, 
  0xec, 0xee, 0xf1, 0xf3, 0xf4, 0xf6, 0xf8, 0xf9, 0xfb, 0xfc, 0xfd, 0xfe, 0xfe, 0xff, 0xff, 0xff, 
  0x00, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfc, 0xfb, 0xf9, 0xf8, 0xf6, 0xf4, 0xf3, 0xf1, 0xee, 
  0xec, 0xea, 0xe7, 0xe4, 0xe1, 0xde, 0xdb, 0xd8, 0xd4, 0xd1, 0xcd, 0xc9, 0xc5, 0xc1, 0xbd, 0xb9, 
  0xb5, 0xb0, 0xab, 0xa7, 0xa2, 0x9d, 0x98, 0x93, 0x8e, 0x88, 0x83, 0x7e, 0x78, 0x73, 0x6d, 0x67, 
  0x61, 0x5c, 0x56, 0x50, 0x4a, 0x44, 0x3e, 0x38, 0x31, 0x2b, 0x25, 0x1f, 0x19, 0x12, 0x0c, 0x06, 
};


/*
 * 内部関数の宣言
 */
static int16_t BulletGetSin(uint8_t angle);
static int16_t BulletGetCos(uint8_t angle);


/*
 * 敵弾を初期化する
 */
void BulletInitialize(void)
{
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 敵弾の初期化
  for (int8_t i = 0x00; i < BULLET_N; i++) {

    // 状態の設定
    bullet->state = BULLET_STATE_NULL;

    // 次へ
    ++bullet;
  }
}

/*
 * 敵弾を更新する
 */
void BulletUpdate(void)
{
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 敵弾の更新
  for (int8_t i = 0x00; i < BULLET_N; i++) {

    // 状態別の処理
    switch (bullet->state & 0xf0) {
      
    // 移動
    case BULLET_STATE_MOVE:
      {
        // 移動
        int16_t mx = BulletGetCos(bullet->angle);
        int16_t my = BulletGetSin(bullet->angle);
        bullet->positionX += mx;
        bullet->positionY += my;
  
        // 移動の完了
        int8_t x = (int8_t)(bullet->positionX >> 8);
        int8_t y = (int8_t)(bullet->positionY >> 8);
        if (x <= (GAME_VIEW_LEFT - 0x03) || x >= (GAME_VIEW_RIGHT + 0x03) || y <= (GAME_VIEW_TOP - 0x03) || y >= (GAME_VIEW_BOTTOM + 0x03)) {
  
          // 敵弾の削除
          bullet->state = BULLET_STATE_NULL;
        }
      }
      break;

    // その他
    default:
      break;
    }

    // 次へ
    ++bullet;
  }
}

/*
 * 敵弾を描画する
 */
void BulletRender(void)
{
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 敵弾の描画
  for (int8_t i = 0x00; i < BULLET_N; i++) {

    // 状態別の処理
    switch (bullet->state & 0xf0) {
      
    // 移動
    case BULLET_STATE_MOVE:
      {
        AppClipPattern(
          0x0006, 
          0x0007, 
          (int8_t)(bullet->positionX >> 8) - 0x04 + GAME_VIEW_OX, 
          (int8_t)(bullet->positionY >> 8) - 0x04 + GAME_VIEW_OY, 
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
    ++bullet;
  }
}

/*
 * 敵弾を登録する
 */
bool BulletEntry(int8_t x, int8_t y, int8_t angle)
{
  // 結果の初期化
  bool result = false;
  
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 画面内
  if (GAME_VIEW_LEFT + 0x02 <= x && x <= GAME_VIEW_RIGHT - 0x01 && GAME_VIEW_TOP + 0x02 <= y && y <= GAME_VIEW_BOTTOM - 0x01) {
  
    // 敵弾の走査
    for (int8_t i = 0x00; i < BULLET_N; i++) {
  
      // 空の敵弾
      if (bullet->state == BULLET_STATE_NULL) {
  
        // 敵弾の登録
        bullet->state = BULLET_STATE_MOVE;
        bullet->positionX = (int16_t)x << 8;
        bullet->positionY = (int16_t)y << 8;
        bullet->angle = angle;
        result = true;
        break;
      }
  
      // 次へ
      ++bullet;
    }
  }

  // 終了
  return result;
}

/*
 * 敵弾の存在を取得する
 */
bool BulletIsLive(int8_t index)
{
  return bullets[index].state == BULLET_STATE_MOVE ? true : false;
}

/*
 * 敵弾を削除する
 */
void BulletKill(void)
{
  for (int8_t i = 0x00; i < BULLET_N; i++) {
    bullets[i].state = BULLET_STATE_NULL;
  }
}
void BulletKill(int8_t index)
{
  bullets[index].state = BULLET_STATE_NULL;
}

/*
 * 敵弾の矩形を取得する
 */
bool BulletGetRect(int8_t index, int8_t *rect)
{
  bool result = BulletIsLive(index);
  if (result) {
    int8_t x = (int8_t)(bullets[index].positionX >> 8);
    int8_t y = (int8_t)(bullets[index].positionY >> 8);
    rect[GAME_RECT_LEFT] = x - 0x03;
    rect[GAME_RECT_RIGHT] = x + 0x02;
    rect[GAME_RECT_TOP] = y - 0x03;
    rect[GAME_RECT_BOTTOM] = y + 0x02;
  }
  return result;
}

/*
 * 三角関数を取得する
 */
static int16_t BulletGetSin(uint8_t angle)
{
  int16_t value = (int16_t)pgm_read_byte(bulletSins + (angle & 0x7f));
  if ((angle & 0x7f) == 0x40) {
    value = 0x0100;
  }
  if (angle >= 0x80) {
    value = -value;
  }
  return value;
}
static int16_t BulletGetCos(uint8_t angle)
{
  return BulletGetSin(angle + 0x40);
}

