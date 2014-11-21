/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_TimBase.h
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver gestion TimBase
 *
 *********************************************************************/

#ifndef DRV_TIMBASE_H
#define DRV_TIMBASE_H

/********************************************************************
 * Includes
 */
#include "BSP_carte.h"

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
void TimeBase_init(
	Mapping_GPIO_e	IdPinPwm,			/**<[in] ID de la Pin ou générer la PWM*/
	uint32_t 	TIM_Periode,		/**<[in] Periode de comptage */
	uint32_t 	TIM_Prescaler 		/**<[in] Prescaler du TimeBase */
);

#endif
