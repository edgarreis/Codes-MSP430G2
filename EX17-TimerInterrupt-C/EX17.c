/// EXERCICIO 17 ///

#include <msp430.h>

int flag_pisca;

/* Configurar uC */
	void Configurar_uC(void)
	{
   		 WDTCTL = WDTPW + WDTHOLD;    		/* Desabilita o watchdog */
    		 BCSCTL1 = CALBC1_1MHZ;       		/* Configura��o do clock para 1MHz */
   		 DCOCTL = CALDCO_1MHZ;        		/* Configura��o do clock para 1MHz */
    		 P1DIR = P1DIR | 0x41;        		/* seta os leds como saida */
   		 P1REN = P1REN | 0x08;        		/* Habilita resistor de pullup/down na chave s2 */ 
  		 P1OUT = P1OUT | 0x08;        		/* Define resistor de pull up */
	}
/* Configurar interrup��o P1.3 */
	void Configurar_Interr_P13(void)
	{
		 P1IE  = 0x08;		     	 	/* Habilita interrup��o do bo�o p1.3 */
		 P1IES = 0x08;		      		/* Habilita interrup��o na borda de descida */
		 P1IFG = 0x00;		      		/* Zera a flag de interrup��o da porta 1 */
	}
/* Configurar Timer0_A */
	void Configurar_Timer0_A(void)
	{
		TACTL |= TASSEL_2 + ID_3 + MC_0;  	/* clock= SMCLK, div= /8, contagem MC_0 = parado, (MC_1 = Up mode => TACCR0) */
		TACCR0 = 0x7A12;	           	/* 0x7A12 = 31250 decimal * 8us => 250ms */
		TACCTL0 |= CCIE;		   	/* Habilita captura/compara��o com rela��o a IFG */
	}	   

/* PROGRAMA PRINCIPAL */
	void main(void)			      	
	{
		Configurar_uC();
		Configurar_Interr_P13();
		Configurar_Timer0_A();		

		 
		 __enable_interrupt(); 			/* Habilitar Interrup��es */  		 		    
    		 P1OUT = P1OUT & ~0x41;   		/* Inicializa os leds desligados */
		 flag_pisca = 0;			/* Inicializa flag_pisca em zero, n�o piscar */		
    		while(1); 		    		/* Loop Infinito */			
	}

/* Interrup��o do P1.3 */
#pragma vector = PORT1_VECTOR		
__interrupt void interr_P1(void)
	{
		flag_pisca ^= 1;			/* Inverte flag_pisca */
		if (flag_pisca == 1)
			{
				TACTL |= MC_1; 		/* Inicia timer no modo Up */
				P1OUT |= 0x01;          /*Acende o led Vermelho*/	
			}		
		else
			{
				TACTL |= MC_0; 		/* Para a contagem do Timer */
				P1OUT &= ~0x41;       	/* Desliga os Leds */
			}
		
		 P1IFG = 0x00;		      		/* Zera a flag de interrup��o da porta 1 */
	}

/* Interrup��o do Timer0_A */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interr_piscar(void)
	{
		if (flag_pisca == 1)
		{		
			P1OUT ^= 0x41; 			/* Inverte o estado dos leds */
			P1IFG = 0x00;		      	/* Zera a flag de interrup��o da porta 1 */	
		}
		else
		{
			P1OUT &= ~0x41;       		/* Desliga os Leds */
			P1IFG = 0x00;		      	/* Zera a flag de interrup��o da porta 1 */
		}	

	}
    
