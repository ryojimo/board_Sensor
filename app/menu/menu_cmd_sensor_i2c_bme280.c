/**************************************************************************//*!
 *  @file           menu_cmd_sensor_i2c_bme280.c
 *  @brief          [APP] メニュー・コマンド
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2017.12.02
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
    AppIfPc_Printf( "    bme280       \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "    Ex.)\n\r" );
    AppIfPc_Printf( "        >bme280  \n\r" );
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
    SHalSensor_t*   dataAtmos = NULL;   ///< 気圧センサのデータ構造体
    SHalSensor_t*   dataHumi  = NULL;   ///< 湿度センサのデータ構造体
    SHalSensor_t*   dataTemp  = NULL;   ///< 温度センサのデータ構造体

    DBG_PRINT_TRACE( "\n\r" );

    AppIfPc_Printf( "if you push any keys, break.\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 4, 0 );
#if 0
    AppIfLcd_Puts( "bme280" );
#endif
    // キーを押されるまでループ
    while( EN_FALSE == IsEnterSw() )
    {
        // センサデータを取得
        dataAtmos = HalSensorBME280_Get( EN_SEN_BME280_ATMOS );
        dataHumi  = HalSensorBME280_Get( EN_SEN_BME280_HUMI );
        dataTemp  = HalSensorBME280_Get( EN_SEN_BME280_TEMP );
#if 0
        // PC ターミナル表示
        AppIfPc_Printf( "(atmos, humi, temp) = ( %5.2f, %5.2f, %5.2f ) \r",
                        dataAtmos->cur, dataHumi->cur, dataTemp->cur
                      );
#endif
        // LCD 表示
        AppIfLcd_CursorSet( 0, 0 );
        AppIfLcd_Printf( "%5.2fhPa", dataAtmos->cur );
        AppIfLcd_CursorSet( 0, 1 );
        AppIfLcd_Printf( "%3.2f%%  %3.2f'C", dataHumi->cur, dataTemp->cur );

        // PC ターミナル表示
        AppIfPc_Printf( "(atmos, humi, temp) = ( %5.2fhPa, %5.2f%%, %5.2f'C ) \n\r",
                        dataAtmos->cur, dataHumi->cur, dataTemp->cur
                      );

        // 1 秒スリープ
        usleep( 1000 * 1000 );
    }

    AppIfPc_Printf( "\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_CursorSet( 0, 0 );

    return EN_MENU_MSG_DONE;
}


/**************************************************************************//*!
 * @brief     BME280 ( 気圧・湿度・温度 ) センサの値を表示する。
 * @attention g_menuCmd 配列の内部構造
 *              g_menuCmd[0][] : コマンド名
 *              g_menuCmd[1][] : オプション
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_SI_Bme280(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    ExecuteCmd( &g_menuCmd[1][0], g_optTable );

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

