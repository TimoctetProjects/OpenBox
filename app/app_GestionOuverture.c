/*********************************************************************
 * *******************************************************************
 *
 * @file	app_GestionOuverture.c
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Applicatif de gestion d'ouverture de la boite
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "app_GestionOuverture.h"

/********************************************************************
 * Private defines
 */

/********************************************************************
 * Private macros
 */


/********************************************************************
 * Private Fonctions
 */

/********************************************************************
 * Private Types
 */
typedef enum {

	GestionOuverture_Etape_init = 0,
	GestionOuverture_Etape_main,

}Liste_Etapes_GestionOuverture_e;


/********************************************************************
 * Private variables
 */


/********************************************************************
* Private Fonctions prtotypes
*/

/********************************************************************
 * Exported Fonctions
 */
/**-------------------------------------------------------------------
 *
 * @brief	Main de la gestion d'ouverture de la boite
 *
 */
void
GestionOuverture_main(
	void
) {
	static Liste_Etapes_GestionOuverture_e Etape = GestionOuverture_Etape_init;
	static Bool_e Start_Done = FALSE;

	switch(Etape) {

		case GestionOuverture_Etape_init:
			Etape = GestionOuverture_Etape_main;
			break;


		case GestionOuverture_Etape_main:
			if(UltraSon_getValue_ms() < 2000) {

				if(!Start_Done) {
					TSW_Start(TSW_KAMEHAMEHAAAA, 5000);
					Start_Done = TRUE;
				}




				if(TSW_GetStatus(TSW_KAMEHAMEHAAAA) == STATUS_FINIS) {
					MicroServo_SetNewPosition(MicroServo_Position_zero);
				} else	MicroServo_SetNewPosition(MicroServo_Position_moins45);



			} else {
				TSW_Reset(TSW_KAMEHAMEHAAAA);
				Start_Done = FALSE;
				MicroServo_SetNewPosition(MicroServo_Position_moins90);

			}
			break;
	}
}
