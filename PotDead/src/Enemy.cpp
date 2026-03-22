/*
 * Enemy.cpp : エネミー
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
#include "pattern.h"


/*
 * 変数の定義
 */

// エネミー
static struct Enemy enemys[ENEMY_N];

// エネミーの数
static uint8_t enemyDead;
static uint8_t enemyRest;

// エリア
static const uint8_t enemyAreaXys[] PROGMEM = {
  0x00, 0x00, 
  0x80, 0x00, 
  0x00, 0x80, 
  0x80, 0x80, 
};

// 拡大縮小
static const uint8_t enemyScaleXs[] PROGMEM = {
  0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
  0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 
  0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 
  0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 
  0x01, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 
  0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 
  0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 
  0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 
  0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
  0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 
  0x02, 0x02, 0x03, 0x02, 0x02, 0x03, 0x02, 0x03, 0x02, 0x02, 0x03, 0x02, 0x02, 0x03, 0x02, 0x03, 
  0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 
  0x02, 0x03, 0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x02, 0x03, 0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 
  0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 
  0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 
  0x03, 0x03, 0x03, 0x04, 0x03, 0x03, 0x03, 0x04, 0x03, 0x03, 0x03, 0x04, 0x03, 0x03, 0x03, 0x04, 
  0x03, 0x03, 0x04, 0x03, 0x03, 0x04, 0x03, 0x04, 0x03, 0x03, 0x04, 0x03, 0x03, 0x04, 0x03, 0x04, 
  0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 
  0x03, 0x04, 0x03, 0x04, 0x04, 0x03, 0x04, 0x04, 0x03, 0x04, 0x03, 0x04, 0x04, 0x03, 0x04, 0x04, 
  0x03, 0x04, 0x04, 0x04, 0x03, 0x04, 0x04, 0x04, 0x03, 0x04, 0x04, 0x04, 0x03, 0x04, 0x04, 0x04, 
  0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
};
static const uint8_t enemyScaleYs[] PROGMEM = {
  0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
  0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 
  0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 
  0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 
  0x01, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 
  0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 
  0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 
  0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 
  0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
  0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 
  0x02, 0x02, 0x03, 0x02, 0x02, 0x03, 0x02, 0x03, 
  0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 
  0x02, 0x03, 0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 
  0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 
  0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x03, 0x03, 0x03, 0x04, 0x03, 0x03, 0x03, 0x02, 
  0x03, 0x03, 0x04, 0x03, 0x03, 0x04, 0x03, 0x01, 
  0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x00, 
  0x03, 0x04, 0x03, 0x04, 0x04, 0x03, 0x03, 0x00, 
  0x03, 0x04, 0x04, 0x04, 0x03, 0x04, 0x02, 0x00, 
  0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x00, 
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 
};

// パターン
static const uint16_t enemyPatterns[] PROGMEM = {
  0x0120, 0x0160, 0x0100, 0x0140, 
};

// 描画順
static uint8_t enemyDrawOrders[ENEMY_N];


/*
 * 内部関数の宣言
 */
static struct Enemy *EnemyGetEmpty(void);
static void EnemyBorn(struct Enemy *enemy, uint8_t area);
static void EnemyStay(struct Enemy *enemy);
static void EnemyMove(struct Enemy *enemy);
static void EnemyDead(struct Enemy *enemy);
static bool EnemyIsNearPlayer(struct Enemy *enemy);
static void EnemyCalcView(void);
static void EnemyDraw(void);
static void EnemyDraw(struct Enemy *enemy);


/*
 * エネミーを初期化する
 */
void EnemyInitialize(void)
{
  // エネミーの数の設定
  enemyDead = 0x00;
  enemyRest = ENEMY_TOTAL;
  
  // エネミーの走査
  for (int8_t i = 0x00; i < ENEMY_N; i++) {

    // ID の設定
    enemys[i].id = 0x80 | i;

    // エネミーの生成
    EnemyBorn(&enemys[i], i & 0x03);
  }
}

/*
 * エネミーを更新する
 */
