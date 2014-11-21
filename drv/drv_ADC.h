/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_ADC.h
 *
 * @author	Duclos Timothe
 *
 * @date	02/10/2014
 *
 * @brief	Driver ADC
 *
 *********************************************************************/

#ifndef DRV_ADC_H
#define DRV_ADC_H

/********************************************************************
 * Includes
 */
#include "bsp_carte.h"

/********************************************************************
 * Exported defines
 */
#define ADC_NB_ChannelConf_MAX	18

/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */

typedef enum {

	ADC_Ready = 0,
	ADC_ConvEnCours,

	ADC_ChannelUnconfigured = 10,

}ADC_ConvStatus_e;

typedef struct {

	Mapping_GPIO_e 	pinID;

	uint8_t		ConversionContinue;	// Conversion Continue du canal
	uint32_t	LastValue_mV;		// Valeur de la derniere conversion
	uint32_t*	pStoreValue_mV;		// Pointeur ou stocker la donnee convertie
	pFunction	pFct_CallbackEOC;	// Pointeur vers la fonction callback fin de conversion du Channel

} ADC_ChannelConf_t;

/********************************************************************
 * Exported Function
 */
ADC_ConvStatus_e
ADC_ConversionContinue_Activer(
		Mapping_GPIO_e IDPin,		/**<[in] ID de la Pin*/
		uint8_t ID_Channel		/**<[in] ID de la conf du channel dans le tableau*/
);

uint8_t
ADC_ChannelConfigure(
	ADC_ChannelConf_t 	ADC_Channel 	/**<[in] Structure de configuration d'un  ADC. */
);

ADC_ConvStatus_e
ADC_ConversionContinue_Desactiver(
		Mapping_GPIO_e IDPin,		/**<[in] ID de la Pin*/
		uint8_t ID_Channel		/**<[in] ID de la conf du channel dans le tableau*/
);

ADC_ConvStatus_e
ADC_GetConversionStatus(
		uint32_t	periph		/**<[in] Periph ADC. */
);

void
ADC_StartConversion(
		uint32_t	periph,		/**<[in] Periph ADC. */
		uint8_t 	ID_Channel	/**<[in] Channel ADC. */
);

uint32_t
ADC_Get_mV(
		uint32_t ADCPeriph,
		uint8_t ID_Channel
);

#endif
