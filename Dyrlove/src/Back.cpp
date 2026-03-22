/*
 * Back.cpp : 背景
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Back.h"


/*
 * 変数の定義
 */

// ステージ
static int8_t backStage;
static const uint8_t backStageFlags[] PROGMEM = {
  BACK_FLAG_NULL, 
  BACK_FLAG_EARTH | BACK_FLAG_STAR | BACK_FLAG_SDF1, 
  BACK_FLAG_BODDOLE_ZER, 
  BACK_FLAG_BODDOLE_ZER | BACK_FLAG_MINMAY, 
  BACK_FLAG_BODDOLE_ZER | BACK_FLAG_MINMAY, 
  BACK_FLAG_STAR, 
};

// フラグ
static uint8_t backFlag;

// 地球
static struct Earth earth;

// 星
static struct Star stars[BACK_STAR_N];

// SDF-1
static struct Sdf1 sdf1;

// ボドルザー
static struct BoddoleZer boddoleZer;

// ミンメイ
static struct Minmay minmay;
static const uint8_t minmayMasks[] PROGMEM = {
  0x00, 0x00, 0x00, 
  0x00, 0x18, 0x00, 
  0x00, 0x3c, 0x00, 
  0x00, 0x7e, 0x00, 
  0x80, 0xff, 0x01, 
  0xc0, 0xff, 0x03, 
  0xe0, 0xff, 0x07, 
  0xf0, 0xff, 0x0f, 
  0xf8, 0xff, 0x1f, 
  0xfc, 0xff, 0x3f, 
  0xfe, 0xff, 0x7f, 
  0xff, 0xff, 0xff, 
};


/*
 * 内部関数の宣言
 */


/*
 * 背景を初期化する
 */
void BackInitialize(void)
{
  // ステージの初期化
  backStage = BACK_STAGE_NULL;

  // フラグの初期化
  backFlag = BACK_FLAG_NULL;
  
  // 地球の初期化
  {
    earth.positionY = 0x2c;
    earth.frame = 0x00;
  }
  
  // 星の初期化
  {
    struct Star *star = stars;
    for (int8_t i = 0x00; i < BACK_STAR_N; i++) {
      star->positionX = (int16_t)(SystemGetRandom() & 0x7f) << 8;
      star->positionY = (i << 3) + (SystemGetRandom() & 0x07) + 0x08;
      star->speed = (SystemGetRandom() & 0x07) + 0x04;
      ++star;
    }
  }
  
  // SDF-1 の初期化
  {
    sdf1.positionX = +0x44;
    sdf1.positionY = -0x44;
    sdf1.frame = 0x00;
  }

  // ボドルザーの初期化
  {
    boddoleZer.mask = 0x00;
    boddoleZer.frame = 0x00;
  }

  // ミンメイの初期化
  {
    minmay.pattern = 0x0000;
    minmay.mask = 0x00;
    minmay.frame = 0x00;
  }
}

/*
 * 背景を更新する
 */
