/**************************************************************************//*!
 *  @file           opt_cmd_si_bme280.c
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
static void       GetData( EHalSensorBME280_t which );
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
    AppIfPc_Printf( " Main option)                                                  \n\r" );
    AppIfPc_Printf( "     -w, --si_bme280 : get the value of a sensor(I2C), BME280. \n\r" );
    AppIfPc_Printf( "                                                               \n\r" );
    AppIfPc_Printf( " Sub option)                                                   \n\r" );
    AppIfPc_Printf( "     -h, --help  : display the help menu.                      \n\r" );
    AppIfPc_Printf( "     -j, --json  : get the all values of json format.          \n\r" );
    AppIfPc_Printf( "     -m, --menu  : menu mode.                                  \n\r" );
    AppIfPc_Printf( "                                                               \n\r" );
    AppIfPc_Printf( "     -a, --atmos : get the value of atmosphere.                \n\r" );
    AppIfPc_Printf( "     -u, --humi  : get the value of humidity.                  \n\r" );
    AppIfPc_Printf( "     -t, --temp  : get the value of temperature.               \n\r" );
    AppIfPc_Printf( "                                                               \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                      \n\r" );
    AppIfPc_Printf( "     -w           -a      \n\r" );
    AppIfPc_Printf( "     --si_bme280  --atmos \n\r" );
    AppIfPc_Printf( "     -w           -h      \n\r" );
    AppIfPc_Printf( "     --si_bme280  --help  \n\r" );
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
    EHalSensorBME280_t     which   ///< [in] 対象のセンサ
){
    DBG_PRINT_TRACE( "GetData() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorBME280_Get( which );

    switch( which )
    {
    case EN_SEN_BME280_ATMOS : AppIfLcd_Printf( "%5.2f (hPa)", data->cur ); break;
    case EN_SEN_BME280_HUMI  : AppIfLcd_Printf( "%5.2f (%%)",  data->cur ); break;
    case EN_SEN_BME280_TEMP  : AppIfLcd_Printf( "%5.2f ('C)",  data->cur ); break;
    default                  : DBG_PRINT_ERROR( "Invalid argument. \n\r" ); break;
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
    SHalSensor_t*   dataAtmos = NULL;   ///< 気圧センサのデータ構造体
    SHalSensor_t*   dataHumi  = NULL;   ///< 湿度センサのデータ構造体
    SHalSensor_t*   dataTemp  = NULL;   ///< 温度センサのデータ構造体

    // センサデータを取得
    dataAtmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    dataHumi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    dataTemp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "%5.2fhPa", dataAtmos->cur );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%3.2f%%  %3.2f'C", dataHumi->cur, dataTemp->cur );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"si_bme280\", \"value\": {\"atmos\": %5.2f, \"humi\": %5.2f, \"temp\": %5.2f}}",
                    dataAtmos->cur,
                    dataHumi->cur,
                    dataTemp->cur );
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
OptCmd_SiBme280Menu(
    void
){
    SHalSensor_t*   dataAtmos = NULL;   ///< 気圧センサのデータ構造体
    SHalSensor_t*   dataHumi  = NULL;   ///< 湿度センサのデータ構造体
    SHalSensor_t*   dataTemp  = NULL;   ///< 温度センサのデータ構造体

    DBG_PRINT_TRACE( "OptCmd_SiBme280Menu() \n\r" );
    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_Clear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        dataAtmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
        dataHumi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
        dataTemp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

        // PC ターミナル表示
        AppIfPc_Printf( "(atmos, humi, temp) = ( %5.2fhPa, %5.2f%%, %5.2f'C ) \n\r",
                        dataAtmos->cur, dataHumi->cur, dataTemp->cur
                      );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "%5.2fhPa", dataAtmos->cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3.2f%%  %3.2f'C", dataHumi->cur, dataTemp->cur );

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
OptCmd_SiBme280(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmaut";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,   has_arg,     *flag, val }, // 説明
        { "help",  no_argument, NULL,  'h' },
        { "json",  no_argument, NULL,  'j' },
        { "menu",  no_argument, NULL,  'm' },
        { "atmos", no_argument, NULL,  'a' },
        { "humi",  no_argument, NULL,  'u' },
        { "temp",  no_argument, NULL,  't' },
        { 0,       0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SiBme280() \n\r" );
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
        case 'm': OptCmd_SiBme280Menu(); break;
        case 'a': GetData( EN_SEN_BME280_ATMOS ); break;
        case 'u': GetData( EN_SEN_BME280_HUMI  ); break;
        case 't': GetData( EN_SEN_BME280_TEMP  ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

