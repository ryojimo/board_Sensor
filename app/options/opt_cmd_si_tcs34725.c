/**************************************************************************//*!
 *  @file           opt_cmd_si_tcs34725.c
 *  @brief          [APP] オプション・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.07.16
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
/*! @enum                                                */
//********************************************************
typedef enum tagEColor
{
    EN_RED   = 0x00,   ///< @var : RED
    EN_GREEN = 0x01,   ///< @var : GREEN
    EN_BLUE  = 0x02,   ///< @var : BLUE
    EN_CLEAR = 0x03    ///< @var : CLEAR
} EColor_t;


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
static void       GetData( EColor_t which );
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
    AppIfPc_Printf( "     -n, --si_tcs34725 : get the value of a sensor(I2C), TCS34725. \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( " Sub option)                                                       \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.                           \n\r" );
    AppIfPc_Printf( "     -j, --json : get the values of json format.                   \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                                       \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( "     -r, --red   : get the value, Red.                             \n\r" );
    AppIfPc_Printf( "     -g, --green : get the value, Green.                           \n\r" );
    AppIfPc_Printf( "     -b, --blue  : get the value, Blue.                            \n\r" );
    AppIfPc_Printf( "     -c, --clear : get the value, Clear.                           \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                       \n\r" );
    AppIfPc_Printf( "     -n             -h     \n\r" );
    AppIfPc_Printf( "     --si_tcs34725  --help \n\r" );
    AppIfPc_Printf( "     -n             -j     \n\r" );
    AppIfPc_Printf( "     --si_tcs34725  --json \n\r" );
    AppIfPc_Printf( "     -n             -r     \n\r" );
    AppIfPc_Printf( "     --si_tcs34725  --red  \n\r" );
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
    EColor_t        which   ///< [in] 対象の色
){
    SHalSensor_t    dataR;  ///< センサデータの構造体
    SHalSensor_t    dataG;  ///< センサデータの構造体
    SHalSensor_t    dataB;  ///< センサデータの構造体
    SHalSensor_t    dataC;  ///< センサデータの構造体

    SHalSensor_t*   data;   ///< センサデータの構造体へのポインタ

    DBG_PRINT_TRACE( "GetData() \n\r" );

    // センサデータを取得
    HalSensorTCS34725_Get( &dataR, &dataG, &dataB, &dataC );

    switch( which )
    {
    case EN_RED   : data = &dataR; break;
    case EN_GREEN : data = &dataG;  break;
    case EN_BLUE  : data = &dataB; break;
    case EN_CLEAR : data = &dataC; break;
    default       : break;
    }

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f", data->cur );

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
    SHalSensor_t    dataR;  ///< センサデータの構造体
    SHalSensor_t    dataG;  ///< センサデータの構造体
    SHalSensor_t    dataB;  ///< センサデータの構造体
    SHalSensor_t    dataC;  ///< センサデータの構造体

    DBG_PRINT_TRACE( "GetJson() \n\r" );

    // センサデータを取得
    HalSensorTCS34725_Get( &dataR, &dataG, &dataB, &dataC );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"si_tcs34725\", \"value\": {\"r\": %3.2f, \"g\": %3.2f, \"b\": %3.2f, \"c\": %3.2f}}",
                    dataR.cur,
                    dataG.cur,
                    dataB.cur,
                    dataC.cur
                   );
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
OptCmd_SiTcs34725Menu(
    void
){
    SHalSensor_t    dataR;  ///< センサデータの構造体
    SHalSensor_t    dataG;  ///< センサデータの構造体
    SHalSensor_t    dataB;  ///< センサデータの構造体
    SHalSensor_t    dataC;  ///< センサデータの構造体

    char            buff[APP_LCD_MAX_SCROLL];

    DBG_PRINT_TRACE( "OptCmd_SiTcs34725Menu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_ScrollClear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        HalSensorTCS34725_Get( &dataR, &dataG, &dataB, &dataC );

        // LCD スクロール表示
        memset( buff, '\0', sizeof(buff) );
        sprintf( buff, "R: %3.2f, G: %3.2f, B: %3.2f, C: %3.2f", dataR.cur, dataG.cur, dataB.cur, dataC.cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Scroll( buff );

        // PC ターミナル表示
        AppIfPc_Printf( "{ R: %3.2f, G: %3.2f, B: %3.2f, C: %3.2f } \n\r",
                        dataR.cur, dataG.cur, dataB.cur, dataC.cur
                      );
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
OptCmd_SiTcs34725(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmrgbc";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help",  no_argument, NULL,  'h' },
        { "json",  no_argument, NULL,  'j' },
        { "menu",  no_argument, NULL,  'm' },
        { "red",   no_argument, NULL,  'r' },
        { "green", no_argument, NULL,  'g' },
        { "blue",  no_argument, NULL,  'b' },
        { "clear", no_argument, NULL,  'c' },
        { 0,       0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SiTcs34725() \n\r" );

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
        case 'm': OptCmd_SiTcs34725Menu(); break;
        case 'r': GetData( EN_RED   ); break;
        case 'g': GetData( EN_GREEN ); break;
        case 'b': GetData( EN_BLUE  ); break;
        case 'c': GetData( EN_CLEAR ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

