/**************************************************************************//*!
 *  @file           hal_cmn.h
 *  @brief          [HAL] 共通 API を宣言したヘッダファイル。
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
#ifndef _HAL_CMN_H_
#define _HAL_CMN_H_


//********************************************************
/* include                                               */
//********************************************************
#include <stdio.h>
#include <unistd.h>


//********************************************************
/*! @def                                                 */
//********************************************************
#define I2C_SLAVE_BMX055_MAG    (0x13)
#define I2C_SLAVE_PIC18F14K50   (0x17)
#define I2C_SLAVE_BMX055_ACC    (0x19)
#define I2C_SLAVE_TCS34725      (0x29)
#define I2C_SLAVE_TSL2561       (0x39)
#define I2C_SLAVE_LCD           (0x3C)
#define I2C_SLAVE_GP2Y0E03      (0x40)
#define I2C_SLAVE_HDC1000       (0x40)
#define I2C_SLAVE_PCA9685       (0x41)
#define I2C_SLAVE_ADS1015       (0x48)
#define I2C_SLAVE_ADT7410       (0x48)
#define I2C_SLAVE_LPS25H        (0x5C)
#define I2C_SLAVE_BMX055_GYRO   (0x69)
#define I2C_SLAVE_BME280        (0x76)

#define MCP3208_MAX_VALE        (0x0F60)
#define ADS1015_MAX_VALE        (0x0672)


//********************************************************
/*! @enum                                                */
//********************************************************
//*************************************
// 一般用途で使用する型
//*************************************
typedef enum tagEHalBool
{
    EN_FALSE = 0,           ///< @var : FALSE (= 偽 )
    EN_TRUE                 ///< @var : TRUE  (= 真 )
} EHalBool_t;


typedef enum tagEHalOutputLevel
{
    EN_LOW = 0,             ///< @var : LOW  (= 0  V out )
    EN_HIGH = 1             ///< @var : HIGH (= 3.3V out )
} EHalOputputLevel_t;


typedef enum tagEHalState
{
    EN_STANDBY = 0,         ///< @var : スタンバイ (= 初期値 )
    EN_READY,               ///< @var : レディ (準備完了)
    EN_RUNNING              ///< @var : ランニング
} EHalState_t;


//*************************************
// デバイスを区別するための型
//*************************************
// AD コンバータの Ch 区別に使用する型
typedef enum tagEHalCh
{
    EN_CH0 = 0,    ///< @var : Ch 0
    EN_CH1,        ///< @var : Ch 1
    EN_CH2,        ///< @var : Ch 2
    EN_CH3,        ///< @var : Ch 3
    EN_CH4,        ///< @var : Ch 4
    EN_CH5,        ///< @var : Ch 5
    EN_CH6,        ///< @var : Ch 6
    EN_CH7         ///< @var : Ch 7
} EHalCh_t;


//********************************************************
/*! @struct                                              */
//********************************************************
// センサ変数に使用する型
typedef struct tagSHalSensor
{
    double              cur;        ///< @var : 現在値
    double              ofs;        ///< @var : 初期化時に設定するセンサのオフセット値
    double              max;        ///< @var : 最大値
    double              min;        ///< @var : 最小値
    double              err;        ///< @var : cur - ofs
    int                 cur_rate;   ///< @var : 割合に換算した現在値 ( %     )
    unsigned int        cur_vol;    ///< @var : 電圧に換算した現在値 ( mV    )
} SHalSensor_t;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
void            HalCmn_UpdateSenData( SHalSensor_t* curData, double newData );
void            HalCmn_CopySenData( SHalSensor_t* curData, SHalSensor_t* newData );

EHalBool_t      HalCmnGpio_Init( void );
void            HalCmnGpio_Fini( void );

EHalBool_t      HalCmnI2c_Init( void );
void            HalCmnI2c_Fini( void );
EHalBool_t      HalCmnI2c_SetSlave( unsigned char address );
EHalBool_t      HalCmnI2c_Write( unsigned char* data, unsigned int size );
EHalBool_t      HalCmnI2c_Read( unsigned char* data, unsigned int size );

EHalBool_t      HalCmnSpi_Init( void );
void            HalCmnSpi_Fini( void );
EHalBool_t      HalCmnSpi_Send( unsigned char data );
EHalBool_t      HalCmnSpi_SendN( unsigned char* data, int );
EHalBool_t      HalCmnSpi_SendBuffer( unsigned char* data, int size );
EHalBool_t      HalCmnSpi_RecvN( unsigned char*  send, unsigned char*  recv, unsigned int size );

int             HalCmnI2cAds1015_Get( EHalCh_t which );
unsigned int    HalCmnSpiMcp3208_Get( EHalCh_t which );


#endif /* _HAL_CMN_H_ */

