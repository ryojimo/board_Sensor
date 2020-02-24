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
void  OptCmd_Help( void );                           // オプション一覧を表示する
void  OptCmd_Version( void );                        // Version を表示する
void  OptCmd_Menu( int argc, char *argv[] );         // Menu モードへ移動する

void  OptCmd_I2cLcd( int argc, char *argv[] );       // LCD を実行する
void  OptCmd_I2cPca9685( int argc, char *argv[] );   // PCA9685 を制御する
void  OptCmd_Led( int argc, char *argv[] );          // LED を点灯する
void  OptCmd_MotorSV( int argc, char *argv[] );      // サーボモータを制御する

void  OptCmd_PushSwitch( int argc, char *argv[] );   // Push Switch の値を取得する
void  OptCmd_PushSwitchLoop( void );

void  OptCmd_Relay( int argc, char *argv[] );        // リレーを ON / OFF する

void  OptCmd_Sensors( void );                        // すべてのセンサの値を取得する

void  OptCmd_SaAcc( int argc, char *argv[] );       // SENSOR (ADC) 加速度センサ
void  OptCmd_SaAccMenu( void );

void  OptCmd_SaGyro( int argc, char *argv[] );      // SENSOR (ADC) ジャイロセンサ
void  OptCmd_SaGyroMenu( void );

void  OptCmd_SaPm( int argc, char *argv[] );        // SENSOR (ADC) ポテンショメータ
void  OptCmd_SaPmMenu( void );

void  OptCmd_SiBme280( int argc, char *argv[] );    // SENSOR (I2C) BME280   ( 気圧・湿度・温度 )
void  OptCmd_SiBme280Menu( void );

void  OptCmd_SiGp2y0e03( int argc, char *argv[] );  // SENSOR (I2C) GP2Y0E03 ( 距離 )
void  OptCmd_SiGp2y0e03Menu( void );

void  OptCmd_SiLps25h( int argc, char *argv[] );    // SENSOR (I2C) LPS25H   ( 気圧・温度 )
void  OptCmd_SiLps25hMenu( void );

void  OptCmd_SiTsl2561( int argc, char *argv[] );   // SENSOR (I2C) TSL2561  ( 照度 )
void  OptCmd_SiTsl2561Menu( void );

void  OptCmd_Time( int argc, char *argv[] );         // 日時情報を表示する
void  OptCmd_TimeMenu( void );


#endif  /* _APP_OPTIONS_H_ */

