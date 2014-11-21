/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_ADC.c
 *
 * @author	Duclos Timothe
 *
 * @date	02/10/2014
 *
 * @brief	Driver ADC
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "drv_ADC.h"

/********************************************************************
 * Private defines
 */
#define ADC1_RANGE_MAX_mV	3300
#define ADC1_RANGE_MAX_BITS	4096

/********************************************************************
 * Private macros
 */
#define __ADC_IsIT_EndOfConversion(periph)		( ADC_GetITStatus(periph, ADC_IT_EOC) == SET		)
#define __ADC_Increment_NbChannelConf(periph)		( ADC_NbChannelConf[ADC_GetADCPeriph(periph)]++ 	)
#define __ADC_IsPeriphUninitialised(Periph)		( IsADCInitialized[ADC_GetADCPeriph(Periph)] == FALSE 	)
#define __ADC_SetInitialisedTRUE(Periph)		( IsADCInitialized[ADC_GetADCPeriph(Periph)] =  TRUE  	)

#define __ADC_IsChannelConfigured(id, periph)		( (id >= ADC_NbChannelConf[ADC_GetADCPeriph(periph)]) )

#define __ADC_IsNewChannelAskedForPossible(periph)	(ADC_NbChannelConf[ADC_GetADCPeriph(periph)] >= ADC_NB_ChannelConf_MAX)
#define __ADC_getLastID_Available(id, periph)		( id = ADC_NbChannelConf[ADC_GetADCPeriph(periph)] )
#define __ADC_IsNbChannelMaxReached(periph)		(ADC_NbChannelConf[ADC_GetADCPeriph(periph)] < ADC_NB_ChannelConf_MAX)

#define __ADC_SaveNewValue(periph, val)			( ADC_ChannelConf[ADC_GetADCPeriph(periph)][CurrentID[ADC_GetADCPeriph(periph)]].LastValue_mV =  val)

#define __ADC_SetConversionContinueState(id, periph, state)	( ADC_ChannelConf[ADC_GetADCPeriph(periph)][id].ConversionContinue = state )
#define __ADC_CopieConfig(Conf, periph, ID)			( (Conf) = (ADC_ChannelConf_t*) &(ADC_ChannelConf[ADC_GetADCPeriph(periph)][ID]) )

#define	__ADC_IsAlreadyConfigured(periph, Channel, ID)	( 		( Mapping_GPIO[ADC_ChannelConf[ADC_GetADCPeriph(periph)][ID].pinID].Periph    == periph)     \
								&&	( Mapping_GPIO[ADC_ChannelConf[ADC_GetADCPeriph(periph)][ID].pinID].Parametre == Channel )	)

#define __ADCIRQ_CanValueBeStoredAway(periph)		( ADC_ChannelConf[periph][CurrentID[periph]].pStoreValue_mV 	!= 0 )
#define __ADCIRQ_IsEOC_CallbackFuncConfigured(periph)	( ADC_ChannelConf[periph][CurrentID[periph]].pFct_CallbackEOC 	!= 0 )

#define	__ADCIRQ_StoreValueAway(periph)			( *(ADC_ChannelConf[periph][CurrentID[periph]].pStoreValue_mV) = ADC_ChannelConf[periph][CurrentID[periph]].LastValue_mV )
#define	__ADCIRQ_CallEOC_CallbackFunc(periph)		( ADC_ChannelConf[periph][CurrentID[periph]].pFct_CallbackEOC((void *)ADC_ChannelConf[periph][CurrentID[periph]].LastValue_mV) )


/********************************************************************
 * Private Types
 */

typedef enum {

	Periph_ADC1 = 0,
	Periph_ADC2,
	Periph_ADC3,

	nb_ADCPeriph,

	err_NotADCPeriph

}Liste_PeriphADC_e;

typedef enum {

	ADC_OK = 0,

	nb_Etats_ADC,

}Etat_ADC_e;

/********************************************************************
 * Private variables
 */
static 		Bool_e			IsADCInitialized	[nb_ADCPeriph]			= {0}	;
static volatile ADC_ConvStatus_e 	ConvStatus		[nb_ADCPeriph]			= {0}	;
static volatile uint8_t 		CurrentID		[nb_ADCPeriph]			= {0}	;
static volatile uint8_t 		SequenceID		[nb_ADCPeriph]			= {0}	;
static volatile uint8_t 		SequenceEnCours		[nb_ADCPeriph]			= {0}	;

static volatile ADC_ChannelConf_t 	ADC_ChannelConf		[nb_ADCPeriph][ADC_NB_ChannelConf_MAX]	= {0};
static 		uint8_t 		ADC_NbChannelConf	[nb_ADCPeriph]				= {0};

/********************************************************************
 * Private Fonctions prototypes
 */

