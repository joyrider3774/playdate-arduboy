/*
 * Game.cpp : ゲーム
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


/*
 * 変数の定義
 */

// 状態
static uint8_t gameState;

// フェード
static const uint8_t gameFadeIndexs[] PROGMEM = {
  0x06, 0x00, 0x09, 0x08, 0x05, 0x03, 0x04, 0x0d, 0x07, 0x02, 0x0f, 0x0b, 0x0e, 0x01, 0x0a, 0x0c, 
};
static uint8_t gameFadePatterns[0x08];
static uint8_t gameFade;

// フレーム
static uint8_t gameFrame;

// 文字列
static const char gameStringStart[] PROGMEM = "PETIT OF THE DEAD";
static const char gameStringOver[] PROGMEM = "YOU ARE DEAD";
static const char gameStringClear[] PROGMEM = "YOU HAVE SURVIVED";


/*
 * 内部関数の宣言
 */
static void GameStart(void);
static void GamePlay(void);
static void GameOver(void);
static void GameClear(void);
static void GameSetFade(void);
static bool GameUpdateFade(void);


/*
 * ゲームを初期化する
 */
void GameInitialize(void)
{
  // フィールドの初期化
  FieldInitialize();

  // プレイヤの初期化
  PlayerInitialize();

  // エネミーの初期化
  EnemyInitialize();
  
  // 状態の設定
  gameState = GAME_STATE_START;

  // アプリケーションの更新
  AppSetState(APP_STATE_GAME_UPDATE);
}

/*
 * ゲームを更新する
 */
void GameUpdate(void)
{
  // 状態別の処理
  switch (gameState & 0xf0) {

  // ゲームスタート
  case GAME_STATE_START:
    GameStart();
    break;

  // ゲームプレイ
  case GAME_STATE_PLAY:
    GamePlay();
    break;

  // ゲームオーバー
  case GAME_STATE_OVER:
    GameOver();
    break;

  // ゲームクリア
  case GAME_STATE_CLEAR:
    GameClear();
    break;

  // その他
  default:
    break;
  }
}

/*
 * ゲームを開始する
 */
