/**************************************************************************//*!
 *  @file           opt_sa_acc.c
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
static void Help( void );
static EHalBool_t IsEnterSw( void );
static void       Loop( void );


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
    AppIfPc_Printf( "     -h, --help : display the help menu.                         \n\r" );
    AppIfPc_Printf( "     -x, --x    : get the value of x-axis.                       \n\r" );
    AppIfPc_Printf( "     -y, --y    : get the value of y-axis.                       \n\r" );
    AppIfPc_Printf( "     -z, --z    : get the value of z-axis.                       \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format.             \n\r" );
    AppIfPc_Printf( "     -l, --loop : get the all values until the PushSW is pushed. \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                  \n\r" );
    AppIfPc_Printf( "     -a        -x     \n\r" );
    AppIfPc_Printf( "     --sa_acc  --x    \n\r" );
    AppIfPc_Printf( "     -a        -h     \n\r" );
    AppIfPc_Printf( "     --sa_acc  --help \n\r" );
    AppIfPc_Printf("\x1b[39m");
    AppIfPc_Printf( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     Enter SW が押されたか？どうかを返す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static EHalBool_t
IsEnterSw(
    void
){
    return HalPushSw_Get( EN_PUSH_SW_2 );
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Loop(
    void
){
    SHalSensor_t*   x;  ///< センサデータの構造体 : 加速度センサ X 方向
    SHalSensor_t*   y;  ///< センサデータの構造体 : 加速度センサ Y 方向
    SHalSensor_t*   z;  ///< センサデータの構造体 : 加速度センサ Z 方向

    DBG_PRINT_TRACE( "Loop() \n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfPc_Printf( "range : -2g -> +2g.         \n\r" );

    AppIfLcd_CursorSet( 0, 1 );

    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        // センサデータを取得
        x = HalSensorAcc_Get( EN_SEN_ACC_X );
        y = HalSensorAcc_Get( EN_SEN_ACC_Y );
        z = HalSensorAcc_Get( EN_SEN_ACC_Z );

        // PC ターミナル表示
        AppIfPc_Printf( "acc (x, y, z)=(0x%04X, 0x%04X, 0x%04X)=(%04d, %04d, %04d)=(%3d%%, %3d%%, %3d%%) \r",
                        (int)x->cur, (int)y->cur, (int)z->cur,
                        (int)x->cur, (int)y->cur, (int)z->cur,
                        x->cur_rate, y->cur_rate, z->cur_rate
                      );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "x%04X y%04X z%04X", (int)x->cur, (int)y->cur, (int)z->cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3d%%  ", x->cur_rate );
        AppIfLcd_Printf( "%3d%%  ", y->cur_rate );
        AppIfLcd_Printf( "%3d%%",   z->cur_rate );
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
Opt_Sa_Acc(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjlxyz";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "loop", no_argument, NULL,  'l' },
        { "x",    no_argument, NULL,  'x' },
        { "y",    no_argument, NULL,  'y' },
        { "z",    no_argument, NULL,  'z' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "Opt_Sa_Acc() \n\r" );
    SHalSensor_t*   dataX;
    SHalSensor_t*   dataY;
    SHalSensor_t*   dataZ;

    dataX = HalSensorAcc_Get( EN_SEN_ACC_X );
    dataY = HalSensorAcc_Get( EN_SEN_ACC_Y );
    dataZ = HalSensorAcc_Get( EN_SEN_ACC_Z );

    AppIfLcd_CursorSet( 0, 1 );

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_ERROR( "invalid option. : \"%c\" \n\r", optopt );
            Help();
            goto err;
            break;
        } else if( opt == 'h' )
        {
            Help();
            goto err;
            break;
        } else if( opt == 'j' )
        {
            AppIfLcd_Printf( "%04X, %04X, %04X", (int)dataX->cur, (int)dataY->cur, (int)dataZ->cur );

            AppIfPc_Printf( "{\"sensor\": \"sa_acc\", \"value\": {\"x\": %d, \"y\": %d, \"z\": %d}}",
                            (int)dataX->cur,
                            (int)dataY->cur,
                            (int)dataZ->cur );
            AppIfPc_Printf( "\n\r" );
            break;
        } else if( opt == 'l' )
        {
            Loop();
            break;
        }

        switch( opt )
        {
        case 'x': AppIfLcd_Printf( "0x%04X", (int)dataX->cur ); AppIfPc_Printf( "%d \n\r", (int)dataX->cur ); break;
        case 'y': AppIfLcd_Printf( "0x%04X", (int)dataY->cur ); AppIfPc_Printf( "%d \n\r", (int)dataY->cur ); break;
        case 'z': AppIfLcd_Printf( "0x%04X", (int)dataZ->cur ); AppIfPc_Printf( "%d \n\r", (int)dataZ->cur ); break;
        default: break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

