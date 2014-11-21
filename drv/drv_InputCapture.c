/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_InputCapture.c
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver InputCapture
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "drv_InputCapture.h"


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
 * Private Variables
 */
uint32_t CaptureValue;

/********************************************************************
 * Private Function prototypes
 */
void
InputCapture_Activer(
	Mapping_GPIO_e IdPinIC
);

void
InputCapture_Desactiver(
	Mapping_GPIO_e IdPinIC
);

void
InputCapture_IT_init(
	uint32_t Periph,
	uint32_t Channel
) ;

/********************************************************************
 * Exported Fonctions Definition
 */
typedef enum {

	IC_OK = 0,
	IC_Err_Unavailable,

}IC_Status_e;

/**------------------------------------------------------------------
 *
 * @brief
 *
 */
uint8_t
InputCapture_init(
	Mapping_GPIO_e	IdPinIC,	/**<[in] ID de la Pin ou générer la PWM*/
	uint32_t 	Periode_us	/**<[in] Periode de comptage */
) {


	uint32_t 		TIM_Prescaler 	= 1;	/** Prescaler du TimeBase */
	uint32_t 		TIM_Periode 	= 1;	/** Periode de comptage du TimeBase */

	TIM_ICInitTypeDef 	TIM_ICInitStruct = {
			.TIM_Channel 		= TIM_Channel_3,
			.TIM_ICPolarity 	= TIM_ICPolarity_BothEdge,
			.TIM_ICSelection	= TIM_ICSelection_DirectTI,
			.TIM_ICPrescaler	= TIM_ICPSC_DIV1,
			.TIM_ICFilter		= 0x0
	};

	if (IdPinIC >= nb_GPIO)
		return IC_Err_Unavailable;

	//TIM_ICStructInit(&TIM_ICInitStruct);

	TIM_Prescaler = (((SystemCoreClock / 1000000) / 2) - 1);
	TIM_Periode = (Periode_us * 2) - 1;

	TimeBase_init	(IdPinIC,	TIM_Periode, 	TIM_Prescaler	);
	TIM_ICInit(	(TIM_TypeDef*) Mapping_GPIO[IdPinIC].Periph,
			&TIM_ICInitStruct				);



	if( Mapping_GPIO[IdPinIC].Etat_Interruption == Interrupt_ON ) {

		InputCapture_IT_init(	Mapping_GPIO[IdPinIC].Periph,
					Mapping_GPIO[IdPinIC].Parametre	);
	}

	TIM_Cmd( (TIM_TypeDef*) Mapping_GPIO[IdPinIC].Periph, ENABLE );



	return IC_OK;
}

void
TIM3_IRQHandler(
	void
) {
	static 	uint8_t 	nb_IT	= 0;
	static	uint32_t	FirstVal, LastVal;

	if(nb_IT == 1) {

		LastVal  = TIM_GetCapture3(TIM3);
		TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);

		if(FirstVal > LastVal)		CaptureValue = FirstVal - LastVal;
		else				CaptureValue = LastVal  - FirstVal;
		nb_IT = 0;

	}else {
		nb_IT++;
		FirstVal = TIM_GetCapture3(TIM3);
	}
}

/********************************************************************
 * Private Fonctions Definition
 */
uint32_t
InputCapture_GetValue(
	Mapping_GPIO_e IdPinIC
) {
	return CaptureValue;
}

void
InputCapture_Desactiver(
	Mapping_GPIO_e IdPinIC
) {

}

void
InputCapture_IT_init(
		uint32_t Periph,
		uint32_t Channel
) {

	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel 			= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	switch(Channel) {

		case (uint32_t) TIM_Channel_1:		TIM_ITConfig((TIM_TypeDef* )Periph, TIM_IT_CC1, DISABLE);
		case (uint32_t) TIM_Channel_2:		TIM_ITConfig((TIM_TypeDef* )Periph, TIM_IT_CC2, DISABLE);
		case (uint32_t) TIM_Channel_3:		TIM_ITConfig((TIM_TypeDef* )Periph, TIM_IT_CC3, DISABLE);
		case (uint32_t) TIM_Channel_4:		TIM_ITConfig((TIM_TypeDef* )Periph, TIM_IT_CC4, DISABLE);

		default:				return;
	}
}
