/**************************************************************************//*!
 *  @file           opt_led.c
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
    AppIfPc_Printf( " Main option)                     \n\r" );
    AppIfPc_Printf( "     -l, --led : control the LED. \n\r" );
    AppIfPc_Printf( "                                  \n\r" );
    AppIfPc_Printf( " Sub option)                             \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu. \n\r" );
    AppIfPc_Printf( "     -n, --num  : number to light up.    \n\r" );
    AppIfPc_Printf( "                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                \n\r" );
    AppIfPc_Printf( "     -l     -n  5   \n\r" );
    AppIfPc_Printf( "     --led  --num=5 \n\r" );
    AppIfPc_Printf("\x1b[39m");
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
Opt_Led(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hn:";
    const struct    option longopts[] = {
      //{ *name,  has_arg,           *flag, val }, // 説明
        { "help", no_argument,       NULL,  'h' },
        { "num",  required_argument, NULL,  'n' },
        { 0,      0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    unsigned int    num;

    DBG_PRINT_TRACE( "Opt_Led() \n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt );
            Help();
            goto err;
            break;
        } else if( opt == 'h' )
        {
            Help();
            goto err;
            break;
        } else if( opt == 'n' )
        {
            DBG_PRINT_TRACE( "optarg = %s \n\r", optarg );
            sscanf( optarg, "%X", &num );
            DBG_PRINT_TRACE( "num = %d \n\r", num );
            HalLed_Set( num );
            break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

