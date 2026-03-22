/*
 * Title.cpp : タイトル
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Title.h"
#include "pattern.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t titleState;

// ロゴ
static uint8_t titleLogo;
static int8_t titleLogoX;
static int8_t titleLogoY;

// 文字列
static const char titleString[] PROGMEM = "PETIT OF THE DEAD";

// サウンド
static const char titleSoundOption[] PROGMEM = "T1O4B1";


/*
 * 内部関数の宣言
 */


/*
 * タイトルを初期化する
 */
void TitleInitialize(void)
{
  // ロゴの設定
  titleLogo = 0x00;
  titleLogoX = 0x00;
  titleLogoY = 0x00;
  
  // 状態の設定
  titleState = TITLE_STATE_STAY;

  // アプリケーションの更新
  AppSetState(APP_STATE_TITLE_UPDATE);
}

/*
 * タイトルを更新する
 */
void TitleUpdate(void)
{
  // 状態別の処理
  switch (titleState & 0xf0) {

  // 待機
  case TITLE_STATE_STAY:
    {
      // ロゴの更新
      if ((++titleLogo & 0x20) == 0x00) {
        titleLogoX = (int8_t)((SystemGetRandom() & 0x04) >> 1) - 0x01;
        titleLogoY = (int8_t)((SystemGetRandom() & 0x04) >> 1) - 0x01;
      } else {
        titleLogoX = 0x00;
        titleLogoY = 0x00;
      }
      
      // A ボタンで開始
      if (SystemIsInputEdge(A_BUTTON)) {
        titleState = TITLE_STATE_START;

      // B ボタンでサウンドの切り替え
      } else if (SystemIsInputEdge(B_BUTTON)) {
        SystemSwitchSound();
        if (SystemIsSoundEnable()) {
          SystemRequestSound(titleSoundOption, false);
        }
      }
    }
    break;

  // スタート
  case TITLE_STATE_START:
    {
      // アプリケーションの更新
      AppSetState(APP_STATE_GAME_INITIALIZE);
    }
    break;
    
  // その他
  default:
    break;
  }
  
  // 描画
  {
    // 描画の開始
    SystemClearVideo(WHITE);

    // エネミーの描画
    {
      uint8_t *video = SystemGetVideoBuffer() + 0x0030;
      uint8_t *pattern = patternTable + (0x0020 << 3);
      for (int8_t x = 0x00; x < 0x0a; x++) {
        uint16_t pd = ((uint16_t)pgm_read_byte(pattern + 0x0080) << 8) | pgm_read_byte(pattern + 0x0000);
        uint16_t pc = ((uint16_t)pgm_read_byte(pattern + 0x0180) << 8) | pgm_read_byte(pattern + 0x0100);
        uint8_t *v = video;
        pd >>= 1;
        pc >>= 1;
        for (int8_t y = 0x00; y < 0x08; y++) {
          uint8_t p = (pc & 0x01) == 0x00 ? 0xff : 0x00;
          if ((pd & 0x01) != 0x00) {
            p = 0xff;
          }
          for (int8_t i = 0x00; i < 0x08; i++) {
            *v++ = p;
          }
          v += WIDTH - 0x08;
          pd >>= 1;
          pc >>= 1;
        }
        video += 0x08;
        ++pattern;
      }
    }

    // タイトルの描画
    {
      int8_t x = 0x1e + titleLogoX;
      int8_t y = 0x1d + titleLogoY;
      AppDrawString(titleString, x - 0x01, y + 0x00);
      AppDrawString(titleString, x + 0x01, y + 0x00);
      AppDrawString(titleString, x + 0x00, y - 0x01);
      AppDrawString(titleString, x + 0x00, y + 0x01);
      AppClipString(titleString, x + 0x00, y + 0x00);
    }

    // サウンドの描画
    if (SystemIsSoundEnable()) {
      AppClipPattern(0x05, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    } else {
      AppClipPattern(0x02, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // 描画の完了
    SystemDisplayVideo();
  }
}


