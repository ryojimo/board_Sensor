/**************************************************************************//*!
 *  @file           opt_cmd_led.c
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
#include <stdlib.h>

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
static int        GetValue( char* str );


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
    AppIfPc_Printf( " Main option)                     \n\r" );
    AppIfPc_Printf( "     -l, --led : control the LED. \n\r" );
    AppIfPc_Printf( "                                  \n\r" );
    AppIfPc_Printf( " Sub option)                              \n\r" );
    AppIfPc_Printf( "     -h, --help  : display the help menu. \n\r" );
    AppIfPc_Printf( "     -m, --menu  : menu mode.             \n\r" );
    AppIfPc_Printf( "     -c, --clear : clear to light up.     \n\r" );
    AppIfPc_Printf( "     -n, --num   : number to light up.    \n\r" );
    AppIfPc_Printf( "                                          \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                \n\r" );
    AppIfPc_Printf( "     -l     -h      \n\r" );
    AppIfPc_Printf( "     --led  --help  \n\r" );
    AppIfPc_Printf( "     -l     -m      \n\r" );
    AppIfPc_Printf( "     --led  --menu  \n\r" );
    AppIfPc_Printf( "     -l     -n  5   \n\r" );
    AppIfPc_Printf( "     --led  --num=5 \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     LED を光らせる値を取得する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static int
GetValue(
    char*   str     ///< [in] 文字列
){
    int     value;

    DBG_PRINT_TRACE( "GetValue() \n\r" );
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    value = strtol( (const char*)str, NULL, 10 );
    DBG_PRINT_TRACE( "value = %d \n\r", value );
    return value;
}


/**************************************************************************//*!
 * @brief     イルミネーション点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_LedMenu(
    void
){
    unsigned int    value = 0x1;

    DBG_PRINT_TRACE( "OptCmd_LedMenu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // LED 点灯
        HalLed_Set( value );
        value = ( value << 1 ) % 0x0F;

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%02X(cnt)  ", value );

        // PC ターミナル表示
        AppIfPc_Printf( "LED: { value: 0x%02X(cnt) } \r", value );

        // 500ms スリープ
        usleep( 500 * 1000 );
    }

    AppIfPc_Printf( "\n\r" );

    // LED 消灯
    HalLed_Set( 0x00 );
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
OptCmd_Led(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hmcn:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "menu",   no_argument,       NULL,  'm' },
        { "clear",  no_argument,       NULL,  'c' },
        { "num",    required_argument, NULL,  'n' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    int             value = 0;

    DBG_PRINT_TRACE( "OptCmd_Led() \n\r" );

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
        case 'm': OptCmd_LedMenu(); break;
        case 'c': HalLed_Set( 0x00 ); break;
        case 'n': value = GetValue( optarg );
                  AppIfLcd_CursorSet( 0, 1 );
                  AppIfLcd_Printf( "%02d(cnt)  ", value );
                  HalLed_Set( value );
        break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

