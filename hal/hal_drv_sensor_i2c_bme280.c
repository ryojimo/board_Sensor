/**************************************************************************//*!
 *  @file           hal_sensor_i2c_bme280.c
 *  @brief          [HAL] SENSOR (I2C) BME280 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           https://github.com/BoschSensortec/BME280_driver
 *                  https://www.switch-science.com/catalog/2323/
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2017.12.02
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "./bme280/bme280.h"
#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


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
static struct bme280_dev   g_dev;

static SHalSensor_t     g_dataAtmos;    // センサの値
static SHalSensor_t     g_dataHumi;     // センサの値
static SHalSensor_t     g_dataTemp;     // センサの値


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );

static void         InitDevice( void );
static void         user_delay_ms( uint32_t period );
static int8_t       user_i2c_read(  uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
static int8_t       user_i2c_write( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );

static void         SetOffset( void );

static EHalBool_t   GetData( void );




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

    g_dataAtmos.cur = 0;            // cur = センサの現在値 ( mbar )
    g_dataAtmos.ofs = 0;            // ofs = 初期化時に設定したセンサのオフセット値
    g_dataAtmos.max = 0;            // max = センサの最大値
    g_dataAtmos.min = 0xFFFFFFFF;   // min = センサの最小値
    g_dataAtmos.err = 0;            // err = cur - ofs
    g_dataAtmos.cur_rate = 0;       // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataAtmos.cur_vol = 0;        // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataHumi.cur = 0;             // cur = センサの現在値 ( % )
    g_dataHumi.ofs = 0;
    g_dataHumi.max = 0;
    g_dataHumi.min = 0xFFFFFFFF;
    g_dataHumi.err = 0;
    g_dataHumi.cur_rate = 0;
    g_dataHumi.cur_vol = 0;

    g_dataTemp.cur = 0;             // cur = センサの現在値 ( degree )
    g_dataTemp.ofs = 0;
    g_dataTemp.max = 0;
    g_dataTemp.min = 0xFFFFFFFF;
    g_dataTemp.err = 0;
    g_dataTemp.cur_rate = 0;
    g_dataTemp.cur_vol = 0;

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

    int8_t      rslt = BME280_OK;
    uint8_t     settings_sel;

    HalCmnI2c_SetSlave( BME280_I2C_ADDR_PRIM );

    g_dev.dev_id   = BME280_I2C_ADDR_PRIM;
    g_dev.intf     = BME280_I2C_INTF;
    g_dev.read     = user_i2c_read;
    g_dev.write    = user_i2c_write;
    g_dev.delay_ms = user_delay_ms;

    rslt = bme280_init( &g_dev );
    if( rslt != BME280_OK )
    {
        DBG_PRINT_ERROR( "Fail to initialize the device. \n\r" );
        DBG_PRINT_ERROR( "rslt = %d \n\r", rslt );
        goto err;
    }

    /* Recommended mode of operation: Indoor navigation */
    g_dev.settings.osr_h        = BME280_OVERSAMPLING_1X;
    g_dev.settings.osr_p        = BME280_OVERSAMPLING_4X;
    g_dev.settings.osr_t        = BME280_OVERSAMPLING_2X;
    g_dev.settings.filter       = BME280_FILTER_COEFF_OFF;
    g_dev.settings.standby_time = BME280_STANDBY_TIME_10_MS;

    settings_sel =  BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings( settings_sel, &g_dev );
    rslt = bme280_set_sensor_mode( BME280_NORMAL_MODE, &g_dev );

err:
    return;
}


/**************************************************************************//*!
 * @brief     period ms の wait を入れる。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
user_delay_ms(
    uint32_t    period
){
    DBG_PRINT_TRACE( "period = %d \n\r", period );

    /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
    usleep( period * 1000 );

}


/**************************************************************************//*!
 * @brief     I2C スレーブデバイスから値を読み出す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    0 : 成功, 0 以外 : 失敗
 *************************************************************************** */
