/*********************************************************************
 * *******************************************************************
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Main de l'Open Box
 *
 *********************************************************************
 *********************************************************************/


/********************************************************************
 * Includes
 */
#include "app_GestionOuverture.h"
#include "app_AffDebug.h"

/********************************************************************
 * Private Define
 */

/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
) {
	ServoDirection_ListePosition_e	pos = MicroServo_Position_moins90;
	Bool_e test = FALSE;

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	TSW_init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);

	pos = MicroServo_Position_moins90;
	MicroServo_SetNewPosition(MicroServo_Position_moins90);

	//------------ Affichage version logicielle
	#ifdef MODE_DEBUG_ON
		TSW_Start(Timer_Mesure_TourBoucle, 1000);
	#endif

	//--------------------------------------------------------------
	//------------ Boucle principale
	while(1)
	{
		//--------- Gestion du Tick
		TSW_HANDLE();

		//--------- Gestion affichage debug
		#ifdef MODE_DEBUG_ON
			Console_main();
			if(TSW_GetStatus(Timer_Affichage_Refresh) == STATUS_FINIS) {
				TSW_Start(Timer_Affichage_Refresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif

		//--------- Lecture des entrees
		if(TSW_GetStatus(TIMER_GPIO_UPDATE) == STATUS_FINIS) {
			BtnPssr_LaunchUpdate();
			TSW_Start(TIMER_GPIO_UPDATE, DUREE_OPERATION_GPIO_ms);
		}


		if(BtnPssr_ReadState(Button_WKP) == Button_State_Pressed) {

			if(test == FALSE) {

				if(pos == MicroServo_Position_moins90) {

					MicroServo_SetNewPosition(MicroServo_Position_moins45);
					pos = MicroServo_Position_moins45;
				}

				else {
					MicroServo_SetNewPosition(MicroServo_Position_moins90);
					pos = MicroServo_Position_moins90;
				}

				test = TRUE;
			}

		} else test = FALSE;

		//-------- ServoDirection
		MicroServo_Main();
		UltraSon_main();
	}
}

