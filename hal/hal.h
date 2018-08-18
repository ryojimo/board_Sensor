/**************************************************************************//*!
 *  @file           hal.h
 *  @brief          [HAL] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常         : Hal[デバイス名]_処理名()
 *                      割込ハンドラ : Hal[デバイス名]_IH_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.05
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _HAL_H_
#define _HAL_H_


//********************************************************
/* include                                               */
//********************************************************
#include "hal_cmn.h"


//********************************************************
/*! @def                                                 */
//********************************************************
/* NULL */
#ifndef NULL
  #define NULL              (0)
#endif

/* OFF */
#ifndef OFF
  #define OFF               (0)
#endif

/* ON */
#ifndef ON
  #define ON                (1)
#endif

/* OK */
#ifndef OK
  #define OK                (1)
#endif

/* EOF */
#ifndef EOF
  #define EOF               (-1)
#endif


//********************************************************
/*! @enum                                                */
//********************************************************
//*************************************
// モード選択のための型
//*************************************
// LCD の動作モードに使用する型
typedef enum tagEHalLcdMode
{
    EN_LCD_CMD = 0,         ///< @var : コマンドモード設定の定義 ( RS = 0 )
    EN_LCD_DAT              ///< @var : データモード  設定の定義 ( RS = 1 )
} EHalLcdMode_t;


//*************************************
// ステータスの型
//*************************************
// モーターのステータスに使用する型
typedef enum tagEHalMotorState
{
    EN_MOTOR_STANDBY = 0,   ///< @var : スタンバイ (= 初期値 )
    EN_MOTOR_BRAKE,         ///< @var : ブレーキ中
    EN_MOTOR_CCW,           ///< @var : CCW 方向に回転中
    EN_MOTOR_CW,            ///< @var : CW  方向に回転中
    EN_MOTOR_STOP           ///< @var : 停止中
} EHalMotorState_t;


//*************************************
// PIC コマンドのための型
//*************************************
// I2C Slave デバイスへ送信する I2C コマンドの型
// I2C Slave デバイスのソースコードと合わせる必要あり
typedef enum tagEHalI2cCmd
{
    EN_I2C_CMD_NONE = 0,
    EN_I2C_CMD_LED,
    EN_I2C_CMD_MOTOR,
    EN_I2C_CMD_USB_KEYBOARD
} EHalI2cCmd_t;


//*************************************
// デバイスを区別するための型
//*************************************
// SENSOR (ADC) 加速度計センサの区別に使用する型
typedef enum tagEHalSensorAcc
{
    EN_SEN_ACC_X = 0,               ///< @var : X 軸方向センサ
    EN_SEN_ACC_Y,                   ///< @var : Y 軸方向センサ
    EN_SEN_ACC_Z,                   ///< @var : Z 軸方向センサ
} EHalSensorAcc_t;


// SENSOR (ADC) ジャイロセンサの区別に使用する型
typedef enum tagEHalSensorGyro
{
    EN_SEN_GYRO_G1 = 0,             ///< @var : G1 軸方向センサ
    EN_SEN_GYRO_G2,                 ///< @var : G2 軸方向センサ
} EHalSensorGyro_t;


// SENSOR (I2C) BME280 センサの区別に使用する型
typedef enum tagEHalSensorBME280
{
    EN_SEN_BME280_ATMOS = 0,        ///< @var : 気圧センサ
    EN_SEN_BME280_HUMI,             ///< @var : 湿度センサ
    EN_SEN_BME280_TEMP              ///< @var : 温度センサ
} EHalSensorBME280_t;


// SENSOR (I2C) LPS25H センサの区別に使用する型
typedef enum tagEHalSensorLPS25H
{
    EN_SEN_LPS25H_ATMOS = 0,        ///< @var : 気圧センサ
    EN_SEN_LPS25H_TEMP              ///< @var : 温度センサ
} EHalSensorLPS25H_t;


// SENSOR (I2C) TSL2561 センサの区別に使用する型
typedef enum tagEHalSensorTSL2561
{
    EN_SEN_TSL2561_BROADBAND = 0,   ///< @var : 照度センサ BROADBAND
    EN_SEN_TSL2561_IR,              ///< @var : 照度センサ IR
    EN_SEN_TSL2561_LUX              ///< @var : 照度センサ LUX
} EHalSensorTSL2561_t;


// プッシュ・スイッチの区別に使用する型
typedef enum tagEHalPushSw
{
    EN_PUSH_SW_0 = 0,               ///< @var : プッシュ・スイッチ 0
    EN_PUSH_SW_1,                   ///< @var : プッシュ・スイッチ 1
    EN_PUSH_SW_2                    ///< @var : プッシュ・スイッチ 2
} EHalPushSw_t;


