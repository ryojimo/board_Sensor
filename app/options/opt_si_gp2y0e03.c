/**************************************************************************//*!
 *  @file           opt_si_gp2y0e03.c
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
static void       Help( void );
static EHalBool_t IsEnterSw( void );
static void       Loop( void );


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
    AppIfPc_Printf( " Main option)                                                      \n\r" );
    AppIfPc_Printf( "     -x, --si_gp2y0e03 : get the value of a sensor(I2C), GP2Y0E03. \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( " Sub option)                                                     \n\r" );
    AppIfPc_Printf( "     -h, --help  : display the help menu.                        \n\r" );
    AppIfPc_Printf( "     -j, --json  : get the all values of json format.            \n\r" );
    AppIfPc_Printf( "     -l, --loop : get the all values until the PushSW is pushed. \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                       \n\r" );
    AppIfPc_Printf( "     -x             -j     \n\r" );
    AppIfPc_Printf( "     --si_gp2y0e03  --json \n\r" );
    AppIfPc_Printf( "     -x             -h     \n\r" );
    AppIfPc_Printf( "     --si_gp2y0e03  --help \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     Enter SW が押されたか？どうかを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static EHalBool_t
IsEnterSw(
    void
){
    return HalPushSw_Get( EN_PUSH_SW_2 );
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static void
Loop(
    void
){
    SHalSensor_t*   data;   ///< センサデータの構造体

    DBG_PRINT_TRACE( "Loop() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        // センサデータを取得
        data  = HalSensorGP2Y0E03_Get();

        // PC ターミナル表示
        AppIfPc_Printf( "distance = %02d cm \n\r", (int)data->cur );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02d cm", (int)data->cur );

        // 1 秒スリープ
        usleep( 1000 * 1000 );
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
Opt_Si_Gp2y0e03(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjl";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,   has_arg,     *flag, val }, // 説明
        { "help",  no_argument, NULL,  'h' },
        { "json",  no_argument, NULL,  'j' },
        { "loop",  no_argument, NULL,  'l' },
        { 0,       0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "Opt_Si_Gp2y0e03() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorGP2Y0E03_Get();

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
            AppIfLcd_Printf( "%5.2f cm", data->cur );

            AppIfPc_Printf( "{\"sensor\": \"si_gp2y0e03\", \"value\": %5.2f}",
                            data->cur );
            AppIfPc_Printf( "\n\r" );
        } else if( opt == 'l' )
        {
            Loop();
            break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

