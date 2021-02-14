/**************************************************************************//*!
 *  @file           opt_si_tsl2561.c
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
static void       GetData( EHalSensorTSL2561_t which );
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
    AppIfPc_Printf( " Main option)                                                    \n\r" );
    AppIfPc_Printf( "     -z, --si_tsl2561 : get the value of a sensor(I2C), TSL2561. \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf( " Sub option)                                                     \n\r" );
    AppIfPc_Printf( "     -h, --help      : display the help menu.                    \n\r" );
    AppIfPc_Printf( "     -j, --json      : get the all values of json format.        \n\r" );
    AppIfPc_Printf( "     -m, --menu  : menu mode.                                    \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf( "     -b, --broadband : get the value of ?.                       \n\r" );
    AppIfPc_Printf( "     -i, --ir        : get the value of ?.                       \n\r" );
    AppIfPc_Printf( "     -l, --lux       : get the value of LUX.                     \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                       \n\r" );
    AppIfPc_Printf( "     -z            -l      \n\r" );
    AppIfPc_Printf( "     --si_tsl2561  --lux   \n\r" );
    AppIfPc_Printf( "     -z            -h      \n\r" );
    AppIfPc_Printf( "     --si_tsl2561  --help  \n\r" );
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
    EHalSensorTSL2561_t     which   ///< [in] 対象のセンサ
){
    DBG_PRINT_TRACE( "GetData() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorTSL2561_Get( which );

    switch( which )
    {
    case EN_SEN_TSL2561_BROADBAND : AppIfLcd_Printf( "%5.2f (BB)",  data->cur ); break;
    case EN_SEN_TSL2561_IR        : AppIfLcd_Printf( "%5.2f (IR)",  data->cur ); break;
    case EN_SEN_TSL2561_LUX       : AppIfLcd_Printf( "%5.2f (LUX)", data->cur ); break;
    default                       : DBG_PRINT_ERROR( "Invalid argument. \n\r" ); break;
    }

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
    DBG_PRINT_TRACE( "GetJson() \n\r" );
    SHalSensor_t*   dataBB = NULL;    ///< 照度センサのデータ構造体
    SHalSensor_t*   dataIR = NULL;    ///< 照度センサのデータ構造体
    SHalSensor_t*   dataLUX = NULL;   ///< 照度センサのデータ構造体

    dataBB  = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
    dataIR  = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
    dataLUX = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

    AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", dataBB->cur, dataIR->cur, dataLUX->cur );

    AppIfPc_Printf( "{\"sensor\": \"si_tsl2561\", \"value\": {\"broadband\": %5.2f, \"ir\": %5.2f, \"lux\": %5.2f}}",
                    dataBB->cur,
                    dataIR->cur,
                    dataLUX->cur );
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
OptCmd_SiTsl2561Menu(
    void
){
    SHalSensor_t*   dataBb = NULL;      ///< 照度センサのデータ構造体
    SHalSensor_t*   dataIr = NULL;      ///< 照度センサのデータ構造体
    SHalSensor_t*   dataLux = NULL;     ///< 照度センサのデータ構造体

    DBG_PRINT_TRACE( "OptCmd_SiTsl2561Menu() \n\r" );
    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_Clear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        dataBb  = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
        dataIr  = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
        dataLux = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

        // PC ターミナル表示
        AppIfPc_Printf( "(broadband, Ir, Lux) = ( %5.2f, %5.2f, %5.2f ) \n\r",
                        dataBb->cur, dataIr->cur, dataLux->cur
                      );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "Ir : %5.2f     ", dataIr->cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "Lux: %5.2f     ", dataLux->cur );

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
OptCmd_SiTsl2561(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmbil";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,       has_arg,     *flag, val }, // 説明
        { "help",      no_argument, NULL,  'h' },
        { "json",      no_argument, NULL,  'j' },
        { "menu",      no_argument, NULL,  'm' },
        { "broadband", no_argument, NULL,  'b' },
        { "ir",        no_argument, NULL,  'i' },
        { "lux",       no_argument, NULL,  'l' },
        { 0,           0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SiTsl2561() \n\r" );
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
        case '?': DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': Help(); break;
        case 'j': GetJson(); break;
        case 'm': OptCmd_SiTsl2561Menu(); break;
        case 'b': GetData( EN_SEN_TSL2561_BROADBAND ); break;
        case 'i': GetData( EN_SEN_TSL2561_IR        ); break;
        case 'l': GetData( EN_SEN_TSL2561_LUX       ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

