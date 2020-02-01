/**************************************************************************//*!
 *  @file           opt_si_bme280.c
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
static EHalBool_t IsEnterSw( void );
static void       Help( void );


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
    AppIfPc_Printf( " Sub option)                                                     \n\r" );
    AppIfPc_Printf( "     -h, --help  : display the help menu.                        \n\r" );
    AppIfPc_Printf( "     -a, --atmos : get the value of atmosphere.                  \n\r" );
    AppIfPc_Printf( "     -u, --humi  : get the value of humidity.                    \n\r" );
    AppIfPc_Printf( "     -t, --temp  : get the value of temperature.                 \n\r" );
    AppIfPc_Printf( "     -j, --json  : get the all values of json format.            \n\r" );
    AppIfPc_Printf( "     -l, --loop : get the all values until the PushSW is pushed. \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
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
 * @brief     気圧センサのデータを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Opt_SiBme280Atmos(
    void
){
    DBG_PRINT_TRACE( "Opt_SiBme280Atmos() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    AppIfLcd_Printf( "%5.2f (hPa)", data->cur );
    AppIfPc_Printf( "%5.2f \n\r", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     湿度センサのデータを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Opt_SiBme280Humi(
    void
){
    DBG_PRINT_TRACE( "Opt_SiBme280Humi() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    AppIfLcd_Printf( "%5.2f (%%)",  data->cur );
    AppIfPc_Printf( "%5.2f \n\r", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     温度センサのデータを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Opt_SiBme280Temp(
    void
){
    DBG_PRINT_TRACE( "Opt_SiBme280Temp() \n\r" );
    SHalSensor_t*   data;

    data = HalSensorBME280_Get( EN_SEN_BME280_TEMP );
    AppIfLcd_Printf( "%5.2f ('C)",  data->cur );
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
void
Opt_SiBme280Json(
    void
){
    DBG_PRINT_TRACE( "Opt_SiBme280Json() \n\r" );
    SHalSensor_t*   dataAtmos = NULL;   ///< 気圧センサのデータ構造体
    SHalSensor_t*   dataHumi  = NULL;   ///< 湿度センサのデータ構造体
    SHalSensor_t*   dataTemp  = NULL;   ///< 温度センサのデータ構造体

    dataAtmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    dataHumi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    dataTemp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

    AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", dataAtmos->cur, dataHumi->cur, dataTemp->cur );

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
Opt_SiBme280Loop(
    void
){
    DBG_PRINT_TRACE( "Opt_SiBme280Loop() \n\r" );

    SHalSensor_t*   dataAtmos = NULL;   ///< 気圧センサのデータ構造体
    SHalSensor_t*   dataHumi  = NULL;   ///< 湿度センサのデータ構造体
    SHalSensor_t*   dataTemp  = NULL;   ///< 温度センサのデータ構造体

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
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
Opt_Si_Bme280(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjlaut";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,   has_arg,     *flag, val }, // 説明
        { "help",  no_argument, NULL,  'h' },
        { "json",  no_argument, NULL,  'j' },
        { "loop",  no_argument, NULL,  'l' },
        { "atmos", no_argument, NULL,  'a' },
        { "humi",  no_argument, NULL,  'u' },
        { "temp",  no_argument, NULL,  't' },
        { 0,       0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "Opt_Si_Bme280() \n\r" );
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
        case 'j': Opt_SiBme280Json(); break;
        case 'l': Opt_SiBme280Loop(); break;
        case 'a': Opt_SiBme280Atmos(); break;
        case 'u': Opt_SiBme280Humi(); break;
        case 't': Opt_SiBme280Temp(); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif
