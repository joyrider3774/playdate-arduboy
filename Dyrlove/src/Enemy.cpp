/*
 * Enemy.cpp : エネミー
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// エネミー
static struct Enemy enemys[ENEMY_N];

// ジェネレータ
static struct EnemyGenerator enemyGenerator;

// パターン
static const int8_t enemyPatterns[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0xc0, 0xe0, 0xd0, 0xd0, 0xf0, // キルトラ・レウェース
  0xc2, 0xc2, 0xe2, 0xd2, 0xd2, 0xf2, // ジナール
  0xc4, 0xc4, 0xe4, 0xd4, 0xd4, 0xf4, // ゼントコム・イン
  0xc6, 0xc6, 0xe6, 0xd6, 0xd6, 0xf6, // ケル・カリア
  0xc8, 0xc8, 0xe8, 0xd8, 0xd8, 0xf8, // リガード
  0xca, 0xca, 0xea, 0xda, 0xda, 0xfa, // グラージ
  0xcc, 0xcc, 0xec, 0xdc, 0xdc, 0xfc, // クァドラン・ロー
  0xce, 0xce, 0xee, 0xde, 0xde, 0xfe, // ヌージャデル・ガー
  0x8a, 0xaa, 0xaa, 0x9a, 0xba, 0xba, // ボドルザー
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ボドルザー(0)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ボドルザー(1)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ボドルザー(2)
};

// 矩形
static const int8_t enemyRects[] PROGMEM = {
   0x00,  0x00,  0x00,  0x00, 
  -0x04,  0x03, -0x03,  0x03, // キルトラ・レウェース
  -0x04,  0x03, -0x03,  0x03, // ジナール
  -0x04,  0x03, -0x03,  0x03, // ゼントコム・イン
  -0x04,  0x03, -0x03,  0x02, // ケル・カリア
  -0x02,  0x04, -0x06,  0x00, // リガード
  -0x04,  0x06, -0x05,  0x02, // グラージ
  -0x04,  0x03, -0x07,  0x06, // クァドラン・ロー
  -0x06,  0x02, -0x07,  0x06, // ヌージャデル・ガー
  -0x08,  0x07, -0x08,  0x07, // ボドルザー
  -0x10,  0x07, -0x08,  0x07, // ボドルザー(0)
  -0x20,  0x0f, -0x08,  0x07, // ボドルザー(1)
  -0x20,  0x17, -0x08,  0x07, // ボドルザー(2)
};

// 発射位置
static const int8_t enemyFirePositions[] PROGMEM = {
   0x00,  0x00, 
   0x00,  0x00, // キルトラ・レウェース
   0x00,  0x00, // ジナール
   0x00,  0x00, // ゼントコム・イン
   0x00,  0x00, // ケル・カリア
   0x04, -0x02, // リガード
   0x06,  0x02, // グラージ
   0x03, -0x02, // クァドラン・ロー
   0x05, -0x05, // ヌージャデル・ガー
   0x07,  0x03, // ボドルザー
   0x00,  0x00, // ボドルザー(0)
   0x00,  0x00, // ボドルザー(1)
   0x00,  0x00, // ボドルザー(2)
};

// 耐久力
static const uint8_t enemyLifes[] PROGMEM = {
  0x00, 
  0x01, // キルトラ・レウェース
  0x01, // ジナール
  0x01, // ゼントコム・イン
  0x01, // ケル・カリア
  0x04, // リガード
  0x04, // グラージ
  0x08, // クァドラン・ロー
  0x08, // ヌージャデル・ガー
  0xc8, // ボドルザー
  0xff, // ボドルザー(0)
  0xff, // ボドルザー(1)
  0xff, // ボドルザー(2)
};

// スコア
static const uint8_t enemyScores[] PROGMEM = {
  0x00, 
  0x01, // キルトラ・レウェース
  0x01, // ジナール
  0x01, // ゼントコム・イン
  0x01, // ケル・カリア
  0x04, // リガード
  0x04, // グラージ
  0x08, // クァドラン・ロー
  0x08, // ヌージャデル・ガー
  0x64, // ボドルザー
  0x00, // ボドルザー(0)
  0x00, // ボドルザー(1)
  0x00, // ボドルザー(2)
};

// 位置（ボドルザー）
static const int8_t enemyBoddoleZerPositions[] PROGMEM = {
   0x32, -0x08, 
   0x30, -0x18, 
   0x20,  0x08, 
   0x20,  0x18, 
};

// 三角関数
static const uint8_t enemyAtanAngles[] PROGMEM = {
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0x20, 0x12, 0x0d, 0x09, 0x08, 0x06, 0x05, 
  0x40, 0x2d, 0x20, 0x17, 0x12, 0x0f, 0x0d, 0x0b, 
  0x40, 0x32, 0x28, 0x20, 0x1a, 0x16, 0x12, 0x10, 
  0x40, 0x36, 0x2d, 0x25, 0x20, 0x1b, 0x17, 0x15, 
  0x40, 0x37, 0x30, 0x29, 0x24, 0x20, 0x1c, 0x19, 
  0x40, 0x39, 0x32, 0x2d, 0x28, 0x23, 0x20, 0x1c, 
  0x40, 0x3a, 0x34, 0x2f, 0x2a, 0x26, 0x23, 0x20, 
};
static const uint8_t enemyAtanOffsets[] PROGMEM = {
  0x00, 0x80, 0x00, 0x80, 
};


/*
 * 内部関数の宣言
 */