void EnemyUpdate(void)
{
  // エネミーの生成
  if (enemyRest > 0x00) {
    struct Enemy *enemy = EnemyGetEmpty();
    if (enemy != NULL) {
      uint8_t area = 0x00;
      uint8_t px, py;
      if (PlayerGetPosition(&px, &py)) {
        if (px < 0x80) {
          area |= 0x01;
        }
        if (py < 0x80) {
          area |= 0x02;
        }
        EnemyBorn(enemy, area);
      }
    }
  }
  
  // エネミーの走査
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    
    // 状態別の処理
    switch (enemys[i].state & 0xf0) {
  
    // 何もしない
    case ENEMY_STATE_NULL:
      break;
  
    // 待機
    case ENEMY_STATE_STAY:
      EnemyStay(&enemys[i]);
      break;
  
    // 移動
    case ENEMY_STATE_MOVE:
      EnemyMove(&enemys[i]);
      break;
  
    // 死亡
    case ENEMY_STATE_DEAD:
      EnemyDead(&enemys[i]);
      break;
  
    // その他
    default:
      break;
    }
  }

  // ビューの計算
  EnemyCalcView();
}

/*
 * エネミーを描画する
 */
void EnemyRender(void)
{
  // エネミーの描画
  EnemyDraw();
}

/*
 * 空のエネミーを取得する
 */
static struct Enemy *EnemyGetEmpty(void)
{
  struct Enemy *enemy = NULL;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemys[i].state == ENEMY_STATE_NULL) {
      enemy = &enemys[i];
      break;
    }
  }
  return enemy;
}

/*
 * エネミーを生成する
 */
static void EnemyBorn(struct Enemy *enemy, uint8_t area)
{
  // 位置の設定
  {
    uint8_t areaX = pgm_read_byte(enemyAreaXys + (area << 1) + 0x00);
    uint8_t areaY = pgm_read_byte(enemyAreaXys + (area << 1) + 0x01);
    uint8_t x = 0x10;
    uint8_t y = 0x10;
    uint8_t r = (SystemGetRandom() >> 2) + 0x01;
    while (r > 0x00) {
      do {
        x += 0x10;
        if (x >= 0x70) {
          x = 0x10;
          y += 0x10;
          if (y >= 0x70) {
            y = 0x10;
          }
        }
      } while (FieldGetMap(x + areaX, y + areaY) != 0x00);
      --r;
    }
    enemy->positionX = (uint16_t)(x + areaX + 0x08) << 8;
    enemy->positionY = (uint16_t)(y + areaY + 0x08) << 8;
  }

  // 向きの設定
  enemy->angle = SystemGetRandom();;

  // アニメーションの設定
  enemy->animation = enemy->angle;

  // ビューの設定
  enemy->viewScale = 0x00;
  enemy->viewMask = 0xffff;

  // 体力の設定
  enemy->life = ENEMY_LIFE;

  // ダメージの設定
  enemy->damage = 0x00;

  // 状態の設定
  enemy->state = ENEMY_STATE_MOVE;

  // エネミーの数の更新
  --enemyRest;
}

/*
 * エネミーを待機する
 */
static void EnemyStay(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // 状態の更新
    ++enemy->state;
  }

  // ダメージの更新
  if (enemy->damage > 0x00) {
    if (--enemy->damage == 0x00) {
      if (enemy->life == 0x00) {
        enemy->state = ENEMY_STATE_DEAD;
      }
    }
  }
  
  // パターンの更新
  ++enemy->animation;
}

/*
 * エネミーを移動する
 */
