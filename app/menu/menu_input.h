/**************************************************************************//*!
 *  @file           menu_input.h
 *  @brief          [APP] menu/ フォルダ内向け公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常関数 : MenuInput_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.07.04
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_MENU_INPUT_H_
#define _APP_MENU_INPUT_H_


//********************************************************
/* include                                               */
//********************************************************
// なし


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


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
void            MenuInput_Init( void );

EHalBool_t      IsMinusSw( void );
EHalBool_t      IsPlusSw( void );
EHalBool_t      IsEnterSw( void );
EHalBool_t      IsPlusMinusSw( void );
EHalBool_t      IsAllSw( void );

void            MenuInput_Display( int no );

void            MenuInput_Switch_Clear( void );
EAppMenuMsg_t   MenuInput_Switch_Get( int* no );

void            MenuInput_Volume_Clear( void );
EAppMenuMsg_t   MenuInput_Volume_Get( int* no );


#endif  /* _APP_MENU_INPUT_H_ */

