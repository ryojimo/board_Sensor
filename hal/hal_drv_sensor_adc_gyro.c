/**************************************************************************//*!
 *  @file           hal_drv_sensor_adc_gyro.c
 *  @brief          [HAL] SENSOR (ADC) ジャイロセンサ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2017.09.12
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
static SHalSensor_t     g_dataG1;   // センサの値 ( ジャイロ G1 方向 )
static SHalSensor_t     g_dataG2;   // センサの値 ( ジャイロ G2 方向 )


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

    g_dataG1.cur = 0;                // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataG1.ofs = 0;                // ofs = 初期化時に設定したセンサのオフセット値
    g_dataG1.max = MCP3208_MAX_VALE; // max = センサの最大値
    g_dataG1.min = MCP3208_MAX_VALE; // min = センサの最小値
    g_dataG1.err = 0;                // err = cur - ofs
    g_dataG1.cur_rate = 0;           // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataG1.cur_vol = 0;            // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataG2.cur = 0;
    g_dataG2.ofs = 0;
    g_dataG2.max = MCP3208_MAX_VALE;
    g_dataG2.min = MCP3208_MAX_VALE;
    g_dataG2.err = 0;
    g_dataG2.cur_rate = 0;
    g_dataG2.cur_vol = 0;

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

    data = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
    g_dataG1.ofs = data->cur;

    data = HalSensorGyro_Get( EN_SEN_GYRO_G2 );
    g_dataG2.ofs = data->cur;

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
HalSensorGyro_Init(
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
HalSensorGyro_Fini(
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
HalSensorGyro_Get(
    EHalSensorGyro_t        which   ///< [in] 対象のセンサ
){
    SHalSensor_t*           ret = NULL;
    unsigned int            data = 0;

    DBG_PRINT_TRACE( "\n\r" );

    switch( which )
    {
    case EN_SEN_GYRO_G1 :
#if 0
        data = HalCmnSpiMcp3208_Get( EN_CH3 );
#else
        data = 0;
#endif
        HalCmn_UpdateSenData( &g_dataG1, (double)data );
        ret = &g_dataG1;
    break;
    case EN_SEN_GYRO_G2 :
#if 0
        data = HalCmnSpiMcp3208_Get( EN_CH4 );
#else
        data = 0;
#endif
        HalCmn_UpdateSenData( &g_dataG2, (double)data );
        ret = &g_dataG2;
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

