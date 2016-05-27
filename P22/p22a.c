/***************************** Exercício 01 ***********************************************/
/* Piscar leds verde -> vermelho -> verde e vermelho					                  */
/* PEDRO MARIZ 										 									  */
/******************************************************************************************/

#include <msp430.h>
#include <msp430g2452.h>

	int flag_pisca, flag_subiu;
	signed int TA0_delta;

/* Configurar uC */
	void Configurar_uC(void)
	{
   		 WDTCTL = WDTPW + WDTHOLD;    		/* Desabilita o watchdog */
   		 BCSCTL1 = CALBC1_1MHZ;       		/* Configuração do clock para 1MHz */
   		 DCOCTL = CALDCO_1MHZ;        		/* Configuração do clock para 1MHz */
                 P1DIR = P1DIR | 0x41;        		/* seta os leds como saida */
   		 P1REN = P1REN | 0x08;        		/* Habilita resistor de pullup/down na chave s2 */
  		 P1OUT = P1OUT | 0x08;        		/* Define resistor de pull up */
	}
/* Configurar interrupção P1.3 */
	void Configurar_Interr_P13(void)
	{
		 P1IE  = 0x08;		     	 	/* Habilita interrupção do boão p1.3 */
		 P1IES = 0x08;		      		/* Habilita interrupção na borda de descida */
		 P1IFG = 0x00;		      		/* Zera a flag de interrupção da porta 1 */
	}
/* Configurar Timer0_A */
	void Configurar_Timer0_A(void)
	{
		TACTL |= TASSEL_2 + ID_3 + MC_0;  	/* clock= SMCLK, div= /8, contagem MC_3 = Up/down mode => TACCR0-00h) */
		TACCR0 = 0xF424;	           		/* 0xF424 = 62500 decimal * 8us => 500ms */
		TACCTL0 |= CCIE;		   			/* Habilita captura/comparação com relação a IFG */
		P1IFG = 0x00;
	}

/* Função sequencia dos leds */

	void Sequencia_leds(void)
	{
		TA0_delta = (TA0R - TA0_delta);                          /* Variável que indica a direçao da contagem */

		if (flag_pisca == 1)
			{

						if (TA0_delta > 0)					/* se delta positivo, contagem crescente */
						{
							if(TA0R <= 31250)   /*31250*/
								{

								P1OUT &= ~(0x41);          		/*Apaga o led Vermelho */
								P1OUT |= 0x40;          		/*Acende o led Verde */
								}
							else if (TA0R < 62500) /*62500*/
									{

									P1OUT &= ~(0x40);          		/*Apaga o led Verde */
									P1OUT |= 0x01;          		/*Acende o led Vermelho */

									}
						}

						if (TA0_delta < 0)						/* se delta negativo, contagem decrescente */
						{
							if(TA0R >= 31250)  /*31250*/
								{

									P1OUT |= 0x41;          		/*Acende o leds Verde */
								}
								else if (TA0R > 0)
								{

									P1OUT &= ~(0x41);          		/*Apaga o leds Verde e vermelho */
								}

						}
			}
	}



/* PROGRAMA PRINCIPAL */
	void main(void)
	{
		Configurar_uC();
		Configurar_Interr_P13();
		Configurar_Timer0_A();
		P1OUT = P1OUT & ~0x41;   			/* Inicializa os leds desligados */
		 __enable_interrupt(); 				/* Habilitar Interrupções */

		flag_pisca = 0;						/* Inicializa flag_pisca em um, piscar */

    	while(1)
    	{
    		TA0_delta = TA0R;				/* Salva valor no contador antes da operação de comparação */
    		Sequencia_leds();
    	}
	}

/* Interrupção do P1.3 */
#pragma vector = PORT1_VECTOR
__interrupt void interr_P1(void)
	{
		flag_pisca ^= 1;					/* Inverte flag_pisca */

		if (flag_pisca == 1)
			{
				TACTL |= MC_3; 				/* Inicia timer no modo Up/Down */
			}

		if (flag_pisca == 0)
			{
				TACTL &= ~MC_3; 			/* Para a contagem do Timer, BIT 4 e 5 = 0*/
			}

		 P1IFG = 0x00;		      			/* Zera a flag de interrupção da porta 1 */
	}

/* Interrupção do Timer0_A */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interr_piscar(void)
	{

	    P1IFG = 0x00;		      		/* Zera a flag de interrupção da porta 1 */
	}
