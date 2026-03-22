/*
 * Game.cpp : ゲーム
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Back.h"
#include "Player.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t gameState;

// スコア
static uint16_t gameScore;

// フレーム
static uint16_t gameFrame;

// 文字列
static const char gameStringScore[] PROGMEM = "SCORE-";
static const char gameStringHiScore[] PROGMEM = "HI-SCORE";
static const char gameStringClear[] PROGMEM = "JUST A SIMPLE LOVE SONG.";
static const char gameStringOver[] PROGMEM = "GAME OVER";

// BGM
static const char gameBgm[] PROGMEM = 
  // テンポ
  "T4"
  // 前奏
  "L3O5R5RDC+5O4AE&E9O5R5RED5C+O4F+&F+9"
  "L3O5R5RDC+5O4AE&E9O5R5RED5C+O4F+&F+7G+7"
  "L5O4EF+G+AF+G+L3AO5C+O4BO5C+"
  // いまあなたのこえがきこえる〜
  "L3O4R5F+F+F+F+F+F+G+G+G+G+G+F+5EREEEEF+G+F+&F+5R5RC+C+C+"
  "L3O4C+D&D5RO3BO4DF+F+E5EE5DC+&C+7R7R9"
  // いまあなたのすがたがみえる〜
  "L3O4R5F+F+F+F+F+F+G+G+G+G+&G+G+F+ER5EEEF+G+F+&F+5R5RC+C+C+"
  "L3O4C+D&D5RO3BO4DF+F+E5EE5DC+&C+7R7R9"
  // きのうまで〜
  "L3O4R5E+E+&E+G+5F+&F+5R5R5F+F+E+E+E+E+E+5G+F+&F+5R5R7"
  "L5O4RF+G+AG+7F+7E9&E8R"
  // おぼえていますか〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  // それははじめての〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5REEEO5DDDD&D5O4F+F+&F+G+&G+5R5O4E5B5&BA&A5A5&A7R7"
  // 間奏
  "L3O5R5RDC+5O4AE&E9O5R5RED5C+O4F+&F+9"
  "L3O5R5RDC+5O4AE&E9O5R5RED5C+O4F+&F+7G+7"
  "L3O3A7&AAO4C+EG+5AF+&F+6F+F+5G+E&E7"
  "L5O4EDADDCGCO3B6O4C+3D7&D9D6E3F7&F9"
  "L3O5EEEEE5O4GG&GAA5&A7O5FFFFF5O4AA&ABB5RO5DEF"
  "L3O5G6CC7A6EE7A6FF7"
  "L3O4RF+G+ABO5C+DF+O4E5F+5G+5A5F+5G+5AO5C+O4BO5C+"
  // いまあなたのこえがきこえる〜
  "L3O4R5F+F+F+F+F+F+G+G+G+G+G+F+5EREEEEF+G+F+&F+5R5RC+C+C+"
  "L3O4C+D&D5RO3BO4DF+F+E5EE5DC+&C+7R7R9"
  // いまあなたのすがたがみえる〜
  "L3O4R5F+F+F+F+F+F+G+G+G+G+&G+G+F+ER5EEEF+G+F+&F+5R5RC+C+C+"
  "L3O4C+D&D5RO3BO4DF+F+E5EE5DC+&C+7R7R9"
  // きのうまで〜
  "L3O4R5E+E+&E+G+5F+&F+5R5R5F+F+E+E+E+E+E+5G+F+&F+5R5R7"
  "L5O4RF+G+AG+7F+7E9&E8R"
  // おぼえていますか〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  // それははじめての〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5REEEO5DDDD&D5O4F+F+&F+G+&G+5R5O4E5B5&BA&A5A5&A7R7"
  // もうひとりぼっちじゃない〜
  "L3O5C+8C+F+E7RC+C+O4BO5C+8F+E&E7R7"
  "L3O5C+8C+F+E7RC+C+O4BO5C+9R9"
  // おぼえていますか〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5R5EEO5DDDDD5O4F+F+&F+G+&G+5R7"
  // それははじめての〜
  "L3O5C+C+C+C+C+5O4EE&EF+&F+5REEEO5DDDD&D5O4F+F+&F+G+&G+5R5O4E5B5&BA&A5A5&A9";


/*
 * 内部関数の宣言
 */