static void EnemyGenerateNext(void);
static int8_t EnemyCountEmpty(void);
static struct Enemy *EnemyGetEmpty(void);
static bool EnemyFireAngle(struct Enemy *enemy, uint8_t angle);
static bool EnemyFirePlayer(struct Enemy *enemy, int8_t focus);
static uint8_t EnemyCalcAngle(int8_t y, int8_t x);
static bool EnemyOutOfView(struct Enemy *enemy);
static void EnemyGenerateQuiltraLeves(void);
static void EnemyUpdateQuiltraLeves(struct Enemy *enemy);
static void EnemyGenerateGnerl(void);
static void EnemyUpdateGnerl(struct Enemy *enemy);
static void EnemyGenerateZentcom1(void);
static void EnemyUpdateZentcom1(struct Enemy *enemy);
static void EnemyGenerateQuelQuallie(void);
static void EnemyUpdateQuelQuallie(struct Enemy *enemy);
static void EnemyGenerateRegult(void);
static void EnemyUpdateRegult(struct Enemy *enemy);
static void EnemyGenerateGlaug(void);
static void EnemyUpdateGlaug(struct Enemy *enemy);
static void EnemyGenerateQueadluunRau(void);
static void EnemyUpdateQueadluunRau(struct Enemy *enemy);
static void EnemyGenerateNousjadeulGer(void);
static void EnemyUpdateNousjadeulGer(struct Enemy *enemy);
static void EnemyGenerateBoddoleZer(void);
static void EnemyUpdateBoddoleZer(struct Enemy *enemy);
static void EnemyUpdateBoddoleZerNull(struct Enemy *enemy);
static void EnemyGenerateBomb(void);
static void EnemyUpdateBomb(struct Enemy *enemy);

/*
 * エネミーを初期化する
 */
void EnemyInitialize(void)
{
  // エネミーの取得
  struct Enemy *enemy = enemys;
  
  // エネミーの初期化
  for (int8_t i = 0x00; i < ENEMY_N; i++) {

    // 種類の設定
    enemy->type = ENEMY_TYPE_NULL;

    // 次へ
    ++enemy;
  }

  // ジェネレータの初期化
  {
    // ステージの設定
    enemyGenerator.stage = ENEMY_STAGE_NULL;
  }
}

/*
 * エネミーを更新する
 */
void EnemyUpdate(void)
{
  // ジェネレータの更新
  {
    // プレイヤの存在
    if (PlayerIsLive()) {

      // ザコステージ
      if (enemyGenerator.stage == ENEMY_STAGE_ZAKO) {
        
        // フレームの更新
        if (enemyGenerator.frame > 0x00) {
          --enemyGenerator.frame;
        }
  
        // フレームの更新の完了あるいは敵が少数
        if (enemyGenerator.frame == 0x00 || (enemyGenerator.order > 0x08 && EnemyCountEmpty() > ENEMY_N - 0x03)) {

          // 敵の種類の設定
          if (enemyGenerator.type == ENEMY_TYPE_NULL) {
            uint8_t order = enemyGenerator.order & 0x07;
            if (enemyGenerator.order < 0x04) {
              enemyGenerator.type = order + ENEMY_TYPE_QUILTRA_LEVES;
            } else if (order == 0x03) {
              enemyGenerator.type = (SystemGetRandom() & 0x07) + ENEMY_TYPE_QUILTRA_LEVES;
            } else if (order < 0x07) {
              enemyGenerator.type = (SystemGetRandom() & 0x03) + ENEMY_TYPE_QUILTRA_LEVES;
            } else {
              enemyGenerator.type = (SystemGetRandom() & 0x03) + ENEMY_TYPE_REGULT;
            }
            // enemyGenerator.type = ENEMY_TYPE_QUILTRA_LEVES;
            // enemyGenerator.type = ENEMY_TYPE_GNERL;
            // enemyGenerator.type = ENEMY_TYPE_ZENTCOM_1;
            // enemyGenerator.type = ENEMY_TYPE_QUEL_QUALLIE;
            // enemyGenerator.type = ENEMY_TYPE_REGULT;
            // enemyGenerator.type = ENEMY_TYPE_GLAUG;
            // enemyGenerator.type = ENEMY_TYPE_QUEADLUUN_RAU;
            // enemyGenerator.type = ENEMY_TYPE_NOUSJADEUL_GER;
            enemyGenerator.direction = order == 0x00 ? ENEMY_DIRECTION_LEFT : (SystemGetRandom() & 0x02) - 0x01;
          }
        }
        
      // ボスステージ
      } else if (enemyGenerator.stage == ENEMY_STAGE_BOSS) {
        if (enemyGenerator.order == 0x00) {
          enemyGenerator.type = ENEMY_TYPE_BODDOLE_ZER;
          enemyGenerator.direction = ENEMY_DIRECTION_LEFT;
        }

      // フィニッシュステージ
      } else if (enemyGenerator.stage == ENEMY_STAGE_FINISH) {
        if (enemyGenerator.frame > 0x00) {
          --enemyGenerator.frame;
        } else {
          enemyGenerator.type = ENEMY_TYPE_BOMB;
          enemyGenerator.direction = (SystemGetRandom() & 0x02) - 0x01;
        }
      }
        
      // 種類別の処理
      switch (enemyGenerator.type) {
    
      // キルトラ・レウェース
      case ENEMY_TYPE_QUILTRA_LEVES:
        EnemyGenerateQuiltraLeves();
        break;
        
      // ジナール
      case ENEMY_TYPE_GNERL:
        EnemyGenerateGnerl();
        break;
  
      // ゼントコム・イン
      case ENEMY_TYPE_ZENTCOM_1:
        EnemyGenerateZentcom1();
        break;
  
      // ケル・カリア
      case ENEMY_TYPE_QUEL_QUALLIE:
        EnemyGenerateQuelQuallie();
        break;
        
      // リガード
      case ENEMY_TYPE_REGULT:
        EnemyGenerateRegult();
        break;
        
      // グラージ
      case ENEMY_TYPE_GLAUG:
        EnemyGenerateGlaug();
        break;
      
      // クァドラン・ロー
      case ENEMY_TYPE_QUEADLUUN_RAU:
        EnemyGenerateQueadluunRau();
        break;
        
      // ヌージャデル・ガー
      case ENEMY_TYPE_NOUSJADEUL_GER:
        EnemyGenerateNousjadeulGer();
        break;

      // ボドルザー
      case ENEMY_TYPE_BODDOLE_ZER:
        EnemyGenerateBoddoleZer();
        break;

      // 爆発
      case ENEMY_TYPE_BOMB:
        EnemyGenerateBomb();
        break;
  
      // その他
      default:
        break;
      }
    }
  }
  
  // エネミーの更新
  {
    struct Enemy *enemy = enemys;
    for (int8_t i = 0x00; i < ENEMY_N; i++) {
  
      // 種類別の処理
      switch (enemy->type) {
  
      // キルトラ・レウェース
      case ENEMY_TYPE_QUILTRA_LEVES:
        EnemyUpdateQuiltraLeves(enemy);
        break;
        
      // ジナール
      case ENEMY_TYPE_GNERL:
        EnemyUpdateGnerl(enemy);
        break;
  
      // ゼントコム・イン
      case ENEMY_TYPE_ZENTCOM_1:
        EnemyUpdateZentcom1(enemy);
        break;
  
      // ケル・カリア
      case ENEMY_TYPE_QUEL_QUALLIE:
        EnemyUpdateQuelQuallie(enemy);
        break;
        
      // リガード
      case ENEMY_TYPE_REGULT:
        EnemyUpdateRegult(enemy);
        break;
        
      // グラージ
      case ENEMY_TYPE_GLAUG:
        EnemyUpdateGlaug(enemy);
        break;
      
      // クァドラン・ロー
      case ENEMY_TYPE_QUEADLUUN_RAU:
        EnemyUpdateQueadluunRau(enemy);
        break;
        
      // ヌージャデル・ガー
      case ENEMY_TYPE_NOUSJADEUL_GER:
        EnemyUpdateNousjadeulGer(enemy);
        break;

      // ボドルザー
      case ENEMY_TYPE_BODDOLE_ZER:
        EnemyUpdateBoddoleZer(enemy);
        break;

      // ボドルザー(0)(1)(2)
      case ENEMY_TYPE_BODDOLE_ZER_NULL_0:
      case ENEMY_TYPE_BODDOLE_ZER_NULL_1:
      case ENEMY_TYPE_BODDOLE_ZER_NULL_2:
        EnemyUpdateBoddoleZerNull(enemy);
        break;
        
      // 爆発
      case ENEMY_TYPE_BOMB:
        EnemyUpdateBomb(enemy);
        break;
        
      // その他
      default:
        break;
      }

      // ダメージの更新
      if (enemy->damage > 0x00) {
        --enemy->damage;
      }
  
      // 次へ
      ++enemy;
    }
  }
}

