/**************************************************************************//*!
 *  @file           menu.c
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
#include <string.h>

#include "../../hal/hal.h"
#include "../../sys/sys.h"

#include "../if_lcd/if_lcd.h"
#include "../if_pc/if_pc.h"

#include "menu.h"
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
extern unsigned char        g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];
extern const SAppMenuCmd_t  g_menuCmdTable[ MAX_CMD_NUM ];


//********************************************************
/* Global Parameter  ( File Scope )                      */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし


/**************************************************************************//*!
 * @brief     シェル
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppMenu_Main(
    unsigned char*  str     ///< [in] コマンドライン
){
    EAppMenuMsg_t   ret = EN_MENU_MSG_ERR_FATAL;    // 戻り値

    DBG_PRINT_TRACE( "\n\r" );

    // 初期化
    MenuCmd_Init();
    MenuInput_Init();

    MenuInput_Switch_Clear();
    MenuInput_Volume_Clear();

    // 引数 str の処理
    if( strlen( (const char*)str ) > 0 )
    {
        ret = ParseCmd( str );
        if( ret == EN_MENU_MSG_DONE )
        {
            ret = ExecuteCmd( &g_menuCmd[0][0], g_menuCmdTable );
            if( ret != EN_MENU_MSG_DONE )
            {
                DBG_PRINT_ERROR( "\n\r input error : %s\n\r", str );
            }
        }
    }

    // メイン処理
    SelectCmd();

    return;
}


#ifdef __cplusplus
    }
#endif

