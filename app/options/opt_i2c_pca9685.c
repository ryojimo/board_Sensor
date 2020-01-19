/**************************************************************************//*!
 *  @file           opt_i2c_pca9685.c
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
    AppIfPc_Printf( " Main option)                                                      \n\r" );
    AppIfPc_Printf( "     -e, --i2cpca9685 : control the (I2C) PCA9685.                 \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf( " Sub option)                                                       \n\r" );
    AppIfPc_Printf( "     -h               --help              : display the help menu. \n\r" );
    AppIfPc_Printf( "     -c number,       --ch=number         : the target channnel.   \n\r" );
    AppIfPc_Printf( "     -r float-number, --rate=float-number : the duty-rate.         \n\r" );
    AppIfPc_Printf( "                                                                   \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                                                               \n\r" );
    AppIfPc_Printf( "     -e           -c   <number>  -r     <float-number>             \n\r" );
    AppIfPc_Printf( "     --i2cpca9685 --ch=<number>  --rate=<float-number>             \n\r" );
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
Opt_I2cPca9685(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hc:r:";
    const struct    option longopts[] = {
      //{ *name,  has_arg,           *flag, val }, // 説明
        { "help", no_argument,       NULL,  'h' },
        { "ch",   required_argument, NULL,  'c' },
        { "rate", required_argument, NULL,  'r' },
        { 0,      0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    int             ch = 0;
    double          rate = 0;
    char*           endptr;

    DBG_PRINT_TRACE( "Opt_I2cPca9685() \n\r" );
    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );

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
        }

        switch( opt )
        {
        case 'c': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); ch   = strtol( (const char*)optarg, NULL, 10 ); break;
        case 'r': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); rate = strtod( (const char*)optarg, &endptr ); break;
        default: break;
        }
    }

    DBG_PRINT_TRACE( "(ch, rate) = (%d, %2.4f) \n\r", ch, rate );
    DBG_PRINT_TRACE( "endptr     = %s \n\r", endptr );

    if( 0 <= ch && ch <= 15 )
    {
        HalI2cPca9685_SetPwmDuty( ch, EN_MOTOR_CW, rate );
    } else
    {
        DBG_PRINT_ERROR( "invalid ch number error. Please input between 0 and 15. : %d \n\r", ch );
        goto err;
    }

//  usleep( 1000 * 1000 );  // 2s 待つ
err :
    return;
}


#ifdef __cplusplus
    }
#endif

