/**************************************************************************//*!
 *  @file           opt_cmd_sa_acc.c
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
static void       GetData( EHalSensorAcc_t which );
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
    AppIfPc_Printf( " Main option)                                            \n\r" );
    AppIfPc_Printf( "     -a, --sa_acc : get the value of a sensor(A/D), Acc. \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf( " Sub option)                                             \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.                 \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format.     \n\r" );
    AppIfPc_Printf( "     -m, --menu : menu mode.                             \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf( "     -x, --x    : get the value of x-axis.               \n\r" );
    AppIfPc_Printf( "     -y, --y    : get the value of y-axis.               \n\r" );
    AppIfPc_Printf( "     -z, --z    : get the value of z-axis.               \n\r" );
    AppIfPc_Printf( "                                                         \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                  \n\r" );
    AppIfPc_Printf( "     -a        -h     \n\r" );
    AppIfPc_Printf( "     --sa_acc  --help \n\r" );
    AppIfPc_Printf( "     -a        -j     \n\r" );
    AppIfPc_Printf( "     --sa_acc  --json \n\r" );
    AppIfPc_Printf( "     -a        -x     \n\r" );
    AppIfPc_Printf( "     --sa_acc  --x    \n\r" );
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
    EHalSensorAcc_t     which   ///< [in] 対象のセンサ
){
    SHalSensor_t*       data;

    DBG_PRINT_TRACE( "GetData() \n\r" );

    // センサデータを取得
    data = HalSensorAcc_Get( which );

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
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    DBG_PRINT_TRACE( "GetJson() \n\r" );

    // センサデータを取得
    dataX = HalSensorAcc_Get( EN_SEN_ACC_X );
    dataY = HalSensorAcc_Get( EN_SEN_ACC_Y );
    dataZ = HalSensorAcc_Get( EN_SEN_ACC_Z );

    // LCD 表示
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "x%03X  y%03X  z%03X", (int)dataX->cur, (int)dataY->cur, (int)dataZ->cur );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%03d%%  ", dataX->cur_rate );
    AppIfLcd_Printf( "%03d%%  ", dataY->cur_rate );
    AppIfLcd_Printf( "%03d%%",  dataZ->cur_rate );

    // PC ターミナル表示
    AppIfPc_Printf( "{\"sensor\": \"sa_acc\", \"value\": {\"x\": %d, \"y\": %d, \"z\": %d}} \r",
                    (int)dataX->cur,
                    (int)dataY->cur,
                    (int)dataZ->cur );
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_SaAccMenu(
    void
){
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    DBG_PRINT_TRACE( "OptCmd_SaAccMenu() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfPc_Printf( "range : -2g -> +2g.         \n\r" );

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // センサデータを取得
        dataX = HalSensorAcc_Get( EN_SEN_ACC_X );
        dataY = HalSensorAcc_Get( EN_SEN_ACC_Y );
        dataZ = HalSensorAcc_Get( EN_SEN_ACC_Z );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%03d%%  ", dataX->cur_rate );
        AppIfLcd_Printf( "%03d%%  ", dataY->cur_rate );
        AppIfLcd_Printf( "%03d%%",  dataZ->cur_rate );

        // PC ターミナル表示
        AppIfPc_Printf( "X: { A/D: 0x%04X(digit), rate: %03d(%%) }, Y: { A/D: 0x%04X(digit), rate: %03d(%%) }, Z: { A/D: 0x%04X(digit), rate: %03d(%%) } \r",
                        (int)dataX->cur, dataX->cur_rate,
                        (int)dataY->cur, dataY->cur_rate,
                        (int)dataZ->cur, dataZ->cur_rate );
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
OptCmd_SaAcc(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmxyz";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "menu", no_argument, NULL,  'm' },
        { "x",    no_argument, NULL,  'x' },
        { "y",    no_argument, NULL,  'y' },
        { "z",    no_argument, NULL,  'z' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_SaAcc() \n\r" );
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
        case '?': DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt ); break;
        case 'h': Help(); break;
        case 'j': GetJson(); break;
        case 'm': OptCmd_SaAccMenu(); break;
        case 'x': GetData( EN_SEN_ACC_X ); break;
        case 'y': GetData( EN_SEN_ACC_Y ); break;
        case 'z': GetData( EN_SEN_ACC_Z ); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

