/*********************************************************************
 * *******************************************************************
 *
 * @file	app_AffDebug.c
 *
 * @author	Duclos Timothe
 *
 * @date	30/09/2014
 *
 * @brief	Gestion de l'affichage console
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "app_AffDebug.h"

/********************************************************************
 * Private defines
 */

/** Configuration affichage */
#define PERIODE_RAFRAICHISSEMENT_AFFICHAGE_ms	50

#define TAILLE_TABULATION		5

#define NB_CHAR_NOM			30
#define NB_CHAR_VALUE			50
#define NB_CHAR_PERIPH			17

#define NB_COLONNES			3
#define Nom_Colonne1			"Nom"
#define Nom_Colonne2			"Pin"
#define Nom_Colonne3			"Valeur"

#define	Char_Separateur_Y		".."

#define OFFSET_LIGNE_SEPARATRICE	20

#define NULL_STRING			"NULL"


/********************************************************************
 * Private Macros
 */
#define __Offset_X_Colonne1			(	(8 * TAILLE_TABULATION)					)
#define __Offset_X_Colonne2			(	__Offset_X_Colonne1 	+ (4 * TAILLE_TABULATION)	)
#define __Offset_X_Colonne3			(	__Offset_X_Colonne2 	+ (4 * TAILLE_TABULATION)	)

#define __Taile_Ligne_Separatrice 		(	(NB_COLONNES		* NB_CHAR_NOM)	/	2	)

#define	__SetNullString(str)			( 	__strncpy (str, NULL_STRING	) 	)

#define __IsDataDisplayOn(id)			(	DonneAffiche[id].toBeDisplayed == TRUE			)

#define __IsDataChanged(id, buf)		(	__strncmp( DonneAffiche[id].Preivous_Value, buf ) != 0	)
#define __CopyNewData(id, buf)			__strncpy( DonneAffiche[b_Affichage].Preivous_Value, (char *)buf )
#define __GetDeltaStringLength(id, buf)		__stnlen( buf)  - __stnlen( DonneAffiche[id].Preivous_Value )
#define __IsNewDataShorter(diff)		( 	diff < 0			)

#define __TrBClMoy_SetValue_ms(str, val)	sprintf((char *)str , "%d ms", (int)(val/NB_TOURBOUCLE_ECHANTILLON))
#define __TrBClMoy_SetValue_us(str, val)	sprintf((char *)str , "%d us", (int)((val*1000)/NB_TOURBOUCLE_ECHANTILLON))

#define __GetValue_string(id, type, str)	DonneAffiche[id].toString((void *)type, DonneAffiche[id].ID, str)


/********************************************************************
 * Private Types
 */
typedef enum {

	EtatTimestamp = 0,
	TrBclMoy_ms,

	EtatBoutonDiscovery_WKP,

	EtatPWM_SERVO_DIRECTION,
	EtatPWM_SCLK_CAMERA,

	EtatPotentiometre_1,

	PositionDirection,

	nb_Donnes,

}ListeDonneAffiche_e;

typedef enum {

	Ligne_Titre_Tableau = 3,
	Ligne_Separateur,
	Premiere_Ligne_Tableau,

}Offset_Y_Lignes;

typedef enum {

	Colonne_Nom = 0,
	Colonne_Pin,
	Colonne_Valeur,

}Nom_Colonnes_e;

typedef struct {

	uint8_t		Nom		[NB_CHAR_NOM];
	uint8_t		ID;


	uint8_t		Preivous_Value	[NB_CHAR_VALUE];

	pFunction	toString;

	Bool_e		toBeDisplayed;

}DonneAffiche_s;


/********************************************************************
 * Private Fonctions prototypes
 */

inline uint8_t
Affichage_Intro (
		void
);

inline uint8_t
Affichage_Data (
		void
);

void
Timestamp_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

void
TrBclMoy_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

/********************************************************************
 * Private variables
 */
static 		uint32_t 	DureeTourBoucleMoyen_ms		=	0;
static const	uint32_t	offsetColonne	[NB_COLONNES] 	= 	{

		__Offset_X_Colonne1-10,	__Offset_X_Colonne2,	__Offset_X_Colonne3
};

