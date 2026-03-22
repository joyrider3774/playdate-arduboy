/*
 * Game.h : ゲーム
 */
#ifndef _GAME_H_
#define _GAME_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define GAME_STATE_NULL                 (0x00)
#define GAME_STATE_START                (0x10)
#define GAME_STATE_PLAY                 (0x20)
#define GAME_STATE_CLEAR                (0x30)
#define GAME_STATE_OVER                 (0x40)
#define GAME_STATE_END                  (0x50)

// ビュー
#define GAME_VIEW_OX                    (0x40)
#define GAME_VIEW_OY                    (0x20)
#define GAME_VIEW_LEFT                  (-0x40)
#define GAME_VIEW_RIGHT                 (+0x3f)
#define GAME_VIEW_TOP                   (-0x20)
#define GAME_VIEW_BOTTOM                (+0x1f)

// 矩形
#define GAME_RECT_LEFT                  (0x00)
#define GAME_RECT_RIGHT                 (0x01)
#define GAME_RECT_TOP                   (0x02)
#define GAME_RECT_BOTTOM                (0x03)
#define GAME_RECT_SIZE                  (0x04)

// ステータス
#define GAME_STATUS_HEIGHT              (0x05)


/*
 *  型宣言
 */


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void GameInitialize(void);
extern void GameUpdate(void);


#endif

