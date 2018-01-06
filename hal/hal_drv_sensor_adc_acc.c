/**************************************************************************//*!
 *  @file           hal_sensor_adc_acc.c
 *  @brief          [HAL] SENSOR (ADC) 加速度センサ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.24
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
static SHalSensor_t     g_dataX;    // センサの値 ( 加速度センサ X 方向 )
static SHalSensor_t     g_dataY;    // センサの値 ( 加速度センサ Y 方向 )
static SHalSensor_t     g_dataZ;    // センサの値 ( 加速度センサ Z 方向 )


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );




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

    g_dataX.cur = 0;                // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataX.ofs = 0;                // ofs = 初期化時に設定したセンサのオフセット値
    g_dataX.max = MCP3208_MAX_VALE; // max = センサの最大値
    g_dataX.min = MCP3208_MAX_VALE; // min = センサの最小値
    g_dataX.err = 0;                // err = cur - ofs
    g_dataX.cur_rate = 0;           // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataX.cur_vol = 0;            // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataY.cur = 0;
    g_dataY.ofs = 0;
    g_dataY.max = MCP3208_MAX_VALE;
    g_dataY.min = MCP3208_MAX_VALE;
    g_dataY.err = 0;
    g_dataY.cur_rate = 0;
    g_dataY.cur_vol = 0;

    g_dataZ.cur = 0;
    g_dataZ.ofs = 0;
    g_dataZ.max = MCP3208_MAX_VALE;
    g_dataZ.min = MCP3208_MAX_VALE;
    g_dataZ.err = 0;
    g_dataZ.cur_rate = 0;
    g_dataZ.cur_vol = 0;

    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention シングルモードで動作。
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

    data = HalSensorAcc_Get( EN_SEN_ACC_X );
    g_dataX.ofs = data->cur;

    data = HalSensorAcc_Get( EN_SEN_ACC_Y );
    g_dataY.ofs = data->cur;

    data = HalSensorAcc_Get( EN_SEN_ACC_Z );
    g_dataZ.ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     初期化処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 初期化成功, EN_FALSE : 初期化失敗
 *************************************************************************** */
EHalBool_t
HalSensorAcc_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize spi port. \n\r" );
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
HalSensorAcc_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     センサ変数のアドレスを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数のアドレス
 *************************************************************************** */
SHalSensor_t*
HalSensorAcc_Get(
    EHalSensorAcc_t     which   ///< [in] 対象のセンサ
){
    SHalSensor_t*       ret = NULL;
    unsigned int        data = 0;

    DBG_PRINT_TRACE( "\n\r" );

    switch( which )
    {
    case EN_SEN_ACC_X :
        data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_0 );
        HalCmn_UpdateSenData( &g_dataX, (double)data );
        ret = &g_dataX;
    break;
    case EN_SEN_ACC_Y :
        data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_1 );
        HalCmn_UpdateSenData( &g_dataY, (double)data );
        ret = &g_dataY;
    break;
    case EN_SEN_ACC_Z :
        data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_2 );
        HalCmn_UpdateSenData( &g_dataZ, (double)data );
        ret = &g_dataZ;
    break;
    default : 
        DBG_PRINT_ERROR( "Invalid argument. \n\r" );
    break;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