static void EnemyMove(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // 移動の設定
    {
      uint8_t px, py;
      if (PlayerGetPosition(&px, &py) && (SystemGetRandom() & 0x03) > 0x00) {
        int16_t dx = (int16_t)px - (int16_t)((enemy->positionX >> 8) & 0xff);
        int16_t dy = (int16_t)py - (int16_t)((enemy->positionY >> 8) & 0xff);
        enemy->angle = AppGetAtan(dy, dx);
        enemy->move = (SystemGetRandom() & 0x3f) + 0x40;
      } else {
        enemy->angle = SystemGetRandom();
        enemy->move = (SystemGetRandom() & 0x3f) + 0x30;
      }
    }

    // 状態の更新
    ++enemy->state;
  }

  // ダメージの更新
  if (enemy->damage > 0x00) {
    if (--enemy->damage == 0x00) {
      if (enemy->life == 0x00) {
        enemy->state = ENEMY_STATE_DEAD;
      }
    }
    
  // プレイヤの近くにいる場合は待機
  } else if (EnemyIsNearPlayer(enemy)) {
    
    // 移動の完了
    if (--enemy->move == 0x00) {
      enemy->state = ENEMY_STATE_MOVE;
    }

  // 移動
  } else {
    
    // 移動量の取得
    int16_t mx = AppGetCos(enemy->angle) >> 2;
    int16_t my = AppGetSin(enemy->angle) >> 2;
    uint8_t hit = 0x00;

    // X 方向の移動
    if (mx < 0x0000) {
      enemy->positionX += mx;
      uint8_t x = (enemy->positionX >> 8) - ENEMY_COLLISION_SIZE;
      uint8_t y = enemy->positionY >> 8;
      if (FieldGetMap(x, y - ENEMY_COLLISION_SIZE) != 0x00 || FieldGetMap(x, y + ENEMY_COLLISION_SIZE) != 0) {
        enemy->positionX = ((x & 0xf0) + 0x10 + ENEMY_COLLISION_SIZE) << 8;
        hit |= 0x01;
      }
    } else if (mx > 0x0000) {
      enemy->positionX += mx;
      uint8_t x = (enemy->positionX >> 8) + ENEMY_COLLISION_SIZE;
      uint8_t y = enemy->positionY >> 8;
      if (FieldGetMap(x, y - ENEMY_COLLISION_SIZE) != 0x00 || FieldGetMap(x, y + ENEMY_COLLISION_SIZE) != 0) {
        enemy->positionX = ((x & 0xf0) - 0x01 - ENEMY_COLLISION_SIZE) << 8;
        hit |= 0x01;
      }
    }

    // Y 方向の移動
    if (my < 0x0000) {
      enemy->positionY += my;
      uint8_t x = enemy->positionX >> 8;
      uint8_t y = (enemy->positionY >> 8) - ENEMY_COLLISION_SIZE;
      if (FieldGetMap(x - ENEMY_COLLISION_SIZE, y) != 0x00 || FieldGetMap(x + ENEMY_COLLISION_SIZE, y) != 0) {
        enemy->positionY = ((y & 0xf0) + 0x10 + ENEMY_COLLISION_SIZE) << 8;
        hit |= 0x02;
      }
    } else if (my > 0x0000) {
      enemy->positionY += my;
      uint8_t x = enemy->positionX >> 8;
      uint8_t y = (enemy->positionY >> 8) + ENEMY_COLLISION_SIZE;
      if (FieldGetMap(x - ENEMY_COLLISION_SIZE, y) != 0x00 || FieldGetMap(x + ENEMY_COLLISION_SIZE, y) != 0) {
        enemy->positionY = ((y & 0xf0) - 0x01 - ENEMY_COLLISION_SIZE) << 8;
        hit |= 0x02;
      }
    }

    // 移動の完了、あるいは移動できない
    if (--enemy->move == 0x00 || hit == 0x03) {
      enemy->state = ENEMY_STATE_MOVE;
    }
  }

  // アニメーションの更新
  ++enemy->animation;
}

/*
 * エネミーが死亡する
 */
static void EnemyDead(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // 状態の更新
    ++enemy->state;
  }

  // ビューの更新
  enemy->viewMask >>= 1;
  if (enemy->viewMask == 0x0000) {

    // エネミーの数の更新
    ++enemyDead;
    
    // 状態の更新
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * エネミーにヒットする
 */
void EnemyHit(uint8_t id, uint8_t power)
{
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if ((enemys[i].state & 0xf0) != ENEMY_STATE_NULL && enemys[i].id == id && enemys[i].life > 0x00) {
      enemys[i].damage = ENEMY_DAMAGE;
      if (enemys[i].life <= power) {
        enemys[i].life = 0x00;
      } else {
        enemys[i].life -= power;
      }
      break;
    }
  }
}

/*
 * エネミーの状態を設定する
 */
void EnemySetState(uint8_t state)
{
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if ((enemys[i].state & 0xf0) != ENEMY_STATE_NULL && enemys[i].life > 0x00) {
      enemys[i].state = state;
    }
  }
}

/*
 * エネミーの生存する数を取得する
 */
uint8_t EnemyGetLive(void)
{
  return ENEMY_TOTAL - enemyDead;
}

/*
 * 座標がエネミーの近くかどうかを判定する
 */
bool EnemyIsNear(uint8_t x, uint8_t y)
{
  bool result = false;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if ((enemys[i].state & 0xf0) != ENEMY_STATE_NULL && enemys[i].life > 0x00) {
      uint8_t dx = (enemys[i].positionX >> 8) - x;
      uint8_t dy = (enemys[i].positionY >> 8) - y;
      if (((uint8_t)-ENEMY_NEAR_SIZE < dx || dx < ENEMY_NEAR_SIZE) && ((uint8_t)-ENEMY_NEAR_SIZE < dy || dy < ENEMY_NEAR_SIZE)) {
        result = true;
        break;
      }
    }
  }
  return result;
}

