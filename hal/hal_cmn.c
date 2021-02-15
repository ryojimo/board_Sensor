/**************************************************************************//*!
 *  @file           hal_cmn.c
 *  @brief          [HAL 層] 共通 API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2015.09.25
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "hal_cmn.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし




/**************************************************************************//*!
 * @brief     SENSOR 変数を更新する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalCmn_UpdateSenData(
    SHalSensor_t*   curData,    ///< [in] 対象の SENSOR 変数
    double          newData     ///< [in] 新しい値
){
    DBG_PRINT_TRACE( "\n\r" );
    DBG_PRINT_TRACE( "curData->cur = %5.2f \n\r", curData->cur );
    DBG_PRINT_TRACE( "curData->max = %5.2f \n\r", curData->max );
    DBG_PRINT_TRACE( "curData->min = %5.2f \n\r", curData->min );
    DBG_PRINT_TRACE( "newData      = %5.2f \n\r", newData );


    curData->cur = newData;

    if( curData->max < newData )
    {
        curData->max = newData;
    }

    if( curData->min > newData )
    {
        curData->min = newData;
    }

    curData->err = curData->cur - curData->ofs;

    curData->cur_rate = (int)( (curData->cur / curData->max) * 100 );
    return;
}


#ifdef __cplusplus
    }
#endif

