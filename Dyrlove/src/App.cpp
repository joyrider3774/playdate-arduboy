/*
 * App.cpp : アプリケーション
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Title.h"
#include "Game.h"
#include "pattern.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t appState;

// スコア
static uint16_t appScore;


/*
 * 内部関数の宣言
 */


/*
 * アプリケーションを初期化する
 */
void AppInitialize(void)
{
  // スコアの初期化
  appScore = 0x0064;
  
  // 状態の初期化
  appState = APP_STATE_TITLE_INITIALIZE;
}

/*
 * アプリケーションを更新する
 */
void AppUpdate(void)
{
  // 状態別の処理
  switch (appState) {

  // タイトルの初期化
  case APP_STATE_TITLE_INITIALIZE:
    TitleInitialize();
    break;

  // タイトルの更新
  case APP_STATE_TITLE_UPDATE:
    TitleUpdate();
    break;

  // ゲームの初期化
  case APP_STATE_GAME_INITIALIZE:
    GameInitialize();
    break;

  // ゲームの更新
  case APP_STATE_GAME_UPDATE:
    GameUpdate();
    break;

  // その他
  default:
    break;
  }

  // 乱数の更新
  if (SystemIsInputPush(0xff)) {
    SystemGetRandom();
  }
}

/*
 * 状態を設定する
 */
void AppSetState(uint8_t state)
{
  appState = state;
}

/*
 * スコアを取得する
 */
uint16_t AppGetScore(void)
{
  return appScore;
}

/*
 * スコアを設定する
 */
void AppSetScore(uint16_t score)
{
  if (appScore < score) {
    appScore = score;
  }
}

/*
 * パターンを描画する
 */
void AppDrawPattern(int16_t draw, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemDrawPattern(patternTable + (draw << 3), x, y, width, flip);
}
void AppClipPattern(int16_t draw, int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemClipPattern(patternTable + (clip << 3), x, y, width, flip);
  SystemDrawPattern(patternTable + (draw << 3), x, y, width, flip);
}
void AppMaskPattern(int16_t draw, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemMaskPattern(patternTable + (draw << 3), mask, x, y, width, flip);
}

/*
 * 文字列を描画する
 */
void AppDrawString(const char *string, int8_t x, int8_t y)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0') {
    SystemDrawPattern(patternTable + (0x01e0 << 3) + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
  }
}
void AppDrawString(const char *string, int8_t x, int8_t y, int8_t length)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0' && length > 0x00) {
    SystemDrawPattern(patternTable + (0x01e0 << 3) + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
    --length;
  }
}

/*
 * スコアを描画する
 */
void AppDrawScore(uint16_t score, int8_t x, int8_t y)
{
  char string[0x06];
  for (int8_t i = 0x04; i >= 0x00; i--) {
    string[i] = score % 10;
    score /= 10;
  }
  string[0x05] = 0;
  for (int8_t i = 0x00; i < 0x06; i++) {
    SystemDrawPattern(patternTable + (0x01e8 << 3) + (string[i] << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
  }
}

