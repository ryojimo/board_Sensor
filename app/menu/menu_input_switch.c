/**************************************************************************//*!
 *  @file           menu_input_switch.c
 *  @brief          [APP] メニュー・アプリ
 *  @author         Ryoji Morita
 *  @attention      Push SW によるコマンド番号を更新する関数を定義。
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
#include "../../hal/hal.h"

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
extern const SAppMenuCmd_t g_menuCmdTable[ MAX_CMD_NUM ];


//********************************************************
/* Global Parameter  ( File Scope )                      */
//********************************************************
static int     sg_no;  // 現在のコマンド番号


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static EAppMenuMsg_t   GetCmdNo( void );




/**************************************************************************//*!
 * @brief     コマンド番号を更新する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ入力に変化があったら   = EN_MENU_MSG_DONE
 *            センサ入力に変化がなかったら = EN_MENU_MSG_ACK
 *************************************************************************** */
static EAppMenuMsg_t
GetCmdNo(
    void
){
    EAppMenuMsg_t   ret = EN_MENU_MSG_ACK;    // 戻り値
    int             max = 0;
    int             value = 0;

    DBG_PRINT_TRACE( "\n\r" );

    // 全コマンド数をセット
    max = sizeof(g_menuCmdTable) / sizeof(SAppMenuCmd_t);

    // 実行するコマンド番号を決定
    if( EN_TRUE == IsMinusSw() )
    {
        value--;
    }

    if( EN_TRUE == IsPlusSw() )
    {
        value++;
    }

    // センサに変化があった場合は sg_no を更新する
    if( value != 0 )
    {
        ret = EN_MENU_MSG_DONE;
        sg_no = sg_no + value;
        if( sg_no <= 0 )
        {
            sg_no = 1;
        }

        if( sg_no >= max - 1 )
        {
            sg_no = max - 2;
        }
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     ファイルスコープの変数をクリアする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
MenuInput_Switch_Clear(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    sg_no  = 0;

    return;
}


/**************************************************************************//*!
 * @brief     Push SW
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    コマンド番号更新あり = EN_MENU_MSG_DONE
 *            コマンド番号更新なし = EN_MENU_MSG_ACK
 *************************************************************************** */
EAppMenuMsg_t
MenuInput_Switch_Get(
    int*            no  ///< [in][out] コマンド番号
){
    EAppMenuMsg_t   ret = EN_MENU_MSG_ACK;    // 戻り値

    DBG_PRINT_TRACE( "\n\r" );

    // コマンド番号を更新
    sg_no = *no;
    ret = GetCmdNo();
    if( ret == EN_MENU_MSG_DONE )
    {
        *no = sg_no;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

