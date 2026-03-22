/*
 * Bullet.h : 敵弾
 */
#ifndef _BULLET_H_
#define _BULLET_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define BULLET_N                        (0x20)

// 状態
#define BULLET_STATE_NULL               (0x00)
#define BULLET_STATE_MOVE               (0x10)

// 矩形
#define BULLET_RECT_LEFT                (0x00)
#define BULLET_RECT_RIGHT               (0x01)
#define BULLET_RECT_TOP                 (0x02)
#define BULLET_RECT_BOTTOM              (0x03)
#define BULLET_RECT_SIZE                (0x04)


/*
 *  型宣言
 */

// 自弾
struct Bullet {

  // 状態
  uint8_t state;

  // 位置
  int16_t positionX;
  int16_t positionY;

  // 角度
  int8_t angle;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void BulletInitialize(void);
extern void BulletUpdate(void);
extern void BulletRender(void);
extern bool BulletEntry(int8_t x, int8_t y, int8_t angle);
extern bool BulletIsLive(int8_t index);
extern void BulletKill(void);
extern void BulletKill(int8_t index);
extern bool BulletGetRect(int8_t index, int8_t *rect);


#endif

