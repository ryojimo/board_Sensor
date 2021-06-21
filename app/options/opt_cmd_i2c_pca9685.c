/**************************************************************************//*!
 *  @file           opt_cmd_i2c_pca9685.c
 *  @brief          [APP] オプション・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2020.01.19
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <getopt.h>
#include <stdlib.h>

#include "../../hal/hal.h"

#include "../if_button/if_button.h"
#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

//#define DBG_PRINT
#define MY_NAME "OPT"
#include "../log/log.h"


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// getopt() で使用
extern char *optarg;
extern int  optind, opterr, optopt;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void       Help( void );
static void       Run( int ch, EHalMotorState_t status, double rate );
static int        GetChannel( char* str );
static double     GetRate( char* str );


/**************************************************************************//*!
 * @brief     HELP を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Help(
    void
){
    DBG_PRINT_TRACE( "Help() \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( " Main option)                                                      \n\r" );
    AppIfPc_Printf( "     -e, --i2cpca9685 : control the (I2C) PCA9685.                 \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( " Sub option)                                                       \n\r" );
    AppIfPc_Printf( "     -h,              --help              : display the help menu. \n\r" );
    AppIfPc_Printf( "     -m,              --menu              : menu mode.             \n\r" );
    AppIfPc_Printf( "     -c number,       --ch=number         : the target channnel.   \n\r" );
    AppIfPc_Printf( "     -r float-number, --rate=float-number : the duty-rate.         \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                                                               \n\r" );
    AppIfPc_Printf( "     -e            -h                                              \n\r" );
    AppIfPc_Printf( "     --i2cpca9685  --help                                          \n\r" );
    AppIfPc_Printf( "     -e            -m                                              \n\r" );
    AppIfPc_Printf( "     --i2cpca9685  --menu                                          \n\r" );
    AppIfPc_Printf( "     -e            -c   <number>  -r     <float-number>            \n\r" );
    AppIfPc_Printf( "     --i2cpca9685  --ch=<number>  --rate=<float-number>            \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータをまわす。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Run(
    int                 ch,     ///< [in] 対象の channel
    EHalMotorState_t    status, ///< [in] モータの状態
    double              rate    ///< [in] デューティ比 : 0% ～ 100% まで
){
    DBG_PRINT_TRACE( "Run() \n\r" );
    DBG_PRINT_TRACE( "(ch, status, rate) = (%02d, %02d, %2.4f) \n\r", ch, status, rate );

    if( 0 <= ch && ch <= 15 )
    {
        // LCD に表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02.4f(\%)  ", rate );

        HalI2cPca9685_SetPwmDuty( ch, status, rate );
    } else
    {
        DBG_PRINT_ERROR( "invalid ch number error. Please input between 0 and 15. : %d \n\r", ch );
    }

    return;
}


/**************************************************************************//*!
 * @brief     Channel 番号を取得する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static int
GetChannel(
    char*   str     ///< [in] 文字列
){
    int     ch;

    DBG_PRINT_TRACE( "GetChannel() \n\r" );
    DBG_PRINT_TRACE( "str = %s \n\r", str );
    ch   = strtol( (const char*)str, NULL, 10 );
    DBG_PRINT_TRACE( "ch = %d \n\r", ch );
    return ch;
}


/**************************************************************************//*!
 * @brief     rate を取得する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static double
GetRate(
    char*   str     ///< [in] 文字列
){
    char*   endptr;
    double  rate = 0;

    DBG_PRINT_TRACE( "GetRate() \n\r" );
    DBG_PRINT_TRACE( "str = %s \n\r", str );
    rate = strtod( (const char*)str, &endptr );
    DBG_PRINT_TRACE( "rate = %2.4f \n\r", rate );
    return rate;
}


/**************************************************************************//*!
 * @brief     Volume を使ってサーボモータをまわす。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_I2cPca9685Menu(
    void
){
    SHalSensor_t*   data;   ///< ボリュームのデータ構造体
    double          rate;   ///< サーボモータを回す値

    DBG_PRINT_TRACE( "OptCmd_I2cPca9685Menu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.   \n\r" );
    AppIfPc_Printf( "motor speed changes by volume. \n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        data = HalSensorPm_Get();

        // サーボモータを回す値を計算する。
        rate = 2.8 + (( 12.5 - 2.8 ) * data->cur_rate) / 100;

        // PC ターミナル表示
        AppIfPc_Printf( "motor SV : rate = %2.4f(\%) \r", rate );

        // モータ制御
        Run(  0, EN_MOTOR_CW, rate );
        Run(  1, EN_MOTOR_CW, rate );
        Run(  2, EN_MOTOR_CW, rate );
        Run(  3, EN_MOTOR_CW, rate );
        Run(  4, EN_MOTOR_CW, rate );
        Run(  5, EN_MOTOR_CW, rate );
        Run(  6, EN_MOTOR_CW, rate );
        Run(  7, EN_MOTOR_CW, rate );
        Run(  8, EN_MOTOR_CW, rate );
        Run(  9, EN_MOTOR_CW, rate );
        Run( 10, EN_MOTOR_CW, rate );
        Run( 11, EN_MOTOR_CW, rate );
        Run( 12, EN_MOTOR_CW, rate );
        Run( 13, EN_MOTOR_CW, rate );
        Run( 14, EN_MOTOR_CW, rate );
        Run( 15, EN_MOTOR_CW, rate );
    }

    AppIfPc_Printf( "\n\r" );

    // モータを停止
    // 4% 設定 ( 無視されるがとりあえずセット )
    HalI2cPca9685_SetPwmDuty(  0, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  1, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  2, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  3, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  4, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  5, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  6, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  7, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  8, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty(  9, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 10, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 11, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 12, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 13, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 14, EN_MOTOR_STOP, 4 );
    HalI2cPca9685_SetPwmDuty( 15, EN_MOTOR_STOP, 4 );
    return;
}


/**************************************************************************//*!
 * @brief     実行する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_I2cPca9685(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hmc:r:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "menu",   no_argument,       NULL,  'm' },
        { "ch",     required_argument, NULL,  'c' },
        { "rate",   required_argument, NULL,  'r' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    int             ch = 0;
    double          rate = 0;
    int             endFlag = 0;

    DBG_PRINT_TRACE( "OptCmd_I2cPca9685() \n\r" );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        // -1 : 処理するオプションが無くなった場合
        // '?': optstring で指定していない引数が見つかった場合
        if( opt == -1 )
        {
            break;
        }

        switch( opt )
        {
        case '?': endFlag = 1; DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': endFlag = 1; Help(); break;
        case 'm': endFlag = 1; OptCmd_I2cPca9685Menu(); break;
        case 'c':              ch   = GetChannel( optarg ); break;
        case 'r':              rate = GetRate( optarg ); break;
        default: break;
        }
    }

    if( endFlag == 0 )
    {
        Run( ch, EN_MOTOR_CW, rate );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

