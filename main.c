/**************************************************************************//*!
 *  @file           main.c
 *  @brief          main() を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.12.03
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <stdlib.h>
#include <string.h>

#include "./app/if_lcd/if_lcd.h"
#include "./app/menu/menu.h"
#include "./hal/hal.h"
#include "./sys/sys.h"


//#define DBG_PRINT
#define MY_NAME "MAI"
#include "./app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         Run_Menu( unsigned char* str );
static void         Run_Help( void );
static void         Run_Acc( char* dir );
static void         Run_BME280_Atmosphere( void );
static void         Run_LPS25H_Atmosphere( void );
static void         Run_GP2Y0E03_Distance( void );
static void         Run_BME280_Humidity( void );
static void         Run_Gyro( char* dir );
static void         Run_TSL2561_Illuminance( void );
static void         Run_Relay( char* str );
static void         Run_BME280_Temperature( void );
static void         Run_LPS25H_Temperature( void );

static void         Run_AllSensors( void );




/**************************************************************************//*!
 * @brief     メニューを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Menu(
    unsigned char*  str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    Sys_ShowInfo();
    AppMenu_Main( str );
    return;
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
Run_Help(
    void
){
    DBG_PRINT_TRACE( "MenuCmd_Help() \n\r" );

    printf( " OPTION        : DESCRIPTION                                           \n\r" );
    printf( "=======================================================================\n\r" );
    printf( " help          : display the command option list.                      \n\r" );
    printf( " menu          : Go to menu mode.                                      \n\r" );
    printf( " sensor        : Get the value of all sensors.                         \n\r" );
    printf( "                                                                       \n\r" );
    printf( " acc [dir]     : Get the value of a Acc sensor.                        \n\r" );
    printf( "                   x: X direction.                                     \n\r" );
    printf( "                   y: Y direction.                                     \n\r" );
    printf( "                   z: Z direction.                                     \n\r" );
    printf( " atmos         : Get the value of a LPS25H atmosphere sensor.          \n\r" );
    printf( " dist          : Get the value of a GP2Y0E03 distance sensor.          \n\r" );
    printf( " gyro [dir]    : Get the value of a Gyro sensor.                       \n\r" );
    printf( "                   g1: G1 direction.                                   \n\r" );
    printf( "                   g2: G2 direction.                                   \n\r" );
    printf( " led           : Control the LED.                                      \n\r" );
    printf( " lux           : Get the value of a TSL2561 luminance sensor.          \n\r" );
    printf( " motorsv       : Control the SAVO motor.                               \n\r" );
    printf( " relay [o/f]   : Control the Relay circuit.                            \n\r" );
    printf( "                  on : ON  relay switch.                               \n\r" );
    printf( "                  off: OFF relay switch.                               \n\r" );
    printf( " temp          : Get the value of a LPS25H  temperaturre sensor.       \n\r" );
    printf( "                                                                       \n\r" );
    printf( " b_atmos       : Get the value of a BME280 atmosphere sensor.          \n\r" );
    printf( " b_humi        : Get the value of a BME280 humidity sensor.            \n\r" );
    printf( " b_temp        : Get the value of a BME280 temperaturre sensor.        \n\r" );
    printf( "\n\r" );

    return;
}


/**************************************************************************//*!
 * @brief     加速度センサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Acc(
    char*  str     ///< [in] 文字列
){
    EHalSensorAcc_t which = EN_SEN_ACC_X;
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "x", strlen("x") ) )
    {
        which = EN_SEN_ACC_X;
    } else if( 0 == strncmp( str, "y", strlen("y") ) )
    {
        which = EN_SEN_ACC_Y;
    } else if( 0 == strncmp( str, "z", strlen("z") ) )
    {
        which = EN_SEN_ACC_Z;
    }

    data = HalSensorAcc_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%4X", (int)data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%d", (int)data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     気圧センサ ( LPS25H ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_LPS25H_Atmosphere(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f hPa", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     気圧センサ ( BME280 ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_BME280_Atmosphere(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f hPa", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     距離センサ ( GP2Y0E03 ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_GP2Y0E03_Distance(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorGP2Y0E03_Get();

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f cm", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     ジャイロセンサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Gyro(
    char*  str     ///< [in] 文字列
){
    EHalSensorGyro_t    which = EN_SEN_ACC_X;
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "g1", strlen("g1") ) )
    {
        which = EN_SEN_GYRO_G1;
    } else if( 0 == strncmp( str, "g2", strlen("g2") ) )
    {
        which = EN_SEN_GYRO_G2;
    }

    data = HalSensorGyro_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%4X", (int)data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%d", (int)data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     温度センサ ( BME280 ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_BME280_Humidity(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorBME280_Get( EN_SEN_BME280_HUMI );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f %%", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     照度センサ ( TSL2561 ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_TSL2561_Illuminance(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f lux", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     リレーを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Relay(
    char*  str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "on", strlen("on") ) )
    {
        HalRelay_Set( EN_HIGH );
    } else if( 0 == strncmp( str, "off", strlen("off") ) )
    {
        HalRelay_Set( EN_LOW );
    }
    return;
}


/**************************************************************************//*!
 * @brief     温度センサ ( LPS25H ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_LPS25H_Temperature(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f 'C", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     温度センサ ( BME280 ) を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_BME280_Temperature(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f 'C", data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     すべてのセンサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_AllSensors(
    void  ///< [in] ナシ
){
    SHalSensor_t*   dataAccX;
    SHalSensor_t*   dataAccY;
    SHalSensor_t*   dataAccZ;
    SHalSensor_t*   dataAtmos;
    SHalSensor_t*   dataDist;
    SHalSensor_t*   dataGyroG1;
    SHalSensor_t*   dataGyroG2;
    SHalSensor_t*   dataLux;
    SHalSensor_t*   dataTemp;

    DBG_PRINT_TRACE( "\n\r" );

    dataAccX   = HalSensorAcc_Get( EN_SEN_ACC_X );
    dataAccY   = HalSensorAcc_Get( EN_SEN_ACC_Y );
    dataAccZ   = HalSensorAcc_Get( EN_SEN_ACC_Z );
    dataAtmos  = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
    dataDist   = HalSensorGP2Y0E03_Get();
    dataGyroG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
    dataGyroG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );
    dataLux    = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );
    dataTemp   = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );

    // node.js サーバへデータを渡すための printf()
    printf( "{ " );
    printf( "\"acc_x\"  :%d,    ", (int)dataAccX->cur );
    printf( "\"acc_y\"  :%d,    ", (int)dataAccY->cur );
    printf( "\"acc_z\"  :%d,    ", (int)dataAccZ->cur );
    printf( "\"atmos\"  :%5.2f, ", dataAtmos->cur );
    printf( "\"dist\"   :%5.2f, ", dataDist->cur );
    printf( "\"gyro_g1\":%d,    ", (int)dataGyroG1->cur );
    printf( "\"gyro_g2\":%d,    ", (int)dataGyroG2->cur );
    printf( "\"lux\"    :%5.2f, ", dataLux->cur );
    printf( "\"temp\"   :%5.2f, ", dataTemp->cur );
    printf( "}" );
    return;
}


/**************************************************************************//*!
 * @brief     メイン
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
int main(int argc, char *argv[ ])
{
    int             i = 0;
    int             data = 0;
    unsigned char   str[256];
    unsigned char*  pt;

    Sys_Init();

#if 0
    DBG_PRINT_ERROR( "test error \n\r" );
    DBG_PRINT_WARN(  "test warn  \n\r" );
    DBG_PRINT_TRACE( "test trace \n\r" );
    DBG_PRINT_DEBUG( "test debug \n\r" );
#endif

    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );

    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "cmd:%s", argv[1] );

    if( argc == 1 )
    {
        Run_Help();
    }

    if( argc > 1 && 0 == strncmp( argv[1], "menu", strlen("menu") ) )
    {
        memset( str, '\0', sizeof(str) );
        pt = (unsigned char*)str;

        // 引数部分を取り出す
        for( i = 2; i < argc; i++ )
        {
            DBG_PRINT_TRACE( "argv[%d] = %s \n\r", i, argv[i] );
            strncat( (char*)pt, argv[i], strlen( argv[i] ) );
            pt += strlen( argv[i] );
            strncat( (char*)pt, " ", strlen( " " ) );
            pt += strlen( " " );
        }

        Run_Menu( str );
    } else if( argc > 1 && 0 == strncmp( argv[1], "help", strlen("help") ) )
    {
        Run_Help();
    } else if( argc > 1 && 0 == strncmp( argv[1], "acc", strlen("acc") ) )
    {
        Run_Acc( argv[2] );
    } else if( argc > 1 && 0 == strncmp( argv[1], "atmos", strlen("atmos") ) )
    {
        Run_LPS25H_Atmosphere();
    } else if( argc > 1 && 0 == strncmp( argv[1], "dist", strlen("dist") ) )
    {
        Run_GP2Y0E03_Distance();
    } else if( argc > 1 && 0 == strncmp( argv[1], "gyro", strlen("gyro") ) )
    {
        Run_Gyro( argv[2] );
    } else if( argc > 1 && 0 == strncmp( argv[1], "led", strlen("led") ) )
    {
        HalLed_Set( 0x0F );
    } else if( argc > 1 && 0 == strncmp( argv[1], "lux", strlen("lux") ) )
    {
        Run_TSL2561_Illuminance();
    } else if( argc > 1 && 0 == strncmp( argv[1], "motorsv", strlen("motorsv") ) )
    {
        data = atoi( (const char*)argv[2] );
        DBG_PRINT_TRACE( "data = %d \n", data );
        HalMotorSV_SetPwmDuty( EN_MOTOR_CW, data );
//        usleep( 1000 * 1000 );  // 2s 待つ
    } else if( argc > 1 && 0 == strncmp( argv[1], "relay", strlen("relay") ) )
    {
        Run_Relay( argv[2] );
    } else if( argc > 1 && 0 == strncmp( argv[1], "temp", strlen("temp") ) )
    {
        Run_LPS25H_Temperature();
    } else if( argc > 1 && 0 == strncmp( argv[1], "b_atmos", strlen("b_atmos") ) )
    {
        Run_BME280_Atmosphere();
    } else if( argc > 1 && 0 == strncmp( argv[1], "b_humi", strlen("b_humi") ) )
    {
        Run_BME280_Humidity();
    } else if( argc > 1 && 0 == strncmp( argv[1], "b_temp", strlen("b_temp") ) )
    {
        Run_BME280_Temperature();
    } else if( argc > 1 && 0 == strncmp( argv[1], "sensor", strlen("sensor") ) )
    {
        Run_AllSensors();
    } else
    {
        DBG_PRINT_ERROR( "\"%s\" is a invalid command. \n", argv[1] );
        Run_Help();
    }

    Sys_Fini();
    return 0;
}


#ifdef __cplusplus
    }
#endif
