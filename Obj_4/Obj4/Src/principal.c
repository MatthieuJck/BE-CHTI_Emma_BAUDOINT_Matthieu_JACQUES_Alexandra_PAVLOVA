#include "gassp72.h"
#include "etat.h"
#define M2TIR 0Xf0a3d

extern void timer_callback(void);

volatile unsigned short int dma_buf[64];
//tableau compteur
int compt[6] = {0};
int score[6] = {0};
int indice;

int M2(int , volatile unsigned short int *);


extern void timer_callback(void);
extern short Son; 
extern short LongueurSon; 
extern short PeriodeSonMicroSec;
type_etat StructSon;


void sys_callback()
	{
		
	GPIO_Set(GPIOB, 1);
	// Démarrage DMA pour 64 points
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	//Calcul de la DFT pour les 6 fréquences
	//On a un pas de 5kHz
	//k=17 -> 85 kHz
	int s;
  int max=-100000;
	int min = 1000000;
	
		//On calcul F1 à F4
	for(int k=17;k<=24 ; k++) {
		//compt[k] = M2(k,dma_buf);
		s=M2(k,dma_buf);
		/*Test de la dft*/
	
		if( max < s ) 
		{
			max = s;
		}
		if(min > s )
		{
			min =s;
		}
		
		if (s >= M2TIR)
		{
			switch(k){
				case 17 : compt[0]++; indice =0; break;
				case 18 : compt[1]++; indice =1;break;
				case 19 : compt[2]++; indice =2;break;
				case 20 : compt[3]++; indice =3;break;
				case 23 : compt[4]++; indice=4;break;
				case 24 : compt[5]++;	indice=5;break;
				default : break;
			}
		}
		else
		{
			switch(k){
				case 17 : compt[0]=0; break;
				case 18 : compt[1]=0; break;
				case 19 : compt[2]=0; break;
				case 20 : compt[3]=0; break;
				case 23 : compt[4]=0; break;
				case 24 : compt[5]=0;	break;
				default : break;}
			/*compt[0]=0;
			compt[1]=0;
			compt[2]=0;
			compt[3]=0;
			compt[4]=0;
			compt[5]=0;*/
		}
		
	}
	GPIO_Clear(GPIOB, 1);
	
	
}





int main(void)
{

	int SYSTICK_PER = 360000;
	
		// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// PA2 (ADC voie 2) = entrée analog
	GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
	// PB1 = sortie pour profilage à l'oscillo
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	// PB14 = sortie pour LED
	GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);

	// activation ADC, sampling time 1us
	Init_TimingADC_ActiveADC_ff( ADC1, 0x33 );
	Single_Channel_ADC( ADC1, 2 );
	// Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
	Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
	// Config DMA pour utilisation du buffer dma_buf (a créér)
	Init_ADC1_DMA1( 0, dma_buf );

	// Config Timer, période exprimée en périodes horloge CPU (72 MHz)
	Systick_Period_ff( SYSTICK_PER );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
	Systick_Prio_IT( 3, sys_callback );
	SysTick_On;
	SysTick_Enable_IT;
	
	float Periode_PWM_en_Tck = 720000;
	/*int resolution;*/
	StructSon.son=&Son;
	/*StructSon.position = 0;*/
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
	
	/*Variable qui permet de n'augmenter le socre qu'une seule fois quand on est à compt[indice]==7*/
	int DejaFait = 0;
	
	while	(1)
		{
			/*On considère qu'on a reçu un signal si on a 3 DFT consécutives */
			if(compt[indice] == 6 && DejaFait==0)
			{
				score[indice]=score[indice]+1;
				StructSon.position = 0;
				DejaFait=1;
			}
			else if (compt[indice] == 0)
			{
				StructSon.position = -1;
				DejaFait=0;
			}

				
		}
}