static void GameStart(void);
static void GamePlay(void);
static void GameClear(void);
static void GameOver(void);
static void GameEnd(void);
static void GameHit(void);
static void GameUpdateStatus(void);
static void GameRenderStatus(void);


/*
 * ゲームを初期化する
 */
void GameInitialize(void)
{
  // 背景の初期化
  BackInitialize();
  
  // プレイヤの初期化
  PlayerInitialize();

  // 自弾の初期化
  ShotInitialize();

  // エネミーの初期化
  EnemyInitialize();

  // 敵弾の初期化
  BulletInitialize();

  // スコアの初期化
  gameScore = 0x0000;
  
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

  // 開始
  case GAME_STATE_START:
    GameStart();
    break;

  // プレイ
  case GAME_STATE_PLAY:
    GamePlay();
    break;

  // クリア
  case GAME_STATE_CLEAR:
    GameClear();
    break;
    
  // オーバー
  case GAME_STATE_OVER:
    GameOver();
    break;
    
  // 終了
  case GAME_STATE_END:
    GameEnd();
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

    // 背景の設定
    BackSetStage(BACK_STAGE_ZAKO);
    
    // プレイヤの設定
    PlayerSetState(PLAYER_STATE_START);

    // サウンドの開始
    SystemRequestSound(gameBgm, true);

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // 背景の更新
    BackUpdate();
    
    // プレイヤの更新
    PlayerUpdate();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);
  
    // 背景の描画
    BackRender();
    
    // プレイヤの描画
    PlayerRender();
  
    // ステータスの描画
    GameRenderStatus();
  
    // 描画の完了
    SystemDisplayVideo();
  }

  // プレイヤの監視
  if (!PlayerIsState(PLAYER_STATE_START)) {

    // 状態の更新
    gameState = GAME_STATE_PLAY;
  }
}

/*
 * ゲームをプレイする
 */
static void GamePlay(void)
{
  // 状態別の処理
  switch (gameState & 0x0f) {

  // 0x00: ザコ戦の開始
  case 0x00:
    {
      PlayerSetState(PLAYER_STATE_PLAY);
      EnemySetStage(ENEMY_STAGE_ZAKO);
      ++gameState;
    }
    break;

  // 0x01: ザコ戦の監視
  case 0x01:
    {
      if (!BackIsStage(BACK_STAGE_ZAKO)) {
        EnemySetStage(ENEMY_STAGE_NULL);
        ++gameState;
      }
    }
    break;

  // 0x02: ザコ戦の完了
  case 0x02:
    {
      if (!EnemyIsLive()) {
        ++gameState;
      }
    }
    break;

  // 0x03: ボス戦の準備
  case 0x03:
    {
      BackSetStage(BACK_STAGE_WARNING);
      PlayerSetState(PLAYER_STATE_PLAY);
      ++gameState;
    }
    break;

  // 0x04: ボス戦の開始
  case 0x04:
    {
      if (!BackIsStage(BACK_STAGE_WARNING)) {
        EnemySetStage(ENEMY_STAGE_BOSS);
        ++gameState;
      }
    }
    break;

  // 0x05: ボス戦の監視
  case 0x05:
    {
      if (EnemyGetLife(ENEMY_TYPE_BODDOLE_ZER) == 0x00) {
        BackSetStage(BACK_STAGE_FINISH);
        PlayerSetState(PLAYER_STATE_STAY);
        EnemyKill();
        EnemySetStage(ENEMY_STAGE_FINISH);
        BulletKill();
        ++gameState;
      }
    }
    break;

  // 0x06: ボス戦の完了
  case 0x06:
    {
      if (!BackIsStage(BACK_STAGE_FINISH)) {
        gameState = GAME_STATE_CLEAR;
      }
    }
    break;

  // その他
  default:
    break;
  }

  // 更新
  {
    // ヒット判定
    GameHit();
    
    // 背景の更新
    BackUpdate();
    
    // プレイヤの更新
    PlayerUpdate();
  
    // 自弾の更新
    ShotUpdate();
  
    // エネミーの更新
    EnemyUpdate();
  
    // 敵弾の更新
    BulletUpdate();
  
    // ステータスの更新
    GameUpdateStatus();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);
  
    // 背景の描画
    BackRender();
    
    // プレイヤの描画
    PlayerRender();
  
    // エネミーの描画
    EnemyRender();
  
    // 敵弾の描画
    BulletRender();
  
    // 自弾の描画
    ShotRender();
  
    // ステータスの描画
    GameRenderStatus();
  
    // 描画の完了
    SystemDisplayVideo();
  }

  // プレイヤの監視
  if (!PlayerIsLive()) {

    // 状態の更新
    gameState = GAME_STATE_OVER;
  }
}

