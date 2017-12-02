/**************************************************************************//*!
 *  @file           menu_cmd.c
 *  @brief          [APP] メニュー・アプリ
 *  @author         Ryoji Morita
 *  @attention      新規にコマンドを追加する場合、コマンドテーブルのNULLの上に登録する。
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.07.04
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

#include "../../hal/hal.h"

#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

#include "menu_base.h"
#include "menu_input.h"

//#define DBG_PRINT
#define MY_NAME "APP"
#include "../log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
/* コマンドテーブル */
const SAppMenuCmd_t g_menuCmdTable[ MAX_CMD_NUM ] =
{
    { ""            , MenuCmd_Help },
    { "help"        , MenuCmd_Help },
    { "i2clcd"      , MenuCmd_I2cLcd },
    { "led"         , MenuCmd_Led },
    { "motorsv"     , MenuCmd_MotorSV },
    { "pushsw"      , MenuCmd_PushSw },
    { "relay"       , MenuCmd_Relay },
    { "sa_acc"      , MenuCmd_SA_Acc },
    { "sa_gyro"     , MenuCmd_SA_Gyro },
    { "sa_pm"       , MenuCmd_SA_Pm },
    { "si_bme280"   , MenuCmd_SI_Bme280 },
    { "si_gp2y0e03" , MenuCmd_SI_Gp2y0e03 },
    { "si_lps25h"   , MenuCmd_SI_Lps25h },
    { "si_tsl2561"  , MenuCmd_SI_Tsl2561 },
    { "time"        , MenuCmd_Time },
    { "END    "  , NULL }
};

/* コマンド・バッファ */
unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


//********************************************************
/* Global Parameter  ( File Scope )                      */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static char*            Strlower( char* str );
static int              SetCmd( unsigned char* in, int num );
static EAppMenuMsg_t    Execute( unsigned char* name, const SAppMenuCmd_t*  table );




/**************************************************************************//*!
 * @brief     文字列をすべて小文字にする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    小文字にした文字列の先頭アドレス
 *************************************************************************** */
static char*
Strlower(
    char*   str /* 大文字、小文字を含む文字列 */
){
    char* ret;

    ret = str;
    while( *str != '\0' )
    {
        *str = tolower( (int)(*str) );
        str++;
    }
    return ret;
}


/**************************************************************************//*!
 * @brief     初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
MenuCmd_Init(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    memset( &g_menuCmd[0][0], '\0', SYS_MAX_CMD_BUFF );

    return;
}


/**************************************************************************//*!
 * @brief     コマンド・バッファにコマンド、オプションをセットする。
 * @attention なし。
 * @note      コマンド、オプションの番号
 *               g_menuCmd[0][] : コマンド名
 *               g_menuCmd[1][] : オプション01
 *               g_menuCmd[2][] : オプション02
 *               g_menuCmd[3][] : オプション03
 *               g_menuCmd[4][] : オプション04
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    コマンド名、or オプション名の長さ
 *************************************************************************** */
static int
SetCmd(
    unsigned char*  in,     ///< [in] コマンド名 or オプション名
    int             num     ///< [in] コマンド、オプションの番号 ( コマンドの時は 0, オプションは 1 ～ 4 )
){
    unsigned char*  start;  // 呼び出されたときの in のアドレス
    int             i = 0;

    DBG_PRINT_TRACE( "in  = %s \n\r", in );
    DBG_PRINT_TRACE( "num = %d \n\r", num );

    start = in;
    while( ( ' ' != *in ) && ( 0 != isprint( *in ) ) )
    {
        g_menuCmd[ num ][ i++ ] = *in++;
    }

    g_menuCmd[ num ][ i ] = '\0';   // 最後に'\0'を入れる

    return in - start;
}


/**************************************************************************//*!
 * @brief     入力文字列をパースして g_menuCmd[][] コマンド・バッファへセットする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    成功時 = EN_MENU_MSG_DONE , 失敗時 = EN_MENU_MSG_ERR_FATAL
 *************************************************************************** */