//********************************************************
/*! @struct                                              */
//********************************************************
// 時間変数に使用する型
typedef struct tagSHalTime
{
    unsigned long       wait;   ///< @var : wait カウンタ  ( 単位:  usec ) :   1 min で自動 0 クリア

    unsigned int        usec;   ///< @var : 内部時計       ( 単位:  usec ) : 100 usec  ごとに加算
    unsigned int        msec;   ///< @var : 内部時計       ( 単位:  msec ) :   1 msec  ごとに加算
    unsigned char       sec;    ///< @var : 内部時計       ( 単位:   sec ) :   1 sec   ごとに加算
    unsigned char       min;    ///< @var : 内部時計       ( 単位:   min ) :   1 min   ごとに加算
    unsigned char       hour;   ///< @var : 内部時計       ( 単位:     h ) :   1 h     ごとに加算
    unsigned char       day;    ///< @var : 内部カレンダー ( 単位:   day ) :   1 day   ごとに加算
    unsigned short      month;  ///< @var : 内部カレンダー ( 単位: month ) :   1 month ごとに加算
    unsigned short      year;   ///< @var : 内部カレンダー ( 単位:  西暦 ) :   1 year  ごとに加算
} SHalTime_t;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// I2C 通信 Command API
EHalBool_t      HalI2cCmd_Init( void );
void            HalI2cCmd_Fini( void );
EHalBool_t      HalI2cCmd_Set( EHalI2cCmd_t cmd, unsigned char data );
EHalBool_t      HalI2cCmd_SetKeycode( unsigned char keycode );

// I2C LCD API
EHalBool_t      HalI2cLcd_Init( void );
void            HalI2cLcd_Fini( void );
EHalBool_t      HalI2cLcd_Write( EHalLcdMode_t rs, unsigned char code );

// LED API
EHalBool_t      HalLed_Init( void );
void            HalLed_Fini( void );
void            HalLed_Set( unsigned char value );

// サーボモータ API
EHalBool_t      HalMotorSV_Init( void );
void            HalMotorSV_Fini( void );
void            HalMotorSV_SetPwmDuty( EHalMotorState_t status, int rate );

// プッシュ・スイッチ API
EHalBool_t      HalPushSw_Init( void );
void            HalPushSw_Fini( void );
EHalBool_t      HalPushSw_Get( EHalPushSw_t );

// リレー API
EHalBool_t      HalRelay_Init( void );
void            HalRelay_Fini( void );
void            HalRelay_Set( EHalOputputLevel_t value );

// SENSOR (ADC) 加速度センサ API
EHalBool_t      HalSensorAcc_Init( void );
void            HalSensorAcc_Fini( void );
SHalSensor_t*   HalSensorAcc_Get( EHalSensorAcc_t which );

// SENSOR (ADC) ジャイロセンサ API
EHalBool_t      HalSensorGyro_Init( void );
void            HalSensorGyro_Fini( void );
SHalSensor_t*   HalSensorGyro_Get( EHalSensorGyro_t which );

// SENSOR (ADC) ポテンショメータ API
EHalBool_t      HalSensorPm_Init( void );
void            HalSensorPm_Fini( void );
SHalSensor_t*   HalSensorPm_Get( void );

// SENSOR (I2C) BME280 API
EHalBool_t      HalSensorBME280_Init( void );
void            HalSensorBME280_Fini( void );
SHalSensor_t*   HalSensorBME280_Get( EHalSensorBME280_t which );

// SENSOR (I2C) GP2Y0E03 API
EHalBool_t      HalSensorGP2Y0E03_Init( void );
void            HalSensorGP2Y0E03_Fini( void );
SHalSensor_t*   HalSensorGP2Y0E03_Get( void );

// SENSOR (I2C) LPS25H API
EHalBool_t      HalSensorLPS25H_Init( void );
void            HalSensorLPS25H_Fini( void );
SHalSensor_t*   HalSensorLPS25H_Get( EHalSensorLPS25H_t which );

// SENSOR (I2C) TSL2561 API
EHalBool_t      HalSensorTSL2561_Init( void );
void            HalSensorTSL2561_Fini( void );
SHalSensor_t*   HalSensorTSL2561_Get( EHalSensorTSL2561_t which );

// 時間 API
EHalBool_t      HalTime_Init( void );
SHalTime_t*     HalTime_GetLocaltime( void );
SHalTime_t*     HalTime_GetUTC( void );


#endif /* _HAL_H_ */

