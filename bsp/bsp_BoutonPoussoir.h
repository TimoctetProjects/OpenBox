/*********************************************************************
 * *******************************************************************
 *
 * @file	bsp_BoutonPoussoir.h
 *
 * @author	Duclos Timothe
 *
 * @date	19/07/2014
 *
 * @brief	Gestion des Boutons
 *
 *********************************************************************/

#ifndef BSP_BOUTONPOUSSOIR_H
#define BSP_BOUTONPOUSSOIR_H


/********************************************************************
 * Includes
 */
#include "drv_GPIO.h"
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

/** @brief Enumeration des etats des boutons */
typedef enum {

	Button_State_Unpressed = 0,
	Button_State_Pressed,

	Button_Inexistant,

}BoutonPoussoir_State_e;

/** @brief Enumeration des boutons */
typedef enum {

	Button_WKP = 0,

	nb_BTNS,
}BoutonPoussoir_e;


/** @brief Structure des boutons */
typedef struct {

	BoutonPoussoir_State_e 	State;		/**< Etat du bouton */
	uint8_t 		ID_Pin_Mappin;	/**< ID de la pin dans le tableau de mapping */

}BoutonPoussoir_s;

/********************************************************************
 * Exported Functions
 */

/**------------------------------------------------------------------
 *
 * @brief	Fonction permettant d'initialiser l'objet bouton
 *
 * @return	void
 *
 */
void
BtnPssr_init(
		void
);

/**------------------------------------------------------------------
 *
 * @brief	Fonction permettant de lire si le bouton a ete relache
 *
 * @return	TRUE ou FALSE
 *
 */
Bool_e
BtnPssr_Is_Unleashed(
		BoutonPoussoir_e ID_Btn
);

/**------------------------------------------------------------------
 *
 * @brief	Fonction permettant d'actualiser l'etat du btn poussoir
 *
 * @return	void
 *
 */
void
BtnPssr_LaunchUpdate(
		void
);

/**------------------------------------------------------------------
 *
 * @brief	Fonction permettant de lire l'etat d'un bouton poussoir
 *
 * @return	L'etat du bouton poussoir
 *
 */
BoutonPoussoir_State_e
BtnPssr_ReadState(
		BoutonPoussoir_e ID_Btn		/**<[in] ID du bouton poussoir. */
);

/**------------------------------------------------------------------
 *
 * @brief	Fonction permettant de lire l'etat d'un bouton poussoir
 *
 */
uint8_t
BtnPssr_ReadIDPin(
		BoutonPoussoir_e ID_Btn		/**<[in] ID du bouton poussoir. */
);

/**------------------------------------------------------------------
 *
 * @brief	Conversion en string de l'etat d'un bouton
 *
 */
void
Button_Value_toString(

		toString_Possibilities_e	Field,
		BoutonPoussoir_e 		ID_Button,
		uint8_t*			pString
);

#endif // BSP_BOUTONPOUSSOIR_H
