/*
 * Shot.h : 自弾
 */
#ifndef _SHOT_H_
#define _SHOT_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define SHOT_N                          (0x04)

// 状態
#define SHOT_STATE_NULL                 (0x00)
#define SHOT_STATE_MOVE_LEFT            (0x10)
#define SHOT_STATE_MOVE_RIGHT           (0x20)
#define SHOT_STATE_HIT                  (0x30)

// 向き
#define SHOT_DIRECTION_LEFT             (-0x01)
#define SHOT_DIRECTION_RIGHT            (+0x01)

// 速度
#define SHOT_SPEED                      (0x04)


/*
 *  型宣言
 */

// 自弾
struct Shot {

  // 状態
  uint8_t state;

  // 位置
  int8_t positionX;
  int8_t positionY;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void ShotInitialize(void);
extern void ShotUpdate(void);
extern void ShotRender(void);
extern void ShotEntry(int8_t x, int8_t y, int8_t direction);
extern void ShotHit(int8_t index);
extern bool ShotGetPosition(int8_t index, int8_t *x, int8_t *y);


#endif

