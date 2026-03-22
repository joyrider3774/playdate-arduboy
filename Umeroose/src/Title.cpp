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
static const char titleStringLogo[] PROGMEM = "UMEROOSE";
static const char titleStringScore[] PROGMEM = "MAX LEVEL";

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
    SystemClearVideo(BLACK);

    // タイトルの描画
    {
      AppDrawPattern(0x0019, 0x28, 0x1a, 0x30, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // スコアの描画
    AppDrawString(titleStringScore, 0x50, 0x39);
    AppDrawNumber(AppGetScore(), 0x78, 0x39, 0x02);

    // サウンドの描画
    if (SystemIsSoundEnable()) {
      AppDrawPattern(0x0005, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    } else {
      AppDrawPattern(0x0002, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // 描画の完了
    SystemDisplayVideo();
  }
}


