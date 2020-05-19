#include "gassp72.h"
#define M2TIR 0Xf0a3d

extern void timer_callback(void);

volatile unsigned short int dma_buf[64];
//tableau compteur
int compt[6] = {0};
int score[6] = {0};
int indice;

int M2(int , volatile unsigned short int *);


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
	
	while	(1)
		{

				
		}
}
