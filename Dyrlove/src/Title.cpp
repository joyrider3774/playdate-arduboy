/*
 * Title.cpp : タイトル
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Title.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t titleState;

// フレーム
static uint16_t titleFrame;

// 文字列
static const char titleStringScore[] PROGMEM = "SCORE-";


/*
 * 内部関数の宣言
 */


/*
 * タイトルを初期化する
 */
void TitleInitialize(void)
{
  // フレームの設定
  titleFrame = 0x0000;
  
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
      // フレームの更新
      ++titleFrame;

      // A ボタンで開始
      if (SystemIsInputEdge(A_BUTTON)) {
        titleFrame = 0x0000;
        titleState = TITLE_STATE_START;

      // B ボタンでサウンドの切り替え
      } else if (SystemIsInputEdge(B_BUTTON)) {
        SystemSwitchSound();
      }
    }
    break;

  // スタート
  case TITLE_STATE_START:
    {
      // フレームの更新
      titleFrame += 0x0010;

      // フレームの更新の完了
      if (titleFrame >= 0x0400) {
        AppSetState(APP_STATE_GAME_INITIALIZE);
      }
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

    // SDF-1 の描画
    AppDrawPattern(0x0100, 0x00, 0x00, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0110, 0x00, 0x08, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0120, 0x00, 0x10, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0130, 0x00, 0x18, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0140, 0x00, 0x20, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0150, 0x00, 0x28, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0160, 0x00, 0x30, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0170, 0x00, 0x38, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    
    // ロゴの描画
    AppDrawPattern(0x0050, 0x58, 0x28, 0x28, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0060, 0x58, 0x30, 0x28, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0070, 0x58, 0x38, 0x28, SYSTEM_VIDEO_FLIP_NORMAL);

    // スコアの描画
    AppDrawString(titleStringScore, 0x50, 0x00);
    AppDrawScore(AppGetScore(), 0x68, 0x00);

    // PRESS A To START の描画
    if ((titleFrame & 0x0020) == 0x0000) {
      AppDrawPattern(0x0058, 0x20, 0x1d, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // サウンドの描画
    if (SystemIsSoundEnable()) {
      SystemClipByte(0x7f, 0x04, 0x36, 0x18);
      AppDrawPattern(0x0065, 0x04, 0x36, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    } else {
      SystemClipByte(0x7f, 0x04, 0x36, 0x10);
      AppDrawPattern(0x0055, 0x04, 0x36, 0x10, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  
    // 描画の完了
    SystemDisplayVideo();
  }
}


