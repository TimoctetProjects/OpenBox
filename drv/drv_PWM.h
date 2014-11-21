/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_PWM.h
 *
 * @author	Duclos Timothe
 *
 * @date	16/09/2014
 *
 * @brief	Driver générant un signal PWM sur une broche pré-configurée
 *
 *********************************************************************/

#ifndef DRV_PWM_H
#define DRV_PWM_H

/********************************************************************
 * Includes
 */
#include "BSP_carte.h"
#include "util_Console.h"

/********************************************************************
 * Exported defines
 */

/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */

typedef enum {

	Err_PeriodAskedForIsUnreachable = 0,
	Err_PWM_Unavailable,

}PWM_Err_Status_e;

/********************************************************************
 * Exported Function
 */

/**------------------------------------------------------------------
 * @brief	Initialisation de la PWM
 *
 */
uint8_t
PWM_Init(
			Mapping_GPIO_e	IdPinPwm,		/**<[in] ID de la Pin ou générer la PWM*/
			uint32_t 	Periode_us,		/**<[in] Periode de comptage */
			uint32_t	Ratio_pr100		/**<[in] Ratio du signal PWM en % */
);

/**------------------------------------------------------------------
 * @brief	Desinitialisation de la PWM
 *
 */
void
PWM_Desinit(
		Mapping_GPIO_e	IdPinPwm		/**<[in] ID de la Pin ou générer la PWM*/
);

/**------------------------------------------------------------------
 *
 * @brief	Activation du signal PWM
 *
 * @return	PWM State
 *
 */
uint8_t
Pwm_Activer(
		Mapping_GPIO_e IdPinPwm		/** <[in] ID de la PIN où générer le signal PWM */
);

/**------------------------------------------------------------------
 *
 * @brief	Desactivation du signal PWM
 *
 * @return	PWM State
 *
 */
uint8_t
Pwm_Desactiver(
		Mapping_GPIO_e IdPinPwm		/** <[in] ID de la PIN où le signal PWM était généré */
);

/**------------------------------------------------------------------
 * @brief	Lecture de la configuration d'une PWM
 *
 * @return	void
 *
 */
void
PWM_GetConfiguration(
			Mapping_GPIO_e	IdPinPwm,	/**<[in] ID de la Pin ou générer la PWM */
			uint32_t* 	Periode_us,	/**<[out] Periode de la PWM en microsecondes */
			uint8_t*	Ratio_pr100	/**<[out] Ratio de la PWM en pr100 */
);

/**------------------------------------------------------------------
 *
 * @brief	ToString Valeur PWM
 *
 */
void
PWM_Value_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
);

#endif


/*
    Set timer prescaller
    Timer count frequency is set with

    timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)

    In our case, we want a max frequency for timer, so we set prescaller to 0
    And our timer will have tick frequency

    timer_tick_frequency = 84000000 / (0 + 1) = 84000000
*/
/*
    Set timer period when it have reset
    First you have to know max value for timer
    In our case it is 16bit = 65535
    To get your frequency for PWM, equation is simple

    PWM_frequency = timer_tick_frequency / (TIM_Period + 1)

    If you know your PWM frequency you want to have timer period set correct

    TIM_Period = timer_tick_frequency / PWM_frequency - 1

    In our case, for 10Khz PWM_frequency, set Period to

    TIM_Period = 84000000 / 10000 - 1 = 8399

    If you get TIM_Period larger than max timer value (in our case 65535),
    you have to choose larger prescaler and slow down timer tick frequency
*/
/*
    To get proper duty cycle, you have simple equation

    pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

    where DutyCycle is in percent, between 0 and 100%

    25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
    50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
    75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
    100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

    Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/
