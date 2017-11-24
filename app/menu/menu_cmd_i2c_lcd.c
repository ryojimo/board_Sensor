/**************************************************************************//*!
 *  @file           cmd_i2c_lcd.c
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
#define OPTION_NUM  (3)


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static EAppMenuMsg_t   PrintFormat( void );
static EAppMenuMsg_t   Clear( void );
static EAppMenuMsg_t   Init( void );
static EAppMenuMsg_t   Display( void );


//********************************************************
/* Global Parameter  ( Local Scope )                    */
//********************************************************
/* オプションテーブル */
static const SAppMenuCmd_t g_optTable[] =
{
    { ""         , PrintFormat },
    { "h"        , PrintFormat },
    { "help"     , PrintFormat },
    { "clear"    , Clear },
    { "clean"    , Clear },
    { "init"     , Init },
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
    AppIfPc_Printf( "    1. lcd [OPTION01] \n\r" );
    AppIfPc_Printf( "        OPTION01 : clear : clear LCD Display.\n\r" );
    AppIfPc_Printf( "                   clean : clear LCD Display.\n\r" );
    AppIfPc_Printf( "                   init  : initialize LCD Display.\n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "        Ex.)\n\r" );
    AppIfPc_Printf( "            >lcd clear \n\r" );
    AppIfPc_Printf( "            >lcd CleAr \n\r" );
    AppIfPc_Printf( "            >lcd clean \n\r" );
    AppIfPc_Printf( "            >lcd CLEAN \n\r" );
    AppIfPc_Printf( "            >lcd init \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    2. lcd [OPTION01] [OPTION02] [OPTION03] \n\r" );
    AppIfPc_Printf( "        OPTION01 : X-direction. ( 0 -> 15 )\n\r" );
    AppIfPc_Printf( "        OPTION02 : Y-direction. ( 0 ->  1 )\n\r" );
    AppIfPc_Printf( "        OPTION03 : strings. \n\r" );
    AppIfPc_Printf( "        Ex.)\n\r" );
    AppIfPc_Printf( "            >lcd 0 0 TEST_PROGRAM \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LCD をクリアする。
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

    AppIfLcd_Clear();
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LCD を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Init(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    HalI2cLcd_Init();
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LCD に座標を指定して文字列を表示する。
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
    int             i;
    int             xy[2];

    DBG_PRINT_TRACE( "\n\r" );

    for( i = 0; i < OPTION_NUM - 1; i++ )
    {
        xy[i] = atoi( (const char*)&g_menuCmd[i+1][0] );
    }

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        AppIfLcd_CursorSet( xy[0], xy[1] );
        AppIfLcd_Puts( (const char*)&g_menuCmd[3][0] );
    }

    AppIfPc_Printf( "\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     LCD に文字列を表示する。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション01
 *              g_menuCmd[2][] : オプション02
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_I2cLcd(
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

