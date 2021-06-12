/**************************************************************************//*!
 *  @file           opt_cmd_si_bmx055_gyro.c
 *  @brief          [APP] オプション・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.06.08
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
static void       GetData( EHalSensorBMX055_t which );
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
    AppIfPc_Printf( " Main option)                                            \n\r" );
    AppIfPc_Printf( "     -a, --si_bmx055_gyro : get the value of a sensor(I2C), BMX055 GYRO. \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf( " Sub option)                                             \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.                 \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format.     \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                             \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf( "     -x, --x    : get the value of x-axis.               \n\r" );
    AppIfPc_Printf( "     -y, --y    : get the value of y-axis.               \n\r" );
    AppIfPc_Printf( "     -z, --z    : get the value of z-axis.               \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                         \n\r" );
    AppIfPc_Printf( "     -a               -h     \n\r" );
    AppIfPc_Printf( "     --si_bmx055_gyro  --help \n\r" );
    AppIfPc_Printf( "     -a               -j     \n\r" );
    AppIfPc_Printf( "     --si_bmx055_gyro  --json \n\r" );
    AppIfPc_Printf( "     -a               -x     \n\r" );
    AppIfPc_Printf( "     --si_bmx055_gyro  --x    \n\r" );
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
    EHalSensorBMX055_t     which   ///< [in] 対象のセンサ
){
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "GetData() \n\r" );

    // センサデータを取得
    data = HalSensorBMX055Gyro_Get( which );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%03X %04d %03d%%", (int)data->cur, (int)data->cur, (int)data->cur_rate );

    // PC ターミナル表示
    AppIfPc_Printf( "%d \n\r", (int)data->cur );
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
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    DBG_PRINT_TRACE( "GetJson() \n\r" );

    // センサデータを取得
    dataX = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_X );
    dataY = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Y );
    dataZ = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Z );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "x%4.0fy%4.0fz%4.0f", dataX->cur, dataY->cur, dataZ->cur );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%03d%% ", dataX->cur_rate );
    AppIfLcd_Printf( "%03d%% ", dataY->cur_rate );
    AppIfLcd_Printf( "%03d%%",  dataZ->cur_rate );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"si_bmx055_gyro\", \"value\": {\"x\": %+5.2f, \"y\": %+5.2f, \"z\": %+5.2f}} \r",
                    dataX->cur,
                    dataY->cur,
                    dataZ->cur );
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_SiBmx055GyroMenu(
    void
){
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    DBG_PRINT_TRACE( "OptCmd_SiBmx055GyroMenu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfPc_Printf( "range : -2g -> +2g.         \n\r" );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        dataX = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_X );
        dataY = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Y );
        dataZ = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Z );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%03d%%  ", dataX->cur_rate );
        AppIfLcd_Printf( "%03d%%  ", dataY->cur_rate );
        AppIfLcd_Printf( "%03d%%",  dataZ->cur_rate );

        // PC ターミナル表示
        AppIfPc_Printf( "{ x: %+5.2f(?), y: %+5.2f(?), z: %+5.2f(?) } \n\r",
                        dataX->cur, dataY->cur, dataZ->cur
                      );
        // 500ms スリープ
        usleep( 500 * 1000 );
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
OptCmd_SiBmx055Gyro(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmxyz";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "menu", no_argument, NULL,  'm' },
        { "x",    no_argument, NULL,  'x' },
        { "y",    no_argument, NULL,  'y' },
        { "z",    no_argument, NULL,  'z' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SiBmx055Gyro() \n\r" );
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
        case 'm': OptCmd_SiBmx055GyroMenu(); break;
        case 'x': GetData( EN_SEN_BMX055_X ); break;
        case 'y': GetData( EN_SEN_BMX055_Y ); break;
        case 'z': GetData( EN_SEN_BMX055_Z ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

