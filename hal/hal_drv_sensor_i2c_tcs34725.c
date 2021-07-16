/**************************************************************************//*!
 *  @file           hal_drv_sensor_i2c_tcs34725.c
 *  @brief          [HAL] SENSOR (I2C) TCS34725 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.07.15
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "hal_cmn.h"
#include "hal.h"


#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define TCS34725_COMMAND_BIT      (0x80)

#define TCS34725_ENABLE           (0x00)
#define TCS34725_ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS34725_ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS34725_ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define TCS34725_ID               (0x12)    /* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */

#define TCS34725_ATIME            (0x01)    /* Integration time */
#define TCS34725_CONTROL          (0x0F)    /* Set the gain level for the sensor */

#define TCS34725_CDATAL           (0x14)    /* Clear channel data */
#define TCS34725_CDATAH           (0x15)
#define TCS34725_RDATAL           (0x16)    /* Red channel data */
#define TCS34725_RDATAH           (0x17)
#define TCS34725_GDATAL           (0x18)    /* Green channel data */
#define TCS34725_GDATAH           (0x19)
#define TCS34725_BDATAL           (0x1A)    /* Blue channel data */
#define TCS34725_BDATAH           (0x1B)


//********************************************************
/*! @enum                                                */
//********************************************************
typedef enum tagEHalIntegrationTime
{
    TCS34725_INTEGRATIONTIME_2_4MS  = 0xFF,   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
    TCS34725_INTEGRATIONTIME_24MS   = 0xF6,   /**<  24ms  - 10 cycles  - Max Count: 10240 */
    TCS34725_INTEGRATIONTIME_50MS   = 0xEB,   /**<  50ms  - 20 cycles  - Max Count: 20480 */
    TCS34725_INTEGRATIONTIME_101MS  = 0xD5,   /**<  101ms - 42 cycles  - Max Count: 43008 */
    TCS34725_INTEGRATIONTIME_154MS  = 0xC0,   /**<  154ms - 64 cycles  - Max Count: 65535 */
    TCS34725_INTEGRATIONTIME_700MS  = 0x00    /**<  700ms - 256 cycles - Max Count: 65535 */
} EHalIntegrationTime_t;

typedef enum tagEHalGain
{
    TCS34725_GAIN_1X                = 0x00,   /**<  No gain  */
    TCS34725_GAIN_4X                = 0x01,   /**<  4x gain  */
    TCS34725_GAIN_16X               = 0x02,   /**<  16x gain */
    TCS34725_GAIN_60X               = 0x03    /**<  60x gain */
} EHalGain_t;


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
static SHalSensor_t     g_dataRed;    // センサの値
static SHalSensor_t     g_dataGreen;  // センサの値
static SHalSensor_t     g_dataBlue;   // センサの値
static SHalSensor_t     g_dataC;      // センサの値


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );

static EHalBool_t   Enable( void );
static EHalBool_t   SetIntegrationTime( EHalIntegrationTime_t it );
static EHalBool_t   SetGain( EHalGain_t gain );

static EHalBool_t   GetData( void );