/*
 * プレイヤが近くにいるかどうかを判定する
 */
static bool EnemyIsNearPlayer(struct Enemy *enemy)
{
  bool result = false;
  if ((enemy->state & 0xf0) != ENEMY_STATE_NULL && enemy->life > 0x00) {
    uint8_t px, py;
    if (PlayerGetPosition(&px, &py)) {
      uint8_t dx = (enemy->positionX >> 8) - px;
      uint8_t dy = (enemy->positionY >> 8) - py;
      if (((uint8_t)-ENEMY_NEAR_SIZE < dx || dx < ENEMY_NEAR_SIZE) && ((uint8_t)-ENEMY_NEAR_SIZE < dy || dy < ENEMY_NEAR_SIZE)) {
        result = true;
      }
    }
  }
  return result;
}

/*
 * エネミーのビューを計算する
 */
static void EnemyCalcView(void)
{
  // 描画の初期化
  for (int8_t i = 0; i < ENEMY_N; i++) {
    enemys[i].viewScale = 0xff;
    enemyDrawOrders[i] = i;
  }
  
  // プレイヤの取得
  uint8_t px, py;
  if (PlayerGetPosition(&px, &py)) {
    uint8_t pa = PlayerGetAngle();
    int16_t ps = AppGetSin(-pa);
    int16_t pc = AppGetCos(-pa);

    // エネミーの走査
    for (int8_t i = 0; i < ENEMY_N; i++) {
      if ((enemys[i].state & 0xf0) != ENEMY_STATE_NULL) {

        // 透視変換
        int16_t dx = (int16_t)(uint8_t)(enemys[i].positionX >> 8) - px;
        int16_t dy = (int16_t)(uint8_t)(enemys[i].positionY >> 8) - py;
        int16_t ex = (dx * pc - dy * ps) >> 8;
        int16_t ey = (dx * ps + dy * pc) >> 8;
        int16_t ay = ey >= 0x00 ? ey : -ey;
        if (0x00 < ex && ex < FIELD_DISTANCE_FAR && ex > ay) {
          uint8_t scale = FieldGetDistanceHeight(ex);
          if (scale > 0x00) {
            if (scale < ENEMY_SCALE_MIN) {
              scale = ENEMY_SCALE_MIN;
            } else if (scale > ENEMY_SCALE_MAX) {
              scale = ENEMY_SCALE_MAX;
            }
          }
          enemys[i].viewX = 0x0040 * ey / ex + WIDTH / 2 + (enemys[i].damage > 0x00 ? ((enemys[i].damage & 0x02) << 1) - 0x02 : 0x00);
          enemys[i].viewScale = scale;
          enemys[i].viewDistance = ex;
          enemys[i].viewPattern = pgm_read_word(enemyPatterns + ((uint8_t)((enemys[i].angle - pa) + 0x20) >> 6));
        }
      }
    }
  }

  // 描画順の設定
  for (int8_t i = 0x01; i < ENEMY_N; i++) {
    int8_t t = enemyDrawOrders[i];
    if (enemys[enemyDrawOrders[i - 0x01]].viewScale > enemys[t].viewScale) {
      int8_t j = i;
      do {
        enemyDrawOrders[j] = enemyDrawOrders[j - 1];
        --j;
      } while (j > 0 && enemys[enemyDrawOrders[j - 1]].viewScale > enemys[t].viewScale);
      enemyDrawOrders[j] = t;
    }
  }
}

/*
 * エネミーを描画する
 */
