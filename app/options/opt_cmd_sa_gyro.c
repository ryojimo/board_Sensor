/**************************************************************************//*!
 *  @file           opt_cmd_sa_gyro.c
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
static void       GetData( EHalSensorGyro_t which );
static void       GetJson( void );


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
    AppIfPc_Printf( " Main option)                                              \n\r" );
    AppIfPc_Printf( "     -g, --sa_gyro : get the value of a sensor(A/D), Gyro. \n\r" );
    AppIfPc_Printf( "                                                           \n\r" );
    AppIfPc_Printf( " Sub option)                                               \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.                   \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format.       \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                               \n\r" );
    AppIfPc_Printf( "                                                           \n\r" );
    AppIfPc_Printf( "     -x, --g1   : get the value of g1-axis.                \n\r" );
    AppIfPc_Printf( "     -y. --g2   : get the value of g2-axis.                \n\r" );
    AppIfPc_Printf( "                                                           \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                   \n\r" );
    AppIfPc_Printf( "     -g         -h     \n\r" );
    AppIfPc_Printf( "     --sa_gyro  --help \n\r" );
    AppIfPc_Printf( "     -g         -j     \n\r" );
    AppIfPc_Printf( "     --sa_gyro  --json \n\r" );
    AppIfPc_Printf( "     -g         -x     \n\r" );
    AppIfPc_Printf( "     --sa_gyro  --g1   \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     データを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
GetData(
    EHalSensorGyro_t    which   ///< [in] 対象のセンサ
){
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "GetData() \n\r" );

    // センサデータを取得
    data = HalSensorGyro_Get( which );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "0x%03X %04d %03d%%", (int)data->cur, (int)data->cur, (int)data->cur_rate );

    // PC ターミナル表示
    AppIfPc_Printf( "%d \n\r", (int)data->cur );
    return;
}


/**************************************************************************//*!
 * @brief     JSON 形式でデータを表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
GetJson(
    void
){
    SHalSensor_t*   dataG1;
    SHalSensor_t*   dataG2;

    DBG_PRINT_TRACE( "GetJson() \n\r" );

    // センサデータを取得
    dataG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
    dataG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "g1:%04d g2:%04d", (int)dataG1->cur, (int)dataG2->cur );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%03d(%%) %03d(%%)", dataG1->cur_rate, dataG2->cur_rate );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"sa_gyro\", \"value\": {\"g1\": %d, \"g2\": %d}} \r",
                    (int)dataG1->cur,
                    (int)dataG2->cur );
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
void
OptCmd_SaGyroMenu(
    void
){
    SHalSensor_t*   dataG1;
    SHalSensor_t*   dataG2;

    DBG_PRINT_TRACE( "OptCmd_SaGyroMenu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        dataG1 = HalSensorGyro_Get( EN_SEN_GYRO_G1 );
        dataG2 = HalSensorGyro_Get( EN_SEN_GYRO_G2 );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%03d(%%) %03d(%%)", dataG1->cur_rate, dataG2->cur_rate );

        // PC ターミナル表示
        AppIfPc_Printf( "G1: { A/D: 0x%04X(digit), rate: %03d(%%) }, G2: { A/D: 0x%04X(digit), rate: %03d(%%) } \r",
                        (int)dataG1->cur, dataG1->cur_rate,
                        (int)dataG2->cur, dataG2->cur_rate );
    }

    AppIfPc_Printf( "\n\r" );
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
OptCmd_SaGyro(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmxy";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "menu", no_argument, NULL,  'm' },
        { "g1",   no_argument, NULL,  'x' },
        { "g2",   no_argument, NULL,  'y' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SaGyro() \n\r" );

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
        case '?': DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': Help(); break;
        case 'j': GetJson(); break;
        case 'm': OptCmd_SaGyroMenu(); break;
        case 'x': GetData( EN_SEN_GYRO_G1 ); break;
        case 'y': GetData( EN_SEN_GYRO_G2 ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

