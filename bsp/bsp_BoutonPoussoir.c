/*********************************************************************
 * *******************************************************************
 *
 * @file	bsp_BoutonPoussoir.c
 *
 * @author	Duclos Timothe
 *
 * @date	19/07/2014
 *
 * @brief	Gestion des Boutons
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "bsp_BoutonPoussoir.h"

/********************************************************************
 * Private defines
 */
#define BOUTON_APPUYE_STRING		"Appuye"
#define BOUTON_NONAPPUYE_STRING		"Non appuye"

/********************************************************************
 * Private macros
 */
#define __BoutonPoussoir_IsIDInvalid(ID)			(	(ID < 0)	||	(ID > nb_BTNS)		)
#define __BoutonPoussoir_SetButtonPressedString(String)		(	__strncat( String, BOUTON_APPUYE_STRING)	)
#define	__BoutonPoussoir_SetButtonUnpressedString(String)	(	__strncat(String, BOUTON_NONAPPUYE_STRING)	)

/********************************************************************
 * Private types
 */


/********************************************************************
 * Private variables
 */
static 	BoutonPoussoir_s BoutonPoussoir	[nb_BTNS] = {	{Button_State_Unpressed, BTN_WKP}	};


/********************************************************************
 * Private Fonctions
 */

/********************************************************************
 * Exported Fonctions
 */

/*********************************************************************
 *
 * @brief	Fonction permettant d'initialiser l'objet bouton
 *
 */
void BtnPssr_init(void)
{
	uint8_t b_Btn;
	for(b_Btn=0; b_Btn<nb_BTNS; b_Btn++) {
		BoutonPoussoir[b_Btn].State = Button_State_Unpressed;
	}
}

/*********************************************************************
 *
 * @brief	Fonction permettant de lire si le bouton a ete relache
 *
 */
Bool_e BtnPssr_Is_Unleashed(BoutonPoussoir_e ID_Btn)
{
	// TODO

	return FALSE;
}

/*********************************************************************
 *
 * @brief	Fonction permettant d'actualiser l'etat du btn poussoir
 *
 */
void BtnPssr_LaunchUpdate(void)
{
	uint8_t b_Btn;
	Etat_e 	GPIOState;

	for(b_Btn=0; b_Btn<nb_BTNS; b_Btn++) {
		GPIOState = GPIO_Get(BoutonPoussoir[b_Btn].ID_Pin_Mappin);

		switch(GPIOState) {

			case	ETAT_ACTIF	:	BoutonPoussoir[b_Btn].State = Button_State_Pressed;		break;
			case	ETAT_INACTIF	:	BoutonPoussoir[b_Btn].State = Button_State_Unpressed;		break;
			default			:									break;
		}
	}
}

/*********************************************************************
 *
 * @brief	Fonction permettant de lire l'etat d'un bouton poussoir
 *
 */
BoutonPoussoir_State_e
BtnPssr_ReadState(
	BoutonPoussoir_e ID_Btn		/**<[in] ID du bouton poussoir. */
) {
	if __BoutonPoussoir_IsIDInvalid(ID_Btn)
		return Button_Inexistant;

	return BoutonPoussoir[ID_Btn].State;
}

/*********************************************************************
 *
 * @brief	Fonction permettant de lire l'etat d'un bouton poussoir
 *
 */
uint8_t
BtnPssr_ReadIDPin(
		BoutonPoussoir_e ID_Btn		/**<[in] ID du bouton poussoir. */
) {
	if __BoutonPoussoir_IsIDInvalid(ID_Btn)
		return Button_Inexistant;

	return BoutonPoussoir[ID_Btn].ID_Pin_Mappin;
}

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
) {

	switch(Field) {

		//----------------------------------------------------------
		case toString_Getpin:	toString_GetPeriphral	(Mapping_GPIO[BoutonPoussoir[ID_Button].ID_Pin_Mappin].GpioPeripheral, pString);
					toString_GetPin		(Mapping_GPIO[BoutonPoussoir[ID_Button].ID_Pin_Mappin].GpioPin, 	 pString);
					break;

		//----------------------------------------------------------
		case toString_GetValue:
					if(BtnPssr_ReadState(ID_Button) == Button_State_Pressed) {
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_verte);
						__BoutonPoussoir_SetButtonPressedString(pString);
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);

					} else	{
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_jaune);
						__BoutonPoussoir_SetButtonUnpressedString(pString);
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);

					} break;

		//----------------------------------------------------------
		default:
					break;
	}
}
