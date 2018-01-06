/**************************************************************************//*!
 *  @file           hal_sensor_i2c_tsl2561.c
 *  @brief          [HAL] SENSOR (I2C) TSL2561 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.08.28
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "./tsl2561/TSL2561.h"
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
static SHalSensor_t     g_dataBroadband;    // センサの値
static SHalSensor_t     g_dataIr;           // センサの値
static SHalSensor_t     g_dataLux;          // センサの値


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
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

    g_dataBroadband.cur = 0;             // cur = センサの現在値 ( degree )
    g_dataBroadband.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataBroadband.max = 0;             // max = センサの最大値
    g_dataBroadband.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataBroadband.err = 0;             // err = cur - ofs
    g_dataBroadband.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataBroadband.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataIr.cur = 0;
    g_dataIr.ofs = 0;
    g_dataIr.max = 0;
    g_dataIr.min = 0xFFFFFFFF;
    g_dataIr.err = 0;
    g_dataIr.cur_rate = 0;
    g_dataIr.cur_vol = 0;

    g_dataLux.cur = 0;
    g_dataLux.ofs = 0;
    g_dataLux.max = 0;
    g_dataLux.min = 0xFFFFFFFF;
    g_dataLux.err = 0;
    g_dataLux.cur_rate = 0;
    g_dataLux.cur_vol = 0;

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

    data = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
    g_dataBroadband.ofs = data->cur;

    data = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
    g_dataIr.ofs = data->cur;

    data = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );
    g_dataLux.ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR TSL2561 を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalSensorTSL2561_Init(
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
    }

    SetOffset();
    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR TSL2561 を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorTSL2561_Fini(
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
    double              data_d = 0;     // センサの計測値

    int                 rc;
    uint16_t            broadband = 0;
    uint16_t            ir = 0;
    uint32_t            lux=0;

    DBG_PRINT_TRACE( "\n\r" );

    // prepare the sensor
    // (the first parameter is the raspberry pi i2c master controller attached to the TSL2561, the second is the i2c selection jumper)
    // The i2c selection address can be one of: TSL2561_ADDR_LOW, TSL2561_ADDR_FLOAT or TSL2561_ADDR_HIGH
    TSL2561 light1 = TSL2561_INIT( 1, TSL2561_ADDR_FLOAT );

    // initialize the sensor
    rc = TSL2561_OPEN( &light1 );
    if( rc != 0 )
    {
        fprintf( stderr, "Error initializing TSL2561 sensor (%s). Check your i2c bus (es. i2cdetect)\n", strerror(light1.lasterr) );
        // you don't need to TSL2561_CLOSE() if TSL2561_OPEN() failed, but it's safe doing it.
        TSL2561_CLOSE( &light1 );
        return ret;
    }

    // set the gain to 1X (it can be TSL2561_GAIN_1X or TSL2561_GAIN_16X)
    // use 16X gain to get more precision in dark ambients, or enable auto gain below
    rc = TSL2561_SETGAIN( &light1, TSL2561_GAIN_1X );
    if( rc != 0 )
    {
        DBG_PRINT_ERROR( "TSL2561_SETGAIN() error. \n\r" );
        ret = EN_FALSE;
        goto err;
    }

    // set the integration time 
    // (TSL2561_INTEGRATIONTIME_402MS or TSL2561_INTEGRATIONTIME_101MS or TSL2561_INTEGRATIONTIME_13MS)
    // TSL2561_INTEGRATIONTIME_402MS is slower but more precise, TSL2561_INTEGRATIONTIME_13MS is very fast but not so precise
    rc = TSL2561_SETINTEGRATIONTIME( &light1, TSL2561_INTEGRATIONTIME_101MS );
    if( rc != 0 )
    {
        DBG_PRINT_ERROR( "TSL2561_SETINTEGRATIONTIME() error. \n\r" );
        ret = EN_FALSE;
        goto err;
    }

    // sense the luminosity from the sensor (lux is the luminosity taken in "lux" measure units)
    // the last parameter can be 1 to enable library auto gain, or 0 to disable it
    rc = TSL2561_SENSELIGHT( &light1, &broadband, &ir, &lux, 1 );
    if( rc != 0 )
    {
        DBG_PRINT_ERROR( "TSL2561_SENSELIGHT() error. \n\r" );
        ret = EN_FALSE;
        goto err;
    }

    // グローバル変数を更新する
    data_d = broadband;
    HalCmn_UpdateSenData( &g_dataBroadband, data_d );

    data_d = ir;
    HalCmn_UpdateSenData( &g_dataIr, data_d );

    data_d = lux;
    HalCmn_UpdateSenData( &g_dataLux, data_d );

    ret = EN_TRUE;
err:
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
HalSensorTSL2561_Get(
    EHalSensorTSL2561_t which   ///< [in] 対象のセンサ
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
    case EN_SEN_TSL2561_BROADBAND : ret = &g_dataBroadband; break;
    case EN_SEN_TSL2561_IR        : ret = &g_dataIr;  break;
    case EN_SEN_TSL2561_LUX       : ret = &g_dataLux;  break;
    default : 
        DBG_PRINT_ERROR( "Invalid argument. \n\r" );
    break;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

