/*
 * Game.cpp : ゲーム
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t gameState;

// フレーム
static uint16_t gameFrame;

// レベル
static uint8_t gameLevel;

// フィールド
static uint8_t gameFields[0x20];

// プレイヤ
static struct GamePlayer gamePlayer;

// ブロック
static struct GameBlock gameBlock;
static const uint8_t gameBlockMoves[] PROGMEM = {
  0x04, 0x03, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 
};

// 文字列
static const char gameStringStart[] PROGMEM = "LEVEL";
static const char gameStringOver[] PROGMEM = "GAME OVER";
static const char gameStringClear[] PROGMEM = "LEVEL    CLEAR";
static const char gameStringCongratulations[] PROGMEM = "CONGRATULATIONS!";
static const char gameStringAllClear[] PROGMEM = "ALL LEVEL CLEARED";

// サウンド
static const char gameSoundStart[] PROGMEM = "T1O4C3R0C2R0C3R2C3E3R0E3G5";
static const char gameSoundPlug[] PROGMEM = "T1O4LC3E3D3";
static const char gameSoundOver[] PROGMEM = "T1O4E3D3C3R0E3D3C3R2O3C3D3E3F3R5";
static const char gameSoundClear[] PROGMEM = "T4O4E4D4C3&C3R2F4E4D3&D3R2G4F4E4D4C5R5";


/*
 * 内部関数の宣言
 */
static void GameStart(void);
static void GamePlay(void);
static void GameOver(void);
static void GameClear(void);
static void GameDrawField(void);
static void GameDrawPlayer(void);
static void GameDrawBlock(void);


/*
 * ゲームを初期化する
 */