static int8_t
user_i2c_read(
    uint8_t     dev_id,
    uint8_t     reg_addr,
    uint8_t     *reg_data,
    uint16_t    len
){
    int8_t      rslt = 0;           /* Return 0 for Success, non-zero for failure */
    EHalBool_t  ret = EN_FALSE;

    DBG_PRINT_TRACE( "(dev_id, reg_addr, len)=(0x%02X, 0x%02X, %d) \n\r", dev_id, reg_addr, len );

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */
    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    ret = HalCmnI2c_Write( (unsigned char*)&reg_addr, 1 );
    ret = HalCmnI2c_Read( (unsigned char*)reg_data, (unsigned int)len );

    if( ret == EN_TRUE )
    {
        rslt = 0;
    } else {
        rslt = -1;
    }
    return rslt;
}


/**************************************************************************//*!
 * @brief     I2C スレーブデバイスに値を書き込む。
 * @attention len の値 (= reg_data[] の長さ ) は MAX64 まで
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    0 : 成功, 0 以外 : 失敗
 *************************************************************************** */
static int8_t
user_i2c_write(
    uint8_t     dev_id,
    uint8_t     reg_addr,
    uint8_t     *reg_data,
    uint16_t    len
){
    int8_t          rslt = 0;           /* Return 0 for Success, non-zero for failure */
    EHalBool_t      ret = EN_FALSE;
    int             i = 0;
    unsigned char   buff[64];

    DBG_PRINT_TRACE( "(dev_id, reg_addr, len)=(0x%02X, 0x%02X, %d) \n\r", dev_id, reg_addr, len );

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */
    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    if( len < 64 )
    {
        buff[0] = reg_addr;
        for( i = 1; i < len; i++ )
        {
            buff[ i ] = *reg_data++;
        }
        ret = HalCmnI2c_Write( buff, len + 1 );
    } else
    {
        DBG_PRINT_ERROR( "len needs under 64. len = %d \n\r", len );
        ret = EN_FALSE;
    }

    if( ret == EN_TRUE )
    {
        rslt = 0;
    } else {
        rslt = -1;
    }
    return rslt;
}

/**************************************************************************//*!
 * @brief     センサのオフセット値をセットする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
SetOffset(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
    g_dataAtmos.ofs = data->cur;

    data = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
    g_dataHumi.ofs = data->cur;

    data = HalSensorBME280_Get( EN_SEN_BME280_TEMP );
    g_dataTemp.ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR BME280 を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalSensorBME280_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize I2C port. \n\r" );
        return ret;
    } else
    {
        InitDevice();
        SetOffset();
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR BME280 を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorBME280_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     センサ値を取得する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
GetData(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;

    int8_t              rslt;
    struct bme280_data  comp_data;

    DBG_PRINT_TRACE( "\n\r" );

    HalCmnI2c_SetSlave( BME280_I2C_ADDR_PRIM );

    /* Delay while the sensor completes a measurement */
    g_dev.delay_ms( 70 );
    rslt = bme280_get_sensor_data( BME280_ALL, &comp_data, &g_dev );

    if( rslt == BME280_OK )
    {
        // グローバル変数を更新する
        HalCmn_UpdateSenData( &g_dataAtmos, comp_data.pressure / 100 );
        HalCmn_UpdateSenData( &g_dataHumi,  comp_data.humidity );
        HalCmn_UpdateSenData( &g_dataTemp,  comp_data.temperature );
        ret = EN_TRUE;
    } else
    {
        DBG_PRINT_ERROR( "error: rslt = %d \n\r", rslt );
        ret = EN_FALSE;
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数へのポインタ
 *************************************************************************** */
SHalSensor_t*
HalSensorBME280_Get(
    EHalSensorBME280_t  which   ///< [in] 対象のセンサ
){
    EHalBool_t          res = EN_FALSE;
    SHalSensor_t*       ret = NULL;

    DBG_PRINT_TRACE( "\n\r" );

    res = GetData();
    if( res == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to get data from i2c slave. \n\r" );
        goto err;
    }

err:
    switch( which )
    {
    case EN_SEN_BME280_ATMOS : ret = &g_dataAtmos; break;
    case EN_SEN_BME280_HUMI  : ret = &g_dataHumi;  break;
    case EN_SEN_BME280_TEMP  : ret = &g_dataTemp;  break;
    default : 
        DBG_PRINT_ERROR( "Invalid argument. \n\r" );
    break;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

