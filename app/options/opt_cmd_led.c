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
static void       Run( int data );
static void       RunIllumination( void );
static int        GetData( char* str );


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
    AppIfPc_Printf( " Sub option)                             \n\r" );
    AppIfPc_Printf( "     -h, --help         : display the help menu. \n\r" );
    AppIfPc_Printf( "     -i, --illumination : light up.              \n\r" );
    AppIfPc_Printf( "     -n, --num          : number to light up.    \n\r" );
    AppIfPc_Printf( "                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                \n\r" );
    AppIfPc_Printf( "     -l     -n  5   \n\r" );
    AppIfPc_Printf( "     --led  --num=5 \n\r" );
    AppIfPc_Printf( "     --l    -i      \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     LED を点灯する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run(
    int           data     ///< [in] LED を光らせる値
){
    DBG_PRINT_TRACE( "Run() \n\r" );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%02d", data );

    HalLed_Set( data );
    return;
}


/**************************************************************************//*!
 * @brief     イルミネーション点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
RunIllumination(
    void
){
    DBG_PRINT_TRACE( "RunIllumination() \n\r" );

    unsigned int    value = 0x1;

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // PC ターミナル表示
        AppIfPc_Printf( "LED : data = %02d \r", value );

        // LED 表示
        Run( value );
        value = ( value << 1 ) % 0x0F;

        // 500ms スリープ
        usleep( 500 * 1000 );
    }

    HalLed_Set( 0x00 );
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
GetData(
    char*   str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "GetData() \n\r" );
    int     data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    data = strtol( (const char*)str, NULL, 10 );
    DBG_PRINT_TRACE( "ch = %d \n\r", data );
    return data;
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
    const char      optstring[] = "hin:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,          has_arg,           *flag, val }, // 説明
        { "help",         no_argument,       NULL,  'h' },
        { "illumination", no_argument,       NULL,  'i' },
        { "num",          required_argument, NULL,  'n' },
        { 0,              0,                 NULL,   0  }, // termination
    };
    int             data = 0;
    int             endFlag = 0;

    DBG_PRINT_TRACE( "OptCmd_Led() \n\r" );
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
        case '?': endFlag = 1; DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': endFlag = 1; Help(); break;
        case 'i': endFlag = 1; RunIllumination(); break;
        case 'n':              data = GetData( optarg ); break;
        default: break;
        }
    }

    if( endFlag == 0 )
    {
        Run( data );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

