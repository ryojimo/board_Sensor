/**************************************************************************//*!
 *  @file           opt_menu.c
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
#include <string.h>
#include <getopt.h>

#include "../../hal/hal.h"
#include "../../sys/sys.h"

#include "../if_button/if_button.h"
#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"
#include "../menu/menu.h"

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
    AppIfPc_Printf( " Main option)                            \n\r" );
    AppIfPc_Printf( "     -m, --menu : go to the menu mode.   \n\r" );
    AppIfPc_Printf( "                                         \n\r" );
    AppIfPc_Printf( " Sub option)                             \n\r" );
    AppIfPc_Printf( "     -h, --help   : display the help menu. \n\r" );
    AppIfPc_Printf( "     -o, --option : option menu.           \n\r" );
    AppIfPc_Printf( "                                           \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                            \n\r" );
    AppIfPc_Printf( "     -m      -h                 \n\r" );
    AppIfPc_Printf( "     -m      -o      \"\"       \n\r" );
    AppIfPc_Printf( "     -m      -o      \"help\"   \n\r" );
    AppIfPc_Printf( "     -m      -o      \"led il\" \n\r" );
    AppIfPc_Printf( "     --menu  --help             \n\r" );
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
OptCmd_Menu(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "ho:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "option", required_argument, NULL,  'o' },
        { 0,        0,                 NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_Menu() \n\r" );
    AppIfLcd_Clear();
    Sys_ShowInfo();

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
        case 'o': AppMenu_Main( (unsigned char*)optarg ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

