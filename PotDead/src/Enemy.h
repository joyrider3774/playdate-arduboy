/*
 * Enemy.h : エネミー
 */
#ifndef _ENEMY_H_
#define _ENEMY_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define ENEMY_N                         (0x10)

// 総数
#define ENEMY_TOTAL                     (100)

// 状態
#define ENEMY_STATE_NULL                (0x00)
#define ENEMY_STATE_STAY                (0x10)
#define ENEMY_STATE_MOVE                (0x20)
#define ENEMY_STATE_DEAD                (0x30)

// コリジョン
#define ENEMY_COLLISION_SIZE            (0x04)

// スケール
#define ENEMY_SCALE_MIN                 (0x08)
#define ENEMY_SCALE_MAX                 (0x40)

// 体力
#define ENEMY_LIFE                      (0x07)

// ダメージ
#define ENEMY_DAMAGE                    (0x05)

// 近さ
#define ENEMY_NEAR_SIZE                 (0x10)


/*
 *  型宣言
 */

// エネミー
struct Enemy {

  // ID
  uint8_t id;
  
  // 状態
  uint8_t state;

  // 位置
  int16_t positionX;
  int16_t positionY;

  // 向き
  uint8_t angle;

  // アニメーション
  uint8_t animation;

  // ビュー
  int16_t viewX;
  uint8_t viewScale;
  uint8_t viewDistance;
  uint16_t viewPattern;
  uint16_t viewMask;

  // 体力
  uint8_t life;

  // ダメージ
  uint8_t damage;

  // 移動
  uint8_t move;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void EnemyInitialize(void);
extern void EnemyUpdate(void);
extern void EnemyRender(void);
extern void EnemyHit(uint8_t id, uint8_t power);
extern void EnemySetState(uint8_t state);
extern uint8_t EnemyGetLive(void);
extern bool EnemyIsNear(uint8_t x, uint8_t y);


#endif


