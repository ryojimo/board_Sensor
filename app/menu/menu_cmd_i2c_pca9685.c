/**************************************************************************//*!
 *  @file           menu_cmd_i2c_pca9685.c
 *  @brief          [APP] メニュー・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2019.01.17
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <stdlib.h>

#include "../../hal/hal.h"

#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

#include "menu_base.h"
#include "menu_input.h"

#define DBG_PRINT
#define MY_NAME "APP"
#include "../log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define OPTION_NUM  (3)


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static EAppMenuMsg_t    PrintFormat( void );
static EAppMenuMsg_t    Stop( void );
static EAppMenuMsg_t    Cw( void );
static EAppMenuMsg_t    Ccw( void );
static EAppMenuMsg_t    Volume( void );


//********************************************************
/* Global Parameter  ( Local Scope )                    */
//********************************************************
/* オプションテーブル */
static const SAppMenuCmd_t g_optTable[] =
{
    { ""         , PrintFormat },
    { "h"        , PrintFormat },
    { "help"     , PrintFormat },
    { "s"        , Stop },
    { "stop"     , Stop },
    { "cw"       , Cw },
    { "ccw"      , Ccw },
    { "vol"      , Volume },
    { "END    "  , NULL }
};


/**************************************************************************//*!
 * @brief     コマンドの書式を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
PrintFormat(
    void
){
    AppIfPc_Printf( "    i2cpca9685 [OPTION01] [OPTION02]              \n\r" );
    AppIfPc_Printf( "        OPTION01 : s     : stop to turn.                   \n\r" );
    AppIfPc_Printf( "                 : stop  : stop to turn.                   \n\r" );
    AppIfPc_Printf( "                 : cw    : turn cw.                        \n\r" );
    AppIfPc_Printf( "                 : ccw   : turn ccw.                       \n\r" );
    AppIfPc_Printf( "                 : vol   : change speed by volume control. \n\r" );
    AppIfPc_Printf( "        OPTION02 : <ch>  : target channel.                 \n\r" );
    AppIfPc_Printf( "        OPTION03 : <rate>: duty rate ( 3.0% ~ 12.0% ).     \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    Ex.)                                       \n\r" );
    AppIfPc_Printf( "        >i2cpca9685 s   0  5                   \n\r" );
    AppIfPc_Printf( "        >i2cpca9685 vol 0 10                   \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     サーボモータを停止する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Stop(
    void
){
    int             ch;
    double          rate;
    char*           endptr;

    DBG_PRINT_TRACE( "\n\r" );

    ch   = atoi( (const char*)&g_menuCmd[2][0] );
    rate = strtod( (const char*)&g_menuCmd[3][0], &endptr );

    // MOTOR を停止
    HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_STOP, rate );
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     サーボモータを CW 方向に回転する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Cw(
    void
){
    int             ch;
    double          rate;
    char*           endptr;

    DBG_PRINT_TRACE( "\n\r" );

    ch   = atoi( (const char*)&g_menuCmd[2][0] );
    rate = strtod( (const char*)&g_menuCmd[3][0], &endptr );

    HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_CW, rate );
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     サーボモータを CCW 方向に回転する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Ccw(
    void
){
    int             ch;
    double          rate;
    char*           endptr;

    DBG_PRINT_TRACE( "\n\r" );

    ch   = atoi( (const char*)&g_menuCmd[2][0] );
    rate = strtod( (const char*)&g_menuCmd[3][0], &endptr );

    HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_CCW, rate );
    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     サーボモータをまわす。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Volume(
    void
){
    int             ch;

    SHalSensor_t*   data;       ///< ボリュームのデータ構造体

    DBG_PRINT_TRACE( "\n\r" );

    ch   = atoi( (const char*)&g_menuCmd[2][0] );


    AppIfPc_Printf( "if you push any keys, break.   \n\r" );
    AppIfPc_Printf( "motor speed changes by volume. \n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 4, 0 );
    AppIfLcd_Puts( "motorSV" );

//    HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_CCW, 4 );  // 4% 設定 ( 無視されるがとりあえずセット )

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        // センサデータを取得
        data = HalSensorPm_Get();

        // モータ制御
        HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_CCW, data->cur_rate );

        // PC ターミナル表示
        AppIfPc_Printf( "motor SV : rate = %3d \r", data->cur_rate );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d", data->cur_rate );
    }

    AppIfPc_Printf( "\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    // モータを停止
    HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_STOP, 4 ); // 4% 設定 ( 無視されるがとりあえずセット )

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     サーボモータをまわす。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション01
 *              g_menuCmd[2][] : オプション02
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_I2cPca9685(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    ExecuteCmd( &g_menuCmd[1][0], g_optTable );

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

