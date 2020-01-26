/**************************************************************************//*!
 *  @file           opt_i2c_lcd.c
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
static void Clear( void );
static void Init( void );


/**************************************************************************//*!
 * @brief     LCD をクリアする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static void
Clear(
    void
){
    DBG_PRINT_TRACE( "Clear() \n\r" );
    AppIfLcd_Clear();
    return;
}


/**************************************************************************//*!
 * @brief     LCD を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static void
Init(
    void
){
    DBG_PRINT_TRACE( "Init() \n\r" );
    HalI2cLcd_Init();
    return;
}


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
    AppIfPc_Printf( " Main option)                                                     \n\r" );
    AppIfPc_Printf( "     -c, --i2clcd : control the (I2C) LCD.                        \n\r" );
    AppIfPc_Printf( "                                                                  \n\r" );
    AppIfPc_Printf( " Sub option)                                                      \n\r" );
    AppIfPc_Printf( "     -c,        --clear         : clear the LCD display.          \n\r" );
    AppIfPc_Printf( "     -h,        --help          : display the help menu.          \n\r" );
    AppIfPc_Printf( "     -i,        --init          : init the LCD display.           \n\r" );
    AppIfPc_Printf( "     -x number, --dir_x=number  : the value of x-axis.            \n\r" );
    AppIfPc_Printf( "     -y number, --dir_y=number  : the value of y-axis.            \n\r" );
    AppIfPc_Printf( "     -s string, --string=string : the string to display on LCD.   \n\r" );
    AppIfPc_Printf( "                                                                  \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                                                                 \n\r" );
    AppIfPc_Printf( "     -c        -x      <number>  -y      <number>  -s <string>       \n\r" );
    AppIfPc_Printf( "     --i2clcd  --dir_x=<number>  --dir_y=<number>  --string=<string> \n\r" );
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
Opt_I2cLcd(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "chis:x:y:";
    const struct    option longopts[] = {
      //{ *name,    has_arg,           *flag, val }, // 説明
        { "clear",  no_argument,       NULL,  'c' },
        { "help",   no_argument,       NULL,  'h' },
        { "init",   no_argument,       NULL,  'i' },
        { "string", required_argument, NULL,  's' },
        { "dir_x",  required_argument, NULL,  'x' },
        { "dir_y",  required_argument, NULL,  'y' },
        { 0,        0,                 NULL,   0  }, // termination
    };
    int             longindex = 0;
    int             x = 0;
    int             y = 0;
    char            str[16];

    DBG_PRINT_TRACE( "Opt_I2cLcd() \n\r" );
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
        } else if( opt == 'c' )
        {
            Clear();
            goto err;
            break;
        } else if( opt == 'h' )
        {
            Help();
            goto err;
            break;
        } else if( opt == 'i' )
        {
            Init();
            goto err;
            break;
        }

        switch( opt )
        {
        case 's': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); strncpy( str, (const char*)optarg, 16 ); break;
        case 'x': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); x = strtol( (const char*)optarg, NULL, 10 ); break;
        case 'y': DBG_PRINT_TRACE( "optarg = %s \n\r", optarg ); y = strtol( (const char*)optarg, NULL, 10 ); break;
        default: break;
        }
    }

    DBG_PRINT_TRACE( "(x, y) = (%d, %d) \n\r", x, y );
    DBG_PRINT_TRACE( "str    = %s \n\r", str );
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "                " );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "                " );

    AppIfLcd_CursorSet( x, y );
    AppIfLcd_Printf( str );

err:
    return;
}


#ifdef __cplusplus
    }
#endif

