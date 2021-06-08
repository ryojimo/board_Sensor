/**************************************************************************//*!
 *  @file           opt_cmd_sensors.c
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
static void Help( void );


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
    AppIfPc_Printf( " Main option)                                           \n\r" );
    AppIfPc_Printf( "     -q, --sensors : display the values of all sensors. \n\r" );
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
OptCmd_Sensors(
    void
){

    DBG_PRINT_TRACE( "OptCmd_Sensors() \n\r" );
    SHalSensor_t*   dataSiBme280Atmos;
    SHalSensor_t*   dataSiBme280Humi;
    SHalSensor_t*   dataSiBme280Temp;

    SHalSensor_t*   dataSiBmx055AccX;
    SHalSensor_t*   dataSiBmx055AccY;
    SHalSensor_t*   dataSiBmx055AccZ;
    SHalSensor_t*   dataSiBmx055GyroX;
    SHalSensor_t*   dataSiBmx055GyroY;
    SHalSensor_t*   dataSiBmx055GyroZ;
    SHalSensor_t*   dataSiBmx055MagX;
    SHalSensor_t*   dataSiBmx055MagY;
    SHalSensor_t*   dataSiBmx055MagZ;

    SHalSensor_t*   dataSiGp2y0e03;

    SHalSensor_t*   dataSiLps25hAtmos;
    SHalSensor_t*   dataSiLps25hTemp;

    SHalSensor_t*   dataSiTsl2561;

    dataSiBme280Atmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    dataSiBme280Humi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    dataSiBme280Temp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );

    dataSiBmx055AccX  = HalSensorBMX055Acc_Get( EN_SEN_BMX055_X );
    dataSiBmx055AccY  = HalSensorBMX055Acc_Get( EN_SEN_BMX055_Y );
    dataSiBmx055AccZ  = HalSensorBMX055Acc_Get( EN_SEN_BMX055_Z );
    dataSiBmx055GyroX = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_X );
    dataSiBmx055GyroY = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Y );
    dataSiBmx055GyroZ = HalSensorBMX055Gyro_Get( EN_SEN_BMX055_Z );
    dataSiBmx055MagX  = HalSensorBMX055Mag_Get( EN_SEN_BMX055_X );
    dataSiBmx055MagY  = HalSensorBMX055Mag_Get( EN_SEN_BMX055_Y );
    dataSiBmx055MagZ  = HalSensorBMX055Mag_Get( EN_SEN_BMX055_Z );

    dataSiGp2y0e03    = HalSensorGP2Y0E03_Get();

    dataSiLps25hAtmos = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
    dataSiLps25hTemp  = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );

    dataSiTsl2561     = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

    // node.js サーバへデータを渡すための printf()
    AppIfPc_Printf( "{ " );
    AppIfPc_Printf( "\"si_bme280_atmos\": %5.2f, ", dataSiBme280Atmos->cur );
    AppIfPc_Printf( "\"si_bme280_humi\": %5.2f, ",  dataSiBme280Humi->cur );
    AppIfPc_Printf( "\"si_bme280_temp\": %5.2f, ",  dataSiBme280Temp->cur );

    AppIfPc_Printf( "\"si_bmx055_acc_x\": %5.2f, ",  dataSiBmx055AccX->cur );
    AppIfPc_Printf( "\"si_bmx055_acc_y\": %5.2f, ",  dataSiBmx055AccY->cur );
    AppIfPc_Printf( "\"si_bmx055_acc_z\": %5.2f, ",  dataSiBmx055AccZ->cur );
    AppIfPc_Printf( "\"si_bmx055_gyro_x\": %5.2f, ", dataSiBmx055GyroX->cur );
    AppIfPc_Printf( "\"si_bmx055_gyro_y\": %5.2f, ", dataSiBmx055GyroY->cur );
    AppIfPc_Printf( "\"si_bmx055_gyro_z\": %5.2f, ", dataSiBmx055GyroZ->cur );
    AppIfPc_Printf( "\"si_bmx055_mag_x\": %5.2f, ",  dataSiBmx055MagX->cur );
    AppIfPc_Printf( "\"si_bmx055_mag_y\": %5.2f, ",  dataSiBmx055MagY->cur );
    AppIfPc_Printf( "\"si_bmx055_mag_z\": %5.2f, ",  dataSiBmx055MagZ->cur );

    AppIfPc_Printf( "\"si_gp2y0e03\": %5.2f, ", dataSiGp2y0e03->cur );

    AppIfPc_Printf( "\"si_lps25h_atmos\": %5.2f, ", dataSiLps25hAtmos->cur );
    AppIfPc_Printf( "\"si_lps25h_temp\": %5.2f, ", dataSiLps25hTemp->cur );

    AppIfPc_Printf( "\"si_tsl2561_lux\": %5.2f ", dataSiTsl2561->cur );
    AppIfPc_Printf( "}" );
    AppIfPc_Printf( "\n\r" );
    return;
}


#ifdef __cplusplus
    }
#endif

