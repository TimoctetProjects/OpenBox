/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_USART.h
 *
 * @author	Duclos Timothe
 *
 * @date	22/07/2014
 *
 * @brief	Gestion de l'USART
 *
 *********************************************************************/

#ifndef DRV_USART_H_
#define DRV_USART_H_

/********************************************************************
 * Includes
 */
#include "bsp/bsp_carte.h"
#include "util_FIFO.h"

/********************************************************************
 * Exported defines
 */

/********************************************************************
 * Exported Macros
 */


/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */
typedef enum {

	UsartOK = 0,

	UsartErr_NonInitialized,
	UsartErr_RxTxDifferentPeriph,
	UsartErr_RxOrTxPinInvalid,
	UsartErr_SendErrorTimeout,
	UsartErr_TryingToInitNonUsartPeriph,
	UsartErr_AlreadyConfigured,

	nb_UsartErrs

}UsartState_e;

typedef enum {

	Periph_USART1 = 0,
	Periph_USART2,
	Periph_USART3,
	Periph_USART6,

	nb_Periph_Usart,

	err_USARTPERIPH_NOTHANDLED,

}Liste_Usart_Periph_e;

typedef enum {

	Usart_Pin_BothRxTx,
	Usart_Pin_Tx,
	Usart_Pin_Rx,

}Liste_Pin_Usart_e;

/********************************************************************
 * Exported Function Prototypes
 */

/**------------------------------------------------------------------
 *
 * @brief	Initialiser l'interface USART6
 *
 * @return	Status de l'initialisation
 *
 */
UsartState_e
Usart_InitPeriph(
		Mapping_GPIO_e	ID_PinTX,	/**<[in] ID de la Pin TX. Mettre PIN_NULL si non utilisee */
		Mapping_GPIO_e	ID_PinRX,	/**<[in] ID de la Pin RX. Mettre PIN_NULL si non utilisee */
		uint32_t Baudrate		/**<[in] Valeur de la vitesse de la liaison (en bauds).*/
);

/**------------------------------------------------------------------
 *
 * @brief	Ecriture sur l'USART
 *
 * @return	Status de l'ecriture
 *
 */
int
USART_Write(
		Mapping_GPIO_e	ID_PinTX,	/**<[in] ID de la Pin TX. Mettre PIN_NULL si non utilisee */
		uint8_t*	pBuffer,	/**<[in] Pointeur vers le buffer ou stocker la donnee a lire. */
		uint16_t 	Taille		/**<[in] Nombre d'octet a ecrire. */
);

/**------------------------------------------------------------------
 *
 * @brief	Ecriture sur l'USART
 *
 * @return	Status de l'ecriture
 *
 */
uint8_t
USART_SetInterruptState(
		Liste_Usart_Periph_e	UsartPeriph,	/**<[in] ID de l'USART ou changer l'etat de l'IRQ */
		Liste_Pin_Usart_e	Tx_or_Rx,	/**<[in] Rx Tx ou les deux */
		FunctionalState		State		/**<[in] Etat de l'IRQ */
);

/**------------------------------------------------------------------
 *
 * @brief	Lecture du buffer de reception.
 *
 * return	Nombre de donnees lues
 *
 */
uint16_t
USART_Read(
		uint32_t 	Periph,		/** <[in] Periph Usart ou effectuer la lecture */
		uint8_t* 	pBuffer,	/** <[out] Pointeur vers le Buffer ou ecrire les donnees */
		uint16_t	NbRead		/** <[in] Nombre de donnees a lire */
);

#endif
