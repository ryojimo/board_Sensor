/**************************************************************************//*!
 *  @file           opt_sensors.c
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
    AppIfPc_Printf( "     -s, --sensors : display the values of all sensors. \n\r" );
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
Opt_Sensors(
    void
){

    DBG_PRINT_TRACE( "Opt_Sensors() \n\r" );
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
    AppIfPc_Printf( "{ " );
    AppIfPc_Printf( "\"sa_acc_x\": %d, ", (int)dataSaAccX->cur );
    AppIfPc_Printf( "\"sa_acc_y\": %d, ", (int)dataSaAccY->cur );
    AppIfPc_Printf( "\"sa_acc_z\": %d, ", (int)dataSaAccZ->cur );

    AppIfPc_Printf( "\"sa_gyro_g1\": %d, ", (int)dataSaGyroG1->cur );
    AppIfPc_Printf( "\"sa_gyro_g2\": %d, ", (int)dataSaGyroG2->cur );

    AppIfPc_Printf( "\"si_bme280_atmos\": %5.2f, ", dataSiBme280Atmos->cur );
    AppIfPc_Printf( "\"si_bme280_humi\": %5.2f, ", dataSiBme280Humi->cur );
    AppIfPc_Printf( "\"si_bme280_temp\": %5.2f, ", dataSiBme280Temp->cur );

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

