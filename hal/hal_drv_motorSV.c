/**************************************************************************//*!
 *  @file           hal_motorSV.c
 *  @brief          [HAL] サーボモータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      サーボモータのパルス周期は 15 ～ 20ms にする必要があるらしい。
 *                  15ms -> 66.6666Hz
 *                  20ms -> 50Hz
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.08.11
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <wiringPi.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_SV_OUT    (18)


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

    pinMode( MOTOR_SV_OUT, PWM_OUTPUT );
    pwmSetMode( PWM_MODE_MS );
    pwmSetClock( 192 );
    pwmSetRange( 100 );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     サーボモータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorSV_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize GPIO port. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     サーボモータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorSV_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータを回す。
 * @attention サーボモータのパルス周期は 15 ～ 20ms にする必要があるらしい。
 *            15ms -> 66.6666Hz
 *            20ms -> 50Hz
 * @note      PWM のカウントアップ・カウンタは 5kHz (= 0.2ms ) の速さでカウントアップ
 *            PWM 周波数は 50Hz
 *            100 カウントアップで 1 周期 (= 50Hz )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorSV_SetPwmDuty(
    EHalMotorState_t    status, ///< [in] モータの状態
    int                 rate    ///< [in] デューティ比 : 0% ～ 100% まで
){
    unsigned int        clock = 0;  // PWM のカウンタのカウントアップ周期を設定するために使用するパラメータ
    unsigned int        range = 0;  // PWM の周期を設定するために使用するパラメータ
    unsigned int        value = 0;

    DBG_PRINT_TRACE( "rate    = %d%% \n\r", rate );

    // PWM のカウンタのカウントアップ周期
    // 19.2MHz / clock(=3840) = 5kHz でカウントアップ
    clock = 3840;    // 2 - 4095 まで

    // PWM 周期
    // 5kHz / range(=100) = 50Hz
    range = 100;    // 1 - 4096 まで

    // デューティ比 = value / range
    value = rate / 8 + 2;

    pwmSetClock( clock );
    pwmSetRange( range );

    if( status == EN_MOTOR_CCW ||
        status == EN_MOTOR_CW   )
    {
        pwmWrite( MOTOR_SV_OUT, value );
    } else
    {
        pwmWrite( MOTOR_SV_OUT, 0 );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

