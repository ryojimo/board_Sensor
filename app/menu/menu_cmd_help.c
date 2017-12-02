/**************************************************************************//*!
 *  @file           menu_cmd_help.c
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
#include "../if_pc/if_pc.h"

#include "menu_base.h"
#include "menu_input.h"

//#define DBG_PRINT
#define MY_NAME "APP"
#include "../log/log.h"


//********************************************************
/* Global Parameter  ( Global Scope )                    */
//********************************************************
extern unsigned char g_menuCmd[ SYS_MAX_CMD_OPT_NUM ][ SYS_MAX_CMD_NAME_LEN ];


/**************************************************************************//*!
 * @brief     HELP を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EAppMenuMsg_t 型に従う。
 *************************************************************************** */
EAppMenuMsg_t
MenuCmd_Help(
    void
){
    DBG_PRINT_TRACE( "MenuCmd_Help() \n\r" );

    AppIfPc_Printf( "\t COMMAND    : EXPLAIN                                         \n\r" );
    AppIfPc_Printf( "\t==============================================================\n\r" );
    AppIfPc_Printf( "\t help       : display command list.                           \n\r" );
    AppIfPc_Printf( "\t i2clcd     : display I2C LCD.                                \n\r" );
    AppIfPc_Printf( "\t led        : Light LED.                                      \n\r" );
    AppIfPc_Printf( "\t motorSV    : turn around the SERVO    motor.                 \n\r" );
    AppIfPc_Printf( "\t pushsw     : display count data of push switch.              \n\r" );
    AppIfPc_Printf( "\t relay      : Control Relay.                                  \n\r" );
    AppIfPc_Printf( "\t                                                              \n\r" );
    AppIfPc_Printf( "\t sa_acc     : sensor(adc) display acceleration data. (X,Y,Z)  \n\r" );
    AppIfPc_Printf( "\t sa_gyro    : sensor(adc) display gyro data. (G1,G2)          \n\r" );
    AppIfPc_Printf( "\t sa_pm      : sensor(adc) display Potentiometer data.         \n\r" );
    AppIfPc_Printf( "\t si_bme280  : sensor(i2c) display BME280 sensor data.         \n\r" );
    AppIfPc_Printf( "\t si_gp2y0e03: sensor(i2c) display distance sensor data.       \n\r" );
    AppIfPc_Printf( "\t si_lps25h  : sensor(i2c) display LPS25H data.                \n\r" );
    AppIfPc_Printf( "\t si_tsl2561 : sensor(i2c) display TSL2561 data.               \n\r" );
    AppIfPc_Printf( "\t time       : sensor(i2c) display time/date data.             \n\r" );
    AppIfPc_Printf( "\n\r" );

    return EN_MENU_MSG_DONE;
}


#ifdef __cplusplus
    }
#endif

