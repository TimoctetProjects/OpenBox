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

/********************************************************************
 * Includes
 */
#include "equ_ServoDirection.h"

/********************************************************************
 * Private defines
 */
#define SERVO_DIRECTION_RATIO_POSITION_MOINS90_string	"Position -90deg"
#define SERVO_DIRECTION_RATIO_POSITION_MOINS45_string	"Position -45deg"
#define SERVO_DIRECTION_RATIO_POSITION_0_string		"Position 0deg"
#define SERVO_DIRECTION_RATIO_POSITION_PLUS45_string	"Position +45deg"
#define SERVO_DIRECTION_RATIO_POSITION_PLUS90_string	"Position +90deg"

/********************************************************************
 * Private macros
 */
#define __Insert_Moins90Deg_String(str)		__strncat( str, SERVO_DIRECTION_RATIO_POSITION_MOINS90_string )
#define __Insert_Moins45Deg_String(str)		__strncat( str, SERVO_DIRECTION_RATIO_POSITION_MOINS45_string )
#define __Insert_Moins0Deg_String(str)		__strncat( str, SERVO_DIRECTION_RATIO_POSITION_0_string	      )
#define __Insert_Plus45Deg_String(str)		__strncat( str, SERVO_DIRECTION_RATIO_POSITION_PLUS45_string  )
#define __Insert_Plus90Deg_String(str)		__strncat( str, SERVO_DIRECTION_RATIO_POSITION_PLUS90_string  )

/********************************************************************
 * Private Fonctions
 */

/********************************************************************
 * Private Types
 */

typedef enum {

	ServoDirection_Etape_Initialsiation 	= 0,
	ServoDirection_Etape_Commande,
	ServoDirection_Etape_AttenteCommande,

	nb_ServoDirection_Etapes,

}ServoDirection_Etapes_e;

/********************************************************************
 * Private variables
 */
static ServoDirection_ListePosition_e NewPosition	= MicroServo_Position_zero;
static ServoDirection_ListePosition_e Position 		= SERVO_DIRECTION_RATIO_POSITION_0_deg;

static const ServoDirection_ListePosition_e	ListePosition[nb_MicroServo_Positions] = {

		SERVO_DIRECTION_RATIO_POSITION_MOINS90_deg,
		SERVO_DIRECTION_RATIO_POSITION_MOINS45_deg,
		SERVO_DIRECTION_RATIO_POSITION_0_deg,
		SERVO_DIRECTION_RATIO_POSITION_PLUS45_deg,
		SERVO_DIRECTION_RATIO_POSITION_PLUS90_deg,
};

/********************************************************************
* Private Fonctions prtotypes
*/

/********************************************************************
 * Exported Fonctions
 */

/**-------------------------------------------------------------------
 *
 * @brief	Placement du servomoteur de direction
 *
 */
void
MicroServo_Main(
		void
) {

	static ServoDirection_Etapes_e 		Etape 		= ServoDirection_Etape_Initialsiation;

	switch(Etape) {

		//--------------------------------------------------------------
		case ServoDirection_Etape_Initialsiation:

					PWM_Init	(PIN_PWM_MICRO_SERVO,
							SERVO_DIRECTION_CMD_SERVO_PERIODE_PWM_us,
							SERVO_DIRECTION_RATIO_POSITION_0_deg);
					Pwm_Activer	(PIN_PWM_MICRO_SERVO);
					Etape	 	= ServoDirection_Etape_AttenteCommande;
					break;



		//--------------------------------------------------------------
		case ServoDirection_Etape_Commande:

					Pwm_Desactiver	( PIN_PWM_MICRO_SERVO );
					PWM_Desinit	( PIN_PWM_MICRO_SERVO );
					PWM_Init	( PIN_PWM_MICRO_SERVO,
							SERVO_DIRECTION_CMD_SERVO_PERIODE_PWM_us,
							  ListePosition[Position] );
					Pwm_Activer	( PIN_PWM_MICRO_SERVO );
					Etape	 	= ServoDirection_Etape_AttenteCommande;
					break;



		//--------------------------------------------------------------

		case ServoDirection_Etape_AttenteCommande:
					if(NewPosition != Position) {
						Position = NewPosition;
						Etape = ServoDirection_Etape_Commande;
					} break;



		//--------------------------------------------------------------
		default:
					break;
	}
}


/**-------------------------------------------------------------------
 *
 * @brief	Nouvelle position a adopter
 *
 */
void
MicroServo_SetNewPosition(
		ServoDirection_ListePosition_e	pos	/**<[in] Nouvelle position a adopter */
) {

	if(pos != NewPosition && pos != Position) {
		NewPosition = pos;
	}
}

/**-------------------------------------------------------------------
 *
 * @brief	Lecture de la position actuellement adoptée
 *
 */
ServoDirection_ListePosition_e
MicroServo_GetPosition(
		void
) {

	return Position;
}



/**-------------------------------------------------------------------
 *
 * @brief	toString de la position du servomoteur
 *
 */
void
MicroServo_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
) {
	switch(Field) {

		//------------------------------------------------------------
		case toString_Getpin:	break;

		//------------------------------------------------------------
		case toString_GetValue:

			switch(Position) {

				case	MicroServo_Position_moins90:	__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_cyan);
										__Insert_Moins90Deg_String(pString);
										__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);
										break;

				case	MicroServo_Position_moins45:	__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_jaune);
										__Insert_Moins45Deg_String(pString);
										__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);
										break;

				case	MicroServo_Position_zero:		__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_verte);
										__Insert_Moins0Deg_String(pString);
										__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);
										break;

				case	MicroServo_Position_plus45:		__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_jaune);
										__Insert_Plus45Deg_String(pString);
										__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);
										break;

				case	MicroServo_Position_plus90:		__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_cyan);
										__Insert_Plus90Deg_String(pString);
										__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);
										break;

				default:					break;
			} break;

		//------------------------------------------------------------
		default:
			break;
	}
}