/*
 * ゲームをクリアする
 */
static void GameClear(void)
{
  // 状態別の処理
  switch (gameState & 0x0f) {
    
  // 0x00: 初期化
  case 0x00:
    {
      BackSetStage(BACK_STAGE_CLEAR);
      PlayerSetState(PLAYER_STATE_CLEAR);
      EnemySetStage(ENEMY_STAGE_NULL);
      ++gameState;
    }
    break;

  // 0x01: プレイヤの移動の監視
  case 0x01:
    {
      if (!PlayerIsState(PLAYER_STATE_CLEAR)) {
        gameFrame = 0x0000;
        ++gameState;
      }
    }
    break;

  // 0x02: 残りの耐久力をスコアに加算
  case 0x02:
    {
      if (++gameFrame > 0x0010) {
        uint8_t life = PlayerGetLife();
        if (life > 0x00) {
          --life;
          gameScore += 0x32;
          gameFrame = 0x0000;
          PlayerSetLife(life);
        } else {
          PlayerSetState(PLAYER_STATE_END);
          ++gameState;
        }
      }
    }
    break;

  // 0x03: プレイヤの退場の監視
  case 0x03:
    {
      if (!PlayerIsState(PLAYER_STATE_END)) {
        gameFrame = 0x0000;
        ++gameState;
      }
    }
    break;

  // 0x04: 待機
  case 0x04:
    {
      if (++gameFrame > 0x0008 * 0x0040) {
        gameState = GAME_STATE_END;
      }
    }
    break;

  // その他
  default:
    break;
  }

  // 更新
  {
    // 背景の更新
    BackUpdate();
    
    // プレイヤの更新
    PlayerUpdate();
  
    // エネミーの更新
    EnemyUpdate();
  
    // ステータスの更新
    GameUpdateStatus();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);
  
    // 背景の描画
    BackRender();
    
    // プレイヤの描画
    PlayerRender();
  
    // エネミーの描画
    EnemyRender();
  
    // ステータスの描画
    GameRenderStatus();
  
    // ゲームクリアの描画
    if ((gameState & 0x0f) == 0x04) {
      int8_t length = gameFrame < 0x0018 ? gameFrame : 0x0018;
      AppDrawString(gameStringClear, 0x10, 0x1d, length);
    }
  
    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * ゲームオーバーになる
 */
static void GameOver(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // エネミーの設定
    EnemySetStage(ENEMY_STAGE_NULL);

    // フレームの設定
    gameFrame = 0x0000;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // 背景の更新
    BackUpdate();
    
    // プレイヤの更新
    PlayerUpdate();
  
    // 自弾の更新
    ShotUpdate();
  
    // エネミーの更新
    EnemyUpdate();
  
    // 敵弾の更新
    BulletUpdate();
  
    // ステータスの更新
    GameUpdateStatus();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);
  
    // 背景の描画
    BackRender();
    
    // プレイヤの描画
    PlayerRender();
  
    // エネミーの描画
    EnemyRender();
  
    // 敵弾の描画
    BulletRender();
  
    // 自弾の描画
    ShotRender();
  
    // ステータスの描画
    GameRenderStatus();

    // ゲームオーバーの描画
    if (gameFrame > 0x0060) {
      SystemClipByte(0x1f, 0x2e, 0x1d, 0x24);
      AppDrawString(gameStringOver, 0x2e, 0x1d);
    }
  
    // 描画の完了
    SystemDisplayVideo();
  }

  // 状態の更新
  ++gameFrame;
  if (gameFrame > 0x0008 * 0x0040 || (gameFrame > 0x0002 * 0x0040 && SystemIsInputEdge(A_BUTTON | B_BUTTON))) {
    gameState = GAME_STATE_END;
  }
}

