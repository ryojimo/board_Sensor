/**************************************************************************//*!
 *  @file           menu_cmd_led.c
 *  @brief          [APP] メニュー・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
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
#include <stdlib.h>

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
#define OPTION_NUM  (1)


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static EAppMenuMsg_t    PrintFormat( void );
static EAppMenuMsg_t    Clear( void );
static EAppMenuMsg_t    Illumination( void );
static EAppMenuMsg_t    Display( void );


//********************************************************
/* Global Parameter  ( Local Scope )                    */
//********************************************************
/* オプションテーブル */
static const SAppMenuCmd_t g_optTable[] =
{
    { ""         , Illumination },
    { "h"        , PrintFormat },
    { "help"     , PrintFormat },
    { "clear"    , Clear },
    { "clean"    , Clear },
    { "il"       , Illumination },
    { "END    "  , NULL }
};


/**************************************************************************//*!
 * @brief     コマンドの書式を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
PrintFormat(
    void
){
    AppIfPc_Printf( "    led [OPTION01] \n\r" );
    AppIfPc_Printf( "        OPTION01 : number   : light LED. ( 1 -> 0xF )\n\r" );
    AppIfPc_Printf( "                   clear    : clear LED   \n\r" );
    AppIfPc_Printf( "                   il       : illuminate LED   \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    Ex.)\n\r" );
    AppIfPc_Printf( "        >led clear \n\r" );
    AppIfPc_Printf( "        >led 0x07  \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LED をクリアする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Clear(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    HalLed_Set( 0x00 );
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     イルミネーション点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Illumination(
    void
){
    unsigned int    value = 0x1;

    DBG_PRINT_TRACE( "\n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 4, 0 );
    AppIfLcd_Puts( "led" );

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        HalLed_Set( value );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%02d", value );

        usleep( 500 * 1000 );
        value = ( value << 1 ) % 0x0F;
    }

    AppIfPc_Printf( "\n\r" );

    HalLed_Set( 0x00 );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LED を点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static EAppMenuMsg_t
Display(
    void
){
    int             op[ OPTION_NUM ];   ///< オプション
    int             i;

    DBG_PRINT_TRACE( "g_menuCmd[0] = %s \n\r", g_menuCmd[0] );
    DBG_PRINT_TRACE( "g_menuCmd[1] = %s \n\r", g_menuCmd[1] );

    for( i = 0; i < OPTION_NUM; i++ )
    {
        op[i] = atoi( (char*)&g_menuCmd[i+1][0] );
    }

    DBG_PRINT_TRACE( "op[0] = 0x%02X \n\r", op[0] );

    HalLed_Set( (unsigned char)op[0] );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LED を点灯する。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_Led(
    void
){
    EAppMenuMsg_t   ret;

    DBG_PRINT_TRACE( "\n\r" );

    ret = ExecuteCmd( &g_menuCmd[1][0], g_optTable );

    if( ret == EN_MENU_MSG_ACK )
    {
        Display();
    }

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

