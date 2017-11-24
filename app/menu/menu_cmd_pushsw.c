/**************************************************************************//*!
 *  @file           menu_cmd_pushsw.c
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
static EAppMenuMsg_t    Exec( void );


//********************************************************
/* Global Parameter  ( Local Scope )                    */
//********************************************************
/* オプションテーブル */
static const SAppMenuCmd_t g_optTable[] =
{
    { ""         , Exec },
    { "h"        , PrintFormat },
    { "help"     , PrintFormat },
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
    AppIfPc_Printf( "    pushsw     \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    Ex.)\n\r" );
    AppIfPc_Printf( "        >pushsw \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Exec(
    void
){
    EHalBool_t      status = EN_FALSE;
    unsigned int    data0 = 0;  ///< PUSH SWITCH の押された回数
    unsigned int    data1 = 0;  ///< PUSH SWITCH の押された回数
    unsigned int    data2 = 0;  ///< PUSH SWITCH の押された回数

    SHalSensor_t*   data;       ///< センサデータの構造体 : ポテンショメーター
    int             start = 0;  ///< 開始時のポテンショメーターの値 ( % )
    int             stop = 0;   ///< 終了時のポテンショメーターの値 ( % )

    DBG_PRINT_TRACE( "\n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "SW0= SW1= SW2= " );

    // センサデータを取得
    data = HalSensorPm_Get();
    start = data->cur_rate / 10;
    stop  = data->cur_rate / 10;

    // ポテンショメーターが動かされるまでループ
    while( start - 1 <= stop && stop <= start + 1 )
    {
        // PUSH SWITCH の押された回数を取得
        status = HalPushSw_Get( EN_PUSH_SW_0 );
        if( status == EN_TRUE ){ data0++; }

        status = HalPushSw_Get( EN_PUSH_SW_1 );
        if( status == EN_TRUE ){ data1++; }

        status = HalPushSw_Get( EN_PUSH_SW_2 );
        if( status == EN_TRUE ){ data2++; }

        // PC ターミナル表示
        AppIfPc_Printf( "Push SW : SW0=%02d , SW1=%02d , SW2=%02d \r", data0, data1, data2 );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%4d %4d %4d", data0, data1, data2 );

        // LED 表示
        HalLed_Set( ( data2 % 2 ) << 2 | ( data1 % 2 ) << 1 | ( data0 % 2 ) << 0 );

        // ポテンショメーターの値を取得
        data = HalSensorPm_Get();
        stop = data->cur_rate / 10;
    }

    AppIfPc_Printf( "\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     PUSH SWITCH を押した回数を表示する。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_PushSw(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    ExecuteCmd( &g_menuCmd[1][0], g_optTable );

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

