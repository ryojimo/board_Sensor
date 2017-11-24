/**************************************************************************//*!
 *  @file           menu_cmd_sensor_adc_acc.c
 *  @brief          [APP] メニュー・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
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
#define OPTION_NUM  (1)


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static EAppMenuMsg_t    PrintFormat( void );
static EAppMenuMsg_t    Exec( void );


//********************************************************
/* Global Parameter  ( Local Scope )                    */
//********************************************************
/* オプションテーブル */
static const SAppMenuCmd_t g_optTable[] =
{
    { ""         , Exec },
    { "h"        , PrintFormat },
    { "help"     , PrintFormat },
    { "END    "  , NULL }
};


/**************************************************************************//*!
 * @brief     コマンドの書式を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
PrintFormat(
    void
){
    AppIfPc_Printf( "    acc      \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    Ex.)\n\r" );
    AppIfPc_Printf( "        >acc \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
static EAppMenuMsg_t
Exec(
    void
){
    SHalSensor_t*   x;  ///< センサデータの構造体 : 加速度センサ X 方向
    SHalSensor_t*   y;  ///< センサデータの構造体 : 加速度センサ Y 方向
    SHalSensor_t*   z;  ///< センサデータの構造体 : 加速度センサ Z 方向

    DBG_PRINT_TRACE( "\n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );
    AppIfPc_Printf( "range : -2g -> +2g.         \n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

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

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     acceleration ( 加速度計 ) の値を表示する。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_SA_Acc(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    ExecuteCmd( &g_menuCmd[1][0], g_optTable );

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

