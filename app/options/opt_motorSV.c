/**************************************************************************//*!
 *  @file           opt_motorSV.c
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
    AppIfPc_Printf( " Main option)                                \n\r" );
    AppIfPc_Printf( "     -o, --motorsv : control the SAVO motor. \n\r" );
    AppIfPc_Printf( "                                             \n\r" );
    AppIfPc_Printf( " Sub option)                             \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu. \n\r" );
    AppIfPc_Printf( "     -d, --duty : duty ratio.            \n\r" );
    AppIfPc_Printf( "                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                     \n\r" );
    AppIfPc_Printf( "     -o         -d  5    \n\r" );
    AppIfPc_Printf( "     --motorsv  --duty=5 \n\r" );
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
Opt_MotorSV(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hd:";
    const struct    option longopts[] = {
      //{ *name,  has_arg,           *flag, val }, // 説明
        { "help", no_argument,       NULL,  'h' },
        { "duty", required_argument, NULL,  'd' },
        { 0,      0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    unsigned int    data;

    DBG_PRINT_TRACE( "Opt_MotorSV() \n\r" );

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
        } else if( opt == 'd' )
        {
            DBG_PRINT_TRACE( "optarg = %s \n\r", optarg );
            data = atoi( (const char*)optarg );
            DBG_PRINT_TRACE( "data = %d \n\r", data );
            HalMotorSV_SetPwmDuty( EN_MOTOR_CW, data );
//          usleep( 1000 * 1000 );  // 2s 待つ
            break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

