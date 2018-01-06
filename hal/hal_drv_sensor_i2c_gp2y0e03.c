/**************************************************************************//*!
 *  @file           hal_sensor_i2c_gp2y0e03.c
 *  @brief          [HAL] SENSOR (I2C) GP2Y0E03 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.16
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
#define DISTANCE            (0x5E)


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
static SHalSensor_t     g_data;     // センサの値


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );

static EHalBool_t   GetDist( void );




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

    g_data.cur = 0;             // cur = センサの現在値 ( mm )
    g_data.ofs = 0;             // ofs = 初期化時に設定したセンサのオフセット値
    g_data.max = 0;             // max = センサの最大値
    g_data.min = 0xFFFFFFFF;    // min = センサの最小値
    g_data.err = 0;             // err = cur - ofs
    g_data.cur_rate = 0;        // cur_rate = ( cur / max ) * 100 ( %  )
    g_data.cur_vol = 0;         // cur_vol = 電圧に換算した現在値 ( mV )

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

    data = HalSensorPm_Get();
    g_data.ofs = data->cur;

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
HalSensorGP2Y0E03_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを GP2Y0E03 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_GP2Y0E03 );

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
HalSensorGP2Y0E03_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     距離を取得する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
GetDist(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;

    unsigned char       regAddr;        // コマンドのレジスタ・アドレスをセット
    unsigned char       buff[2];
    int                 data = 0;       // 計算結果の一時保管用
    double              data_d = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // DISTANCE のデータをスレーブデバイスから読み出す
    regAddr = DISTANCE;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write DISTANCE to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // 値を計算する
    data = ( ( buff[0] * 16 + buff[1] ) / 16 ) / 4;
    data_d = (double)data;

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_data, data_d );
#if 0
    AppIfPc_Printf( "data_d         = %f cm \n\r", data_d );
    AppIfPc_Printf( "g_data.cur = %f cm \n\r", g_data.cur );
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
HalSensorGP2Y0E03_Get(
    void  ///< [in] ナシ
){
    EHalBool_t          res = EN_FALSE;
    SHalSensor_t*       ret = NULL;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを GP2Y0E03 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_GP2Y0E03 );

    res = GetDist();
    if( res == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to get data from i2c slave. \n\r" );
        goto err;
    }

err:
    ret = &g_data;

    return ret;
}


#ifdef __cplusplus
    }
#endif

