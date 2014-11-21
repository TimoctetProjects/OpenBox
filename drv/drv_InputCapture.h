/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_InputCapture.h
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver InputCapture
 *
 *********************************************************************/

#ifndef DRV_INPUTCAPTURE_H
#define DRV_INPUTCAPTURE_H

/********************************************************************
 * Includes
 */
#include "drv_TimBase.h"

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
uint8_t
InputCapture_init(
	Mapping_GPIO_e	IdPinIC,	/**<[in] ID de la Pin ou générer la PWM*/
	uint32_t 	Periode_us	/**<[in] Periode de comptage */
);

uint32_t
InputCapture_GetValue(
	Mapping_GPIO_e IdPinIC
);

#endif
