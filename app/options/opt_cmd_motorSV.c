/**************************************************************************//*!
 *  @file           opt_motorSV.c
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
static void       Run( double rate );
static void       RunVolume( void );
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
    AppIfPc_Printf( "     -r int-number, --rate=int-number : the duty-rate.         \n\r" );
    AppIfPc_Printf( "                                                               \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                      \n\r" );
    AppIfPc_Printf( "     -o         -r  5     \n\r" );
    AppIfPc_Printf( "     --motorsv  --rate=5  \n\r" );
    AppIfPc_Printf( "     -o         -v        \n\r" );
    AppIfPc_Printf( "     --motorsv  --volume  \n\r" );
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
    double    rate    ///< [in] デューティ比 : 0% ～ 100% まで
){
    DBG_PRINT_TRACE( "Run() \n\r" );

    // LCD に表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%02.4f(\%)  ", rate );

    HalMotorSV_SetPwmDuty( EN_MOTOR_CW, (int)rate );
    return;
}


/**************************************************************************//*!
 * @brief     Volume を使ってサーボモータをまわす。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
RunVolume(
    void
){
    SHalSensor_t*   data;       ///< ボリュームのデータ構造体
    double          value;      ///< サーボモータを回す値

    DBG_PRINT_TRACE( "RunVolume() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.   \n\r" );
    AppIfPc_Printf( "motor speed changes by volume. \n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        data = HalSensorPm_Get();

        // サーボモータを回す値を計算する。
        value = 2.8 + (( 12.5 - 2.8 ) * data->cur_rate) / 100;

        // PC ターミナル表示
        AppIfPc_Printf( "motor SV : rate = %2.4f(\%) \r", value );

        // モータ制御
        Run( value );
    }

    AppIfPc_Printf( "\n\r" );

    // モータを停止
    HalMotorSV_SetPwmDuty( EN_MOTOR_STOP, 4 ); // 4% 設定 ( 無視されるがとりあえずセット )
    return;
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
    DBG_PRINT_TRACE( "GetRate() \n\r" );
    char*   endptr;
    double  rate = 0;

    DBG_PRINT_TRACE( "str = %s \n\r", str );
    rate = strtod( (const char*)str, &endptr );
    DBG_PRINT_TRACE( "rate = %2.4f \n\r", rate );
    return rate;
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
    const char      optstring[] = "hr:v";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "help",   no_argument,       NULL,  'h' },
        { "rate",   required_argument, NULL,  'r' },
        { "volume", no_argument,       NULL,  'v' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    double          rate = 0;
    int             endFlag = 0;

    DBG_PRINT_TRACE( "OptCmd_MotorSV() \n\r" );
    AppIfLcd_CursorSet( 0, 1 );

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
        case 'v': endFlag = 1; RunVolume(); break;
        case 'r':              rate = GetRate( optarg ); break;
        default: break;
        }
    }

    if( endFlag == 0 )
    {
        Run( rate );
    }

    return;
}


#ifdef __cplusplus
    }
#endif