void GameInitialize(void)
{
  // レベルの初期化
  gameLevel = 0x00;

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

    // レベルの設定
    if (gameLevel < 0x63) {
      ++gameLevel;
    }

    // フィールドの設定
    {
      int8_t length = 0x10 - ((gameLevel - 0x01) & 0x0f);
      if (gameLevel > 0x60 || length < 0x04) {
        length = 0x04;
      }
      int8_t i = 0x00;
      while (i < length) {
        gameFields[i] = 0x01;
        ++i;
      }
      while (i < 0x1e) {
        gameFields[i] = 0x00;
        ++i;
      }
      while (i < 0x20) {
        gameFields[i] = 0x01;
        ++i;
      }
    }
  
    // プレイヤの設定
    {
      gamePlayer.positionX = 0x01;
      gamePlayer.positionY = 0x3b;
      gamePlayer.move = 0x00;
      gamePlayer.animation = 0x00;
    }
    
    // ブロックの設定
    {
      gameBlock.positionX = ((SystemGetRandom() % 0x16) + 0x04) << 2;
      gameBlock.positionY = -0x04;
      gameBlock.move = 0x01;
    }

    // サウンドの再生
    SystemRequestSound(gameSoundStart, false);
  
    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // サウンドの監視
    if (!SystemIsSoundPlaying()) {
      gameState = GAME_STATE_PLAY;
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    GameDrawField();

    // プレイヤの描画
    GameDrawPlayer();

    // ブロックの描画
    GameDrawBlock();

    // ゲームスタートの描画
    AppDrawString(gameStringStart, 0x30, 0x1b);
    AppDrawNumber(gameLevel, 0x48, 0x1b, 0x02);

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

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの更新
    {
      // プレイヤの移動
      if (SystemIsInputPush(A_BUTTON)) {
        if (gamePlayer.positionX < 0x7e) {
          gamePlayer.move = 0x01;
        } else {
          gamePlayer.move = 0x00;
        }
      } else {
        if (gamePlayer.positionX > 0x01) {
          if (gamePlayer.positionX < (0x7f - 0x0a) && gamePlayer.move > 0x00) {
            gameFields[(gamePlayer.positionX + 0x0a) >> 2] = 0x01;
            SystemRequestSound(gameSoundPlug, false);
          }
          gamePlayer.move = -0x01;
        } else {
          gamePlayer.move = 0x00;
        }
      }
      gamePlayer.positionX += gamePlayer.move;

      // アニメーションの更新
      if (gamePlayer.positionX > (0x00 + 0x01)) {
        ++gamePlayer.animation;
      } else {
        gamePlayer.animation = 0x00;
      }
    }

    // ブロックの更新
    {
      if (--gameBlock.move == 0x00) {
        if (++gameBlock.positionY >= 0x3c) {
          int8_t i = gameBlock.positionX >> 2;
          gameFields[i++] = 0x01;
          gameFields[i++] = 0x00;
          gameFields[i++] = 0x00;
          gameFields[i++] = 0x01;
          gameBlock.positionX = ((SystemGetRandom() % 0x16) + 0x04) << 2;
          gameBlock.positionY = -0x04;
        }
        gameBlock.move = pgm_read_byte(gameBlockMoves + ((gameLevel - 0x01) >> 4));
      }
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    GameDrawField();

    // プレイヤの描画
    GameDrawPlayer();

    // ブロックの描画
    GameDrawBlock();

    // 描画の完了
    SystemDisplayVideo();
  }

  // ゲームクリアの判定
  if (gamePlayer.positionX >= 0x7e) {
    gameState = GAME_STATE_CLEAR;
  
  // ゲームオーバーの判定
  } else if (
    gameFields[(gamePlayer.positionX - 0x01) >> 2] == 0x00 && 
    gameFields[(gamePlayer.positionX + 0x01) >> 2] == 0x00
  ) {
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

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // プレイヤの落下
    if (gamePlayer.positionY < 0x44) {
      if (++gamePlayer.positionY == 0x44) {
        SystemRequestSound(gameSoundOver, false);
      }

    // サウンドの監視
    } else if (!SystemIsSoundPlaying()) {
      AppSetScore(gameLevel);
      AppSetState(APP_STATE_TITLE_INITIALIZE);
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    GameDrawField();

    // プレイヤの描画
    GameDrawPlayer();

    // ブロックの描画
    // GameDrawBlock();

    // ゲームオーバーの表示
    AppDrawString(gameStringOver, 0x2e, 0x1b);

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

    // サウンドの再生
    SystemRequestSound(gameSoundClear, false);

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // サウンドの監視
    if (!SystemIsSoundPlaying()) {
      if (gameLevel < 0x63) {
        gameState = GAME_STATE_START;
      } else {
        AppSetScore(gameLevel);
        AppSetState(APP_STATE_TITLE_INITIALIZE);
      }
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    GameDrawField();

    // プレイヤの描画
    GameDrawPlayer();

    // ブロックの描画
    // GameDrawBlock();

    // ゲームクリアの表示
    if (gameLevel < 0x63) {
      AppDrawString(gameStringClear, 0x24, 0x1b);
      AppDrawNumber(gameLevel, 0x3c, 0x1b, 0x02);
    } else {
      AppDrawString(gameStringCongratulations, 0x20, 0x16);
      AppDrawString(gameStringAllClear, 0x1e, 0x20);
    }

    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * フィールドを描画する
 */
static void GameDrawField(void)
{
  for (int8_t i = 0x00; i < 0x20; i++) {
    if (gameFields[i] != 0x00) {
      uint16_t pattern = 0x0014;
      if (i == 0x00 || gameFields[i - 0x01] != 0x00) {
        pattern |= 0x0001;
      }
      if (i == 0x1f || gameFields[i + 0x01] != 0x00) {
        pattern |= 0x0002;
      }
      AppDrawPattern(pattern, i << 2, 0x3c, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  }
}

/*
 * プレイヤを描画する
 */
static void GameDrawPlayer(void)
{
  AppDrawPattern(0x0010 + ((gamePlayer.animation >> 3) & 0x03), gamePlayer.positionX - 0x01, gamePlayer.positionY - 0x04, 0x03, SYSTEM_VIDEO_FLIP_NORMAL);
}

/*
 * ブロックを描画する
 */
static void GameDrawBlock(void)
{
  AppDrawPattern(0x0018, gameBlock.positionX + 0x00, gameBlock.positionY, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
  AppDrawPattern(0x0018, gameBlock.positionX + 0x0c, gameBlock.positionY, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
}