inline Etat_ADC_e
ADC_init(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
);

inline void
ADC_initRCC(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
);

inline Liste_PeriphADC_e
ADC_GetADCPeriph(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
);

inline uint32_t
prvADCvalue_to_mV(
	uint32_t ADCvalue	/**<[in] ulADCvalue	Valeur de l'ADC. */
);

inline int8_t
prvADC_GetNextActif(
	uint32_t periph		/**<[in] Periph ADC*/
);

/********************************************************************
 * Exported Fonctions
 */
/**-----------------------------------------------------------------------------
 * @brief	Tick ADC.
 *
 */
void ADC1_Tick() {

	// Verification pas de sequence en cours
	if (SequenceEnCours[Periph_ADC1])
		return;

	// Verification ADC dispo
	if (ConvStatus[Periph_ADC1] != ADC_Ready) return;

	// Lancement d'une nouvelle sequence
	SequenceID[Periph_ADC1] = 0;
	if (prvADC_GetNextActif((uint32_t)ADC1) >= 0)
	{
		ADC_StartConversion((uint32_t)ADC1, SequenceID[Periph_ADC1]);
		SequenceEnCours[Periph_ADC1] = TRUE;
	}
}

/**-------------------------------------------------------------------
 *
 * @brief	Configuration d'un canal d'un ADC
 *
 */
uint8_t
ADC_ChannelConfigure(
	ADC_ChannelConf_t 	ADC_Channel 	/**<[in] Structure de configuration d'un  ADC. */
) {

	//-----------------------------------------------------------
	//--------------- Declaration des variables
	ADC_ChannelConf_t* 	pConf 		= 0;
	uint8_t 		ID_ADC_Channel 	= 0;
	uint8_t 		Exist=FALSE;

	//---------------- Init ADC
	if __ADC_IsPeriphUninitialised(Mapping_GPIO[ADC_Channel.pinID].Periph) {
		ADC_init(Mapping_GPIO[ADC_Channel.pinID].Periph);
		__ADC_SetInitialisedTRUE(Mapping_GPIO[ADC_Channel.pinID].Periph);
	}

	//---------------- Verification des parametres d'entree
	if __ADC_IsNewChannelAskedForPossible(Mapping_GPIO[ADC_Channel.pinID].Periph)
		return ADC_NB_ChannelConf_MAX;

	// On verifie que le canal n'est pas deja configure
	while ( (ID_ADC_Channel < ADC_NbChannelConf[ADC_GetADCPeriph(Mapping_GPIO[ADC_Channel.pinID].Periph)]) && (Exist == FALSE) ) {

		if __ADC_IsAlreadyConfigured(	Mapping_GPIO[ADC_Channel.pinID].Periph,
					 	Mapping_GPIO[ADC_Channel.pinID].Parametre,
					 	ID_ADC_Channel					) {

			Exist = TRUE;

		} else {

			ID_ADC_Channel++;
		}
	}

	// Creation d'une nouvelle config si besoin
	if (Exist == FALSE) {

		__ADC_getLastID_Available(ID_ADC_Channel, Mapping_GPIO[ADC_Channel.pinID].Periph);

		// Incrementation du pointeur
		if __ADC_IsNbChannelMaxReached(Mapping_GPIO[ADC_Channel.pinID].Periph)  {
			__ADC_Increment_NbChannelConf( Mapping_GPIO[ADC_Channel.pinID].Periph );
		}

		// Copie de la configuration
		__ADC_CopieConfig(pConf, Mapping_GPIO[ADC_Channel.pinID].Periph, ID_ADC_Channel);
		*pConf = ADC_Channel;
		pConf->LastValue_mV		= 0;
		pConf->ConversionContinue	= FALSE;
	}

	return ID_ADC_Channel;
}

/**-----------------------------------------------------------------------------
 * @brief	Activation de la conversion continue d'un ADC.
 *
 * @return	Etat ADC
 *
 */
ADC_ConvStatus_e
ADC_ConversionContinue_Activer(
		Mapping_GPIO_e IDPin,		/**<[in] ID de la Pin*/
		uint8_t ID_Channel		/**<[in] ID de la conf du channel dans le tableau*/
) {

	if __ADC_IsChannelConfigured(ID_Channel, 	 Mapping_GPIO[IDPin].Periph)		return ADC_ChannelUnconfigured;
	__ADC_SetConversionContinueState(ID_Channel, Mapping_GPIO[IDPin].Periph, TRUE);

	return ADC_Ready;
}

/**-----------------------------------------------------------------------------
 * @brief	Desactivation de la conversion continue d'un ADC.
 *
 * @return	Etat ADC
 *
 */