/**************************************************************************//*!
 * @brief     変数を初期化する。
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

    g_dataRed.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataRed.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataRed.max = 0;             // max = センサの最大値
    g_dataRed.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataRed.err = 0;             // err = cur - ofs
    g_dataRed.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataRed.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataGreen.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataGreen.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataGreen.max = 0;             // max = センサの最大値
    g_dataGreen.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataGreen.err = 0;             // err = cur - ofs
    g_dataGreen.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataGreen.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataBlue.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataBlue.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataBlue.max = 0;             // max = センサの最大値
    g_dataBlue.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataBlue.err = 0;             // err = cur - ofs
    g_dataBlue.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataBlue.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataC.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataC.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataC.max = 0;             // max = センサの最大値
    g_dataC.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataC.err = 0;             // err = cur - ofs
    g_dataC.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataC.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

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
    SHalSensor_t  dataRed;
    SHalSensor_t  dataGreen;
    SHalSensor_t  dataBlue;
    SHalSensor_t  dataC;

    DBG_PRINT_TRACE( "\n\r" );

    HalSensorTCS34725_Get( &dataRed, &dataGreen, &dataBlue, &dataC );
    g_dataRed.ofs   = dataRed.cur;
    g_dataGreen.ofs = dataGreen.cur;
    g_dataBlue.ofs  = dataBlue.cur;
    g_dataC.ofs     = dataC.cur;
    return;
}


/**************************************************************************//*!
 * @brief     初期化処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalSensorTCS34725_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを TCS34725 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_TCS34725 );

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
 * @brief     終了処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorTCS34725_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     デバイスを有効にする
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
Enable(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       config[2];

    config[0] = TCS34725_COMMAND_BIT | TCS34725_ENABLE;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ENABLE to i2c slave. \n\r" );
        return ret;
    }

    config[0] = TCS34725_ENABLE_PON;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ENABLE_PON to i2c slave. \n\r" );
        return ret;
    }

    usleep( 3 * 1000 );

    config[0] = TCS34725_COMMAND_BIT | TCS34725_ENABLE;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ENABLE to i2c slave. \n\r" );
        return ret;
    }

    config[0] = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ENABLE_PON | NABLE_AEN to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     デバイスを無効にする
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
Disable(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       config[2];

    config[0] = TCS34725_COMMAND_BIT | TCS34725_ENABLE;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ENABLE to i2c slave. \n\r" );
        return ret;
    }

    config[0] = ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ~(ENABLE_PON | NABLE_AEN) to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     インテグレーションタイムを設定する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetIntegrationTime(
    EHalIntegrationTime_t   it  ///< [in] インテグレーションタイム
){
    EHalBool_t              ret = EN_FALSE;
    unsigned char           config[2];

    config[0] = TCS34725_COMMAND_BIT | TCS34725_ATIME;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write ATIME to i2c slave. \n\r" );
        return ret;
    }

    config[0] = it;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write I-TIME to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     ゲインを設定する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetGain(
    EHalGain_t          gain  ///< [in] ゲイン
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       config[2];

    config[0] = TCS34725_COMMAND_BIT | TCS34725_CONTROL;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write CONTROL to i2c slave. \n\r" );
        return ret;
    }

    config[0] = gain;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write GAIN to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     データを取得する。
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

    unsigned char       config[2];
    int                 dataRed   = 0;    // 計算結果の一時保管用
    int                 dataGreen = 0;    // 計算結果の一時保管用
    int                 dataBlue  = 0;    // 計算結果の一時保管用
    int                 dataC     = 0;    // 計算結果の一時保管用

    DBG_PRINT_TRACE( "\n\r" );

    SetIntegrationTime( TCS34725_INTEGRATIONTIME_24MS );
    SetGain( TCS34725_GAIN_1X );
    Enable();

    // Red
    config[0] = TCS34725_COMMAND_BIT | TCS34725_RDATAL;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write RDATAL to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( config, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read dataRed from i2c slave. \n\r" );
        goto err;
    }
    dataRed = (config[1] << 8) | config[0];

    // Green
    config[0] = TCS34725_COMMAND_BIT | TCS34725_GDATAL;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write GDATAL to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( config, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read dataGreen from i2c slave. \n\r" );
        goto err;
    }
    dataGreen = (config[1] << 8) | config[0];

    // Blue
    config[0] = TCS34725_COMMAND_BIT | TCS34725_BDATAL;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write BDATAL to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( config, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read dataBlue from i2c slave. \n\r" );
        goto err;
    }
    dataBlue = (config[1] << 8) | config[0];

    // C
    config[0] = TCS34725_COMMAND_BIT | TCS34725_CDATAL;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write CDATAL to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( config, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read dataC from i2c slave. \n\r" );
        goto err;
    }
    dataC = (config[1] << 8) | config[0];

    DBG_PRINT_TRACE( "dataRed   = %d \n\r", dataRed );
    DBG_PRINT_TRACE( "dataGreen = %d \n\r", dataGreen );
    DBG_PRINT_TRACE( "dataBlue  = %d \n\r", dataBlue );
    DBG_PRINT_TRACE( "dataC     = %d \n\r", dataC );
    DBG_PRINT_TRACE( "RGB       = 0x%02X%02X%02X \n\r", dataRed, dataGreen, dataBlue );

    HalCmn_UpdateSenData( &g_dataRed,   dataRed );    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataGreen, dataGreen );  // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataBlue,  dataBlue );   // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataC,     dataC );      // グローバル変数を更新する
    ret = EN_TRUE;

err:
    Disable();
    return ret;
}


/**************************************************************************//*!
 * @brief     センサ変数のアドレスを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorTCS34725_Get(
    SHalSensor_t*       red,    ///< [in] RED
    SHalSensor_t*       green,  ///< [in] GREEN
    SHalSensor_t*       blue,   ///< [in] BLUE
    SHalSensor_t*       c       ///< [in] C
){
    EHalBool_t          res = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを TCS34725 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_TCS34725 );

    res = GetData();
    if( res == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to get data from i2c slave. \n\r" );
        goto err;
    }

err:
    HalCmn_CopySenData( red,   &g_dataRed );
    HalCmn_CopySenData( green, &g_dataGreen );
    HalCmn_CopySenData( blue,  &g_dataBlue );
    HalCmn_CopySenData( c,     &g_dataC );
    return;
}


#ifdef __cplusplus
    }
#endif

