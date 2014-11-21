/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_GPIO.h
 *
 * @author	Duclos Timothe
 *
 * @date	19/07/2014
 *
 * @brief	Driver GPIO
 *
 *********************************************************************/

#ifndef DRV_GPIO_H
#define DRV_GPIO_H

/********************************************************************
 * Includes
 */
#include "BSP/bsp_carte.h"
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


/**--------------------------------------------------------------------
 *
 * @brief	Fonction permettant d'ecrire l'etat d'un PIN GPIO
 *
 * @return	void
 *
 */
void
GPIO_Set(
		Mapping_GPIO_e 	Pin,		/**<[in] ID de la Pin GPIO dont on veut setter l'etat. */
		Etat_e		State		/**<[in] Etat a setter pour la Pin GPIO. */
);

/**--------------------------------------------------------------------
 *
 * @brief	Fonction permettant de lire l'etat d'un PIN GPIO
 *
 * @return	Etat de la Pin GPIO.
 *
 */
Etat_e
GPIO_Get(
		Mapping_GPIO_e 	Pin		/**<[in] ID de la Pin GPIO dont on veut setter l'etat. */
);

/**--------------------------------------------------------------------
 *
 * @brief	Fonction permettant de convertir en string l'etat d'une GPIO
 *
 */
void
GPIO_Value_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

#endif // DRV_GPIO_H
