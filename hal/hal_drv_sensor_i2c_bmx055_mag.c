/**************************************************************************//*!
 *  @file           hal_drv_sensor_i2c_bmx055_mag.c
 *  @brief          [HAL] SENSOR (I2C) BMX055 ドライバ (磁気) API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2021.06.08
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
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
static SHalSensor_t     g_dataX;    // センサの値 ( X 方向 )
static SHalSensor_t     g_dataY;    // センサの値 ( Y 方向 )
static SHalSensor_t     g_dataZ;    // センサの値 ( Z 方向 )


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );

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

    g_dataX.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataX.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataX.max = 0;             // max = センサの最大値
    g_dataX.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataX.err = 0;             // err = cur - ofs
    g_dataX.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataX.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataY.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataY.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataY.max = 0;             // max = センサの最大値
    g_dataY.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataY.err = 0;             // err = cur - ofs
    g_dataY.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataY.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataZ.cur = 0;             // cur = センサの現在値 ( ? )
    g_dataZ.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_dataZ.max = 0;             // max = センサの最大値
    g_dataZ.min = 0xFFFFFFFF;    // min = センサの最小値
    g_dataZ.err = 0;             // err = cur - ofs
    g_dataZ.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataZ.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

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
    DBG_PRINT_TRACE( "\n\r" );

    GetData();
    g_dataX.ofs = g_dataX.cur;
    g_dataY.ofs = g_dataY.cur;
    g_dataZ.ofs = g_dataZ.cur;
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
HalSensorBMX055Mag_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055_MAG に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_MAG );

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
HalSensorBMX055Mag_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     センサ・データを取得する。
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

#if 0
    unsigned char       config[6];
    int                 data = 0;       // 計算結果の一時保管用
    double              data_d = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // Select Mag register(0x4B)
    // Soft reset(0x83)
    config[0] = 0x4B;
    config[1] = 0x83;
    ret = HalCmnI2c_Write( &config[0], 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x4B to i2c slave. \n\r" );
        return ret;
    }

    // Select Mag register(0x4C)
    // Normal Mode, ODR = 10 Hz(0x00)
    config[0] = 0x4C;
    config[1] = 0x00;
    ret = HalCmnI2c_Write( &config[0], 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x4C to i2c slave. \n\r" );
        return ret;
    }

    // Select Mag register(0x4E)
    // X, Y, Z-Axis enabled(0x84)
    config[0] = 0x4E;
    config[1] = 0x84;
    ret = HalCmnI2c_Write( &config[0], 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x4E to i2c slave. \n\r" );
        return ret;
    }

    // Select Mag register(0x51)
    // No. of Repetitions for X-Y Axis = 9(0x04)
    config[0] = 0x51;
    config[1] = 0x04;
    ret = HalCmnI2c_Write( &config[0], 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x51 to i2c slave. \n\r" );
        return ret;
    }

    // Select Mag register(0x52)
    // No. of Repetitions for Z-Axis = 15(0x0F)
    config[0] = 0x52;
    config[1] = 0x0F;
    ret = HalCmnI2c_Write( &config[0], 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x52 to i2c slave. \n\r" );
        return ret;
    }

    // 200ms スリープ
//    usleep( 200 * 1000 );

    // Read 6 bytes of data from register(0x42)
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    config[0] = 0x42;
    ret = HalCmnI2c_Write( &config[0], 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write reg:0x42 to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &config[0], 6 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // 値を計算する : X 軸方向
    data = (config[1] * 256 + (config[0] & 0xF8)) / 8;
    if(data > 4095){ data -= 8192; }
    data_d = (double)data;
    HalCmn_UpdateSenData( &g_dataX, data_d ); // グローバル変数を更新する

    // 値を計算する : Y 軸方向
    data = (config[3] * 256 + (config[2] & 0xF8)) / 8;
    if(data > 4095){ data -= 8192; }
    data_d = (double)data;
    HalCmn_UpdateSenData( &g_dataY, data_d ); // グローバル変数を更新する

    // 値を計算する : Z 軸方向
    data = (config[5] * 256 + (config[4] & 0xFE)) / 2;
    if(data > 16383){ data -= 32768; }
    data_d = (double)data;
    HalCmn_UpdateSenData( &g_dataZ, data_d ); // グローバル変数を更新する

#if 0
    DBG_PRINT_TRACE( "g_dataX.cur = %5.2f(?) \n\r", g_dataX.cur );
    DBG_PRINT_TRACE( "g_dataY.cur = %5.2f(?) \n\r", g_dataY.cur );
    DBG_PRINT_TRACE( "g_dataZ.cur = %5.2f(?) \n\r", g_dataZ.cur );
#endif

#endif
    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     センサ変数のアドレスを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数へのポインタ
 *************************************************************************** */
SHalSensor_t*
HalSensorBMX055Mag_Get(
    EHalSensorBMX055_t  which   ///< [in] 対象のセンサ
){
    EHalBool_t          res = EN_FALSE;
    SHalSensor_t*       ret = NULL;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055_MAG に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_MAG );

    res = GetData();
    if( res == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to get data from i2c slave. \n\r" );
    }

    switch( which )
    {
    case EN_SEN_BMX055_X : ret = &g_dataX; break;
    case EN_SEN_BMX055_Y : ret = &g_dataY; break;
    case EN_SEN_BMX055_Z : ret = &g_dataZ; break;
    default              : DBG_PRINT_ERROR( "Invalid argument. \n\r" ); break;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

