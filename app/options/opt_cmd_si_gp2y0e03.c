/**************************************************************************//*!
 *  @file           opt_cmd_si_gp2y0e03.c
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
#include <string.h>

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
static void       GetData( void );
static void       GetJson( void );


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
    AppIfPc_Printf( " Sub option)                                                       \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.                           \n\r" );
    AppIfPc_Printf( "     -j, --json : get the values of json format.                   \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                                       \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( "     -d, --data : get the value.                                   \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                       \n\r" );
    AppIfPc_Printf( "     -x             -h     \n\r" );
    AppIfPc_Printf( "     --si_gp2y0e03  --help \n\r" );
    AppIfPc_Printf( "     -x             -j     \n\r" );
    AppIfPc_Printf( "     --si_gp2y0e03  --json \n\r" );
    AppIfPc_Printf( "     -x             -d     \n\r" );
    AppIfPc_Printf( "     --si_gp2y0e03  --data \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     データを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
GetData(
    void
){
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "GetData() \n\r" );

    // センサデータを取得
    data = HalSensorGP2Y0E03_Get();

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f(cm)", data->cur );

    // PC ターミナル表示
    AppIfPc_Printf( "%5.2f \n\r", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     JSON 形式でデータを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
GetJson(
    void
){
    SHalSensor_t*   data;   ///< センサデータの構造体

    DBG_PRINT_TRACE( "GetJson() \n\r" );

    // センサデータを取得
    data = HalSensorGP2Y0E03_Get();

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f(cm)", data->cur );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"si_gp2y0e03\", \"value\": %5.2f}",
                    data->cur );
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
void
OptCmd_SiGp2y0e03Menu(
    void
){
    SHalSensor_t*   data;   ///< センサデータの構造体

    char            buff[APP_LCD_MAX_SCROLL];

    DBG_PRINT_TRACE( "OptCmd_SiGp2y0e03Menu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_ScrollClear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        data  = HalSensorGP2Y0E03_Get();

        // LCD スクロール表示
        memset( buff, '\0', sizeof(buff) );
        sprintf( buff, "distance: %02d(cm)", (int)data->cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Scroll( buff );

        // PC ターミナル表示
        AppIfPc_Printf( "{ distance: %02d(cm) } \n\r", (int)data->cur );
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
OptCmd_SiGp2y0e03(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmd";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "menu", no_argument, NULL,  'm' },
        { "data", no_argument, NULL,  'd' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SiGp2y0e03() \n\r" );

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
        case 'j': GetJson(); break;
        case 'm': OptCmd_SiGp2y0e03Menu(); break;
        case 'd': GetData(); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

