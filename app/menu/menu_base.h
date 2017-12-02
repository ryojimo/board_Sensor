/**************************************************************************//*!
 *  @file           menu_base.h
 *  @brief          [APP] menu/ フォルダ内向け公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常関数 : 特にルールなし
 *                      cmd 関数 : MenuCmd_[コマンド名]()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.07.04
 *************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_MENU_BASE_H_
#define _APP_MENU_BASE_H_


//********************************************************
/* include                                               */
//********************************************************
#include "../../hal/hal.h"

#include "menu.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#ifndef OK
  #define OK      (1)
#endif

#ifndef ERR
  #define ERR     (-1)
#endif

#ifndef EOF
  #define EOF     (-1)
#endif

#ifndef NULL
  #define NULL    (0)
#endif

#define MAX_CMD_NUM     (16)     ///< @def : コマンドの最大数

#define SYS_MAX_CMD_NAME_LEN            (16)            ///< @def : コマンド名、オプション名の最大文字列数
#define SYS_MAX_CMD_OPT_NUM             (5)             ///< @def : コマンド名 + オプションの個数の最大値
#define SYS_MAX_CMD_BUFF                ( SYS_MAX_CMD_NAME_LEN * SYS_MAX_CMD_OPT_NUM )


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
typedef struct tagSAppMenuCmd{
    char            *name;              ///< @var : コマンド名
    EAppMenuMsg_t   (*func)( void );    ///< @var : コマンドルーチン
} SAppMenuCmd_t;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
void            MenuCmd_Init( void );

EAppMenuMsg_t   ExecuteCmd( unsigned char* name, const SAppMenuCmd_t*  table );
EAppMenuMsg_t   ParseCmd( unsigned char*  str );
EAppMenuMsg_t   SelectCmd( void );

EAppMenuMsg_t   MenuCmd_Help( void );           // コマンド一覧を表示
EAppMenuMsg_t   MenuCmd_I2cLcd( void );         // LCD を表示する
EAppMenuMsg_t   MenuCmd_Led( void );            // LED を点灯する
EAppMenuMsg_t   MenuCmd_MotorSV( void );        // SERVO MOTOR を回す
EAppMenuMsg_t   MenuCmd_PushSw( void );         // PUSH SWITCH の押された回数を表示
EAppMenuMsg_t   MenuCmd_Relay( void );          // リレーを ON / OFF する
EAppMenuMsg_t   MenuCmd_SA_Acc( void );         // SENSOR (ADC) 加速度センサ
EAppMenuMsg_t   MenuCmd_SA_Gyro( void );        // SENSOR (ADC) ジャイロセンサ
EAppMenuMsg_t   MenuCmd_SA_Pm( void );          // SENSOR (ADC) ポテンショメータ
EAppMenuMsg_t   MenuCmd_SI_Bme280( void );      // SENSOR (I2C) BME280   ( 気圧・湿度・温度 )
EAppMenuMsg_t   MenuCmd_SI_Gp2y0e03( void );    // SENSOR (I2C) GP2Y0E03 ( 距離 )
EAppMenuMsg_t   MenuCmd_SI_Lps25h( void );      // SENSOR (I2C) LPS25H   ( 気圧・温度 )
EAppMenuMsg_t   MenuCmd_SI_Tsl2561( void );     // SENSOR (I2C) TSL2561  ( 照度 )
EAppMenuMsg_t   MenuCmd_Time( void );           // 日時情報を表示する


#endif  /* _APP_MENU_BASE_H_ */

