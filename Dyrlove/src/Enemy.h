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
#define ENEMY_N                         (0x0c)

// 種類
#define ENEMY_TYPE_NULL                 (0x00)
#define ENEMY_TYPE_QUILTRA_LEVES        (0x01)
#define ENEMY_TYPE_GNERL                (0x02)
#define ENEMY_TYPE_ZENTCOM_1            (0x03)
#define ENEMY_TYPE_QUEL_QUALLIE         (0x04)
#define ENEMY_TYPE_REGULT               (0x05)
#define ENEMY_TYPE_GLAUG                (0x06)
#define ENEMY_TYPE_QUEADLUUN_RAU        (0x07)
#define ENEMY_TYPE_NOUSJADEUL_GER       (0x08)
#define ENEMY_TYPE_BODDOLE_ZER          (0x09)
#define ENEMY_TYPE_BODDOLE_ZER_NULL_0   (0x0a)
#define ENEMY_TYPE_BODDOLE_ZER_NULL_1   (0x0b)
#define ENEMY_TYPE_BODDOLE_ZER_NULL_2   (0x0c)
#define ENEMY_TYPE_BOMB                 (0x10)

// 状態
#define ENEMY_STATE_NULL                (0x00)

// 向き
#define ENEMY_DIRECTION_RIGHT           (+0x01)
#define ENEMY_DIRECTION_LEFT            (-0x01)

// パラメータ
#define ENEMY_PARAM_0                   (0x00)
#define ENEMY_PARAM_1                   (0x01)
#define ENEMY_PARAM_2                   (0x02)
#define ENEMY_PARAM_3                   (0x03)
#define ENEMY_PARAM_4                   (0x03)
#define ENEMY_PARAM_N                   (0x05)

// ステージ
#define ENEMY_STAGE_NULL                (0x00)
#define ENEMY_STAGE_ZAKO                (0x01)
#define ENEMY_STAGE_WARNING             (0x02)
#define ENEMY_STAGE_BOSS                (0x03)
#define ENEMY_STAGE_FINISH              (0x04)
#define ENEMY_STAGE_CLEAR               (0x05)

// パターン
#define ENEMY_PATTERN_NORMAL            (0x00)
#define ENEMY_PATTERN_DAMAGE            (0x01)
#define ENEMY_PATTERN_CLIP              (0x02)
#define ENEMY_PATTERN_SIZE              (0x03)
#define ENEMY_PATTERN_N                 (0x02)

// 発射
#define ENEMY_FIRE_POSITION_X           (0x00)
#define ENEMY_FIRE_POSITION_Y           (0x01)
#define ENEMY_FIRE_POSITION_SIZE        (0x02)


/*
 *  型宣言
 */

// エネミー
struct Enemy {

  // 種類
  uint8_t type;

  // 状態
  uint8_t state;

  // 位置
  int16_t positionX;
  int16_t positionY;

  // 向き
  int8_t  direction;

  // 耐久力
  uint8_t life;

  // ダメージ
  uint8_t damage;

  // 発射
  uint8_t fire;

  // フレーム
  uint8_t frame;

  // パラメータ
  int8_t params[ENEMY_PARAM_N];
  
};

// ジェネレータ
struct EnemyGenerator {

  // ステージ
  int8_t stage;

  // 順番
  int8_t order;

  // フレーム
  uint16_t frame;

  // 種類
  int8_t type;

  // 向き
  int8_t direction;
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
extern void EnemySetStage(int8_t stage);
extern bool EnemyIsLive(void);
extern bool EnemyIsLive(int8_t index);
extern bool EnemyGetRect(int8_t index, int8_t *rect);
extern void EnemyKill(void);
extern uint8_t EnemyDamage(int8_t index);
extern uint8_t EnemyGetLife(int8_t type);


#endif