/*
 * ゲームを終了する
 */
static void GameEnd(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // フレームの設定
    gameFrame = 0x0000;

    // サウンドの停止
    SystemStopSound();

    // 状態の更新
    ++gameState;
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);
  
    // ハイスコアの描画
    if (gameScore > AppGetScore()) {
      AppDrawString(gameStringHiScore, 0x30, 0x17);
      AppDrawScore(gameScore, 0x34, 0x23);
    }
  
    // 描画の完了
    SystemDisplayVideo();
  }
  
  // アプリケーションの更新
  ++gameFrame;
  if (gameScore <= AppGetScore() || SystemIsInputEdge(A_BUTTON | B_BUTTON) || gameFrame == 0x0008 * 0x0040) {
    AppSetScore(gameScore);
    AppSetState(APP_STATE_TITLE_INITIALIZE);
  }
}

/*
 * ヒット判定を行う
 */
static void GameHit(void)
{
  // プレイヤの取得
  int8_t px, py;
  bool pf = PlayerGetPosition(&px, &py);

  // エネミーの判定
  for (int8_t e = 0x00; e < ENEMY_N; e++) {

    // 矩形の取得
    int8_t r[GAME_RECT_SIZE];
    if (EnemyGetRect(e, r)) {

      // 自弾との判定
      for (int8_t s = 0x00; s < SHOT_N; s++) {
        int8_t sx, sy;
        if (ShotGetPosition(s, &sx, &sy)) {
          if (r[GAME_RECT_LEFT] <= sx && sx <= r[GAME_RECT_RIGHT] && r[GAME_RECT_TOP] <= sy && sy <= r[GAME_RECT_BOTTOM]) {
            gameScore += EnemyDamage(e);
            ShotHit(s);
          }
        }
      }

      // プレイヤとの判定
      if (pf) {
        if (r[GAME_RECT_LEFT] <= px && px <= r[GAME_RECT_RIGHT] && r[GAME_RECT_TOP] <= py && py <= r[GAME_RECT_BOTTOM]) {
          PlayerDamage();
        }
      }
    }
  }

  // 敵弾の判定
  for (int8_t b = 0x00; b < BULLET_N; b++) {
    
    // 矩形の取得
    int8_t r[GAME_RECT_SIZE];
    if (BulletGetRect(b, r)) {

      // プレイヤとの判定
      if (pf) {
        if (r[GAME_RECT_LEFT] <= px && px <= r[GAME_RECT_RIGHT] && r[GAME_RECT_TOP] <= py && py <= r[GAME_RECT_BOTTOM]) {
          PlayerDamage();
          BulletKill(b);
        }
      }
    }
  }
}

/*
 * ステータスを更新する
 */
static void GameUpdateStatus(void)
{
  // スコアの更新
  {
    if (gameScore > APP_SCORE) {
      gameScore = APP_SCORE;
    }
  }
}

/*
 * ステータスを描画する
 */
static void GameRenderStatus(void)
{
  // ライフの描画
  {
    uint8_t life = PlayerGetLife();
    uint8_t x = 0x00;
    uint8_t i = 0x00;
    while (i < life) {
      AppDrawPattern(0x000c, x, 0x00, 0x0c, SYSTEM_VIDEO_FLIP_NORMAL);
      x += 0x09;
      ++i;
    }
    while (i < PLAYER_LIFE) {
      AppClipPattern(0x000e, 0x000c, x, 0x00, 0x0c, SYSTEM_VIDEO_FLIP_NORMAL);
      x += 0x09;
      ++i;
    }
  }

  // スコアの描画
  {
    AppDrawString(gameStringScore, 0x50, 0x00);
    AppDrawScore(gameScore, 0x68, 0x00);
  }

  // ボドルザーの耐久力の描画
  {
    uint8_t life = EnemyGetLife(ENEMY_TYPE_BODDOLE_ZER);
    if (life > 0x00) {
      SystemClipByte(0x3f, 0x0c, 0x3a, 0xc0);
      AppDrawPattern(0x0020, 0x0c, 0x3a, 0xc0, SYSTEM_VIDEO_FLIP_NORMAL);
      SystemDrawByte(0x0c, 0x0e, 0x3a, (life + 0x01) / 0x02);
    }
  }
}

