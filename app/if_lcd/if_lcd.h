/**************************************************************************//*!
 *  @file           if_lcd.h
 *  @brief          [APP] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常関数 : App[モジュール名]_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.09
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_IF_LCD_H_
#define _APP_IF_LCD_H_


//********************************************************
/* include                                               */
//********************************************************
#include "../../hal/hal.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define APP_LCD_MAX_X       (16)    ///< @def : LCD に表示可能な最大文字数 : X 軸 ( 0 - 15 )
#define APP_LCD_MAX_Y       (2)     ///< @def : LCD に表示可能な最大数     : Y 軸 ( 0 -  1 )
#define APP_LCD_MAX_SCROLL  (64)    ///< @def : スクロール表示させる際の最大文字数


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
void AppIfLcd_Entry( int curDir, int sftDisp );
void AppIfLcd_Ctrl( int disp, int curDisp, int blkDisp );
void AppIfLcd_Shift( int tgt, int dir );
void AppIfLcd_CursorHome( void );
void AppIfLcd_CursorSet( int x, int y );
void AppIfLcd_Clear( void );

int  AppIfLcd_Putc( int c );
int  AppIfLcd_Puts( const char* str );
int  AppIfLcd_Printf( const char* format, ... );

void AppIfLcd_ScrollClear( void );
int  AppIfLcd_Scroll( const char* str );


#endif /* _APP_IF_LCD_H_ */

