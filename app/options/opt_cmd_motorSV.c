/**************************************************************************//*!
 *  @file           opt_cmd_motorSV.c
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
    AppIfPc_Printf( " Main option)                                \n\r" );
    AppIfPc_Printf( "     -o, --motorsv : control the SAVO motor. \n\r" );
    AppIfPc_Printf( "                                             \n\r" );
    AppIfPc_Printf( " Sub option)                                                   \n\r" );
    AppIfPc_Printf( "     -h,            --help            : display the help menu. \n\r" );
    AppIfPc_Printf( "     -m,            --menu            : menu mode.             \n\r" );
    AppIfPc_Printf( "     -r int-number, --rate=int-number : set the duty-rate.     \n\r" );
    AppIfPc_Printf( "                                                               \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                      \n\r" );
    AppIfPc_Printf( "     -o         -h        \n\r" );
    AppIfPc_Printf( "     --motorsv  --help    \n\r" );
    AppIfPc_Printf( "     -o         -m        \n\r" );
    AppIfPc_Printf( "     --motorsv  --menu    \n\r" );
    AppIfPc_Printf( "     -o         -r  5     \n\r" );
    AppIfPc_Printf( "     --motorsv  --rate=5  \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータをまわす rate 値を取得する
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
    double  value = 0;

    DBG_PRINT_TRACE( "GetRate() \n\r" );
    DBG_PRINT_TRACE( "str = %s \n\r", str );
    value = strtod( (const char*)str, &endptr );
    DBG_PRINT_TRACE( "value = %2.4f \n\r", value );
    return value;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention Volume を使ってサーボモータをまわす。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_MotorSvMenu(
    void
){
    SHalSensor_t*   data;       ///< ボリュームのデータ構造体
    double          value;      ///< サーボモータを回す値

    DBG_PRINT_TRACE( "OptCmd_MotorSvMenu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.   \n\r" );
    AppIfPc_Printf( "motor-rotate changes by volume. \n\r" );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        data = HalSensorPm_Get();

        // サーボモータを回す値を計算する
        value = 10 + (( 52 - 10 ) * data->cur_rate) / 100;
//        value = data->cur_rate;

        // モータ制御
        HalMotorSV_SetPwmDuty( EN_MOTOR_CW, (int)value );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02.4f(%%)  ", value );

        // PC ターミナル表示
        AppIfPc_Printf( "motor SV: { rate: %2.4f(%%) } \r", value );
    }

    AppIfPc_Printf( "\n\r" );

    // モータを停止
    HalMotorSV_SetPwmDuty( EN_MOTOR_STOP, 4 ); // 4% 設定 ( 無視されるがとりあえずセット )
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
OptCmd_MotorSV(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hmr:";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "menu",   no_argument,       NULL,  'm' },
        { "rate",   required_argument, NULL,  'r' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    double            rate = 0;
    int               endFlag = 0;
    EHalMotorState_t  status = EN_MOTOR_STANDBY;

    DBG_PRINT_TRACE( "OptCmd_MotorSV() \n\r" );

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
        case 'm': endFlag = 1; OptCmd_MotorSvMenu(); break;
        case 'r':              rate = GetRate( optarg ); status = EN_MOTOR_CW; break;
        default: break;
        }
    }

    if( endFlag == 0 )
    {
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02.4f(%%)  ", rate );
        HalMotorSV_SetPwmDuty( status, (int)rate );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

