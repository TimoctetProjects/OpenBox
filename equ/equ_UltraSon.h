/*********************************************************************
 * *******************************************************************
 *
 * @file	equ_UltraSon.h
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver du module a ultrason
 *
 *********************************************************************/

#ifndef EQU_ULTRASON_H
#define EQU_ULTRASON_H

/********************************************************************
 * Includes
 */
#include "drv_PWM.h"
#include "drv_ADC.h"
#include "util_TSW.h"
#include "util_Console.h"

/********************************************************************
 * Exported defines
 */

/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */


/********************************************************************
 * Exported Function
 */
/**-------------------------------------------------------------------
 *
 * @brief		Gestion du module a ultrason
 *
 */
void
UltraSon_main(
	void
);

void
UltraSon_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

#endif
