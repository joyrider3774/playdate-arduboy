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

// 状態
#define PLAYER_STATE_NULL               (0x00)
#define PLAYER_STATE_STAY               (0x10)
#define PLAYER_STATE_PLAY               (0x20)

// 速度
#define PLAYER_SPEED_MOVE               (0x03)
#define PLAYER_SPEED_TURN               (0x08)

// コリジョン
#define PLAYER_COLLISION_SIZE           (0x03)

// ショット
#define PLAYER_SHOT_PISTOL              (0x00)
#define PLAYER_SHOT_RIFLE               (0x01)
#define PLAYER_SHOT_MACHINEGUN          (0x02)
#define PLAYER_SHOT_HIT                 (0x01)
#define PLAYER_SHOT_CHANGE              (0x08)

// 時間
#define PLAYER_TIME                     (5 * 60 * 30)

// 弾
#define PLAYER_BULLET                   (999)

// 体力
#define PLAYER_LIFE                     (100)

// ダメージ
#define PLAYER_DAMAGE                   (0x06)


/*
 *  型宣言
 */

// プレイヤ
struct Player {

  // 状態
  uint8_t state;

  // 位置
  int16_t positionX;
  int16_t positionY;

  // 向き
  uint8_t angle;

  // ショット
  uint8_t shot;
  uint8_t shotReload;
  uint8_t shotChange;
  uint8_t shotHit;
  uint8_t shotFire;

  // 時間
  uint16_t time;

  // 弾
  uint16_t bullet;

  // 体力
  uint8_t life;

  // ダメージ
  uint8_t damage;
  int8_t damageX;
  int8_t damageY;
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
extern bool PlayerIsState(uint8_t state);
extern void PlayerSetState(uint8_t state);
extern bool PlayerIsLive(void);
extern bool PlayerGetPosition(uint8_t *x, uint8_t *y);
extern uint8_t PlayerGetAngle(void);


#endif


