/**************************************************************************//*!
 *  @file           hal_i2c_cmd.c
 *  @brief          [HAL] I2C 通信ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           http://www.picfun.com/c15.html
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2017.07.07
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
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );




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
 * @brief     初期化処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cCmd_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを PIC18F14K50 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PIC18F14K50 );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize I2C port. \n\r" );
        return ret;
    }

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
HalI2cCmd_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     I2C Slave デバイス (= PIC18F14K50 ) へコマンドを送信する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cCmd_Set(
    EHalI2cCmd_t        cmd,    ///< [in] 送信するコマンド
    unsigned char       data    ///< [in] 送信する 1 Byte データ
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       buff[2];

    // I2C スレーブデバイスを PIC18F14K50 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PIC18F14K50 );

    buff[0] = cmd;
    buff[1] = data;
    DBG_PRINT_TRACE( "buff[0]=0x%X \n\r", buff[0] );
    DBG_PRINT_TRACE( "buff[1]=0x%X \n\r", buff[1] );
    ret = HalCmnI2c_Write( buff, 2 );

    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write cmnd to i2c slave. \n\r" );
        goto err;
    }

err:
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C Slave デバイス (= PIC18F14K50 ) へ USB Keyboard Keycode を送信する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cCmd_SetKeycode(
    unsigned char       keycode    ///< [in] 送信する Keycode
){
    EHalBool_t          ret = EN_FALSE;
    unsigned char       buff;

    // I2C スレーブデバイスを PIC18F14K50 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PIC18F14K50 );

    buff = keycode;
    DBG_PRINT_TRACE( "buff=0x%X \n\r", buff );
    ret = HalCmnI2c_Write( &buff, 1 );

    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write cmnd to i2c slave. \n\r" );
        goto err;
    }

err:
    return ret;
}


#ifdef __cplusplus
    }
#endif