static void EnemyDraw(void)
{
  // エネミーの走査
  for (int8_t i = 0x00; i < ENEMY_N; i++) {

    // エネミーの取得
    struct Enemy *enemy = &enemys[enemyDrawOrders[i]];

    // エネミーの存在
    if ((enemy->state & 0xf0) != ENEMY_STATE_NULL) {
      EnemyDraw(enemy);
    }
  }
}
static void EnemyDraw(struct Enemy *enemy)
{
  // 描画位置の取得
  int16_t drawX = enemy->viewX - (enemy->viewScale >> 1);

  // 画面内の判定
  if (ENEMY_SCALE_MIN <= enemy->viewScale && enemy->viewScale <= ENEMY_SCALE_MAX && FIELD_VIEW_LEFT <= drawX + enemy->viewScale && drawX <= FIELD_VIEW_RIGHT) {

    // パターンの取得
    uint8_t *pattern = patternTable + enemy->viewPattern + ((enemy->animation & 0x20) >> 1);
    
    // ビデオバッファの取得
    uint8_t *videoUpper = SystemGetVideoBuffer() + 0x0180 + drawX;
    uint8_t *videoLower = videoUpper + 0x0080;

    // マスクの取得
    uint8_t maskUpper = (uint8_t)(enemy->viewMask & 0xff);
    uint8_t maskLower = (uint8_t)(enemy->viewMask >> 8);

    // 描画の開始
    uint8_t *scaleX = enemyScaleXs + ((enemy->viewScale - ENEMY_SCALE_MIN) << (4 - 1));
    uint8_t x = 0x00;
    while (x < 0x10 && drawX <= FIELD_VIEW_RIGHT) {
  
      // 同一パターンの横方向への展開
      uint8_t sx = pgm_read_byte(scaleX);
      if (sx > 0x00) {

        // パターンの取得
        uint8_t srcDrawUpper = (uint32_t)(pgm_read_byte(pattern + 0x0000) & maskUpper);
        uint8_t srcDrawLower = (uint32_t)(pgm_read_byte(pattern + 0x0080) & maskLower);
        uint8_t srcClipUpper = (uint32_t)(pgm_read_byte(pattern + 0x0100) & maskUpper);
        uint8_t srcClipLower = (uint32_t)(pgm_read_byte(pattern + 0x0180) & maskLower);
        uint32_t dstDrawUpper = 0x00000000;
        uint32_t dstDrawLower = 0x00000000;
        uint32_t dstClipUpper = 0x00000000;
        uint32_t dstClipLower = 0x00000000;
        uint8_t *scaleY = enemyScaleYs + ((enemy->viewScale - ENEMY_SCALE_MIN) << (3 - 1));
        uint8_t shift = 0x18;
        for (uint8_t y = 0x00; y < 0x08; y++) {
          uint8_t du = srcDrawUpper & 0x80;
          uint8_t cu = srcClipUpper & 0x80;
          uint32_t dl = (uint32_t)(srcDrawLower & 0x01) << 31;
          uint32_t cl = (uint32_t)(srcClipLower & 0x01) << 31;
          uint8_t sy = pgm_read_byte(scaleY);
          shift -= sy;
          while (sy > 0x00) {
            dstDrawUpper = (dstDrawUpper | du) << 1;
            dstClipUpper = (dstClipUpper | cu) << 1;
            dstDrawLower = (dstDrawLower >> 1) | dl;
            dstClipLower = (dstClipLower >> 1) | cl;
            --sy;
          }
          srcDrawUpper <<= 1;
          srcDrawLower >>= 1;
          srcClipUpper <<= 1;
          srcClipLower >>= 1;
          ++scaleY;
        }
        if (shift > 0x00) {
          dstDrawUpper <<= shift;
          dstDrawLower >>= shift;
          dstClipUpper <<= shift;
          dstClipLower >>= shift;
        }
        dstClipUpper = ~dstClipUpper;
        dstClipLower = ~dstClipLower;
      
        // パターンの描画
        while (sx > 0x00 && drawX < FIELD_VIEW_LEFT) {
          ++videoUpper;
          ++videoLower;
          ++drawX;
          --sx;
        }
        while (sx > 0x00 && drawX <= FIELD_VIEW_RIGHT) {
          if (FieldGetViewDistance(drawX) >= enemy->viewDistance) {
            *(videoUpper - 0x0000) &= (uint8_t)(dstClipUpper >> 24);
            *(videoUpper - 0x0000) |= (uint8_t)(dstDrawUpper >> 24);
            *(videoUpper - 0x0080) &= (uint8_t)(dstClipUpper >> 16);
            *(videoUpper - 0x0080) |= (uint8_t)(dstDrawUpper >> 16);
            *(videoUpper - 0x0100) &= (uint8_t)(dstClipUpper >> 8);
            *(videoUpper - 0x0100) |= (uint8_t)(dstDrawUpper >> 8);
            *(videoLower + 0x0000) &= (uint8_t)(dstClipLower >> 8);
            *(videoLower + 0x0000) |= (uint8_t)(dstDrawLower >> 8);
            *(videoLower + 0x0080) &= (uint8_t)(dstClipLower >> 16);
            *(videoLower + 0x0080) |= (uint8_t)(dstDrawLower >> 16);
            *(videoLower + 0x0100) &= (uint8_t)(dstClipLower >> 24);
            *(videoLower + 0x0100) |= (uint8_t)(dstDrawLower >> 24);
            FieldSetViewDistanceAndMap(drawX, enemy->viewDistance, enemy->id);
          }
          ++videoUpper;
          ++videoLower;
          ++drawX;
          --sx;
        }
      }
  
      // 次のパターン
      ++pattern;
      ++scaleX;
      ++x;
    }
  }
}
/*
{
  // 描画位置の取得
  int16_t drawX = enemy->viewX - (enemy->viewScale >> 1);

  // 描画の大きさの取得
  uint8_t drawWidth = enemy->viewScale;
  uint8_t drawHeight = enemy->viewScale >> 1;

  // 画面内の判定
  if (0x0010 <= drawX + drawWidth && drawX < 0x0070) {

    // パターンの取得
    uint8_t *pattern = patternTable + 0x0080;
    
    // ビデオバッファの取得
    uint8_t *videoUpper = SystemGetVideoBuffer() + 0x0180 + drawX;
    uint8_t *videoLower = videoUpper + 0x0080;

    // 横方向の展開
    uint16_t dx = 0x0000;
    uint8_t cx = 0x00;
    uint8_t mx = drawX + drawWidth > 0x0070 ? 0x70 - drawX : drawWidth;

    // 描画の開始
    while (cx < mx) {
  
      // 同一パターンの横方向への展開
      dx += drawWidth;
      if (dx >= 0x10) {

        // パターンの取得
        uint8_t srcDrawUpper = (uint32_t)pgm_read_byte(pattern + 0x0000);
        uint8_t srcDrawLower = (uint32_t)pgm_read_byte(pattern + 0x0080);
        uint8_t srcClipUpper = (uint32_t)pgm_read_byte(pattern + 0x0100);
        uint8_t srcClipLower = (uint32_t)pgm_read_byte(pattern + 0x0180);
        uint32_t dstDrawUpper = 0x00000000;
        uint32_t dstDrawLower = 0x00000000;
        uint32_t dstClipUpper = 0x00000000;
        uint32_t dstClipLower = 0x00000000;
        uint16_t dy = 0x0000;
        uint8_t cy = 0x00;
        do {
          dy += drawHeight;
          while (cy < 0x18 && dy >= 0x08) {
            dstDrawUpper = (dstDrawUpper | (srcDrawUpper & 0x80)) << 1;
            dstClipUpper = (dstClipUpper | (srcClipUpper & 0x80)) << 1;
            dstDrawLower = (dstDrawLower >> 1) | ((uint32_t)(srcDrawLower & 0x01) << 31);
            dstClipLower = (dstClipLower >> 1) | ((uint32_t)(srcClipLower & 0x01) << 31);
            dy -= 0x08;
            ++cy;
          }
          srcDrawUpper <<= 1;
          srcDrawLower >>= 1;
          srcClipUpper <<= 1;
          srcClipLower >>= 1;
        } while (cy < 0x18);
        dstClipUpper = ~dstClipUpper;
        dstClipLower = ~dstClipLower;
      
        // パターンの描画
        while (cx < mx && dx >= 0x10) {
          if (drawX >= 0x0010) {
            *(videoUpper - 0x0000) &= (uint8_t)(dstClipUpper >> 24);
            *(videoUpper - 0x0000) |= (uint8_t)(dstDrawUpper >> 24);
            *(videoUpper - 0x0080) &= (uint8_t)(dstClipUpper >> 16);
            *(videoUpper - 0x0080) |= (uint8_t)(dstDrawUpper >> 16);
            *(videoUpper - 0x0100) &= (uint8_t)(dstClipUpper >> 8);
            *(videoUpper - 0x0100) |= (uint8_t)(dstDrawUpper >> 8);
            *(videoLower + 0x0000) &= (uint8_t)(dstClipLower >> 8);
            *(videoLower + 0x0000) |= (uint8_t)(dstDrawLower >> 8);
            *(videoLower + 0x0080) &= (uint8_t)(dstClipLower >> 16);
            *(videoLower + 0x0080) |= (uint8_t)(dstDrawLower >> 16);
            *(videoLower + 0x0100) &= (uint8_t)(dstClipLower >> 24);
            *(videoLower + 0x0100) |= (uint8_t)(dstDrawLower >> 24);
          }
          ++videoUpper;
          ++videoLower;
          dx -= 0x10;
          ++cx;
          ++drawX;
        }
      }
  
      // 次のパターン
      ++pattern;
    }
  }
}
*/

