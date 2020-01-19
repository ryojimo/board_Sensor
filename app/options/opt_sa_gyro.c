/**************************************************************************//*!
 *  @file           opt_sa_gyro.c
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
    AppIfPc_Printf( " Main option)                                              \n\r" );
    AppIfPc_Printf( "     -g, --sa_gyro : get the value of a sensor(A/D), Gyro. \n\r" );
    AppIfPc_Printf( "                                                           \n\r" );
    AppIfPc_Printf( " Sub option)                                               \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.             \n\r" );
    AppIfPc_Printf( "     -x, --g1   : get the value of g1-axis.          \n\r" );
    AppIfPc_Printf( "     -y. --g2   : get the value of g2-axis.          \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format. \n\r" );
    AppIfPc_Printf( "                                                     \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                   \n\r" );
    AppIfPc_Printf( "     -g         -x     \n\r" );
    AppIfPc_Printf( "     --sa_gyro  --g1   \n\r" );
    AppIfPc_Printf( "     -g         -h     \n\r" );
    AppIfPc_Printf( "     --sa_gyro  --help \n\r" );
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
Opt_Sa_Gyro(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjxy";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "g1",   no_argument, NULL,  'x' },
        { "g2",   no_argument, NULL,  'y' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "Opt_Sa_Gyro() \n\r" );
    SHalSensor_t*   dataG1;
    SHalSensor_t*   dataG2;

    dataG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
    dataG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );

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
        } else if( opt == 'j' )
        {
            AppIfLcd_Printf( "%04X, %04X", (int)dataG1->cur, (int)dataG2->cur );

            AppIfPc_Printf( "{\"sensor\": \"sa_gyro\", \"value\": {\"g1\": %d, \"g2\": %d}}",
                            (int)dataG1->cur,
                            (int)dataG2->cur );
            AppIfPc_Printf( "\n\r" );
        }

        switch( opt )
        {
        case 'x': AppIfLcd_Printf( "0x%04X", (int)dataG1->cur ); AppIfPc_Printf( "%d \n\r", (int)dataG1->cur ); break;
        case 'y': AppIfLcd_Printf( "0x%04X", (int)dataG2->cur ); AppIfPc_Printf( "%d \n\r", (int)dataG2->cur ); break;
        default: break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