/*
 * エネミーを描画する
 */
void EnemyRender(void)
{
  // エネミーの取得
  struct Enemy *enemy = enemys;
  
  // エネミーの描画
  for (int8_t i = 0x00; i < ENEMY_N; i++) {

    // エネミーの存在
    if (enemy->type != ENEMY_TYPE_NULL) {

      // 位置の取得
      int8_t x = (int8_t)(enemy->positionX >> 8) + GAME_VIEW_OX;
      int8_t y = (int8_t)(enemy->positionY >> 8) + GAME_VIEW_OY;

      // 敵機の描画
      if (enemy->type < ENEMY_TYPE_BOMB) {
        int8_t *p = enemyPatterns + (enemy->type * ENEMY_PATTERN_N * ENEMY_PATTERN_SIZE);
        int8_t draw = enemy->damage == 0x00 ? ENEMY_PATTERN_NORMAL : ENEMY_PATTERN_DAMAGE;
        x -= 0x08;
        y -= 0x08;
        for (int8_t i = 0x00; i < ENEMY_PATTERN_N; i++) {
          AppClipPattern(
            pgm_read_byte(p + draw), 
            pgm_read_byte(p + ENEMY_PATTERN_CLIP), 
            x, 
            y, 
            0x10, 
            enemy->direction
          );
          p += ENEMY_PATTERN_SIZE;
          y += 0x08;
        }

      // 爆発の描画
      } else {
        AppDrawPattern(
          0x0008 + (enemy->params[ENEMY_PARAM_0] >> 2), 
          x - 0x04, 
          y - 0x04, 
          0x08, 
          enemy->direction
        );
      }
    }

    // 次へ
    ++enemy;
  }
}

/*
 * エネミーのステージを設定する
 */
void EnemySetStage(int8_t stage)
{
  enemyGenerator.stage = stage;
  enemyGenerator.order = 0x00;
  enemyGenerator.frame = 0x0000;
}

/*
 * エネミーの存在を取得する
 */
bool EnemyIsLive(void)
{
  bool result = false;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (EnemyIsLive(i)) {
      result = true;
      break;
    }
  }
  return result;
}
bool EnemyIsLive(int8_t index)
{
  return enemys[index].type != ENEMY_TYPE_NULL && enemys[index].life > 0x00 ? true : false;
}

/*
 * エネミーの矩形を取得する
 */
bool EnemyGetRect(int8_t index, int8_t *rect)
{
  bool result = EnemyIsLive(index);
  if (result) {
    int8_t *r = enemyRects + (enemys[index].type * GAME_RECT_SIZE);
    int8_t x = (int8_t)(enemys[index].positionX >> 8);
    int8_t y = (int8_t)(enemys[index].positionY >> 8);
    if (enemys[index].direction == ENEMY_DIRECTION_RIGHT) {
      rect[GAME_RECT_LEFT] = x + pgm_read_byte(r + GAME_RECT_LEFT);
      rect[GAME_RECT_RIGHT] = x + pgm_read_byte(r + GAME_RECT_RIGHT);
    } else {
      rect[GAME_RECT_LEFT] = x - pgm_read_byte(r + GAME_RECT_RIGHT);
      rect[GAME_RECT_RIGHT] = x - pgm_read_byte(r + GAME_RECT_LEFT);
    }
    rect[GAME_RECT_TOP] = y + pgm_read_byte(r + GAME_RECT_TOP);
    rect[GAME_RECT_BOTTOM] = y + pgm_read_byte(r + GAME_RECT_BOTTOM);
  }
  return result;
}

/*
 * エネミーを削除する
 */
void EnemyKill(void)
{
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    enemys[i].type = ENEMY_TYPE_NULL;
  }
}

/*
 * エネミーがダメージを受ける
 */
uint8_t EnemyDamage(int8_t index)
{
  uint8_t score = 0x00;
  if (EnemyIsLive(index)) {
    if (--enemys[index].life == 0x00) {
      score = pgm_read_byte(enemyScores + enemys[index].type);
      enemys[index].type = ENEMY_TYPE_BOMB;
      enemys[index].state = ENEMY_STATE_NULL;
    } else {
      enemys[index].damage = 0x02;
    }
  }
  return score;
}

