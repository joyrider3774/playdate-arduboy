/*
 * Field.h : フィールド
 */
#ifndef _FIELD_H_
#define _FIELD_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define FIELD_STATE_NULL                (0x00)

// マップ
#define FIELD_MAP_WIDTH                 (0x10)
#define FIELD_MAP_HEIGHT                (0x10)

// 距離
#define FIELD_DISTANCE_FAR              (0x60)

// ビュー
#define FIELD_VIEW                      (0x30)
#define FIELD_VIEW_LEFT                 (0x10)
#define FIELD_VIEW_RIGHT                (0x6f)
#define FIELD_VIEW_HEIGHT               (0x30)


/*
 *  型宣言
 */


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void FieldInitialize(void);
extern void FieldUpdate(void);
extern void FieldRender(void);
extern void FieldSetCamera(uint8_t positionX, uint8_t positionY, uint8_t angle);
extern uint8_t FieldGetMap(uint8_t positionX, uint8_t positionY);
extern uint8_t FieldGetDistanceHeight(uint8_t distance);
extern uint8_t FieldGetViewDistance(int16_t x);
extern uint8_t FieldGetViewMap(int16_t x);
extern void FieldSetViewDistanceAndMap(int16_t x, uint8_t distance, uint8_t map);


#endif

