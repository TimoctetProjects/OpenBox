/*********************************************************************
 * *******************************************************************
 *
 * @file	equ_UltraSon.c
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver du module a ultrason
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "equ_UltraSon.h"

/********************************************************************
 * Private defines
 */
#define ULTRASON_PERIODE_PWM_CLK_us		20
#define ULTRASON_RATIO_PWM_CLK_pr100		50

#define ULTRASON_PERIODE_MESURE_ADC_ms		60

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

	UltraSon_Etapeinit = 0,
	UltraSon_Etape_main,
	nb_EtapesUltraSon,

}Etapes_e;

typedef struct {
	Mapping_GPIO_e 	ID_Pin_Trig;
	Mapping_GPIO_e 	ID_Pin_Echo;
	uint32_t	ValueEcho_mV;
	uint8_t		IDADC_Channel;
}UltraSon_s;

/********************************************************************
 * Private variables
 */
static UltraSon_s UltraSon = {	.ID_Pin_Trig 	= 	Pin_UltraSon_Trig,
				.ID_Pin_Echo 	= 	Pin_UltraSon_Echo,
				.ValueEcho_mV  	=	0,
				.IDADC_Channel	=	0			};

/********************************************************************
* Private Fonctions prtotypes
*/
static inline void
UltraSon_InitTrig(
	void
);

static inline void
UltraSon_InitEcho(
	void
);

/********************************************************************
 * Exported Fonctions
 */
/**-------------------------------------------------------------------
 *
 * @brief		Gestion du module a ultrason
 *
 */
void
UltraSon_main(
	void
) {
	static Etapes_e Etapes = UltraSon_Etapeinit;

	switch(Etapes) {

		case UltraSon_Etapeinit:	UltraSon_InitTrig();
						UltraSon_InitEcho();
						Etapes = UltraSon_Etape_main;
						break;

		case UltraSon_Etape_main:
						if(TSW_GetStatus(IDTSW_UltraSon_MesureEcho) == STATUS_FINIS) {
							Pwm_Activer	(	UltraSon.ID_Pin_Trig		 );
							TSW_Start(IDTSW_UltraSon_MesureEcho, ULTRASON_PERIODE_MESURE_ADC_ms);
						}
			break;

		default:
			break;
	}
}

/**-------------------------------------------------------------------
 *
 * @brief	toString de la position du servomoteur
 *
 */
void
UltraSon_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
) {

	if(		IDMapping != UltraSon.ID_Pin_Echo
		&& 	IDMapping != UltraSon.ID_Pin_Trig	)

		return;

	switch(Field) {

		case toString_Getpin:
			toString_GetPeriphral	(Mapping_GPIO[IDMapping].GpioPeripheral,pString);
			toString_GetPin		(Mapping_GPIO[IDMapping].GpioPin, 	pString);
			break;


		case toString_GetValue:

			break;
	}
}

/********************************************************************
* Private Fonctions definition
*/
/**-------------------------------------------------------------------
 *
 * @brief		Init Signal Trig du module
 *
 */
static inline void
UltraSon_InitTrig(
	void
) {
	PWM_Init	( 	UltraSon.ID_Pin_Trig,
				ULTRASON_PERIODE_PWM_CLK_us,
				ULTRASON_RATIO_PWM_CLK_pr100	 );
	Pwm_Activer	(	UltraSon.ID_Pin_Trig		 );
}

/**-------------------------------------------------------------------
 *
 * @brief		Init Signal Echo du module
 *
 */
static inline void
UltraSon_InitEcho(
	void
) {
	ADC_ChannelConf_t ADC_Echo;

	ADC_Echo.pinID 			= UltraSon.ID_Pin_Echo;
	ADC_Echo.ConversionContinue	= 0;
	ADC_Echo.LastValue_mV		= 0;
	ADC_Echo.pStoreValue_mV		= &UltraSon.ValueEcho_mV;
	ADC_Echo.pFct_CallbackEOC	= NULL;

	UltraSon.IDADC_Channel = ADC_ChannelConfigure(ADC_Echo);
}
