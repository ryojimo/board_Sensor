/**************************************************************************//*!
 *  @file           opt_cmd_version.c
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
OptCmd_Version(
    void
){
    DBG_PRINT_TRACE( "OptCmd_Version() \n\r" );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "Copyright (C) 2021 Uz Foundation, Inc. \n\r" );
    AppIfPc_Printf( "Licence: Free. \n\r" );
    AppIfPc_Printf( "\n\r" );
    return;
}


#ifdef __cplusplus
    }
#endif

