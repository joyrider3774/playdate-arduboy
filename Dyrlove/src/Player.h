/*
 * Player.h : プレイヤ
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// フォーム
#define PLAYER_FORM_FIGHTER             (0x00)
#define PLAYER_FORM_FIGHTER_GERWALK     (0x01)
#define PLAYER_FORM_GERWALK             (0x02)
#define PLAYER_FORM_GERWALK_BATTROID    (0x03)
#define PLAYER_FORM_BATTROID            (0x04)
#define PLAYER_FORM_BOMB                (0x05)
#define PLAYER_FORM_N                   (0x06)

// 状態
#define PLAYER_STATE_NULL               (0x00)
#define PLAYER_STATE_STAY               (0x10)
#define PLAYER_STATE_START              (0x20)
#define PLAYER_STATE_PLAY               (0x30)
#define PLAYER_STATE_CLEAR              (0x40)
#define PLAYER_STATE_END                (0x50)
#define PLAYER_STATE_BOMB               (0x60)

// 向き
#define PLAYER_DIRECTION_RIGHT          (+0x01)
#define PLAYER_DIRECTION_LEFT           (-0x01)

// 速度
#define PLAYER_SPEED                    (0x01)

// 耐久力
#define PLAYER_LIFE                     (0x08)

// 発射
#define PLAYER_FIRE_INTERVAL            (0x08)

// パターン
#define PLAYER_PATTERN_NORMAL           (0x00)
#define PLAYER_PATTERN_CLIP             (0x01)
#define PLAYER_PATTERN_X                (0x02)
#define PLAYER_PATTERN_Y                (0x03)
#define PLAYER_PATTERN_WIDTH            (0x04)
#define PLAYER_PATTERN_SIZE             (0x05)
#define PLAYER_PATTERN_N                (0x02)


/*
 *  型宣言
 */

// プレイヤ
struct Player {

  // フォーム
  uint8_t form;

  // 状態
  uint8_t state;

  // 位置
  int8_t positionX;
  int8_t positionY;

  // 向き
  int8_t direction;

  // 耐久力
  uint8_t life;

  // ノーダメージ
  uint8_t nodamage;

  // パラメータ
  int8_t param;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void PlayerInitialize(void);
extern void PlayerUpdate(void);
extern void PlayerRender(void);
extern bool PlayerIsLive(void);
extern bool PlayerIsState(uint8_t state);
extern bool PlayerGetPosition(int8_t *x, int8_t *y);
extern void PlayerSetState(uint8_t state);
extern uint8_t PlayerGetLife(void);
extern void PlayerSetLife(uint8_t life);
extern void PlayerDamage(void);


#endif