EAppMenuMsg_t
ParseCmd(
    unsigned char*  str     ///< [in] コマンド文字列
){
    int             cnt = -1;                       // コマンド、オプション数カウンタ
    EAppMenuMsg_t   ret = EN_MENU_MSG_ERR_FATAL;    // 戻り値

    DBG_PRINT_TRACE( "str = %s \n\r", str );

    // 入力文字列をコマンド名と各オプション名に仕分け
    while( '\0' != *str )
    {
        if( 0 != isspace( *str ) )  // 先頭の空白をとばす
        {
            str++;
        } else
        {
            cnt++;
            if( cnt >= SYS_MAX_CMD_OPT_NUM )        // コマンド本体 + オプション数のチェック
            {
                DBG_PRINT_ERROR( "too much.(MAX=%d) : %d\n\r", SYS_MAX_CMD_OPT_NUM, cnt );
                ret = EN_MENU_MSG_ERR_FATAL;
                goto err;
            }

            str += SetCmd( str, cnt );  // g_menuCmd[][] へ コマンド名 or オプション名 をひとつずつ格納

            if( SYS_MAX_CMD_NAME_LEN <= strlen( (char*)g_menuCmd[cnt] ) ) // 名前の長さチェック
            {
                DBG_PRINT_ERROR( "too long.(MAX=%d) : %s\n\r", SYS_MAX_CMD_NAME_LEN, g_menuCmd[cnt] );
                ret = EN_MENU_MSG_ERR_FATAL;
                goto err;
            }
        }
    }

    Strlower( (char*)&g_menuCmd[0][0] );  // コマンド名を小文字列へ変換
    ret = EN_MENU_MSG_DONE;

    DBG_PRINT_TRACE( "cmd = %s \n\r", &g_menuCmd[0][0] );
    DBG_PRINT_TRACE( "opt = %s \n\r", &g_menuCmd[1][0] );
    DBG_PRINT_TRACE( "opt = %s \n\r", &g_menuCmd[2][0] );
    DBG_PRINT_TRACE( "opt = %s \n\r", &g_menuCmd[3][0] );
    DBG_PRINT_TRACE( "opt = %s \n\r", &g_menuCmd[4][0] );

err :
    return ret;
}


/**************************************************************************//*!
 * @brief     コマンド or オプションのテーブルから関数を実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    関数を実行してない場合     = EN_MENU_MSG_ACK
 *            関数を実行した場合         = EN_MENU_MSG_DONE
 *            テーブルに name がない場合 = EN_MENU_MSG_ERR_WARNING
 *************************************************************************** */
static EAppMenuMsg_t
Execute(
    unsigned char*          name,       ///< [in] コマンド or オプションの名前
    const SAppMenuCmd_t*    table       ///< [in] コマンド or オプションのテーブルのポインタ
){
    EAppMenuMsg_t           ret = EN_MENU_MSG_ACK;
    int                     out = -1;
    int                     no = 0;

    DBG_PRINT_TRACE( "name = %s \n\r", name );

    Strlower( (char*)name );    // 小文字列へ変換

    for( no=0; table[no].func != NULL; no++ )
    {
        out = strcmp( (char*)table[no].name, (char*)name );

        if( 0 == out )
        {
            ret = (*table[no].func)();
            break;
        }
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     オプションテーブルをパースして、そのオプションの処理を実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
ExecuteCmd(
    unsigned char*          name,       ///< [in] オプション名
    const SAppMenuCmd_t*    table       ///< [in] オプションテーブルのポインタ
){
    EAppMenuMsg_t           ret = EN_MENU_MSG_ACK;

    DBG_PRINT_TRACE( "name = %s \n\r", name );

    if( 0 != strlen( (char*)name ) )
    {
        // テーブル中からオプションを探して、それを実行する
        ret = Execute( name, table );
    } else
    {
        // name が空ならば、テーブル番号 0 番の処理を実施
        (*table[0].func)();
        ret = EN_MENU_MSG_DONE;
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     コマンドを選択する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
SelectCmd(
    void
){
    EAppMenuMsg_t   retVol = EN_MENU_MSG_ACK;
    EAppMenuMsg_t   retSw = EN_MENU_MSG_ACK;

    EAppMenuMsg_t   ret = EN_MENU_MSG_ACK;

    unsigned char   str[ SYS_MAX_CMD_BUFF ];    // 文字列バッファ
    int             no = 0;                     // コマンド番号

    DBG_PRINT_TRACE( "\n\r" );

    // 初期化
    memset( str, '\0', SYS_MAX_CMD_BUFF );

    // LCD 設定
    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );   // 表示のON, カーソル表示のOFF, カーソルブリンクのOFF
    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( ">" );

    while( EN_FALSE == IsPlusMinusSw() )
    {
        // コマンド名を取得
        retVol = MenuInput_Volume_Get( &no );
        retSw  = MenuInput_Switch_Get( &no );

        if( retVol == EN_MENU_MSG_DONE || retSw == EN_MENU_MSG_DONE  )
        {
            strcpy( (char*)str, (const char*)g_menuCmdTable[no].name );
            MenuInput_Display( no );
        }

        // コマンドを実行
        if( EN_FALSE == IsMinusSw() && EN_FALSE == IsPlusSw() && EN_TRUE == IsEnterSw() )
        {
            if( 0 == strlen( (char*)str ) )
            {
                strcpy( (char*)str, (const char*)g_menuCmdTable[no].name );
            }

            // コマンド実行
            DBG_PRINT_TRACE( "\n\r input : %s \n\r", str );
            ret = ParseCmd( str );
            if( ret == EN_MENU_MSG_DONE )
            {
                ret = ExecuteCmd( &g_menuCmd[0][0], g_menuCmdTable );
                if( ret != EN_MENU_MSG_DONE )
                {
                    DBG_PRINT_ERROR( "\n\r input error : %s\n\r", str );
                }

                memset( &g_menuCmd[0][0], '\0', SYS_MAX_CMD_BUFF );
            }

            memset( str, '\0', SYS_MAX_CMD_BUFF );
            MenuInput_Display( no );
        }

        usleep( 200 * 1000 );
    }

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

