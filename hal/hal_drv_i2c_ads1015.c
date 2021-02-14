/**************************************************************************//*!
 *  @file           hal_drv_i2c_ads1015.c
 *  @brief          [HAL] I2C ADS1015 (AD コンバータ ドライバ) の API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.02.13
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


#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"

#include "../app/if_pc/if_pc.h"


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
static void         InitParam( void );
static EHalBool_t   InitReg( void );

static void         InitDevice( void );




/**************************************************************************//*!
 * @brief     ファイルスコープ内のグローバル変数を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
InitParam(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
InitReg(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     デバイスを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
InitDevice(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     ADS1015 を初期設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cAds1015_Init(
    void
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを ADS1015 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_ADS1015 );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize I2C port. \n\r" );
        return ret;
    } else
    {
        InitDevice();
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C ADS1015 を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalI2cAds1015_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


#if 0
static unsigned char i2cRead(void) {
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff;

    ret = HalCmnI2c_Read( &buff, 1 );
    return buff;
}

static EHalBool_t i2cWrite(unsigned char x) {
    EHalBool_t      ret = EN_FALSE;

    ret = HalCmnI2c_Write( &x, 1 );
    return;
}
#endif

static EHalBool_t writeRegister(unsigned char reg, unsigned short value) {
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];

    DBG_PRINT_TRACE( "\n\r" );
    DBG_PRINT_TRACE( "reg   = 0x%02X \n\r", reg );
    DBG_PRINT_TRACE( "value = 0x%04X \n\r", value );

    buff[0] = reg;
    ret = HalCmnI2c_Write( &buff[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg to i2c slave. \n\r" );
        return ret;
    }

    buff[0] = ( value & 0xFF00 ) >> 8;
    buff[1] = ( value & 0x00FF ) >> 0;
    DBG_PRINT_TRACE( "buff[0] = 0x%02X \n\r", buff[0] );
    DBG_PRINT_TRACE( "buff[1] = 0x%02X \n\r", buff[1] );
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    return EN_TRUE;
}

static unsigned short readRegister(unsigned char reg) {
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];
    unsigned int    data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    DBG_PRINT_TRACE( "reg   = 0x%02X \n\r", reg );

    buff[0] = reg;
    ret = HalCmnI2c_Write( &buff[0], 1 );
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

    DBG_PRINT_TRACE( "buff[0] = 0x%02X \n\r", buff[0] );
    DBG_PRINT_TRACE( "buff[1] = 0x%02X \n\r", buff[1] );

    data = (buff[0] << 8) | buff[1];
    return data;
}


/**************************************************************************//*!
 * @brief     ADS1015 の対象の ch の AD 値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    ADS1015 の AD 値
 *************************************************************************** */
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

unsigned int
HalI2cAds1015_Get(
    unsigned char       ch      ///< [in] 対象の ch ( 0 ～ 3 )
){
    unsigned short      data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    DBG_PRINT_TRACE( "ch = %02d \n\r", ch );

    // Start with default values
    unsigned short config =
        ADS1015_REG_CONFIG_CQUE_NONE |    // 0x0003 Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT |  // 0x0000 Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // 0x0000 Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // 0x0000 Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 0x0080 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_CONTIN;   // 0x0000 Continuous conversion mode

    // Set PGA/voltage range
    config |= ADS1015_REG_CONFIG_PGA_4_096V; // 0x0200

    // Set single-ended input channel
    switch (ch) {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;  // 0x4000
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;   // 0x8000

    // I2C スレーブデバイスを ADS1015 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_ADS1015 );

    // Write config register to the ADC
    writeRegister( ADS1015_REG_POINTER_CONFIG, config );

    // Wait for the conversion to complete
    sleep(2);

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    data = readRegister( ADS1015_REG_POINTER_CONVERT );
    DBG_PRINT_TRACE( "data  = %d     \n\r", data );
    DBG_PRINT_TRACE( "data  = 0x%04X \n\r", data );
    return data;
}


#ifdef __cplusplus
    }
#endif

