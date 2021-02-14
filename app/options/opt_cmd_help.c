/**************************************************************************//*!
 *  @file           opt_help.c
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
#include "../if_pc/if_pc.h"

//#define DBG_PRINT
#define MY_NAME "OPT"
#include "../log/log.h"


/**************************************************************************//*!
 * @brief     実行する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
OptCmd_Help(
    void
){
    DBG_PRINT_TRACE( "OptCmd_Help() \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( " OPTION            : EXPLAIN                                        \n\r" );
    AppIfPc_Printf( "====================================================================\n\r" );
    AppIfPc_Printf( " -h, --help        : display the help menu.                         \n\r" );
    AppIfPc_Printf( " -v, --version     : display the version information.               \n\r" );
    AppIfPc_Printf( " -m, --menu        : go to the menu mode.                           \n\r" );
    AppIfPc_Printf( " -q, --sensors     : display the values of all sensors.             \n\r" );
    AppIfPc_Printf( "                                                                    \n\r" );
    AppIfPc_Printf( " -b, --i2cads1015  : control the (I2C) ADS1015.                     \n\r" );
    AppIfPc_Printf( " -c, --i2clcd      : control the (I2C) LCD.                         \n\r" );
    AppIfPc_Printf( " -e, --i2cpca9685  : control the (I2C) PCA9685.                     \n\r" );
    AppIfPc_Printf( " -o, --motorsv     : control the SAVO motor.                        \n\r" );
    AppIfPc_Printf( " -l, --led         : control the LED.                               \n\r" );
    AppIfPc_Printf( " -r, --relay       : control the Relay circuit.                     \n\r" );
    AppIfPc_Printf( "                                                                    \n\r" );
    AppIfPc_Printf( " -a, --sa_acc      : get the value of a sensor(A/D), Acc.           \n\r" );
    AppIfPc_Printf( " -g, --sa_gyro     : get the value of a sensor(A/D), Gyro.          \n\r" );
    AppIfPc_Printf( " -p, --sa_pm       : get the value of a sensor(A/D), Potentiometer. \n\r" );
    AppIfPc_Printf( " -s, --pushsw      : get the value of a push switch.                \n\r" );
    AppIfPc_Printf( "                                                                    \n\r" );
    AppIfPc_Printf( " -w, --si_bme280   : get the value of a sensor(I2C), BME280.        \n\r" );
    AppIfPc_Printf( " -x, --si_gp2y0e03 : get the value of a sensor(I2C), GP2Y0E03.      \n\r" );
    AppIfPc_Printf( " -y, --si_lps25h   : get the value of a sensor(I2C), LPS25H.        \n\r" );
    AppIfPc_Printf( " -z, --si_tsl2561  : get the value of a sensor(I2C), TSL2561.       \n\r" );
    AppIfPc_Printf( " -t, --time        : get the time.                                  \n\r" );
    AppIfPc_Printf( "\n\r" );
    return;
}


#ifdef __cplusplus
    }
#endif