void BackUpdate(void)
{
  // 地球の更新
  if ((backFlag & BACK_FLAG_EARTH) != 0) {
    if (sdf1.frame == 0xff) {
      if (earth.frame < 0x40) {
        ++earth.frame;
      } else {
        if (earth.positionY < 0x40) {
          ++earth.positionY;
          earth.frame = 0x00;
        } else {
          earth.frame = 0xff;
        }
      }
    }
  }

    
  // 星の更新
  if ((backFlag & BACK_FLAG_STAR) != 0) {
    struct Star *star = stars;
    for (int8_t i = 0x00; i < BACK_STAR_N; i++) {
      star->positionX -= (int16_t)star->speed << 4;
      if (star->positionX < 0x00) {
        star->positionX = 0x7f00;
        star->positionY = (i * 0x08) + (SystemGetRandom() & 0x07) + 0x08;
        star->speed = (SystemGetRandom() & 0x07) + 0x04;
      }
      ++star;
    }
  }
    
  // SDF-1 の更新
  if ((backFlag & BACK_FLAG_SDF1) != 0) {
    if (sdf1.frame < 0x40) {
      ++sdf1.frame;
    } else {
      if (sdf1.positionX > -0x40) {
        --sdf1.positionX;
        ++sdf1.positionY;
        sdf1.frame = 0x00;
      } else {
        sdf1.frame = 0xff;
      }
    }
  }

  // ボドルザーの更新
  if ((backFlag & BACK_FLAG_BODDOLE_ZER) != 0) {
    if (backStage != BACK_STAGE_FINISH) {
      if (boddoleZer.mask != 0xff) {
        if (++boddoleZer.frame > 0x20) {
          boddoleZer.mask = (boddoleZer.mask << 1) + 0x11;
          boddoleZer.frame = 0x00;
        }
      }
    } else {
      if (boddoleZer.mask != 0x00) {
        if (++boddoleZer.frame > 0x60) {
          boddoleZer.mask = (boddoleZer.mask >> 1) & 0x77;
          boddoleZer.frame = 0x00;
        }
      }
    }
  }

  // ミンメイの更新
  if ((backFlag & BACK_FLAG_MINMAY) != 0) {
    ++minmay.pattern;
    ++minmay.mask;
    if (boddoleZer.mask != 0x00) {
      if (minmay.frame < (0x0c << 3) - 0x01) {
        ++minmay.frame;
      }
    } else {
      if (minmay.frame > 0x00) {
        --minmay.frame;
      }
    }
  }

  // ステージの監視
  if (backStage == BACK_STAGE_ZAKO) {
    if (earth.frame == 0xff) {
      BackSetStage(BACK_STAGE_NULL);
    }
  } else if (backStage == BACK_STAGE_WARNING) {
    if (boddoleZer.mask == 0xff) {
      BackSetStage(BACK_STAGE_BOSS);
    }
  } else if (backStage == BACK_STAGE_FINISH) {
    if (minmay.frame == 0x00) {
      BackSetStage(BACK_STAGE_NULL);
    }
  }
}

/*
 * 背景を描画する
 */
void BackRender(void)
{
  // 地球の描画
  if ((backFlag & BACK_FLAG_EARTH) != 0) {
    if (earth.frame < 0xff) {
      AppDrawPattern(0x0180, 0x00, earth.positionY + 0x00, 0x80, SYSTEM_VIDEO_FLIP_NORMAL);
      AppDrawPattern(0x0190, 0x00, earth.positionY + 0x08, 0x80, SYSTEM_VIDEO_FLIP_NORMAL);
      AppDrawPattern(0x01a0, 0x00, earth.positionY + 0x10, 0x80, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  }
    
  // 星の描画
  if ((backFlag & BACK_FLAG_STAR) != 0) {
    if (earth.frame < 0xff) {
      struct Star *star = stars;
      for (int8_t i = 0x00; i < BACK_STAR_N; i++) {
        AppDrawPattern(0x0002, star->positionX >> 8, star->positionY, 0x01, SYSTEM_VIDEO_FLIP_NORMAL);
        ++star;
      }
    }
  }
    
  // SDF-1 の描画
  if ((backFlag & BACK_FLAG_SDF1) != 0) {
    if (sdf1.frame < 0xff) {
      for (int8_t i = 0x00; i < 0x08; i++) {
        AppDrawPattern(0x0100 + i * 0x0010, sdf1.positionX, sdf1.positionY + i * 0x08, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }
  }

  // ボドルザーの描画
  if ((backFlag & BACK_FLAG_BODDOLE_ZER) != 0) {
    if (boddoleZer.mask > 0x00) {
      for (int8_t i = 0x00; i < 0x08; i++) {
        AppMaskPattern(0x0108 + i * 0x0010, boddoleZer.mask, 0x40, i * 0x08, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }
  }

  // ミンメイの描画
  if ((backFlag & BACK_FLAG_MINMAY) != 0) {
    uint16_t pattern = ((minmay.pattern & 0x0200) == 0x0000 ? 0x01b0 : 0x01b6);
    for (int8_t i = 0x00; i < 0x03; i++) {
      uint8_t mask = pgm_read_byte(minmayMasks + (minmay.frame >> 3) * 0x03 + i) & ((minmay.mask & 0x02) == 0x00 ? 0xaa : 0x55);
      AppMaskPattern(pattern + i * 0x0010, mask, 0x10, i * 0x08 + 0x10, 0x30, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  }
}

/*
 * 背景のステージを判定する
 */
bool BackIsStage(int8_t stage)
{
  return backStage == stage ? true : false;
}

/*
 * 背景のステージを設定する
 */
void BackSetStage(int8_t stage)
{
  backStage = stage;
  backFlag = pgm_read_byte(backStageFlags + stage);
}