/*
 * エネミーの耐久力を取得する
 */
uint8_t EnemyGetLife(int8_t type)
{
  uint8_t life = 0x00;
  for (int8_t i = 0; i < ENEMY_N; i++) {
    if (enemys[i].type == type) {
      life = enemys[i].life;
      break;
    }
  }
  return life;
}

/*
 * 次のジェネレータを準備する
 */
static void EnemyGenerateNext(void)
{
  ++enemyGenerator.order;
  enemyGenerator.frame = enemyGenerator.stage == ENEMY_STAGE_ZAKO ? (uint16_t)(SystemGetRandom() & 0x7f) + 0x0060 : 0x0008;
  enemyGenerator.type = ENEMY_TYPE_NULL;
}

/*
 * 空のエネミーの数を取得する
 */
static int8_t EnemyCountEmpty(void)
{
  struct Enemy *enemy = enemys;
  int8_t count = 0x00;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemy->type == ENEMY_TYPE_NULL) {
      ++count;
    }
    ++enemy;
  }
  return count;
}

/*
 * 空のエネミーを取得する
 */
static struct Enemy *EnemyGetEmpty(void)
{
  struct Enemy *enemy = NULL;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemys[i].type == ENEMY_TYPE_NULL) {
      enemy = &enemys[i];
      break;
    }
  }
  return enemy;
}

/*
 * 指定した角度へ弾を撃つ
 */
static bool EnemyFireAngle(struct Enemy *enemy, uint8_t angle)
{
  int8_t *p = enemyFirePositions + enemy->type * ENEMY_FIRE_POSITION_SIZE;
  int8_t x = (int8_t)(enemy->positionX >> 8) + pgm_read_byte(p + ENEMY_FIRE_POSITION_X) * enemy->direction;
  int8_t y = (int8_t)(enemy->positionY >> 8) + pgm_read_byte(p + ENEMY_FIRE_POSITION_Y);
  return BulletEntry(x, y, angle);
}

/*
 * プレイヤへ弾を撃つ
 */
static bool EnemyFirePlayer(struct Enemy *enemy, int8_t focus)
{
  bool result = false;
  int8_t x, y;
  if (PlayerGetPosition(&x, &y)) {
    uint8_t angle = EnemyCalcAngle(y - (int8_t)(enemy->positionY >> 8), x - (int8_t)(enemy->positionX >> 8));
    result = EnemyFireAngle(enemy, angle + focus);
  }
  return result;
}

/*
 * 角度を計算する
 */
static uint8_t EnemyCalcAngle(int8_t y, int8_t x)
{
  // 正数に補正
  int8_t offset = 0x00;
  if (x < 0x00) {
    x = -x;
    if (x < 0x00) {
      x = 0x7f;
    }
    offset |= 0x01;
  }
  if (y < 0x00) {
    y = -y;
    if (y < 0x00) {
      y = 0x7f;
    }
    offset |= 0x02;
  }

  // 参照の取得
  int8_t n = (y >= x) ? y : x;
  while (n >= 0x08) {
    n >>= 1;
    x >>= 1;
    y >>= 1;
  }

  // 角度の取得
  uint8_t angle = pgm_read_byte(enemyAtanAngles + (y << 3) + x);
  if (((offset + 0x01) & 0x02) != 0x00) {
    angle = -angle; 
  }
  angle += pgm_read_byte(enemyAtanOffsets + offset);

  // 終了
  return angle;
}

/*
 * エネミーが画面外に位置する
 */
static bool EnemyOutOfView(struct Enemy *enemy)
{
  bool result = false;
  int8_t x = (int8_t)(enemy->positionX >> 8);
  int8_t y = (int8_t)(enemy->positionY >> 8);
  if (x < (GAME_VIEW_LEFT - 0x08) || (GAME_VIEW_RIGHT + 0x08) < x || y < (GAME_VIEW_TOP - 0x08) || (GAME_VIEW_BOTTOM + 0x08) < y) {
    enemy->type = ENEMY_TYPE_NULL;
    result = true;
  }
  return result;
}

/*
 * キルトラ・レウェースを生成する
 */
static void EnemyGenerateQuiltraLeves(void)
{
  // エネミーの生成数の取得
  static const int8_t n = 0x03;
  
  // エネミーの生成
  if (EnemyCountEmpty() >= n) {

    // 位置の取得
    int16_t x = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x04) << 8) : ((GAME_VIEW_LEFT - 0x04) << 8);

    // エネミーの設定
    for (int8_t i = 0x00; i < n; i++) {
      struct Enemy *enemy = EnemyGetEmpty();
      if (enemy != NULL) {
        enemy->type = ENEMY_TYPE_QUILTRA_LEVES;
        enemy->state = ENEMY_STATE_NULL;
        enemy->positionX = x;
        enemy->positionY = (int16_t)((SystemGetRandom() & 0x07) * 0x07 + 0x03 + (GAME_VIEW_TOP + 0x08)) << 8;
        enemy->direction = enemyGenerator.direction;
        enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_QUILTRA_LEVES);
        enemy->damage = 0x00;
        enemy->frame = (i * 0x20) + 0x01;
      }
    }

    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * キルトラ・レウェースを更新する
 */
static void EnemyUpdateQuiltraLeves(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 初期化
  case 0x00:
    {
      // フレームの更新
      if (--enemy->frame == 0x00) {
        
        // 発射の設定
        enemy->fire = (SystemGetRandom() & 0x7f) + 0x20;
    
        // 状態の更新
        ++enemy->state;
      }
    }
    break;

  // 0x01: 前進
  case 0x01:
    {
      // 移動
      enemy->positionX += (int16_t)enemy->direction << 7;
    
      // 発射
      if (enemy->fire > 0x00) {
        if (--enemy->fire == 0x00) {
          EnemyFirePlayer(enemy, 0x00);
        }
      }
      
      // 移動の完了
      EnemyOutOfView(enemy);
    }
    break;

  // その他
  default:
    break;
  }    
}

/*
 * ジナールを生成する
 */
