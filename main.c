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
static void         Run_Help( void );
static void         Run_Menu( unsigned char* str );
static void         Run_Sensors( void );

static void         Run_I2cLcd( char* str );
static void         Run_Led( char* str );
static void         Run_MotorSV( char* str );
static void         Run_Relay( char* str );

static void         Run_Sa_Acc( char* str );
static void         Run_Sa_Gyro( char* str );
static void         Run_Sa_Pm( void );

static void         Run_Si_BME280( char* str );
static void         Run_Si_GP2Y0E03( void );
static void         Run_Si_LPS25H( char* str );
static void         Run_Si_TSL2561( char* str );

static void         Run_Time( void );




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
    DBG_PRINT_TRACE( "\n\r" );

    printf( " COMMAND             : DESCRIPTION                                    \n\r" );
    printf( "======================================================================\n\r" );
    printf( " help                : display the command option list.               \n\r" );
    printf( " menu                : Go to menu mode.                               \n\r" );
    printf( " sensors             : Get the value of all sensors.                  \n\r" );
    printf( "                                                                      \n\r" );
    printf( " i2clcd <value>      : Control the (I2C) LCD.                         \n\r" );
    printf( " led <value>         : Control the LED.                               \n\r" );
    printf( " motorsv <value>     : Control the SAVO motor.                        \n\r" );
    printf( " relay [OPTION]      : Control the Relay circuit.                     \n\r" );
    printf( "                 on  : ON  relay switch.                              \n\r" );
    printf( "                 off : OFF relay switch.                              \n\r" );
    printf( " sa_acc [OPTION]     : Get the value of a sensor(A/D), Acc.           \n\r" );
    printf( "                   x : X direction.                                   \n\r" );
    printf( "                   y : Y direction.                                   \n\r" );
    printf( "                   z : Z direction.                                   \n\r" );
    printf( " sa_gyro [OPTION]    : Get the value of a sensor(A/D), Gyro .         \n\r" );
    printf( "                  g1 : G1 direction.                                  \n\r" );
    printf( "                  g2 : G2 direction.                                  \n\r" );
    printf( " sa_pm               : Get the value of a sensor(A/D), Potentiometer. \n\r" );
    printf( " si_bme280 [OPTION]  : Get the value of a sensor(I2C), BME280.        \n\r" );
    printf( "               atmos : atmosphere                                     \n\r" );
    printf( "               humi  : humidity                                       \n\r" );
    printf( "               temp  : temperature                                    \n\r" );
    printf( " si_gp2y0e03         : Get the value of a sensor(I2C), GP2Y0E03.      \n\r" );
    printf( " si_lps25h [OPTION]  : Get the value of a sensor(I2C), LPS25H.        \n\r" );
    printf( "               atmos : atmosphere                                     \n\r" );
    printf( "               temp  : temperature                                    \n\r" );
    printf( " si_tsl2561 [OPTION] : Get the value of a sensor(I2C), TSL2561.       \n\r" );
    printf( "           broadband : ?                                              \n\r" );
    printf( "           ir        : ?                                              \n\r" );
    printf( "           lux       : lux                                            \n\r" );
    printf( " time                : Get the time.                                  \n\r" );
    printf( "\n\r" );

    return;
}


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
 * @brief     すべてのセンサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Sensors(
    void  ///< [in] ナシ
){
    SHalSensor_t*   dataSaAccX;
    SHalSensor_t*   dataSaAccY;
    SHalSensor_t*   dataSaAccZ;

    SHalSensor_t*   dataSaGyroG1;
    SHalSensor_t*   dataSaGyroG2;

    SHalSensor_t*   dataSiBme280Atmos;
    SHalSensor_t*   dataSiBme280Humi;
    SHalSensor_t*   dataSiBme280Temp;

    SHalSensor_t*   dataSiGp2y0e03;

    SHalSensor_t*   dataSiLps25hAtmos;
    SHalSensor_t*   dataSiLps25hTemp;

    SHalSensor_t*   dataSiTsl2561;

    DBG_PRINT_TRACE( "\n\r" );

    dataSaAccX   = HalSensorAcc_Get( EN_SEN_ACC_X );
    dataSaAccY   = HalSensorAcc_Get( EN_SEN_ACC_Y );
    dataSaAccZ   = HalSensorAcc_Get( EN_SEN_ACC_Z );

    dataSaGyroG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
    dataSaGyroG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );

    dataSiBme280Atmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    dataSiBme280Humi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    dataSiBme280Temp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

    dataSiGp2y0e03    = HalSensorGP2Y0E03_Get();

    dataSiLps25hAtmos = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
    dataSiLps25hTemp  = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );

    dataSiTsl2561     = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

    // node.js サーバへデータを渡すための printf()
    printf( "{ " );
    printf( "\"sa_acc_x\"       :%d,    ", (int)dataSaAccX->cur );
    printf( "\"sa_acc_y\"       :%d,    ", (int)dataSaAccY->cur );
    printf( "\"sa_acc_z\"       :%d,    ", (int)dataSaAccZ->cur );

    printf( "\"sa_gyro_g1\"     :%d,    ", (int)dataSaGyroG1->cur );
    printf( "\"sa_gyro_g2\"     :%d,    ", (int)dataSaGyroG2->cur );

    printf( "\"si_bme280_atmos\":%5.2f, ", dataSiBme280Atmos->cur );
    printf( "\"si_bme280_humi\" :%5.2f, ", dataSiBme280Humi->cur );
    printf( "\"si_bme280_temp\" :%5.2f, ", dataSiBme280Temp->cur );

    printf( "\"si_gp2y0e03\"    :%5.2f, ", dataSiGp2y0e03->cur );

    printf( "\"si_lps25h_atmos\":%5.2f, ", dataSiLps25hAtmos->cur );
    printf( "\"si_lps25h_temp\" :%5.2f, ", dataSiLps25hTemp->cur );

    printf( "\"si_tsl2561_lux\" :%5.2f, ", dataSiTsl2561->cur );
    printf( "}" );
    return;
}


