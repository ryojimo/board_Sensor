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
#include <stdio.h>
#include <getopt.h>

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
// getopt() で使用
extern char *optarg;
extern int  optind, opterr, optopt;


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
static void         Run_Sa_Pm( char* str );

static void         Run_Si_BME280( char* str );
static void         Run_Si_GP2Y0E03( char* str );
static void         Run_Si_LPS25H( char* str );
static void         Run_Si_TSL2561( char* str );

static void         Run_Time( char* str );




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

    printf( " COMMAND                   : DESCRIPTION                                    \n\r" );
    printf( "============================================================================\n\r" );
    printf( " -m, --menu                : Go to menu mode.                               \n\r" );
    printf( " -h, --help                : display the command option list.               \n\r" );
    printf( " -s, --sensors             : Get the value of all sensors.                  \n\r" );
    printf( "                                                                            \n\r" );
    printf( " -c, --i2clcd <value>      : Control the (I2C) LCD.                         \n\r" );
    printf( " -l, --led <value>         : Control the LED.                               \n\r" );
    printf( " -o, --motorsv <value>     : Control the SAVO motor.                        \n\r" );
    printf( " -r, --relay [OPTION]      : Control the Relay circuit.                     \n\r" );
    printf( "                        on : ON  relay switch.                              \n\r" );
    printf( "                       off : OFF relay switch.                              \n\r" );
    printf( " -a, --sa_acc [OPTION]     : Get the value of a sensor(A/D), Acc.           \n\r" );
    printf( "                         x : X direction.                                   \n\r" );
    printf( "                         y : Y direction.                                   \n\r" );
    printf( "                         z : Z direction.                                   \n\r" );
    printf( "                      json : all values of json format.                     \n\r" );
    printf( " -g, --sa_gyro [OPTION]    : Get the value of a sensor(A/D), Gyro .         \n\r" );
    printf( "                        g1 : G1 direction.                                  \n\r" );
    printf( "                        g2 : G2 direction.                                  \n\r" );
    printf( "                      json : all values of json format.                     \n\r" );
    printf( " -p, --sa_pm               : Get the value of a sensor(A/D), Potentiometer. \n\r" );
    printf( "                      json : value of json format.                          \n\r" );
    printf( " -w, --si_bme280 [OPTION]  : Get the value of a sensor(I2C), BME280.        \n\r" );
    printf( "                     atmos : atmosphere                                     \n\r" );
    printf( "                      humi : humidity                                       \n\r" );
    printf( "                      temp : temperature                                    \n\r" );
    printf( "                      json : all values of json format.                     \n\r" );
    printf( " -x, --si_gp2y0e03         : Get the value of a sensor(I2C), GP2Y0E03.      \n\r" );
    printf( "                      json : value of json format.                          \n\r" );
    printf( " -y, --si_lps25h [OPTION]  : Get the value of a sensor(I2C), LPS25H.        \n\r" );
    printf( "                     atmos : atmosphere                                     \n\r" );
    printf( "                      temp : temperature                                    \n\r" );
    printf( "                      json : all values of json format.                     \n\r" );
    printf( " -x, --si_tsl2561 [OPTION] : Get the value of a sensor(I2C), TSL2561.       \n\r" );
    printf( "                 broadband : ?                                              \n\r" );
    printf( "                        ir : ?                                              \n\r" );
    printf( "                       lux : lux                                            \n\r" );
    printf( "                      json : all values of json format.                     \n\r" );
    printf( " -t, --time                : Get the time.                                  \n\r" );
    printf( "                      json : value of json format.                          \n\r" );
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
    SHalSensor_t*   data;
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "x", strlen("x") ) )
    {
        data = HalSensorAcc_Get( EN_SEN_ACC_X );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%04X", (int)data->cur );
        printf( "%d", (int)data->cur );
    } else if( 0 == strncmp( str, "y", strlen("y") ) )
    {
        data = HalSensorAcc_Get( EN_SEN_ACC_Y );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%04X", (int)data->cur );
        printf( "%d", (int)data->cur );
    } else if( 0 == strncmp( str, "z", strlen("z") ) )
    {
        data = HalSensorAcc_Get( EN_SEN_ACC_Z );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%04X", (int)data->cur );
        printf( "%d", (int)data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        dataX = HalSensorAcc_Get( EN_SEN_ACC_X );
        dataY = HalSensorAcc_Get( EN_SEN_ACC_Y );
        dataZ = HalSensorAcc_Get( EN_SEN_ACC_Z );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%04X, %04X, %04X", (int)dataX->cur, (int)dataY->cur, (int)dataZ->cur );

        printf( "{ " );
        printf( "  \"sensor\": \"sa_acc\"," );
        printf( "  \"value\": {" );
        printf( "    \"x\": %d,", (int)dataX->cur );
        printf( "    \"y\": %d,", (int)dataY->cur );
        printf( "    \"z\": %d ", (int)dataZ->cur );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

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
    SHalSensor_t*       data;
    SHalSensor_t*       dataG1;
    SHalSensor_t*       dataG2;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "g1", strlen("g1") ) )
    {
        data = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%4X", (int)data->cur );
        printf( "%d", (int)data->cur );
    } else if( 0 == strncmp( str, "g2", strlen("g2") ) )
    {
        data = HalSensorGyro_Get( EN_SEN_GYRO_G2 );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "0x%4X", (int)data->cur );
        printf( "%d", (int)data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        dataG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
        dataG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%04X, %04X", (int)dataG1->cur, (int)dataG2->cur );

        printf( "{ " );
        printf( "  \"sensor\": \"sa_gyro\"," );
        printf( "  \"value\": {" );
        printf( "    \"g1\": %d,", (int)dataG1->cur );
        printf( "    \"g2\": %d ", (int)dataG2->cur );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

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
    char*           str     ///< [in] 文字列
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( str == NULL )
    {
        data = HalSensorPm_Get();
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d %%", data->cur_rate );
        printf( "%3d", data->cur_rate );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        data = HalSensorPm_Get();

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d %%", data->cur_rate );

        printf( "{ " );
        printf( "  \"sensor\": \"sa_pm\"," );
        printf( "  \"value\": %3d,", data->cur_rate );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

err :
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
    SHalSensor_t*       data;
    SHalSensor_t*       dataAtmos;
    SHalSensor_t*       dataHumi;
    SHalSensor_t*       dataTemp;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "atmos", strlen("atmos") ) )
    {
        data = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f (hPa)", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "humi", strlen("humi") ) )
    {
        data = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f (%%)", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "temp", strlen("temp") ) )
    {
        data = HalSensorBME280_Get( EN_SEN_BME280_TEMP );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f ('C)", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        dataAtmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
        dataHumi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
        dataTemp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", dataAtmos->cur, dataHumi->cur, dataTemp->cur );

        printf( "{ " );
        printf( "  \"sensor\": \"si_bme280\"," );
        printf( "  \"value\": {" );
        printf( "    \"atmos\": %5.2f,", dataAtmos->cur );
        printf( "    \"humi\": %5.2f,", dataHumi->cur );
        printf( "    \"temp\": %5.2f ", dataTemp->cur );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

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
    char*           str     ///< [in] 文字列
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( str == NULL )
    {
        data = HalSensorGP2Y0E03_Get();
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f cm", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        data = HalSensorGP2Y0E03_Get();
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f cm", data->cur );
        printf( "{ " );
        printf( "  \"sensor\": \"si_gp2y0e03\"," );
        printf( "  \"value\": %5.2f,", data->cur );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

err :
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
    SHalSensor_t*       data;
    SHalSensor_t*       dataAtmos;
    SHalSensor_t*       dataTemp;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "atmos", strlen("atmos") ) )
    {
        data = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f (hPa)", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "temp", strlen("temp") ) )
    {
        data = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f ('C)", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        dataAtmos = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
        dataTemp  = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f, %5.2f", dataAtmos->cur, dataTemp->cur );

        printf( "{ " );
        printf( "  \"sensor\": \"si_lps25h\"," );
        printf( "  \"value\": {" );
        printf( "    \"atmos\": %5.2f,", dataAtmos->cur );
        printf( "    \"temp\": %5.2f ", dataTemp->cur );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

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
    SHalSensor_t*       data;
    SHalSensor_t*       dataBB;
    SHalSensor_t*       dataIR;
    SHalSensor_t*       dataLUX;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( 0 == strncmp( str, "broadband", strlen("broadband") ) )
    {
        data = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f BB", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "ir", strlen("ir") ) )
    {
        data = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f IR", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "lux", strlen("lux") ) )
    {
        data = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f LUX", data->cur );
        printf( "%5.2f", data->cur );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        dataBB  = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
        dataIR  = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
        dataLUX = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", dataBB->cur, dataIR->cur, dataLUX->cur );

        printf( "{ " );
        printf( "  \"sensor\": \"si_tsl2561\"," );
        printf( "  \"value\": {" );
        printf( "    \"broadband\": %5.2f,", dataBB->cur );
        printf( "    \"ir\": %5.2f,",        dataIR->cur );
        printf( "    \"lux\": %5.2f ",       dataLUX->cur );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

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
    char*           str     ///< [in] 文字列
){
    SHalTime_t*     data;

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    if( str == NULL )
    {
        data = HalTime_GetLocaltime();

        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "%4d/%02d/%02d", data->year, data->month, data->day );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02d:%02d:%02d", data->hour, data->min, data->sec );

        printf( "%4d/%02d/%02d %02d:%02d:%02d",
                data->year, data->month, data->day,
                data->hour, data->min, data->sec
              );
    } else if( 0 == strncmp( str, "json", strlen("json") ) )
    {
        data = HalTime_GetLocaltime();

        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "%4d/%02d/%02d", data->year, data->month, data->day );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02d:%02d:%02d", data->hour, data->min, data->sec );

        printf( "{ " );
        printf( "  \"time\": {" );
        printf( "    \"year\" : %4d,",  data->year );
        printf( "    \"month\": %02d,", data->month );
        printf( "    \"day\"  : %02d,", data->day );
        printf( "    \"hour\" : %02d,", data->hour );
        printf( "    \"min\"  : %02d,", data->min );
        printf( "    \"sec\"  : %02d,", data->sec );
        printf( "  }" );
        printf( "}" );
    } else
    {
        DBG_PRINT_ERROR( "invalid argument error. : %s \n\r", str );
        goto err;
    }

err :
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
    int             cmd = 0;
    int             data = 0;
    unsigned char   str[256];
    unsigned char*  pt;

    int             opt = 0;
    const char      optstring[] = "mhsc:l:o:r:a:g:p::w:x::y:z:t::i:u:";
    const struct    option longopts[] = {
      //{ *name,         has_arg,           *flag, val }, // 説明
        { "menu",        no_argument,       NULL,  'm' },
        { "help",        no_argument,       NULL,  'h' },
        { "sensors",     no_argument,       NULL,  's' },
        { "i2clcd",      required_argument, NULL,  'c' },
        { "led",         required_argument, NULL,  'l' },
        { "motorsv",     required_argument, NULL,  'o' },
        { "relay",       required_argument, NULL,  'r' },
        { "sa_acc",      required_argument, NULL,  'a' },
        { "sa_gyro",     required_argument, NULL,  'g' },
        { "sa_pm",       optional_argument, NULL,  'p' },
        { "si_bme280",   required_argument, NULL,  'w' },
        { "si_gp2y0e03", optional_argument, NULL,  'x' },
        { "si_lps25h",   required_argument, NULL,  'y' },
        { "si_tsl2561",  required_argument, NULL,  'z' },
        { "time",        optional_argument, NULL,  't' },
        { "pic",         required_argument, NULL,  'i' },
        { "usbkey",      required_argument, NULL,  'u' },
        { 0,             0,                 NULL,   0  }, // termination
    };
    int longindex = 0;

    Sys_Init();

#if 0
    DBG_PRINT_ERROR( "test error \n\r" );
    DBG_PRINT_WARN(  "test warn  \n\r" );
    DBG_PRINT_TRACE( "test trace \n\r" );
    DBG_PRINT_DEBUG( "test debug \n\r" );

    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );
#endif

    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "cmd:%s", argv[1] );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
//      opt = getopt( argc, argv, optstring );
        DBG_PRINT_TRACE( "opt = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        }

        if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        }

        switch( opt )
        {
        case 'm':
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
        break;
        case 'h': Run_Help(); break;
        case 's': Run_Sensors(); break;
        case 'c': Run_I2cLcd( optarg ); break;
        case 'l': Run_Led( optarg ); break;
        case 'o': Run_MotorSV( optarg ); break;
        case 'r': Run_Relay( optarg ); break;
        case 'a': Run_Sa_Acc( optarg ); break;
        case 'g': Run_Sa_Gyro( optarg ); break;
        case 'p': Run_Sa_Pm( optarg ); break;
        case 'w': Run_Si_BME280( optarg ); break;
        case 'x': Run_Si_GP2Y0E03( optarg ); break;
        case 'y': Run_Si_LPS25H( optarg ); break;
        case 'z': Run_Si_TSL2561( optarg ); break;
        case 't': Run_Time( optarg ); break;
        case 'i': 
            if( argv[2] != NULL )
            {
                cmd  = strtol( (const char*)argv[2], (char**)&pt, 10 );
            }
            if( argv[3] != NULL )
            {
                data = strtol( (const char*)argv[3], (char**)&pt, 10 );
            }

            DBG_PRINT_TRACE( "cmd  = 0x%02X(H) : %02d(d) \n", cmd,  cmd  );
            DBG_PRINT_TRACE( "data = 0x%02X(H) : %02d(d) \n", data, data );
            HalI2cCmd_Set( cmd, data );
        break;
        case 'u': 
            if( optarg != NULL )
            {
                data  = strtol( (const char*)optarg, (char**)&pt, 16 );
            }

            DBG_PRINT_TRACE( "data = 0x%02X(H) : %02d(d) \n", data, data );
            HalI2cCmd_SetKeycode( data );
        break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Run_Help();
        break;
        }
    }

    Sys_Fini();
    return 0;
}


#ifdef __cplusplus
    }
#endif
