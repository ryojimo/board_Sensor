/**************************************************************************//*!
 *  @file           menu_input.c
 *  @brief          [APP] メニュー・アプリ
 *  @author         Ryoji Morita
 *  @attention      menu_input_***.c 用共通関数を定義。
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.07.04
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "../../hal/hal.h"

#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

#include "menu_base.h"
#include "menu_input.h"

//#define DBG_PRINT
#define MY_NAME "APP"
#include "../log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MAX_HISTORY     (4)


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern const SAppMenuCmd_t  g_menuCmdTable[ MAX_CMD_NUM ];


//********************************************************
/* Global Parameter  ( File Scope )                      */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし




/**************************************************************************//*!
 * @brief     初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
MenuInput_Init(
    void
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     Enter SW が押されたか？どうかを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
EHalBool_t IsMinusSw( void ){ return HalPushSw_Get( EN_PUSH_SW_0 ); }
EHalBool_t IsPlusSw( void ){  return HalPushSw_Get( EN_PUSH_SW_1 ); }
EHalBool_t IsEnterSw( void ){ return HalPushSw_Get( EN_PUSH_SW_2 ); }

EHalBool_t IsPlusMinusSw( void )
{
    EHalBool_t      ret = EN_FALSE;
    if( IsMinusSw() && IsPlusSw() )
    {
        ret = EN_TRUE;
    }
    return ret;
}

EHalBool_t IsAllSw( void )
{
    EHalBool_t      ret = EN_FALSE;
    if( IsMinusSw() && IsPlusSw() && IsAllSw() )
    {
        ret = EN_TRUE;
    }
    return ret;
}


/**************************************************************************//*!
 * @brief     どのコマンドを選択してる？かを表示するための関数。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
MenuInput_Display(
    int     no      ///< [in] 表示するメニュー番号
){
//    DBG_PRINT_TRACE( "no = %d \n\r", no );

    // LED 表示
    HalLed_Set( no );

    // LCD 表示
    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( ">" );

    AppIfLcd_CursorSet( 1, 0 ); AppIfLcd_Printf( "%02d.", no );
    AppIfLcd_CursorSet( 4, 0 ); AppIfLcd_Puts( g_menuCmdTable[no].name );

    AppIfLcd_CursorSet( 1, 1 ); AppIfLcd_Printf( "%02d.", no + 1 );
    AppIfLcd_CursorSet( 4, 1 ); AppIfLcd_Puts( g_menuCmdTable[no + 1].name );
    return;
}


#ifdef __cplusplus
    }
#endif