DonneAffiche_s DonneAffiche[nb_Donnes] = {

/**				Nom				ID			Valeur char	Fonction toString			Is To Be Displayed */
/** Timestamp */	{	"Timestamp",			PIN_NULL,		"",		(pFunction)Timestamp_toString,		TRUE		},
/** Tr de boucle moyen*/{	"Temps de Boucle Moyen",	PIN_NULL,		"",		(pFunction)TrBclMoy_toString,		TRUE		},

/** Btn WKP */		{	"Bouton Wake Up (Discovery)",	Button_WKP,		"",		(pFunction)Button_Value_toString,	TRUE		},

/** PWM Servo */	{	"PWM MICRO SERVO",		PIN_PWM_MICRO_SERVO,	"",		(pFunction)PWM_Value_toString,		TRUE		},
/** PWM SCLK */		{	"PWM ULTRA SON TRIG CLK",	Pin_UltraSon_Trig,	"",		(pFunction)PWM_Value_toString,		TRUE		},

/** Echo ultra son */	{	"Echo Ultra Son",		Pin_UltraSon_Echo, 	"",		(pFunction)UltraSon_toString,		TRUE 		},

/** Pos Direction */	{	"Position Servo Direction",	PIN_NULL,		"",		(pFunction)MicroServo_toString,		TRUE 		},
};





/********************************************************************
 * Exported Fonctions
 */

/**------------------------------------------------------------------
 *
 * @brief	Gestion de l'affichage Debug
 *
 */
void
Affichage_Main
(
		void
) {

	static Etape_Affichage_e Etape = Affichage_Initialisation;

	switch(Etape) {

		case Affichage_Initialisation:		Etape = Affichage_Intro();	break;
		case Affichage_DataPinAndValue:		Etape = Affichage_Data();	break;
		default:								break;
	}
}

/**------------------------------------------------------------------
 *
 * @brief	Setter la duree du tour de boucle
 *
 */
void
setDureeTourBoucle(
	void
) {
	static uint16_t	nbTourBoucle = 0;

	nbTourBoucle++;

    	if(nbTourBoucle == NB_TOURBOUCLE_ECHANTILLON) {

    		DureeTourBoucleMoyen_ms = TSW_GetElapsedTime(Timer_Mesure_TourBoucle);

    		TSW_Reset(Timer_Mesure_TourBoucle);
    		TSW_Start(Timer_Mesure_TourBoucle, 1000);
    		nbTourBoucle = 0;
    	}
}


/********************************************************************
 * Private Functions definition
 */
/**-------------------------------------------------------------------
 *
 * @brief	Affichage tableau nom et pin
 *
 */
inline uint8_t
Affichage_Intro(
		void
) {

	uint32_t 	b_Affichage = 0;
	uint8_t		TitreColonne	[NB_COLONNES]	 [NB_CHAR_NOM] = {	Nom_Colonne1,	Nom_Colonne2,	Nom_Colonne3 };
	uint8_t*	Buffer		[NB_CHAR_PERIPH];

	//if(Console_GetPresence() == FALSE)		return Affichage_Initialisation;

	//------------------------ Affichage Ligne Titre Tableau
	for(b_Affichage=0; b_Affichage<NB_COLONNES; b_Affichage++) {

		__VT100CMD_MOVECURSOR_TOPOSITION(Ligne_Titre_Tableau, offsetColonne[b_Affichage]);
		__Console_Send((char *) TitreColonne[b_Affichage]);
	}

	//------------------------ Affichage ligne separatrice
	__VT100CMD_MOVECURSOR_TOPOSITION(Ligne_Separateur, OFFSET_LIGNE_SEPARATRICE);

	for(b_Affichage=0; b_Affichage<__Taile_Ligne_Separatrice; b_Affichage++) {
		__Console_Send(Char_Separateur_Y);
	}

	//------------------------ Affichage nom et pin colonnes
	for(b_Affichage=0; b_Affichage<nb_Donnes; b_Affichage++) {

		if __IsDataDisplayOn(b_Affichage) {

			memset(Buffer, 0, NB_CHAR_PERIPH);

			//--------- Placement curseur et Affichage Nom de la ligne
			__VT100CMD_MOVECURSOR_TOPOSITION(Premiere_Ligne_Tableau+b_Affichage, offsetColonne[Colonne_Nom]);
			__Console_Send((char *)(DonneAffiche[b_Affichage].Nom));

			//--------- Placement curseur et Affichage Pin utilise
			__VT100CMD_MOVECURSOR_TOPOSITION(Premiere_Ligne_Tableau+b_Affichage, offsetColonne[Colonne_Pin]);
			__GetValue_string(b_Affichage, toString_Getpin, Buffer);
			__Console_Send((char *)Buffer);
		}
	}

	return Affichage_DataPinAndValue;
}

