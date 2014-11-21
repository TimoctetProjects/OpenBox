/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_PWM.c
 *
 * @author	Duclos Timothe
 *
 * @date	16/09/2014
 *
 * @brief	Driver générant un signal PWM sur une broche
 * 		pre-configuree
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "drv_TimBase.h"


/********************************************************************
 * Private defines
 */


/********************************************************************
 * Private macros
 */

/********************************************************************
 * Private Types
 */

/********************************************************************
 * Private Function prototypes
 */
static inline void
TimeBase_RccInit(
	uint32_t Periph		/**<[in] Peripheral to initialize */
);

/********************************************************************
 * Exported Fonctions Definition
 */

/**------------------------------------------------------------------
 *
 * @brief	Initialisation du TimeBase
 *
 */
void
TimeBase_init(
		Mapping_GPIO_e	IdPinPwm,		/**<[in] ID de la Pin ou générer la PWM*/
		uint32_t 	TIM_Periode,		/**<[in] Periode de comptage */
		uint32_t 	TIM_Prescaler 		/**<[in] Prescaler du TimeBase */
) {

	//----------------------------------------------------------------------------
	//------------------------ Déclaration et Initialisation ---------------------
	//----------------------------------------------------------------------------
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	/** Structure d'initialisation TimeBase */

	//--------- Initialisation des structures aux valeurs par défaut
	TIM_TimeBaseStructInit	(&TIM_TimeBaseInitStruct);


	//--------- Initialisation de l'horloge
	TimeBase_RccInit(Mapping_GPIO[IdPinPwm].Periph);

	//---------- Remplissage de la structure d'init TimBase
	TIM_TimeBaseInitStruct.TIM_Prescaler		= TIM_Prescaler;
	TIM_TimeBaseInitStruct.TIM_Period		= TIM_Periode;
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;

	//--------- Initialisation de la base de temps
	TIM_TimeBaseInit( (TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph, &TIM_TimeBaseInitStruct );
}


/********************************************************************
 * Private Fonctions Definition
 */
/**------------------------------------------------------------------
 *
 * @brief	Init Clock APB1 ou APB2 en fonction du TIM
 *
 */
static inline void
TimeBase_RccInit(
		uint32_t Periph
) {
	switch(Periph) {

		case (uint32_t) TIM1: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE); 	break;
		case (uint32_t) TIM2: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE); 	break;
		case (uint32_t) TIM3: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE); 	break;
		case (uint32_t) TIM4: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,  ENABLE); 	break;
		case (uint32_t) TIM5: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,  ENABLE); 	break;
		case (uint32_t) TIM6: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,  ENABLE); 	break;
		case (uint32_t) TIM7: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,  ENABLE); 	break;
		case (uint32_t) TIM8: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE); 	break;
		case (uint32_t) TIM9: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9,  ENABLE); 	break;
		case (uint32_t) TIM10: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); 	break;
		case (uint32_t) TIM11: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); 	break;
		case (uint32_t) TIM12: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); 	break;
		case (uint32_t) TIM13: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); 	break;
		case (uint32_t) TIM14: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 	break;
	}
}
