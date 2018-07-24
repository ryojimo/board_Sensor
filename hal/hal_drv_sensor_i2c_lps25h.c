/**************************************************************************//*!
 *  @file           hal_sensor_i2c_lps25h.c
 *  @brief          [HAL] SENSOR (I2C) LPS25H ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                      温度 = 42.5 + [TEMP_OUTH & TEMP_OUT_L] (16bit) / 480
 *                             → [TEMP_OUTH & TEMP_OUT_L] の値は基本的には負数
 * 
 *                      気圧 = [PRESS_OUT_H & PRESS_OUT_L & PRESS_OUT_XL] (24bit) / 4096
 * 
 *  @sa             none.
 *  @note           http://www.picfun.com/c15.html
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.05
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
#define REF_P_XL            (0x08)
#define REF_P_L             (0x09)
#define REF_P_H             (0x0A)
#define WHO_AM_I            (0x0F)
#define RES_CONF            (0x10)

#define CTRL_REG1           (0x20)
#define CTRL_REG2           (0x21)
#define CTRL_REG3           (0x22)
#define CTRL_REG4           (0x23)
#define INT_CFG             (0x24)
#define INT_SOURCE          (0x25)

#define STATUS_REG          (0x27)
#define PRESS_POUT_XL       (0x28)
#define PRESS_OUT_L         (0x29)
#define PRESS_OUT_H         (0x2A)
#define TEMP_OUT_L          (0x2B)
#define TEMP_OUT_H          (0x2C)

#define FIFO_CTRL           (0x2E)
#define FIFO_STATUS         (0x2F)
#define THS_P_L             (0x30)
#define THS_P_H             (0x31)

#define RPDS_L              (0x39)
#define RPDS_H              (0x3A)


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
static SHalSensor_t     g_dataAtmos;  // 気圧センサの値
static SHalSensor_t     g_dataTemp;   // 温度センサの値


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );

static EHalBool_t   GetAtmos( void );
static EHalBool_t   GetTemp( void );




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

    data = HalSensorLPS25H_Get( EN_SEN_LPS25H_ATMOS );
    g_dataAtmos.ofs = data->cur;

    data = HalSensorLPS25H_Get( EN_SEN_LPS25H_TEMP );
    g_dataTemp.ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR LPS25H を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalSensorLPS25H_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LPS25H に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LPS25H );

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
 * @brief     I2C SENSOR LPS25H を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorLPS25H_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     気圧を取得する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
GetAtmos(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       regAddr;        // コマンドのレジスタ・アドレスをセット
    unsigned char       data01 = 0x00;  // PRESS_POUT_XL レジスタの値
    unsigned char       data02 = 0x00;  // PRESS_OUT_L   レジスタの値
    unsigned char       data03 = 0x00;  // PRESS_OUT_H   レジスタの値
    int                 data = 0;       // 計算結果の一時保管用
    double              data_d = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // PRESS_POUT_XL のデータをスレーブデバイスから読み出す
    regAddr = PRESS_POUT_XL;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write PRESS_POUT_XL to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &data01, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // PRESS_OUT_L のデータをスレーブデバイスから読み出す
    regAddr = PRESS_OUT_L;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write PRESS_OUT_L to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &data02, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // PRESS_OUT_H のデータをスレーブデバイスから読み出す
    regAddr = PRESS_OUT_H;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write PRESS_OUT_H to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &data03, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // 値を計算する
    data = ( ( data03 << 16 ) | ( data02 << 8 ) | data01 ) & 0xFFFFFFFF;
    data_d = (double)data / 4096;

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataAtmos, data_d );
#if 0
    AppIfPc_Printf( "data_d          = %f mbar \n\r", data_d );
    AppIfPc_Printf( "g_dataAtmos.cur = %f mbar \n\r", g_dataAtmos.cur );
#endif

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     気温を取得する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
GetTemp(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       regAddr;        // コマンドのレジスタ・アドレスをセット
    unsigned char       data01 = 0x00;  // TEMP_OUT_L レジスタの値
    unsigned char       data02 = 0x00;  // TEMP_OUT_H レジスタの値
    int                 data = 0;       // 計算結果の一時保管用
    double              data_d = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // TEMP_OUT_L のデータをスレーブデバイスから読み出す
    regAddr = TEMP_OUT_L;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write TEMP_OUT_L to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &data01, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // TEMP_OUT_H のデータをスレーブデバイスから読み出す
    regAddr = TEMP_OUT_H;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write PRESS_OUT_L to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &data02, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // 値を計算する
    data = ( ( data02 << 8 ) | data01 ) | 0xFFFF0000;
    data_d = (double)( ~data + 1 ) / 480;
    data_d = 42.5 -data_d;

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataTemp, data_d );
#if 0
    AppIfPc_Printf( "data_d         = %f degree \n\r", data_d );
    AppIfPc_Printf( "g_dataTemp.cur = %f degree \n\r", g_dataTemp.cur );
#endif

    ret = EN_TRUE;
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
HalSensorLPS25H_Get(
    EHalSensorLPS25H_t  which   ///< [in] 対象のセンサ
){
    EHalBool_t          ret = EN_FALSE;
    SHalSensor_t*       data = NULL;
    unsigned char       buff[2];

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LPS25H に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LPS25H );

    // WHO_AM_I のデータをスレーブデバイスから読み出す
    buff[0] = WHO_AM_I;
    ret = HalCmnI2c_Write( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write WHO_AM_I to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        goto err;
    }

    if( buff[0] != 0xBD )
    {
        DBG_PRINT_ERROR( "WHO_AM_I error. \n\r" );
        DBG_PRINT_ERROR( "data = 0x%02X \n\r", buff[0] );
        goto err;
    }

    // CTRL_REG1 に 0x90 を書き込む
    buff[0] = CTRL_REG1;
    buff[1] = 0x90;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write CTRL_REG1 to i2c slave. \n\r" );
        goto err;
    }

    switch( which )
    {
    case EN_SEN_LPS25H_ATMOS : ret = GetAtmos(); break;
    case EN_SEN_LPS25H_TEMP  : ret = GetTemp();  break;
    default                  : break;
    }

    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to get data from i2c slave. \n\r" );
        goto err;
    }

err:
    switch( which )
    {
    case EN_SEN_LPS25H_ATMOS : data = &g_dataAtmos; break;
    case EN_SEN_LPS25H_TEMP  : data = &g_dataTemp;  break;
    default                  : break;
    }

    return data;
}


#ifdef __cplusplus
    }
#endif