/**-------------------------------------------------------------------
 *
 * @brief	Affichage tableau valeurs quand différentes de la précédente
 *
 */
inline uint8_t
Affichage_Data
(
		void
) {

	uint8_t		b_Effacer = 0;
	uint8_t		New_Data	[NB_CHAR_VALUE];
	int32_t		Difference;
	uint32_t 	b_Affichage = 0;

	//if(Console_GetPresence() == FALSE)		return Affichage_Initialisation;

	for(b_Affichage=0; b_Affichage<nb_Donnes; b_Affichage++) {

		if __IsDataDisplayOn(b_Affichage) {

			//---------- RàZ Tableau et recuperation de la nouvelle donne sous forme de chaine
			memset(New_Data, 0, NB_CHAR_VALUE);
			__GetValue_string(b_Affichage, toString_GetValue, New_Data);

			//---------- Test si la donne a ete change
			if __IsDataChanged(b_Affichage, New_Data) {

				//-------------- Copie de la nouvelle donnee pour la prochaine comparaison
				__CopyNewData(b_Affichage, New_Data);

				//-------------- Calcul de la difference en caracter (afin que ca recouvre totalement l'ancienne chaine)
				Difference = __GetDeltaStringLength(b_Affichage, New_Data);

				if __IsNewDataShorter(Difference) {

					for(b_Effacer = 0; b_Effacer < -Difference; b_Effacer++)	{

						if(strlen((char *)New_Data) != NB_CHAR_VALUE)
							strncat((char *)New_Data, " ", 1);
						else
							b_Effacer = -Difference;
					}
				}

				//-------------- Placement curseur et Affichage
				__VT100CMD_MOVECURSOR_TOPOSITION(Premiere_Ligne_Tableau+b_Affichage, offsetColonne[Colonne_Valeur]);
				__Console_Send((char *)New_Data);
			}
		}
	}

	return Affichage_DataPinAndValue;
}


void
Timestamp_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
) {
	uint32_t Timestamp_ms;
	uint32_t secondes;
	uint32_t minutes;

	switch(Field) {

		//----------------------------------------------------------
		case toString_Getpin:	//__SetNullString(pString);
					break;

		//----------------------------------------------------------
		case toString_GetValue: Timestamp_ms = TSW_GetTimestamp();
					if( (Timestamp_ms / 1000) > 0 ) {
						secondes = Timestamp_ms / 1000;
						if(secondes >= 60) {
							minutes = secondes / 60;
							secondes %= 60;
							sprintf((char *) pString, "%d min %d s", (int)minutes, (int)secondes);
						} else {
							sprintf((char *) pString, "0 min %d s", (int)secondes);
						}

					} break;

		//----------------------------------------------------------
		default:
					break;
	}
}

void
TrBclMoy_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
) {
	switch(Field) {

		case toString_Getpin:	break;

		case toString_GetValue: if( (DureeTourBoucleMoyen_ms/NB_TOURBOUCLE_ECHANTILLON) > 1) {
						__TrBClMoy_SetValue_ms(pString, DureeTourBoucleMoyen_ms);

					} else {
						__TrBClMoy_SetValue_us(pString, DureeTourBoucleMoyen_ms);

					} break;

		default:		break;
	}
}


