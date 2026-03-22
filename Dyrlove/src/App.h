/*
   App.h : アプリケーション
*/
#ifndef _APP_H_
#define _APP_H_

/*
   参照ファイル
*/


/*
   マクロの定義
*/

// 状態
#define APP_STATE_NULL                  (0x00)
#define APP_STATE_TITLE_INITIALIZE      (0x01)
#define APP_STATE_TITLE_UPDATE          (0x02)
#define APP_STATE_GAME_INITIALIZE       (0x03)
#define APP_STATE_GAME_UPDATE           (0x04)

// スコア
#define APP_SCORE                       (10000)


/*
    型宣言
*/


/*
   外部変数宣言
*/


/*
   外部関数宣言
*/
extern void AppInitialize(void);
extern void AppUpdate(void);
extern void AppSetState(uint8_t state);
extern uint16_t AppGetScore(void);
extern void AppSetScore(uint16_t score);
extern void AppDrawPattern(int16_t draw, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppClipPattern(int16_t draw, int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppMaskPattern(int16_t draw, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppDrawString(const char *string, int8_t x, int8_t y);
extern void AppDrawString(const char *string, int8_t x, int8_t y, int8_t length);
extern void AppDrawScore(uint16_t score, int8_t x, int8_t y);


#endif

