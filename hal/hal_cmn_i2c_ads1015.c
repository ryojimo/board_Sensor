/**************************************************************************//*!
 *  @file           hal_cmn_i2c_ads1015.c
 *  @brief          [HAL] I2C AD コンバータ ADS1015 のドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           参考サイト：https://github.com/adafruit/Adafruit_ADS1X15
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.02.15
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <math.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define ADS1015_REG_POINTER_MASK        (0x03)   ///< Point mask
#define ADS1015_REG_POINTER_CONVERT     (0x00)   ///< Conversion
#define ADS1015_REG_POINTER_CONFIG      (0x01)   ///< Configuration
#define ADS1015_REG_POINTER_LOWTHRESH   (0x02)   ///< Low threshold
#define ADS1015_REG_POINTER_HITHRESH    (0x03)   ///< High threshold

#define ADS1015_REG_CONFIG_CQUE_NONE    (0x0003) ///< Disable the comparator and put ALERT/RDY in high state (default)
#define ADS1015_REG_CONFIG_CLAT_NONLAT  (0x0000) ///< Non-latching comparator (default)
#define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000) ///< ALERT/RDY pin is low when active (default)
#define ADS1015_REG_CONFIG_CMODE_TRAD   (0x0000) ///< Traditional comparator with hysteresis (default)
#define ADS1015_REG_CONFIG_DR_128SPS    (0x0000) ///< 128 samples per second
#define ADS1015_REG_CONFIG_DR_1600SPS   (0x0080) ///< 1600 samples per second (default)
#define ADS1015_REG_CONFIG_MODE_CONTIN  (0x0000) ///< Continuous conversion mode
#define ADS1015_REG_CONFIG_MODE_SINGLE  (0x0100) ///< Power-down single-shot mode (default)

#define ADS1015_REG_CONFIG_PGA_6_144V   (0x0000) ///< +/-6.144V range = Gain 2/3
#define ADS1015_REG_CONFIG_PGA_4_096V   (0x0200) ///< +/-4.096V range = Gain 1

#define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0
#define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1
#define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2
#define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3

#define ADS1015_REG_CONFIG_OS_SINGLE    (0x8000) ///< Write: Set to start a single-conversion
#define ADS1015_REG_CONFIG_OS_BUSY      (0x0000) ///< Read: Bit = 0 when conversion is in progress


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
// なし




/**************************************************************************//*!
 * @brief     ADS1015 の対象の ch の AD 値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    ADS1015 の AD 値
 *************************************************************************** */
int
HalCmnI2cAds1015_Get(
    EHalCh_t            which   ///< [in] 対象のセンサ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned short      config = 0;
    unsigned char       buff[3];
    int                 data = 0;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを ADS1015 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_ADS1015 );

    // 各種設定値を設定
    config =
        ADS1015_REG_CONFIG_CQUE_NONE |    // 0x0003 Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT |  // 0x0000 Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // 0x0000 Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // 0x0000 Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 0x0080 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_CONTIN;   // 0x0000 Continuous conversion mode

    // PGA/voltage range を設定
    config |= ADS1015_REG_CONFIG_PGA_4_096V; // 0x0200

    // single-ended input channel を設定
    switch( which )
    {
    case 0: config |= ADS1015_REG_CONFIG_MUX_SINGLE_0; break;
    case 1: config |= ADS1015_REG_CONFIG_MUX_SINGLE_1; break;
    case 2: config |= ADS1015_REG_CONFIG_MUX_SINGLE_2; break;
    case 3: config |= ADS1015_REG_CONFIG_MUX_SINGLE_3; break;
    default: DBG_PRINT_ERROR( "invalid channel. ch: %d. \n\r", which ); break;
    }

    // 'start single-conversion' に設定
    config |= ADS1015_REG_CONFIG_OS_SINGLE;   // 0x8000

    // config register に設定値を書き込み
    buff[0] = ADS1015_REG_POINTER_CONFIG;
    buff[1] = ( config & 0xFF00 ) >> 8;
    buff[2] = ( config & 0x00FF ) >> 0;
    ret = HalCmnI2c_Write( buff, 3 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg to i2c slave. \n\r" );
        return ret;
    }

    // Wait for the conversion to complete
    usleep(1000);

    // Read the conversion results
    buff[0] = ADS1015_REG_POINTER_CONVERT;
    ret = HalCmnI2c_Write( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg to i2c slave. \n\r" );
        return ret;
    }
    ret = HalCmnI2c_Read( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // 読み出した値から AD 値を求める
    data = (buff[1] & 0xFF);
    data |= (buff[0] & 0xFF) << 8;
    data = data >> 4;
    DBG_PRINT_TRACE( "data = 0x%04X = %04d \n\r", data, data );

    return data;
}


#ifdef __cplusplus
    }
#endif

