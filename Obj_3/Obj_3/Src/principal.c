#include "gassp72.h"
#include "etat.h"




extern void timer_callback(void);
extern short Son; 
extern short LongueurSon; 
extern short PeriodeSonMicroSec;


type_etat StructSon;

int main(void)
{
	float Periode_PWM_en_Tck = 720000;
	/*int resolution;*/
	StructSon.son=&Son;
	StructSon.position = 0;
	StructSon.taille = LongueurSon;
	StructSon.periode_ticks=PeriodeSonMicroSec*72;
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	//Timer_1234_Init_ff( TIM4, INTERVALLE );
	Timer_1234_Init_ff( TIM4, StructSon.periode_ticks);
	// config port PB0 pour être utilisé par TIM3-CH3
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	// config TIM3-CH3 en mode PWM
	StructSon.resolution = PWM_Init_ff( TIM3, 3, Periode_PWM_en_Tck );
	
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
	// cette fonction doit être conforme à l'AAPCS
	Active_IT_Debordement_Timer( TIM4, 2, timer_callback );
	// lancement du timer
	Run_Timer( TIM4 );
	
	while	(1)
		{
		}
}
