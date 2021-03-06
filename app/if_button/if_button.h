/**************************************************************************//*!
 *  @file           if_button.h
 *  @brief          [APP] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常関数 : App[モジュール名]_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2020.02.24
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_IF_BUTTON_H_
#define _APP_IF_BUTTON_H_


//********************************************************
/* include                                               */
//********************************************************
#include "../../hal/hal.h"


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
EHalBool_t      AppIfBtn_IsMinus( void );
EHalBool_t      AppIfBtn_IsPlus( void );
EHalBool_t      AppIfBtn_IsEnter( void );
EHalBool_t      AppIfBtn_IsPlusMinus( void );
EHalBool_t      AppIfBtn_IsAll( void );


#endif  /* _APP_IF_BUTTON_H_ */

