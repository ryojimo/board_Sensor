/**************************************************************************//*!
 *  @file           opt_si_tsl2561.c
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
    AppIfPc_Printf( " Main option)                                                    \n\r" );
    AppIfPc_Printf( "     -z, --si_tsl2561 : get the value of a sensor(I2C), TSL2561. \n\r" );
    AppIfPc_Printf( "                                                                 \n\r" );
    AppIfPc_Printf( " Sub option)                                              \n\r" );
    AppIfPc_Printf( "     -h, --help      : display the help menu.             \n\r" );
    AppIfPc_Printf( "     -b, --broadband : get the value of ?.                \n\r" );
    AppIfPc_Printf( "     -i, --ir        : get the value of ?.                \n\r" );
    AppIfPc_Printf( "     -l, --lux       : get the value of LUX.              \n\r" );
    AppIfPc_Printf( "     -j, --json      : get the all values of json format. \n\r" );
    AppIfPc_Printf( "                                                          \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                       \n\r" );
    AppIfPc_Printf( "     -z            -l      \n\r" );
    AppIfPc_Printf( "     --si_tsl2561  --lux   \n\r" );
    AppIfPc_Printf( "     -z            -h      \n\r" );
    AppIfPc_Printf( "     --si_tsl2561  --help  \n\r" );
    AppIfPc_Printf("\x1b[39m");
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
Opt_Si_Tsl2561(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjbil";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,       has_arg,     *flag, val }, // 説明
        { "help",      no_argument, NULL,  'h' },
        { "json",      no_argument, NULL,  'j' },
        { "broadband", no_argument, NULL,  'b' },
        { "ir",        no_argument, NULL,  'i' },
        { "lux",       no_argument, NULL,  'l' },
        { 0,           0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "Opt_Si_Tsl2561() \n\r" );
    SHalSensor_t*       dataBB;
    SHalSensor_t*       dataIR;
    SHalSensor_t*       dataLUX;

    dataBB  = HalSensorTSL2561_Get( EN_SEN_TSL2561_BROADBAND );
    dataIR  = HalSensorTSL2561_Get( EN_SEN_TSL2561_IR );
    dataLUX = HalSensorTSL2561_Get( EN_SEN_TSL2561_LUX );

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
            AppIfLcd_Printf( "%5.2f, %5.2f, %5.2f", dataBB->cur, dataIR->cur, dataLUX->cur );

            AppIfPc_Printf( "{\"sensor\": \"si_tsl2561\", \"value\": {\"broadband\": %5.2f, \"ir\": %5.2f, \"lux\": %5.2f}}",
                            dataBB->cur,
                            dataIR->cur,
                            dataLUX->cur );
            AppIfPc_Printf( "\n\r" );
        }

        switch( opt )
        {
        case 'b': AppIfLcd_Printf( "%5.2f (BB)", dataBB->cur );   AppIfPc_Printf( "%5.2f \n\r", dataBB->cur ); break;
        case 'i': AppIfLcd_Printf( "%5.2f (IR)", dataIR->cur );   AppIfPc_Printf( "%5.2f \n\r", dataIR->cur ); break;
        case 'l': AppIfLcd_Printf( "%5.2f (LUX)", dataLUX->cur ); AppIfPc_Printf( "%5.2f \n\r", dataLUX->cur ); break;
        default: break;
        }
    }

err :
    return;
}


#ifdef __cplusplus
    }
#endif

