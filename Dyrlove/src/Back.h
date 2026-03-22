/*
 * Back.h : 背景
 */
#ifndef _BACK_H_
#define _BACK_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// ステージ
#define BACK_STAGE_NULL                 (0x00)
#define BACK_STAGE_ZAKO                 (0x01)
#define BACK_STAGE_WARNING              (0x02)
#define BACK_STAGE_BOSS                 (0x03)
#define BACK_STAGE_FINISH               (0x04)
#define BACK_STAGE_CLEAR                (0x05)

// フラグ
#define BACK_FLAG_NULL                  (0x00)
#define BACK_FLAG_EARTH                 (0x01)
#define BACK_FLAG_STAR                  (0x02)
#define BACK_FLAG_SDF1                  (0x04)
#define BACK_FLAG_BODDOLE_ZER           (0x08)
#define BACK_FLAG_MINMAY                (0x10)

// 星
#define BACK_STAR_N                     (0x07)


/*
 *  型宣言
 */

// 地球
struct Earth {

  // 位置
  int8_t positionY;

  // フレーム
  uint8_t frame;
  
};

// 星
struct Star {

  // 位置
  int16_t positionX;
  int8_t positionY;

  // 速度
  int8_t speed;
  
};

// SDF-1
struct Sdf1 {

  // 位置
  int8_t positionX;
  int8_t positionY;
  
  // フレーム
  uint8_t frame;
  
};

// ボドルザー
struct BoddoleZer {

  // マスク
  uint8_t mask;

  // フレーム
  uint8_t frame;
  
};

// ミンメイ
struct Minmay {

  // パターン
  uint16_t pattern;
  
  // マスク
  uint8_t mask;

  // フレーム
  uint8_t frame;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void BackInitialize(void);
extern void BackUpdate(void);
extern void BackRender(void);
extern bool BackIsStage(int8_t stage);
extern void BackSetStage(int8_t stage);


#endif

