/**************************************************************************//*!
 *  @file           options.h
 *  @brief          [APP] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2020.01.19
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_


//********************************************************
/* include                                               */
//********************************************************
#include "../../hal/hal.h"


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
/* 関数プロトタイプ宣言                                  */
//********************************************************
void  Opt_Help( void );                           // オプション一覧を表示する
void  Opt_Version( void );                        // Version を表示する
void  Opt_Menu( int argc, char *argv[] );         // Menu モードへ移動する

void  Opt_I2cLcd( int argc, char *argv[] );       // LCD を実行する
void  Opt_I2cPca9685( int argc, char *argv[] );   // PCA9685 を制御する
void  Opt_Led( int argc, char *argv[] );          // LED を点灯する
void  Opt_MotorSV( int argc, char *argv[] );      // サーボモータを制御する
void  Opt_PushSwitch( int argc, char *argv[] );   // Push Switch の値を取得する
void  Opt_Relay( int argc, char *argv[] );        // リレーを ON / OFF する

void  Opt_Sensors( void );                        // すべてのセンサの値を取得する
void  Opt_Sa_Acc( int argc, char *argv[] );       // SENSOR (ADC) 加速度センサ
void  Opt_Sa_Gyro( int argc, char *argv[] );      // SENSOR (ADC) ジャイロセンサ
void  Opt_Sa_Pm( int argc, char *argv[] );        // SENSOR (ADC) ポテンショメータ
void  Opt_Si_Bme280( int argc, char *argv[] );    // SENSOR (I2C) BME280   ( 気圧・湿度・温度 )
void  Opt_Si_Gp2y0e03( int argc, char *argv[] );  // SENSOR (I2C) GP2Y0E03 ( 距離 )
void  Opt_Si_Lps25h( int argc, char *argv[] );    // SENSOR (I2C) LPS25H   ( 気圧・温度 )
void  Opt_Si_Tsl2561( int argc, char *argv[] );   // SENSOR (I2C) TSL2561  ( 照度 )

void  Opt_Time( int argc, char *argv[] );         // 日時情報を表示する


#endif  /* _APP_OPTIONS_H_ */

