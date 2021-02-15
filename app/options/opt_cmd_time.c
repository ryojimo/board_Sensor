/**************************************************************************//*!
 *  @file           opt_cmd_time.c
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
static void       GetData( void );
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
    AppIfPc_Printf( " Main option)                        \n\r" );
    AppIfPc_Printf( "     -t, --time : get the time.      \n\r" );
    AppIfPc_Printf( "                                     \n\r" );
    AppIfPc_Printf( " Sub option)                                         \n\r" );
    AppIfPc_Printf( "     -h, --help : display the help menu.             \n\r" );
    AppIfPc_Printf( "     -j, --json : get the all values of json format. \n\r" );
    AppIfPc_Printf( "     -m, --menu  : menu mode.                        \n\r" );
    AppIfPc_Printf( "                                                     \n\r" );
    AppIfPc_Printf( "     -d, --data : get the value.                     \n\r" );
    AppIfPc_Printf( "                                                     \n\r" );
    AppIfPc_Printf("\x1b[36m");
    AppIfPc_Printf( " Ex)                \n\r" );
    AppIfPc_Printf( "     -t      -h     \n\r" );
    AppIfPc_Printf( "     --time  --help \n\r" );
    AppIfPc_Printf( "     -t      -j     \n\r" );
    AppIfPc_Printf( "     --time  --json \n\r" );
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
    void
){
    DBG_PRINT_TRACE( "GetData() \n\r" );
    SHalTime_t*     data;

    data = HalTime_GetLocaltime();
    AppIfPc_Printf( "%4d/%02d/%02d ", data->year, data->month, data->day );
    AppIfPc_Printf( "%02d:%02d:%02d", data->hour, data->min, data->sec );
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
    DBG_PRINT_TRACE( "GetJson() \n\r" );
    SHalTime_t*     data;

    data = HalTime_GetLocaltime();

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "%4d/%02d/%02d", data->year, data->month, data->day );
    AppIfLcd_CursorSet( 0, 1 );
    AppIfLcd_Printf( "%02d:%02d:%02d", data->hour, data->min, data->sec );

    AppIfPc_Printf( "{\"time\": {\"year\": %4d, \"month\": %02d, \"day\": %02d, \"hour\": %02d, \"min\": %02d, \"sec\": %02d}}",
                    data->year,
                    data->month,
                    data->day,
                    data->hour,
                    data->min,
                    data->sec );
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
OptCmd_TimeMenu(
    void
){
    SHalTime_t* date;   ///< 日時データの構造体

    DBG_PRINT_TRACE( "OptCmd_TimeMenu() \n\r" );
    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfLcd_Clear();

    // キーを押されるまでループ
    while( EN_FALSE == AppIfBtn_IsEnter() )
    {
        // 日時データを取得
        date = HalTime_GetLocaltime();

        // PC ターミナル表示
        AppIfPc_Printf( "Date(local) = " );
        AppIfPc_Printf( "%04d/%02d/%02d %02d:%02d:%02d \n\r",
                date->year, date->month, date->day,
                date->hour, date->min,   date->sec );

        // LCD 表示
        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "%04d/%02d/%02d",
                         date->year, date->month, date->day
                         );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%02d:%02d:%02d",
                         date->hour, date->min, date->sec
                         );

        // 1 秒スリープ
        usleep( 1000 * 1000 );
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
OptCmd_Time(
    int             argc,
    char            *argv[]
){
    int             opt = 0;
    const char      optstring[] = "hjmd";
    int             longindex = 0;
    const struct    option longopts[] = {
      //{ *name,  has_arg,     *flag, val }, // 説明
        { "help", no_argument, NULL,  'h' },
        { "json", no_argument, NULL,  'j' },
        { "menu", no_argument, NULL,  'm' },
        { "data", no_argument, NULL,  'd' },
        { 0,      0,           NULL,   0  }, // termination
    };

    DBG_PRINT_TRACE( "OptCmd_Time() \n\r" );
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
        case 'm': OptCmd_TimeMenu(); break;
        case 'd': GetData(); break;
        default: break;
        }
    }

    return;
}


#ifdef __cplusplus
    }
#endif

