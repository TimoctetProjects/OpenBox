/*********************************************************************
 * *******************************************************************
 *
 * @file	config.h
 *
 * @author	Duclos Timothe
 *
 * @date	07/10/2014
 *
 * @brief	Centralisation de differentes configurations
 *
 *********************************************************************
 *********************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

/*****************************************************************
 * Configuration systeme
 */
#define VERSION_SOFT			"Version logicielle v0.1"		/** Version Logiciel */
#define	CONFIG_SYSCLOCK_1ms		(SystemCoreClock / 1000)		/** Frequence interuuption Systick */



/*****************************************************************
 * Configuration des peripheriques
 */
#define VITESSE_USART_DEBUG_bauds	115200					/** Vitesse com USART Debug */



/*****************************************************************
 * Configuration des peripheriques
 */
/** Mode debug */
#define MODE_DEBUG_ON


/*****************************************************************
 * Periode des operations
 */
/** Frequence operations */
#define DUREE_OPERATION_GPIO_ms		10




/*****************************************************************
 * Configuration des equipements
 */
/** Camera */
#define CAMERA_DUREE_IMPUL_SI_ms	15
#define CAMERA_PERIODE_ENVOIE_SI_ms	300
#define CAMERA_RATION_CLOCK_pr100	50
#define CAMERA_PERIODE_CLOCK_us		10
#define NB_ECHABNTILLON_CAM		128


/** Servo Direction */
#define SERVO_DIRECTION_RATIO_POSITION_MOINS90_deg	4		/** Ratio de la PWM pour atteindre une possition de -90° */
#define SERVO_DIRECTION_RATIO_POSITION_MOINS45_deg	7
#define SERVO_DIRECTION_RATIO_POSITION_0_deg		9		/** Ratio de la PWM pour atteindre une possition de 0° */
#define SERVO_DIRECTION_RATIO_POSITION_PLUS45_deg	11
#define SERVO_DIRECTION_RATIO_POSITION_PLUS90_deg	13		/** Ratio de la PWM pour atteindre une possition de +90° */
#define SERVO_DIRECTION_CMD_SERVO_PERIODE_PWM_us	20000		/** Periode du signal PWM commandant le servomoteur en microsecondes*/





#endif
