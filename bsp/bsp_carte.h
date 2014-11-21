/*********************************************************************
 * *******************************************************************
 *
 * @file	bsp_carte.h
 *
 * @author	Duclos Timothe
 *
 * @date	19/07/2014
 *
 * @brief	Config des pins
 *
 *********************************************************************/

#ifndef BSP_CARTE_H
#define BSP_CARTE_H

/********************************************************************
 * Includes
 */
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx.h"
#include "misc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"


/********************************************************************
 * Exported defines
 */
#ifdef NULL
	#undef NULL
#endif
#ifndef NULL
	#define NULL 	0
#endif

#ifndef WEAK
	#define WEAK __attribute__ ((weak))
#endif


#define __NOP				asm("nop")

#define __strncmp(str1, str2)		( 	strncmp( (char *)str1, (char *)str2, strlen( (char *)str2) ) 	)
#define __strncpy(str1, str2)		( 	strncpy( (char *)str1, (char *)str2, strlen( (char *)str2) ) 	)
#define __stnlen(str1)			( 	strlen(  (char *)str1 ) 					)
#define __strncat(str1, str2)		( 	strncat((char *)str1, 	 (char *)str2, 	strlen((char *)str2)) 	)


/********************************************************************
 * Exported types
 */

/** Etats */
typedef enum {

	ETAT_INACTIF = 0,
	ETAT_ACTIF,

}Etat_e;



/** Status */
typedef enum {

	STATUS_KO = 0,
	STATUS_OK,
	STATUS_ENCOURS,
	STATUS_FINIS,

}Status_e;

/** Enum utile a affichage debug */
typedef enum {

	toString_Getpin = 0,
	toString_GetValue,

}toString_Possibilities_e;


/** Booleen */
typedef enum {

	FALSE = 0,
	TRUE,

}Bool_e;

/** Status interruption */
typedef enum  {
	Interrupt_OFF = 0,
	Interrupt_ON
}InterruptState_e;

/** Pointeur de fonction generique */
typedef void (*pFunction)(void*, ...);

/** Enumeration des Pins */
typedef enum {

	/* Pin NULL */
	PIN_NULL = 0,

	/* Console */
	CONSOLE_TX,
	CONSOLE_RX,

	/* Leds Discovery Board */
	PIN_PWM_MICRO_SERVO,

	/* Bouton Poussoir */
	BTN_WKP,

	/* UltraSon */
	Pin_UltraSon_Trig,
	Pin_UltraSon_Echo,

	nb_GPIO,

}Mapping_GPIO_e;


/** Structure pour l'init du Mapping */
typedef struct {

	/** Config Pin */
	GPIO_TypeDef*		GpioPeripheral;
	uint16_t		GpioPin;
	GPIOMode_TypeDef	GpioMode;
	Bool_e			Inverse;
	GPIOSpeed_TypeDef	GPIO_Speed;
	GPIOOType_TypeDef	GPIO_OType;
	GPIOPuPd_TypeDef	GPIO_PuPd;

	uint32_t		AlternateFunction;
	uint32_t		Parametre;
	InterruptState_e	Etat_Interruption;
	Etat_e			EtatInit;
	uint32_t		Periph;

}Mapping_GPIO_s;


/********************************************************************
 * Exported variables
 */

/** Mapping GPIO */
const static Mapping_GPIO_s Mapping_GPIO[nb_GPIO] = {

/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/**						Peripheral,	Pin			Mode: In/Out		Inverse		Spped				PP/OD				Pull-up/Pull-down		Alternate Function		Param			Etat Interrupt		Etat initial			Peripherique		*/
/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** PIN_NULL			*/	{ 	NULL,		NULL,			NULL,			NULL,		NULL,				NULL,				NULL,				NULL,				NULL,			NULL,			NULL,				NULL,			},
/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** CONSOLE_TX			*/	{ 	GPIOC,		GPIO_Pin_6,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_USART6,			NULL,			Interrupt_OFF,		NULL,				(uint32_t) USART6	},
/** CONSOLE_RX  		*/	{ 	GPIOC,		GPIO_Pin_7,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_USART6,			NULL,			Interrupt_OFF,		NULL,				(uint32_t) USART6	},
/** ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** PIN_PWM_MICRO_SERVO  	*/	{ 	GPIOD,		GPIO_Pin_15,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_TIM4,			TIM_Channel_4,		Interrupt_OFF,		ETAT_INACTIF,			(uint32_t) TIM4		},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
/** BTN_WKP			*/	{	GPIOA,		GPIO_Pin_0,		GPIO_Mode_IN,		FALSE,		GPIO_Speed_2MHz,		NULL,				GPIO_PuPd_NOPULL,		NULL,				NULL,			NULL,			ETAT_INACTIF,			NULL			},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
/** Pin_UltraSon_Trig  		*/	{ 	GPIOA,		GPIO_Pin_3,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_TIM2,			TIM_Channel_4,		Interrupt_ON,		ETAT_INACTIF,			(uint32_t) TIM2		},
/** UltraSon_Echo		*/	{	GPIOC,		GPIO_Pin_1,		GPIO_Mode_AN,		FALSE,		GPIO_Speed_2MHz,		NULL,				GPIO_PuPd_NOPULL,		NULL,				ADC_Channel_11,		Interrupt_ON,		ETAT_INACTIF,			(uint32_t) ADC1		},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
};


/********************************************************************
 * Exported functions
 */

/**------------------------------------------------------------------
 *
 * @brief	Init BSP
 *
 */
void
BSP_Init(
		void  /**<[in] Aucun parametre d'entre. */
);

/**------------------------------------------------------------------
 *
 * @brief	Get Peripheral in string
 *
 */
void
toString_GetPeriphral(
		GPIO_TypeDef* 	Peripheral,		/**<[in] Bus GPIOx */
		uint8_t*	String			/**<[in] Buffer */
);

/**------------------------------------------------------------------
 *
 * @brief	Get Pin number in string
 *
 */
void
toString_GetPin(
		uint16_t 		GpioPin,		/**<[in] Pin dont on veut la source */
		uint8_t*		String			/**<[in] Buffer */
);



#endif // BSP_CARTE_H









