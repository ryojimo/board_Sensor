/**************************************************************************//*!
 *  @file           opt_i2c_ads1015.c
 *  @brief          [APP] オプション・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.02.14
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
static void       GetData( int ch );
static void       GetJson( void );
static int        GetChannel( char* str );


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
    AppIfPc_Printf( "     -b, --i2cads1015 : control the (I2C) ADS1015.                 \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( " Sub option)                                                       \n\r" );
    AppIfPc_Printf( "     -h, --help  : display the help menu.                          \n\r" );
    AppIfPc_Printf( "     -j, --json  : get the all values of json format.              \n\r" );
    AppIfPc_Printf( "     -m, --menu  : menu mode.                                      \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( "     -c number, --ch=number : the target channnel.                 \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                                                               \n\r" );
    AppIfPc_Printf( "     -b            -c   <number>                                   \n\r" );
    AppIfPc_Printf( "     --i2cpca9685  --ch=<number>                                   \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータをまわす。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
GetData(
    int           ch      ///< [in] 対象の channel
){
    DBG_PRINT_TRACE( "GetData() \n\r" );
    unsigned int  data;

    if( 0 <= ch && ch <= 3 )
    {
        data = HalI2cAds1015_Get( ch );

        // LCD に表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%d (mV)",  data );

        // PC のターミナルに表示
        AppIfPc_Printf( "%d \n\r", data );
    } else
    {
        DBG_PRINT_ERROR( "invalid ch number error. Please input between 0 and 3. : %d \n\r", ch );
    }

    return;
}


/**************************************************************************//*!
 * @brief     Channel 番号を取得する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static int
GetChannel(
    char*   str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "GetChannel() \n\r" );
    int     ch;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    ch   = strtol( (const char*)str, NULL, 10 );
    DBG_PRINT_TRACE( "ch = %d \n\r", ch );
    return ch;
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
    DBG_PRINT_TRACE( "GetJson() \n\r" );
    unsigned int    data0;
    unsigned int    data1;
    unsigned int    data2;
    unsigned int    data3;

    data0 = HalI2cAds1015_Get( 0 );
    data1 = HalI2cAds1015_Get( 1 );
    data2 = HalI2cAds1015_Get( 2 );
    data3 = HalI2cAds1015_Get( 3 );

    AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", data0, data1, data2, data3 );

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "C0:%04d  C1:%04d", data0, data1 );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "C2:%04d  C3:%04d", data2, data3 );

    AppIfPc_Printf( "{\"sensor\": \"si_ads1015\", \"value\": {\"ch0\": %d, \"ch1\": %d, \"ch2\": %d, \"ch3\": %d}}",
                    data0,
                    data1,
                    data2,
                    data2 );
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
OptCmd_I2cAds1015Menu(
    void
){
    DBG_PRINT_TRACE( "OptCmd_I2cAds1015Menu() \n\r" );
    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_Clear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        GetJson();
        // 0.1 秒スリープ
        usleep( 100 * 1000 );
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
OptCmd_I2cAds1015(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmc:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "json",   no_argument,       NULL,  'j' },
        { "menu",   no_argument,       NULL,  'm' },
        { "ch",     required_argument, NULL,  'c' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    int             ch = 0;
    int             endFlag = 0;

    DBG_PRINT_TRACE( "OptCmd_I2cAds1015() \n\r" );
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
        case 'j': endFlag = 1; GetJson(); break;
        case 'm': endFlag = 1; OptCmd_I2cAds1015Menu(); break;
        case 'c':              ch   = GetChannel( optarg ); break;
        default: break;
        }
    }

    if( endFlag == 0 )
    {
        GetData( ch );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