ADC_ConvStatus_e
ADC_ConversionContinue_Desactiver(
		Mapping_GPIO_e IDPin,		/**<[in] ID de la Pin*/
		uint8_t ID_Channel		/**<[in] ID de la conf du channel dans le tableau*/
) {

	if __ADC_IsChannelConfigured(ID_Channel, Mapping_GPIO[IDPin].Periph)		return ADC_ChannelUnconfigured;
	__ADC_SetConversionContinueState(ID_Channel, Mapping_GPIO[IDPin].Periph, FALSE);

	return ADC_Ready;
}


/**-----------------------------------------------------------------------------
 * @brief	Interruption ADC
 *
 */
void
ADC_IRQHandler(
	void
) {
	uint32_t Value_mV;
	uint32_t ITPeriph;

	//----------------- Recuperation peripherique ayant provoque interruption
	     if __ADC_IsIT_EndOfConversion(ADC1) 	ITPeriph = (uint32_t)ADC1;
	else if __ADC_IsIT_EndOfConversion(ADC2) 	ITPeriph = (uint32_t)ADC2;
	else if __ADC_IsIT_EndOfConversion(ADC3)	ITPeriph = (uint32_t)ADC3;
	else						return;

	Value_mV = prvADCvalue_to_mV( 	ADC_GetConversionValue( (ADC_TypeDef* )ITPeriph ) 	);
	__ADC_SaveNewValue(ITPeriph, Value_mV);

	//----------------- Stockage distant et appel fonction callback
	if __ADCIRQ_CanValueBeStoredAway( ADC_GetADCPeriph(ITPeriph) ) 		__ADCIRQ_StoreValueAway(ADC_GetADCPeriph(ITPeriph));
	if __ADCIRQ_IsEOC_CallbackFuncConfigured( ADC_GetADCPeriph(ITPeriph) ) 	__ADCIRQ_CallEOC_CallbackFunc( ADC_GetADCPeriph(ITPeriph) );

	ConvStatus[ADC_GetADCPeriph(ITPeriph)] = ADC_Ready;

	if (SequenceEnCours[ADC_GetADCPeriph(ITPeriph)]) {

		//----------------- Lancement conversion suivante
		SequenceID[ADC_GetADCPeriph(ITPeriph)]++;

		if (prvADC_GetNextActif(ITPeriph) >= 0) {
			ADC_StartConversion(ITPeriph, SequenceID[ADC_GetADCPeriph(ITPeriph)]);

		} else {
			SequenceEnCours[ADC_GetADCPeriph(ITPeriph)] = FALSE;
		}
	}
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture d'une valeur de conversion de l'ADC.
 *
 * @return	Value mv
 *
 */
uint32_t
ADC_Get_mV(
		uint32_t ADCPeriph,
		uint8_t ID_Channel
) {

	return ADC_ChannelConf[ADC_GetADCPeriph(ADCPeriph)][ID_Channel].LastValue_mV;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture du Status de l'ADC.
 *
 * @return	Status de la conversion.
 */
ADC_ConvStatus_e
ADC_GetConversionStatus(
		uint32_t	periph		/**<[in] Periph ADC. */
) {
	return ConvStatus[ADC_GetADCPeriph(periph)];
}

/**-----------------------------------------------------------------------------
 *
 * @brief	Lancement de la sequence de conversion sur l'ADC.
 *
 */
void
ADC_StartConversion(
		uint32_t	periph,		/**<[in] Periph ADC. */
		uint8_t 	ID_Channel	/**<[in] Channel ADC. */
) {
	uint32_t timeout = 0xFFFF;

	// Atente ADC dispo
	while ((ADC_GetConversionStatus(periph) != ADC_Ready) && (timeout--)) 	asm("nop");
	if (!timeout)								return;

	ADC_RegularChannelConfig(
			(ADC_TypeDef *)periph,
			Mapping_GPIO[ADC_ChannelConf[ADC_GetADCPeriph(periph)][ID_Channel].pinID].Parametre,
			1,
			ADC_SampleTime_3Cycles
	);

	ConvStatus[ADC_GetADCPeriph(periph)] = ADC_ConvEnCours;

	CurrentID[ADC_GetADCPeriph(periph)] = ID_Channel;

	ADC_SoftwareStartConv( (ADC_TypeDef *)periph );
}

/********************************************************************
 * Private Fonctions definition
 */

/**-------------------------------------------------------------------
 *
 * @brief	Initialsiation de l'ADC
 *
 */
inline Etat_ADC_e
ADC_init(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
) {
	int i;

	//------------------------------------------------------
	//--------- Declaration des variables
	ADC_CommonInitTypeDef 	ADC_CommonInitStruct;
	ADC_InitTypeDef 	xADC_Init;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	//-------- Init structure
	ADC_CommonStructInit	(&ADC_CommonInitStruct);
	ADC_StructInit		(&xADC_Init);

	//-------- Remplissage structure
	ADC_CommonInitStruct.ADC_Mode			= ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler		= ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_DMAAccessMode		= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;

	xADC_Init.ADC_Resolution		= ADC_Resolution_12b;
	xADC_Init.ADC_ScanConvMode		= DISABLE;
	xADC_Init.ADC_ContinuousConvMode	= DISABLE;
	xADC_Init.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	xADC_Init.ADC_ExternalTrigConv		= 0;
	xADC_Init.ADC_DataAlign			= ADC_DataAlign_Right;
	xADC_Init.ADC_NbrOfConversion		= 1;

	//---------- Init commune a tous les ADC
	ADC_CommonInit(&ADC_CommonInitStruct);

	//---------- Activation horloges
	ADC_initRCC(PeriphADC);

	//--------- Init ADC
	ADC_Init( (ADC_TypeDef *)PeriphADC, &xADC_Init );

	//--------- Config interruption
	ADC_ITConfig( (ADC_TypeDef *)PeriphADC, ADC_IT_EOC, ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	//------------ Initialisation des channels
	for (i=0; i<ADC_NB_ChannelConf_MAX; i++) {

		ADC_ChannelConf[ADC_GetADCPeriph(PeriphADC)][i].ConversionContinue	= FALSE;
		ADC_ChannelConf[ADC_GetADCPeriph(PeriphADC)][i].LastValue_mV		= 0;
		ADC_ChannelConf[ADC_GetADCPeriph(PeriphADC)][i].pStoreValue_mV		= 0;
		ADC_ChannelConf[ADC_GetADCPeriph(PeriphADC)][i].pFct_CallbackEOC	= 0;
	}


	ADC_NbChannelConf	[ADC_GetADCPeriph(PeriphADC)] 		= 0;
	ConvStatus		[ADC_GetADCPeriph(PeriphADC)]		= ADC_Ready;
	CurrentID 		[ADC_GetADCPeriph(PeriphADC)]		= 0;
	SequenceID		[ADC_GetADCPeriph(PeriphADC)] 		= 0;
	SequenceEnCours		[ADC_GetADCPeriph(PeriphADC)] 		= FALSE;

	//---------------- Activation peripherique
	ADC_Cmd( (ADC_TypeDef *)PeriphADC, ENABLE );

	return ADC_OK;
}

/**-----------------------------------------------------------------------------
 * @brief	Conversion valeur d'ADC en mV.
 *
 * @return	Valeur en mV.
 *
 */
inline uint32_t
prvADCvalue_to_mV(
	uint32_t ADCvalue	/**<[in] ulADCvalue	Valeur de l'ADC. */
) {

	uint32_t Adc_mV;

	Adc_mV = ADCvalue;
	Adc_mV *= ADC1_RANGE_MAX_mV;
	Adc_mV /= (ADC1_RANGE_MAX_BITS - 1);

	return Adc_mV;
}

/**-----------------------------------------------------------------------------
 *
 * @brief	Lancement de la conversion sur le  suivant.
 *
 * @return	Numero de l'ID attrubue au  configure ((-1) si aucun).
 *
 */
inline int8_t
prvADC_GetNextActif(
		uint32_t periph		/**<[in] Periph ADC. */
) {

	uint8_t Found=FALSE;

	// Recherche du  suivant a lancer
	while ((SequenceID[ADC_GetADCPeriph(periph)] < ADC_NbChannelConf[ADC_GetADCPeriph(periph)]) && (Found == FALSE)) {

		if (ADC_ChannelConf[ADC_GetADCPeriph(periph)][SequenceID[ADC_GetADCPeriph(periph)]].ConversionContinue == ETAT_ACTIF)
			Found = TRUE;
		else
			SequenceID[ADC_GetADCPeriph(periph)]++;
	}

	// Lancement de la conversion si  trouve
	if (Found)
		return Found;

	return (-1);
}


/**-------------------------------------------------------------------
 *
 * @brief	Initialsiation des horloges des ADC
 *
 */
inline void
ADC_initRCC(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
) {
		if(PeriphADC == (uint32_t) ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	else  	if(PeriphADC == (uint32_t) ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	else  	if(PeriphADC == (uint32_t) ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
}

/**-------------------------------------------------------------------
 *
 * @brief	Lecture du periph
 *
 */
inline Liste_PeriphADC_e
ADC_GetADCPeriph(
	uint32_t PeriphADC	/**<[in] Periph ADC*/
) {
	     if(PeriphADC == (uint32_t) ADC1)	return Periph_ADC1;
	else if(PeriphADC == (uint32_t) ADC2)	return Periph_ADC2;
	else if(PeriphADC == (uint32_t) ADC3) 	return Periph_ADC3;

	return err_NotADCPeriph;
}
