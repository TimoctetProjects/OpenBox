/*********************************************************************
 * *******************************************************************
 *
 * @file	equ_ServoDirection.h
 *
 * @author	Duclos Timothe
 *
 * @date	22/09/2014
 *
 * @brief	Driver du servomoteur de la direction
 *
 *********************************************************************/

#ifndef EQU_SERVODIRECTION_H
#define EQU_SERVODIRECTION_H

/********************************************************************
 * Includes
 */
#include "drv_PWM.h"
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
typedef enum {

	ServoDirection_Position_moins90	= 0,
	ServoDirection_Position_moins45,

	ServoDirection_Position_zero,

	ServoDirection_Position_plus45,
	ServoDirection_Position_plus90,

	nb_ServoDirection_Positions

}ServoDirection_ListePosition_e;

/********************************************************************
 * Exported Function
 */
/**-------------------------------------------------------------------
 *
 * @brief	Procedure de placement du servomoteur
 *
 */
void
ServoDirection_Main(
		void
);

/**-------------------------------------------------------------------
 *
 * @brief	Nouvelle position a adopter
 *
 */
void
ServoDirection_SetNewPosition(
		ServoDirection_ListePosition_e	Position	/**<[in] Nouvelle position a adopter */
);

/**-------------------------------------------------------------------
 *
 * @brief	Lecture de la position actuellement adoptée
 *
 */
ServoDirection_ListePosition_e
ServoDirection_GetPosition(
		void
);

/**-------------------------------------------------------------------
 *
 * @brief	toString de la position du servomoteur
 *
 */
void
ServoDirection_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

#endif