static void GameStart(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // プレイヤの待機
    PlayerSetState(PLAYER_STATE_STAY);

    // エネミーの待機
    EnemySetState(ENEMY_STATE_STAY);

    // フェードの設定
    GameSetFade();

    // フレームの設定
    gameFrame = 0x10;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの更新
    PlayerUpdate();
    
    // エネミーの更新
    EnemyUpdate();
    
    // フィールドの更新
    FieldUpdate();

    // フェードの更新
    if (GameUpdateFade()) {
      if (--gameFrame == 0x00) {
        gameState = GAME_STATE_PLAY;
      }
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    FieldRender();

    // エネミーの描画
    EnemyRender();

    // プレイヤの描画
    PlayerRender();

    // フェードの描画
    {
      uint8_t *v = SystemGetVideoBuffer();
      for (uint8_t i = 0x00; i < (WIDTH / 0x08) * (HEIGHT / 0x08); i++) {
        for (uint8_t j = 0x00; j < 0x08; j++) {
          *v++ &= gameFadePatterns[j];
        }
      }
    }

    // ゲームスタートの描画
    AppClipString(gameStringStart, 0x1d, 0x1d);
    AppClipString(gameStringStart, 0x1f, 0x1d);
    AppClipString(gameStringStart, 0x1e, 0x1c);
    AppClipString(gameStringStart, 0x1e, 0x1e);
    AppDrawString(gameStringStart, 0x1e, 0x1d);

    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * ゲームをプレイする
 */
static void GamePlay(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // プレイヤの開始
    PlayerSetState(PLAYER_STATE_PLAY);

    // エネミーの開始
    EnemySetState(ENEMY_STATE_MOVE);

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの更新
    PlayerUpdate();
    
    // エネミーの更新
    EnemyUpdate();
    
    // フィールドの更新
    FieldUpdate();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    FieldRender();

    // エネミーの描画
    EnemyRender();

    // プレイヤの描画
    PlayerRender();
  
    // 描画の完了
    SystemDisplayVideo();
  }

  // エネミーの監視
  if (EnemyGetLive() == 0x00) {
    gameState = GAME_STATE_CLEAR;
    
  // プレイヤの監視
  } else if (!PlayerIsLive()) {
    gameState = GAME_STATE_OVER;
  }
}

/*
 * ゲームオーバーになる
 */
static void GameOver(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // フェードの設定
    GameSetFade();

    // フレームの設定
    gameFrame = 0xc0;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの更新
    PlayerUpdate();
    
    // エネミーの更新
    EnemyUpdate();
    
    // フィールドの更新
    FieldUpdate();

    // フェードの更新
    if (GameUpdateFade()) {
      if (--gameFrame == 0x00 || SystemIsInputEdge(A_BUTTON)) {
        AppSetState(APP_STATE_TITLE_INITIALIZE);
      }
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フェードの実行
    if (gameFade > 0x00) {
  
      // フィールドの描画
      FieldRender();
  
      // エネミーの描画
      EnemyRender();
  
      // プレイヤの描画
      PlayerRender();
  
      // フェードの描画
      {
        uint8_t *v = SystemGetVideoBuffer();
        for (uint8_t i = 0x00; i < (WIDTH / 0x08) * (HEIGHT / 0x08); i++) {
          for (uint8_t j = 0x00; j < 0x08; j++) {
            *v++ |= gameFadePatterns[j];
          }
        }
      }

    // フェードの完了
    } else {
      
      // 塗りつぶし
      {
        uint8_t *v = SystemGetVideoBuffer();
        for (uint16_t i = 0x00; i < WIDTH * (HEIGHT / 0x08); i++) {
          *v++ = 0xff;
        }
      }

      // ゲームオーバーの描画
      AppClipString(gameStringOver, 0x28, 0x1d);
    }

    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * ゲームをクリアする
 */
static void GameClear(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // フェードの設定
    GameSetFade();

    // フレームの設定
    gameFrame = 0x30;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの更新
    PlayerUpdate();
    
    // エネミーの更新
    EnemyUpdate();
    
    // フィールドの更新
    FieldUpdate();


    // 少し待機
    if ((gameState & 0x0f) == 0x01) {
      if (--gameFrame == 0x00) {
        gameFrame = 0xc0;
        ++gameState;
      }
      
    // フェードの更新
    } else {
      if (GameUpdateFade()) {
        if (--gameFrame == 0x00 || SystemIsInputEdge(A_BUTTON)) {
          AppSetState(APP_STATE_TITLE_INITIALIZE);
        }
      }
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フェードの実行
    if (gameFade > 0x00) {
  
      // フィールドの描画
      FieldRender();
  
      // エネミーの描画
      EnemyRender();
  
      // プレイヤの描画
      PlayerRender();
  
      // フェードの描画
      {
        uint8_t *v = SystemGetVideoBuffer();
        for (uint8_t i = 0x00; i < (WIDTH / 0x08) * (HEIGHT / 0x08); i++) {
          for (uint8_t j = 0x00; j < 0x08; j++) {
            *v++ &= ~gameFadePatterns[j];
          }
        }
      }

    // フェードの完了
    } else {
      
      // 塗りつぶし
      {
        uint8_t *v = SystemGetVideoBuffer();
        for (uint16_t i = 0x00; i < WIDTH * (HEIGHT / 0x08); i++) {
          *v++ = 0x00;
        }
      }
    }

    // ゲームクリアの描画
    AppClipString(gameStringClear, 0x1d, 0x1d);
    AppClipString(gameStringClear, 0x1f, 0x1d);
    AppClipString(gameStringClear, 0x1e, 0x1c);
    AppClipString(gameStringClear, 0x1e, 0x1e);
    AppDrawString(gameStringClear, 0x1e, 0x1d);

    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * フェードを設定する
 */
static void GameSetFade(void)
{
  for (int8_t i = 0x00; i < 0x08; i++) {
    gameFadePatterns[i] = 0x00;
  }
  gameFade = 0x10 << 1;
}

/*
 * フェードを更新する
 */
static bool GameUpdateFade(void)
{
  bool result = true;
  if (gameFade > 0x00) {
    --gameFade;
    uint8_t fade = pgm_read_byte(gameFadeIndexs + (gameFade >> 1));
    uint8_t i = (fade & 0x0c) >> 1;
    uint8_t n = 0x03 << ((fade & 0x03) << 1);
    gameFadePatterns[i + 0x00] |= n;
    gameFadePatterns[i + 0x01] |= n;
    result = false;
  }
  return result;
}