static void EnemyGenerateGnerl(void)
{
  // エネミーの生成数の取得
  static const int8_t n = 0x03;
  
  // エネミーの生成
  if (EnemyCountEmpty() >= n) {

    // 位置の取得
    int16_t x = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8) : ((GAME_VIEW_LEFT - 0x08) << 8);
    int16_t y = (int16_t)((SystemGetRandom() & 0x07) * 0x07 + 0x03 + (GAME_VIEW_TOP + 0x08)) << 8;
        
    // エネミーの設定
    for (int8_t i = 0x00; i < n; i++) {
      struct Enemy *enemy = EnemyGetEmpty();
      if (enemy != NULL) {
        enemy->type = ENEMY_TYPE_GNERL;
        enemy->state = ENEMY_STATE_NULL;
        enemy->positionX = x;
        enemy->positionY = y;
        enemy->direction = enemyGenerator.direction;
        enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_GNERL);
        enemy->damage = 0x00;
        enemy->frame = (i * 0x10) + 0x01;
      }
    }

    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * ジナールを更新する
 */
static void EnemyUpdateGnerl(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 初期化
  case 0x00:
    {
      // フレームの更新
      if (--enemy->frame == 0x00) {

        // 発射の設定
        enemy->fire = (SystemGetRandom() & 0x3f) + 0x20;
    
        // 移動の設定
        enemy->params[ENEMY_PARAM_0] = 0x00;
    
        // 状態の更新
        ++enemy->state;
      }
    }
    break;

  // 0x01: 前進
  case 0x01:
    {
      // 移動
      if (enemy->params[ENEMY_PARAM_0] == 0x00) {
        int8_t ex = (int8_t)(enemy->positionX >> 8);
        int8_t ey = (int8_t)(enemy->positionY >> 8);
        if ((GAME_VIEW_LEFT + 0x04) <= ex && ex <= (GAME_VIEW_RIGHT - 0x03)) {
          int8_t px, py;
          if (PlayerGetPosition(&px, &py)) {
            int8_t dx = (px > ex) ? (px - ex) : (ex - px);
            int8_t dy = (py > ey) ? (py - ey) : (ey - py);
            if (dx > 0x10 && dy > (dx - 0x10) / 2) {
              enemy->params[ENEMY_PARAM_0] = py > ey > 0x00 ? 0x01 : -0x01;
            }
          }
        }
      }
      enemy->positionX += 0x00c0 * enemy->direction;
      enemy->positionY += (int16_t)enemy->params[ENEMY_PARAM_0] << 6;
  
      // 発射
      if (enemy->fire > 0x00) {
        if (--enemy->fire == 0x00) {
          EnemyFirePlayer(enemy, 0x00);
        }
      }
      
      // 移動の完了
      EnemyOutOfView(enemy);
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * ゼントコム・インを生成する
 */
static void EnemyGenerateZentcom1(void)
{
  // エネミーの生成数の取得
  static const int8_t n = 0x03;
  
  // エネミーの生成
  if (EnemyCountEmpty() >= n) {

    // 位置の取得
    int16_t x = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8) : ((GAME_VIEW_LEFT - 0x08) << 8);
    int16_t y = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;

    // エネミーの設定
    for (int8_t i = 0x00; i < n; i++) {
      struct Enemy *enemy = EnemyGetEmpty();
      if (enemy != NULL) {
        enemy->type = ENEMY_TYPE_ZENTCOM_1;
        enemy->state = ENEMY_STATE_NULL;
        enemy->positionX = x;
        enemy->positionY = y;
        enemy->direction = enemyGenerator.direction;
        enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_ZENTCOM_1);
        enemy->damage = 0x00;
        enemy->frame = (i * 0x10) + 0x01;
      }
    }

    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * ゼントコム・インを更新する
 */
static void EnemyUpdateZentcom1(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {
    
  // 0x00: 初期化
  case 0x00:
    {
      // フレームの更新
      if (--enemy->frame == 0x00) {
    
        // 発射の設定
        enemy->fire = (SystemGetRandom() & 0x7f) + 0x20;
    
        // 移動の設定
        enemy->params[ENEMY_PARAM_0] = 0x18;
        enemy->params[ENEMY_PARAM_1] = 0x02;
    
        // 状態の更新
        ++enemy->state;
      }
    }
    break;

  // 0x01: 前進
  case 0x01:
    {
      // 移動
      enemy->params[ENEMY_PARAM_0] += enemy->params[ENEMY_PARAM_1];
      if (enemy->params[ENEMY_PARAM_0] < -0x18 || 0x18 < enemy->params[ENEMY_PARAM_0]) {
        enemy->params[ENEMY_PARAM_1] = -enemy->params[ENEMY_PARAM_1];
      }
      enemy->positionX += (int16_t)enemy->direction << 7;
      enemy->positionY += (enemy->params[ENEMY_PARAM_0] >= 0x00) ? (int16_t)enemy->params[ENEMY_PARAM_0] << 4 : (-((int16_t)-enemy->params[ENEMY_PARAM_0] << 4));
  
      // 発射
      if (enemy->fire > 0x00) {
        if (--enemy->fire == 0x00) {
          EnemyFirePlayer(enemy, 0x00);
        }
      }
      
      // 移動の完了
      EnemyOutOfView(enemy);
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * ケル・カリアを生成する
 */
static void EnemyGenerateQuelQuallie(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_QUEL_QUALLIE;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8) : ((GAME_VIEW_LEFT - 0x08) << 8);
    enemy->positionY = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_QUEL_QUALLIE);
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * ケル・カリアを更新する
 */
static void EnemyUpdateQuelQuallie(struct Enemy *enemy)
{
  // 初期化
  if (enemy->state == ENEMY_STATE_NULL) {

  }
  
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 初期化
  case 0x00:
    {
      // 移動の設定
      enemy->frame = (SystemGetRandom() & 0x7f) + 0x40;
  
      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x01:  発射前の移動
  case 0x01:
    {
      // 移動
      enemy->positionX += (int16_t)enemy->direction << 7;

      // 移動の完了
      if (--enemy->frame == 0x00) {

        // 発射の設定
        int8_t x, y;
        if (PlayerGetPosition(&x, &y)) {
          uint8_t angle = EnemyCalcAngle(y - (int8_t)(enemy->positionY >> 8), x - (int8_t)(enemy->positionX >> 8));
          if (enemy->direction == ENEMY_DIRECTION_LEFT) {
            enemy->params[ENEMY_PARAM_0] = angle - 0x40;
            enemy->params[ENEMY_PARAM_1] = 0x10;
          } else {
            enemy->params[ENEMY_PARAM_0] = angle + 0x40;
            enemy->params[ENEMY_PARAM_1] = -0x10;
          }
        } else {
          enemy->params[ENEMY_PARAM_0] = 0x80;
          enemy->params[ENEMY_PARAM_1] = 0x10;
        }
        enemy->params[ENEMY_PARAM_2] = 0x00;
        enemy->params[ENEMY_PARAM_3] = 0x00;
   
        // 状態の更新
        ++enemy->state;
      }
    }
    break;

  // 0x02: 回転しながら発射
  case 0x02:
    {
      // 発射
      if (enemy->params[ENEMY_PARAM_3] > 0x00) {
        --enemy->params[ENEMY_PARAM_3];
      } else {
        EnemyFireAngle(enemy, enemy->params[ENEMY_PARAM_0]);
        enemy->params[ENEMY_PARAM_0] += enemy->params[ENEMY_PARAM_1];
        enemy->params[ENEMY_PARAM_3] = 0x04;

        // 発射の完了
        if (++enemy->params[ENEMY_PARAM_2] >= 0x10) {
   
          // 状態の更新
          ++enemy->state;
        }
      }
    }
    break;

  // 0x03: 発射後の移動
  case 0x03:
    {
      // 移動
      enemy->positionX += (int16_t)enemy->direction << 7;

      // 移動の完了
      EnemyOutOfView(enemy);
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * リガードを生成する
 */
static void EnemyGenerateRegult(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_REGULT;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8): ((GAME_VIEW_LEFT - 0x08) << 8);
    enemy->positionY = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_REGULT);
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * リガードを更新する
 */
static void EnemyUpdateRegult(struct Enemy *enemy)
{
  // 初期化
  if (enemy->state == ENEMY_STATE_NULL) {

    // 発射の設定
    enemy->fire = (SystemGetRandom() & 0x1f) + 0x10;

    // 移動の設定
    enemy->params[ENEMY_PARAM_0] = 0x00;

    // 状態の更新
    ++enemy->state;
  }

  // 移動
  {
    int8_t ex = (int8_t)(enemy->positionX >> 8);
    int8_t ey = (int8_t)(enemy->positionY >> 8);
    int8_t px, py;
    if (PlayerGetPosition(&px, &py)) {
      int8_t dx = px - ex;
      if (-0x40 < dx && dx < 0x40) {
        int8_t dy = py - ey;
        if (dy < -0x04) {
          if (enemy->params[ENEMY_PARAM_0] > -0x20) {
            --enemy->params[ENEMY_PARAM_0];
          }
        } else if (dy > 0x04) {
          if (enemy->params[ENEMY_PARAM_0] < 0x20) {
            ++enemy->params[ENEMY_PARAM_0];
          }
        }
      }
    }
    enemy->positionX += enemy->direction << 7;
    enemy->positionY += (int16_t)enemy->params[ENEMY_PARAM_0] << 2;
  }

  // 発射
  if (enemy->fire > 0x00) {
    --enemy->fire;
  } else {
    int8_t px, py;
    if (PlayerGetPosition(&px, &py)) {
      int8_t dx = px - (int8_t)(enemy->positionX >> 8);
      if (enemy->direction * dx > 0x00) {
        if (EnemyFirePlayer(enemy, 0x00)) {
          enemy->fire = (SystemGetRandom() & 0x1f) + 0x10;
        }
      }
    }
  }
  
  // 移動の完了
  EnemyOutOfView(enemy);
}

/*
 * グラージを生成する
 */
static void EnemyGenerateGlaug(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_GLAUG;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8): ((GAME_VIEW_LEFT - 0x08) << 8);
    enemy->positionY = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_GLAUG);
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * グラージを更新する
 */
static void EnemyUpdateGlaug(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 前進の設定
  case 0x00:
    {
      // 発射の設定
      enemy->fire = (SystemGetRandom() & 0x01) + 0x02;

      // 移動の設定
      enemy->params[ENEMY_PARAM_0] = 0x18;
      enemy->params[ENEMY_PARAM_1] = 0x01;
      enemy->frame = (SystemGetRandom() & 0x3f) + 0x30;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x01: 前進の更新
  case 0x01:
    {
      // 移動
      enemy->params[ENEMY_PARAM_0] += enemy->params[ENEMY_PARAM_1];
      if (enemy->params[ENEMY_PARAM_0] < -0x18 || 0x18 < enemy->params[ENEMY_PARAM_0]) {
        enemy->params[ENEMY_PARAM_1] = -enemy->params[ENEMY_PARAM_1];
      }
      enemy->positionX += (int16_t)enemy->direction << 7;
      enemy->positionY += (enemy->params[ENEMY_PARAM_0] >= 0x00) ? (int16_t)enemy->params[ENEMY_PARAM_0] << 3 : (-((int16_t)-enemy->params[ENEMY_PARAM_0] << 3));

      // 移動の完了
      if (!EnemyOutOfView(enemy)) {
        int8_t px, py;
        if (PlayerGetPosition(&px, &py)) {
          int8_t dx = px - (int8_t)(enemy->positionX >> 8);
          if (enemy->direction * dx > 0x00) {
            if (--enemy->frame == 0x00) {
              ++enemy->state;
            }
          }
        }
      }
    }
    break;

  // 0x02: 発射の設定
  case 0x02:
    {
      // 発射の設定
      {
        int8_t px, py;
        enemy->params[ENEMY_PARAM_0] = 0x04;
        enemy->params[ENEMY_PARAM_1] = 0x01;
        enemy->params[ENEMY_PARAM_2] = (enemy->direction - 0x01) * 0x40;
      }

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x03: 発射の更新
  case 0x03:
    {
      // 発射
      if (--enemy->params[ENEMY_PARAM_1] == 0x00) {
        int8_t focus = (int8_t)((SystemGetRandom() & 0x0f) - 0x08);
        EnemyFireAngle(enemy, enemy->params[ENEMY_PARAM_2] + focus);
        enemy->params[ENEMY_PARAM_1] = 0x0c;
        if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
          if (--enemy->fire > 0x00) {
            ++enemy->state;
          } else {
            enemy->state = 0x00;
          }
        }
      }
    }
    break;

  // 0x04: 上下の設定
  case 0x04:
    {
      // 移動の設定
      {
        int8_t px, py;
        if (PlayerGetPosition(&px, &py)) {
          enemy->params[ENEMY_PARAM_0] = py < (int8_t)(enemy->positionY >> 8) ? -0x01 : 0x01;
        } else {
          enemy->params[ENEMY_PARAM_0] = 0x01;
        }
        enemy->frame = (SystemGetRandom() & 0x1f) + 0x10;
      }

      // 状態の更新
      ++enemy->state;
    }
    // break;

  // 0x05: 上下の更新
  case 0x05:
    {
      // 移動
      enemy->positionY += (int16_t)enemy->params[ENEMY_PARAM_0] << 7;

      // 移動の完了
      {
        int8_t y = (int8_t)(enemy->positionY >> 8);
        if (--enemy->frame == 0x00 || y <= GAME_VIEW_TOP + GAME_STATUS_HEIGHT || GAME_VIEW_BOTTOM <= y) {
          enemy->state = 0x02;
        }
      }
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * クァドラン・ローを生成する
 */
static void EnemyGenerateQueadluunRau(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_QUEADLUUN_RAU;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8): ((GAME_VIEW_LEFT - 0x08) << 8);
    enemy->positionY = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_QUEADLUUN_RAU);
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * クァドラン・ローを更新する
 */
static void EnemyUpdateQueadluunRau(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 前進の設定
  case 0x00:
    {
      // 移動の設定
      enemy->params[ENEMY_PARAM_0] = 0x00;
      enemy->frame = (SystemGetRandom() & 0x3f) + 0x20;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x01: 前進の更新
  case 0x01:
    {
      // 移動
      {
        int8_t ex = (int8_t)(enemy->positionX >> 8);
        int8_t ey = (int8_t)(enemy->positionY >> 8);
        int8_t px, py;
        if (PlayerGetPosition(&px, &py)) {
          int8_t dx = px - ex;
          if (-0x40 < dx && dx < 0x40) {
            int8_t dy = py - ey;
            if (dy < -0x04) {
              if (enemy->params[ENEMY_PARAM_0] > -0x20) {
                --enemy->params[ENEMY_PARAM_0];
              }
            } else if (dy > 0x04) {
              if (enemy->params[ENEMY_PARAM_0] < 0x20) {
                ++enemy->params[ENEMY_PARAM_0];
              }
            }
          }
        }
        enemy->positionX += enemy->direction << 7;
        enemy->positionY += (int16_t)enemy->params[ENEMY_PARAM_0] << 2;
      }

      // 移動の完了
      if (enemy->frame > 0x00) {
        --enemy->frame;
      }
      if (!EnemyOutOfView(enemy) && enemy->frame == 0x00) {
        int8_t px, py;
        if (PlayerGetPosition(&px, &py)) {
          int8_t dx = px - (int8_t)(enemy->positionX >> 8);
          if (enemy->direction * dx > 0x00) {
            if (dx < -0x20 || 0x20 < dx) {
              ++enemy->state;
            }
          }
        }
      }
    }
    break;

  // 0x02: 後退の設定
  case 0x02:
    {
      // 発射
      {
        uint8_t angle = (enemy->direction - 0x01) * 0x40;
        int8_t da = -0x20;
        while (da <= 0x20) {
          EnemyFireAngle(enemy, angle + da);
          da += 0x10;
        }
      }

      // 移動の設定
      enemy->frame = 0x18;

      // 状態の更新
      ++enemy->state;
    }
    break;
    
  // 0x03: 後退の更新
  case 0x03:
    {
      // 移動
      enemy->positionX -= enemy->direction << 7;

      // 移動の完了
      if (enemy->frame > 0x00) {
        --enemy->frame;
      }
      if (!EnemyOutOfView(enemy) && enemy->frame == 0x00) {
        enemy->state = 0x00;
      }
    }
    break;
    
  // その他
  default:
    break;
  }
}

/*
 * ヌージャデル・ガーを生成する
 */
static void EnemyGenerateNousjadeulGer(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_NOUSJADEUL_GER;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (enemyGenerator.direction == ENEMY_DIRECTION_LEFT) ? ((GAME_VIEW_RIGHT + 0x08) << 8): ((GAME_VIEW_LEFT - 0x08) << 8);
    enemy->positionY = (int16_t)((SystemGetRandom() & 0x1f) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_NOUSJADEUL_GER);
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * ヌージャデル・ガーを更新する
 */
static void EnemyUpdateNousjadeulGer(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 前進の設定
  case 0x00:
    {
      // 移動の設定
      enemy->frame = (SystemGetRandom() & 0x1f) + 0x18;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x01: 前進の更新
  case 0x01:
    {
      // 移動
      enemy->positionX += 0x00c0 * enemy->direction;

      // 移動の完了
      if (enemy->frame > 0x00) {
        --enemy->frame;
      }
      if (!EnemyOutOfView(enemy) && enemy->frame == 0x00) {
        int8_t px, py;
        if (PlayerGetPosition(&px, &py)) {
          int8_t dx = px - (int8_t)(enemy->positionX >> 8);
          if (enemy->direction * dx > 0x00) {
            if (dx < -0x20 || 0x20 < dx) {
              ++enemy->state;
            }
          }
        }
      }
    }
    break;

  // 0x02: 発射の設定
  case 0x02:
    {
      // 発射と移動の設定
      {
        int8_t px, py;
        enemy->params[ENEMY_PARAM_0] = 0x08;
        enemy->params[ENEMY_PARAM_1] = 0x01;
        enemy->params[ENEMY_PARAM_2] = 0x00;
        if (PlayerGetPosition(&px, &py)) {
          enemy->params[ENEMY_PARAM_2] = py - (int8_t)(enemy->positionY >> 8) > 0x00 ? 0x01 : -0x01;
        }
      }

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x03: 発射の更新
  case 0x03:
    {
      // 移動
      enemy->positionX -= (int16_t)enemy->direction << 5;
      enemy->positionY += (int16_t)enemy->params[ENEMY_PARAM_2] << 7;

      // 移動の完了
      int8_t y = (int8_t)(enemy->positionY >> 8);
      if (y <= GAME_VIEW_TOP + GAME_STATUS_HEIGHT || GAME_VIEW_BOTTOM <= y) {
        enemy->state = 0x00;
      
      // 発射
      } else if (--enemy->params[ENEMY_PARAM_1] == 0x00) {
        EnemyFirePlayer(enemy, (SystemGetRandom() & 0x1f) - 0x10);
        enemy->params[ENEMY_PARAM_1] = 0x08;
        if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
          enemy->state = 0x00;
        }
      }
    }
    break;
    
  // その他
  default:
    break;
  }
}

/*
 * ボドルザーを生成する
 */
static void EnemyGenerateBoddoleZer(void)
{
  // エネミーの生成数の取得
  static const int8_t n = 0x04;
  
  // エネミーの生成
  if (EnemyCountEmpty() >= n) {

    // ボドルザー本の設定
    for (int8_t i = 0x00; i < 0x04; i++) {
      struct Enemy *enemy = EnemyGetEmpty();
      if (enemy != NULL) {
        enemy->type = ENEMY_TYPE_BODDOLE_ZER + i;
        enemy->state = ENEMY_STATE_NULL;
        enemy->positionX = (int16_t)pgm_read_byte(enemyBoddoleZerPositions + i * 0x02 + 0x00) << 8;
        enemy->positionY = (int16_t)pgm_read_byte(enemyBoddoleZerPositions + i * 0x02 + 0x01) << 8;
        enemy->direction = enemyGenerator.direction;
        enemy->life = pgm_read_byte(enemyLifes + ENEMY_TYPE_BODDOLE_ZER + i);
        enemy->damage = 0x00;
        enemy->frame = 0x00;
      }
    }

    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * ボドルザーを更新する
 */
static void EnemyUpdateBoddoleZer(struct Enemy *enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 初期化
  case 0x00:
    {
      // 発射の設定
      enemy->fire = (SystemGetRandom() & 0x1f) + 0x10;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x01: 発射の選択
  case 0x01:
    {
      // 発射の更新
      if (--enemy->fire == 0x00) {
        enemy->state = 0x02 + (SystemGetRandom() & 0x06);
      }
    }
    break;

  // 0x02,0x04: 発射(1)の設定
  case 0x02:
  case 0x04:
    {
      // 発射の設定
      enemy->params[ENEMY_PARAM_0] = 0x08 + (SystemGetRandom() & 0x07);
      enemy->params[ENEMY_PARAM_1] = 0x01;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x03,0x05: 発射(1)の更新
  case 0x03:
  case 0x05:
    {
      // 発射
      if (--enemy->params[ENEMY_PARAM_1] == 0x00) {
        EnemyFirePlayer(enemy, (SystemGetRandom() & 0x1f) - 0x10);
        enemy->params[ENEMY_PARAM_1] = 0x08;
        if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
          enemy->state = 0x00;
        }
      }
    }
    break;
    
  // 0x06: 発射(2)の設定
  case 0x06:
    {
      // 発射の設定
      enemy->params[ENEMY_PARAM_0] = 0x00;
      enemy->params[ENEMY_PARAM_1] = 0x01;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x07: 発射(2)の更新
  case 0x07:
    {
      // 発射
      if (--enemy->params[ENEMY_PARAM_1] == 0x00) {
        uint8_t angle = enemy->params[ENEMY_PARAM_0] + 0x70;
        EnemyFireAngle(enemy, angle < 0x40 ? 0x80 - angle : (angle > 0xc0 ? 0xc0 + (0xc0 - angle) : angle));
        enemy->params[ENEMY_PARAM_0] += 0x08;
        enemy->params[ENEMY_PARAM_1] = 0x06;
        if (enemy->params[ENEMY_PARAM_0] == 0x00) {
          enemy->state = 0x00;
        }
      }
    }
    break;
    
  // 0x08: 発射(3)の設定
  case 0x08:
    {
      // 発射の設定
      enemy->params[ENEMY_PARAM_0] = 0x03;
      enemy->params[ENEMY_PARAM_1] = 0x01;

      // 状態の更新
      ++enemy->state;
    }
    break;

  // 0x09: 発射(3)の更新
  case 0x09:
    {
      // 発射
      if (--enemy->params[ENEMY_PARAM_1] == 0x00) {
        {
          uint8_t angle = 0x78 + (SystemGetRandom() & 0x0f);
          int8_t da = -0x30;
          while (da <= 0x30) {
            EnemyFireAngle(enemy, angle + da);
            da += 0x0c;
          }
        }
        enemy->params[ENEMY_PARAM_1] = 0x18;
        if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
          enemy->state = 0x00;
        }
      }
    }
    break;
    
  // その他
  default:
    break;
  }
}
static void EnemyUpdateBoddoleZerNull(struct Enemy *enemy)
{
  // 耐久力の回復
  enemy->life = 0xff;
}

/*
 * 爆発を生成する
 */
static void EnemyGenerateBomb(void)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    
    // エネミーの設定
    enemy->type = ENEMY_TYPE_BOMB;
    enemy->state = ENEMY_STATE_NULL;
    enemy->positionX = (int16_t)((SystemGetRandom() >> 3) + 0x20) << 8;
    enemy->positionY = (int16_t)((SystemGetRandom() >> 3) - 0x10) << 8;
    enemy->direction = enemyGenerator.direction;
    enemy->life = 0x00;
    enemy->damage = 0x00;
    enemy->frame = 0x00;
    
    // ジェネレータの更新
    EnemyGenerateNext();
  }
}

/*
 * 爆発を更新する
 */
static void EnemyUpdateBomb(struct Enemy *enemy)
{
  // 初期化
  if (enemy->state == ENEMY_STATE_NULL) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x10;

    // 状態の更新
    ++enemy->state;
  }
  
  // パラメータの更新
  if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
    
    // エネミーの削除
    enemy->type = ENEMY_TYPE_NULL;
  }
}

