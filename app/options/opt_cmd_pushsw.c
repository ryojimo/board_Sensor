/**************************************************************************//*!
 *  @file           opt_pushsw.c
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
static void       Help( void );


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
    AppIfPc_Printf( " Main option)                                \n\r" );
    AppIfPc_Printf( "     -s, --switch : control the push switch. \n\r" );
    AppIfPc_Printf( "                                             \n\r" );
    AppIfPc_Printf( " Sub option)                             \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu. \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.             \n\r" );
    AppIfPc_Printf( "                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                  \n\r" );
    AppIfPc_Printf( "     -s        -m     \n\r" );
    AppIfPc_Printf( "     --switch  --menu \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     テストする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_PushSwitchMenu(
    void
){
    EHalBool_t      status = EN_FALSE;
    unsigned int    data0 = 0;  ///< PUSH SWITCH の押された回数
    unsigned int    data1 = 0;  ///< PUSH SWITCH の押された回数
    unsigned int    data2 = 0;  ///< PUSH SWITCH の押された回数

    SHalSensor_t*   data;       ///< センサデータの構造体 : ポテンショメーター
    int             start = 0;  ///< 開始時のポテンショメーターの値 ( % )
    int             stop = 0;   ///< 終了時のポテンショメーターの値 ( % )

    DBG_PRINT_TRACE( "OptCmd_PushSwitchMenu() \n\r" );
    AppIfPc_Printf( "if you change the value of PM, break.\n\r" );
    AppIfLcd_Clear();

    // ポテンショメーターのデータを取得
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
OptCmd_PushSwitch(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hm";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "menu", no_argument, NULL,  'm' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_PushSwitch() \n\r" );
    AppIfLcd_CursorSet( 0, 1 );

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
        case 'm': OptCmd_PushSwitchMenu(); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

