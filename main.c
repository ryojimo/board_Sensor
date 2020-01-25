/**************************************************************************//*!
 *  @file           main.c
 *  @brief          main() を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.12.03
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>

#include "./app/if_lcd/if_lcd.h"
#include "./app/options/options.h"
#include "./hal/hal.h"
#include "./sys/sys.h"

//#define DBG_PRINT
#define MY_NAME "MAI"
#include "./app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// getopt() で使用
extern char *optarg;
extern int  optind, opterr, optopt;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし




/**************************************************************************//*!
 * @brief     メイン
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
int main(int argc, char *argv[])
{
    int             cmd = 0;
    int             data = 0;
    unsigned char*  pt;

    int             opt = 0;
    const char      optstring[] = "a:c:e:g:hi:l:m:o:p:r:st:u:vw:x:y:z:";
    const struct    option longopts[] = {
      //{ *name,         has_arg,           *flag, val }, // 説明
        { "sa_acc",      required_argument, NULL,  'a' },
        { "i2clcd",      required_argument, NULL,  'c' },
        { "i2cpca9685",  required_argument, NULL,  'e' },
        { "sa_gyro",     required_argument, NULL,  'g' },
        { "help",        no_argument,       NULL,  'h' },
        { "pic",         required_argument, NULL,  'i' },
        { "led",         required_argument, NULL,  'l' },
        { "menu",        required_argument, NULL,  'm' },
        { "motorsv",     required_argument, NULL,  'o' },
        { "sa_pm",       required_argument, NULL,  'p' },
        { "relay",       required_argument, NULL,  'r' },
        { "sensors",     no_argument,       NULL,  's' },
        { "time",        required_argument, NULL,  't' },
        { "usbkey",      required_argument, NULL,  'u' },
        { "version",     no_argument,       NULL,  'v' },
        { "si_bme280",   required_argument, NULL,  'w' },
        { "si_gp2y0e03", required_argument, NULL,  'x' },
        { "si_lps25h",   required_argument, NULL,  'y' },
        { "si_tsl2561",  required_argument, NULL,  'z' },
        { 0,             0,                 NULL,   0  }, // termination
    };
    int longindex = 0;

    Sys_Init();

#if 0
    DBG_PRINT_ERROR( "test error \n\r" );
    DBG_PRINT_WARN(  "test warn  \n\r" );
    DBG_PRINT_TRACE( "test trace \n\r" );
    DBG_PRINT_DEBUG( "test debug \n\r" );
#endif

    DBG_PRINT_TRACE( "argc    = %d \n\r", argc );
    DBG_PRINT_TRACE( "argv[0] = %s \n\r", argv[0] );
    DBG_PRINT_TRACE( "argv[1] = %s \n\r", argv[1] );
    DBG_PRINT_TRACE( "argv[2] = %s \n\r", argv[2] );
    DBG_PRINT_TRACE( "argv[3] = %s \n\r", argv[3] );


    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "cmd:%s", argv[1] );

    Sys_ShowInfo();

    while( 1 )
    {
        opt = getopt_long( argc, argv, optstring, longopts, &longindex );
//      opt = getopt( argc, argv, optstring );
        DBG_PRINT_TRACE( "optind = %d \n\r", optind );
        DBG_PRINT_TRACE( "opt    = %c \n\r", opt );

        if( opt == -1 )   // 処理するオプションが無くなった場合
        {
            break;
        } else if( opt == '?' )  // optstring で指定していない引数が見つかった場合
        {
            DBG_PRINT_TRACE( "optopt = %c \n\r", optopt );
            break;
        }

        optind = 1;
        argc = argc - optind;
        argv = argv + optind;

        switch( opt )
        {
        case 'a': Opt_Sa_Acc( argc, argv ); break;
        case 'c': Opt_I2cLcd( argc, argv ); break;
        case 'e': Opt_I2cPca9685( argc, argv ); break;
        case 'g': Opt_Sa_Gyro( argc, argv ); break;
        case 'h': Opt_Help(); break;
        case 'l': Opt_Led( argc, argv ); break;
        case 'm': Opt_Menu( argc, argv ); break;
        case 'o': Opt_MotorSV( argc, argv ); break;
        case 'p': Opt_Sa_Pm( argc, argv ); break;
        case 'r': Opt_Relay( argc, argv ); break;
        case 's': Opt_Sensors(); break;
        case 't': Opt_Time( argc, argv ); break;
        case 'v': Opt_Version(); break;
        case 'w': Opt_Si_Bme280( argc, argv ); break;
        case 'x': Opt_Si_Gp2y0e03( argc, argv ); break;
        case 'y': Opt_Si_Lps25h( argc, argv ); break;
        case 'z': Opt_Si_Tsl2561( argc, argv ); break;

        case 'i': 
            if( argv[2] != NULL )
            {
                cmd  = strtol( (const char*)argv[2], (char**)&pt, 10 );
            }
            if( argv[3] != NULL )
            {
                data = strtol( (const char*)argv[3], (char**)&pt, 10 );
            }

            DBG_PRINT_TRACE( "cmd  = 0x%02X(H) : %02d(d) \n", cmd,  cmd  );
            DBG_PRINT_TRACE( "data = 0x%02X(H) : %02d(d) \n", data, data );
            HalI2cCmd_Set( cmd, data );
        break;
        case 'u': 
            if( optarg != NULL )
            {
                data  = strtol( (const char*)optarg, (char**)&pt, 16 );
            }

            DBG_PRINT_TRACE( "data = 0x%02X(H) : %02d(d) \n", data, data );
            HalI2cCmd_SetKeycode( data );
        break;
        default:
            DBG_PRINT_ERROR( "invalid command/option. : \"%s\" \n\r", argv[1] );
            Opt_Help();
        break;
        }
    }

    Sys_Fini();
    return 0;
}


#ifdef __cplusplus
    }
#endif