/**************************************************************************//*!
 * @brief     I2C LCD を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_I2cLcd(
    char*  str     ///< [in] 文字列
){
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( str );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "                " );

    return;
}


/**************************************************************************//*!
 * @brief     LED を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Led(
    char*           str     ///< [in] 文字列
){
    unsigned int    num;
    DBG_PRINT_TRACE( "str = %s \n\r", str );

    sscanf( str, "%X", &num );
    HalLed_Set( num );

    return;
}


/**************************************************************************//*!
 * @brief     SERVO MOTOR を実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_MotorSV(
    char*   str     ///< [in] 文字列
){
    int     data = 0;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    data = atoi( (const char*)str );
    DBG_PRINT_TRACE( "data = %d \n", data );
    HalMotorSV_SetPwmDuty( EN_MOTOR_CW, data );
//  usleep( 1000 * 1000 );  // 2s 待つ

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
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
    }
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
Run_Sa_Acc(
    char*           str     ///< [in] 文字列
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
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

    data = HalSensorAcc_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%04X", (int)data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%d", (int)data->cur );

err :
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
Run_Sa_Gyro(
    char*               str     ///< [in] 文字列
){
    EHalSensorGyro_t    which = EN_SEN_GYRO_G1;
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "g1", strlen("g1") ) )
    {
        which = EN_SEN_GYRO_G1;
    } else if( 0 == strncmp( str, "g2", strlen("g2") ) )
    {
        which = EN_SEN_GYRO_G2;
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

    data = HalSensorGyro_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%4X", (int)data->cur );

    // node.js サーバへデータを渡すための printf()
    printf( "%d", (int)data->cur );

err :
    return;
}


/**************************************************************************//*!
 * @brief     ポテンショメーターを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Sa_Pm(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorPm_Get();

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%3d %%", data->cur_rate );

    // node.js サーバへデータを渡すための printf()
    printf( "%3d", data->cur_rate );
    return;
}


/**************************************************************************//*!
 * @brief     BME280 センサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Si_BME280(
    char*               str     ///< [in] 文字列
){
    EHalSensorBME280_t  which = EN_SEN_BME280_ATMOS;
    char                unit[4];
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    memset( unit, '\0', sizeof(unit) );

    if( 0 == strncmp( str, "atmos", strlen("atmos") ) )
    {
        which = EN_SEN_BME280_ATMOS;
        strncpy( unit, "hPa", strlen("hPa") );
    } else if( 0 == strncmp( str, "humi", strlen("humi") ) )
    {
        which = EN_SEN_BME280_HUMI;
        strncpy( unit, "%", strlen("%") );
    } else if( 0 == strncmp( str, "temp", strlen("temp") ) )
    {
        which = EN_SEN_BME280_TEMP;
        strncpy( unit, "'C", strlen("'C") );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

    data = HalSensorBME280_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f %s", data->cur, unit );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );

err :
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
Run_Si_GP2Y0E03(
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
 * @brief     LPS25H センサを実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Si_LPS25H(
    char*               str     ///< [in] 文字列
){
    EHalSensorLPS25H_t  which = EN_SEN_LPS25H_ATMOS;
    char                unit[4];
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    memset( unit, '\0', sizeof(unit) );

    if( 0 == strncmp( str, "atmos", strlen("atmos") ) )
    {
        which = EN_SEN_LPS25H_ATMOS;
        strncpy( unit, "hPa", strlen("hPa") );
    } else if( 0 == strncmp( str, "temp", strlen("temp") ) )
    {
        which = EN_SEN_LPS25H_TEMP;
        strncpy( unit, "'C", strlen("'C") );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

    data = HalSensorLPS25H_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f %s", data->cur, unit );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );

err :
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
Run_Si_TSL2561(
    char*               str     ///< [in] 文字列
){
    EHalSensorTSL2561_t which = EN_SEN_TSL2561_BROADBAND;
    char                unit[4];
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    memset( unit, '\0', sizeof(unit) );

    if( 0 == strncmp( str, "broadband", strlen("broadband") ) )
    {
        which = EN_SEN_TSL2561_BROADBAND;
        strncpy( unit, "BB", strlen("BB") );
    } else if( 0 == strncmp( str, "ir", strlen("ir") ) )
    {
        which = EN_SEN_TSL2561_IR;
        strncpy( unit, "IR", strlen("IR") );
    } else if( 0 == strncmp( str, "lux", strlen("lux") ) )
    {
        which = EN_SEN_TSL2561_LUX;
        strncpy( unit, "LUX", strlen("LUX") );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

    data = HalSensorTSL2561_Get( which );

    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%5.2f %s", data->cur, unit );

    // node.js サーバへデータを渡すための printf()
    printf( "%5.2f", data->cur );

err :
    return;
}


/**************************************************************************//*!
 * @brief     時間情報を表示する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run_Time(
    void  ///< [in] ナシ
){
    SHalTime_t*     data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalTime_GetLocaltime();

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "%4d/%02d/%02d", data->year, data->month, data->day );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%02d:%02d:%02d", data->hour, data->min, data->sec );

    // node.js サーバへデータを渡すための printf()
    printf( "%4d/%02d/%02d %02d:%02d:%02d",
            data->year, data->month, data->day,
            data->hour, data->min, data->sec
          );
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
    } else if( argc > 1 && 0 == strncmp( argv[1], "sensors", strlen("sensors") ) )
    {
        Run_Sensors();
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "i2clcd", strlen("i2clcd") ) )
    {
        Run_I2cLcd( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "led", strlen("led") ) )
    {
        Run_Led( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "motorsv", strlen("motorsv") ) )
    {
        Run_MotorSV( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "relay", strlen("relay") ) )
    {
        Run_Relay( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "sa_acc", strlen("sa_acc") ) )
    {
        Run_Sa_Acc( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "sa_gyro", strlen("sa_gyro") ) )
    {
        Run_Sa_Gyro( argv[2] );
    } else if( argc > 1 &&                    0 == strncmp( argv[1], "sa_pm", strlen("sa_pm") ) )
    {
        Run_Sa_Pm();
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "si_bme280", strlen("si_bme280") ) )
    {
        Run_Si_BME280( argv[2] );
    } else if( argc > 1 &&                    0 == strncmp( argv[1], "si_gp2y0e03", strlen("si_gp2y0e03") ) )
    {
        Run_Si_GP2Y0E03();
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "si_lps25h", strlen("si_lps25h") ) )
    {
        Run_Si_LPS25H( argv[2] );
    } else if( argc > 1 && argv[2] != NULL && 0 == strncmp( argv[1], "si_tsl2561", strlen("si_tsl2561") ) )
    {
        Run_Si_TSL2561( argv[2] );
    } else if( argc > 1 &&                    0 == strncmp( argv[1], "time", strlen("time") ) )
    {
        Run_Time();
    } else
    {
        DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
        Run_Help();
    }

    Sys_Fini();
    return 0;
}


#ifdef __cplusplus
    }
#endif
