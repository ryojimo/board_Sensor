/**************************************************************************//*!
 *  @file           opt_cmd_relay.c
 *  @brief          [APP] オプション・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2020.01.19
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <getopt.h>

#include "../../hal/hal.h"

#include "../if_button/if_button.h"
#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

//#define DBG_PRINT
#define MY_NAME "OPT"
#include "../log/log.h"


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// getopt() で使用
extern char *optarg;
extern int  optind, opterr, optopt;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void Help( void );


/**************************************************************************//*!
 * @brief     HELP を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Help(
    void
){
    DBG_PRINT_TRACE( "Help() \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( " Main option)                                 \n\r" );
    AppIfPc_Printf( "     -r, --relay : control the Relay circuit. \n\r" );
    AppIfPc_Printf( "                                              \n\r" );
    AppIfPc_Printf( " Sub option)                                  \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.      \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                  \n\r" );
    AppIfPc_Printf( "     -n, --on   : turn on the relay circuit.  \n\r" );
    AppIfPc_Printf( "     -f, --off  : turn off the relay circuit. \n\r" );
    AppIfPc_Printf( "                                              \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                 \n\r" );
    AppIfPc_Printf( "     -r       -h     \n\r" );
    AppIfPc_Printf( "     --relay  --help \n\r" );
    AppIfPc_Printf( "     -r       -m     \n\r" );
    AppIfPc_Printf( "     --relay  --menu \n\r" );
    AppIfPc_Printf( "     -r       -n     \n\r" );
    AppIfPc_Printf( "     --relay  --on   \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_RelayMenu(
    void
){
    EHalBool_t      status = EN_FALSE;
    EHalBool_t      flag = EN_FALSE;

    SHalSensor_t*   data;       ///< センサデータの構造体 : ポテンショメーター
    int             start = 0;  ///< 開始時のポテンショメーターの値 ( % )
    int             stop = 0;   ///< 終了時のポテンショメーターの値 ( % )

    DBG_PRINT_TRACE( "OptCmd_RelayMenu() \n\r" );

    AppIfPc_Printf( "if you change the value of PM, break.\n\r" );

    // ポテンショメーターのデータを取得
    data = HalSensorPm_Get();
    start = data->cur_rate / 10;
    stop  = data->cur_rate / 10;

    // ポテンショメーターが動かされるまでループ
    while( start - 1 <= stop && stop <= start + 1 )
    {
        // プッシュスイッチで flag に EN_TRUE or EN_FALSE をセット
        status = HalPushSw_Get( EN_PUSH_SW_0 );
        if( status == EN_TRUE ){ flag = EN_TRUE; }

        status = HalPushSw_Get( EN_PUSH_SW_1 );
        if( status == EN_TRUE ){ flag = EN_FALSE; }

        // リレー制御
        switch(flag)
        {
        case EN_FALSE: HalRelay_Set( EN_LOW ); break;
        case EN_TRUE : HalRelay_Set( EN_HIGH ); break;
        default: break;
        }

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        switch(flag)
        {
        case EN_FALSE: AppIfLcd_Printf( "OFF             " ); break;
        case EN_TRUE : AppIfLcd_Printf( "ON              " ); break;
        default: break;
        }

        // PC ターミナル表示
        switch(flag)
        {
        case EN_FALSE: AppIfPc_Printf( "{ RELAY: OFF } \r" ); break;
        case EN_TRUE : AppIfPc_Printf( "{ RELAY: ON  } \r" ); break;
        default: break;
        }

        // ポテンショメーターの値を取得
        data = HalSensorPm_Get();
        stop = data->cur_rate / 10;
    }

    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_Relay(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hmnf";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "menu",   no_argument,       NULL,  'm' },
        { "on",     no_argument,       NULL,  'n' },
        { "off",    no_argument,       NULL,  'f' },
        { 0,        0,                 NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_Relay() \n\r" );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        // -1 : 処理するオプションが無くなった場合
        // '?': optstring で指定していない引数が見つかった場合
        if( opt == -1 )
        {
            break;
        }

        switch( opt )
        {
        case '?': DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': Help(); break;
        case 'm': OptCmd_RelayMenu(); break;
        case 'n': HalRelay_Set( EN_HIGH ); break;
        case 'f': HalRelay_Set( EN_LOW ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

