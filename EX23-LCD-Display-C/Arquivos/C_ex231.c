/***************************** Exercício 23 ***********************************************/
/* Configurar display LCD						                  */
/* PEDRO MARIZ 										  */
/******************************************************************************************/
								    

#include <msp430.h>

#define	RS	0x40		/* Mascara do pino P1.6 */
#define	E	0x80		/* Mascara do pino P1.7 */
#define D7	0x80		/* Mascara do pino P2.7 */
#define D6	0x40		/* Mascara do pino P2.6 */ 
#define D5	0x20		/* Mascara do pino P2.5 */
#define D4	0x10		/* Mascara do pino P2.4 */
	
	/* Configurar uC */
	void Configurar_uC(void)
	{
   		 WDTCTL	 = WDTPW + WDTHOLD;    		/* Desabilita o watchdog */
    		 BCSCTL1 = CALBC1_1MHZ;       		/* Configuração do clock para 1MHz */
   		 DCOCTL  = CALDCO_1MHZ;        		/* Configuração do clock para 1MHz */
    		 P1DIR  |= RS+E;        		/* Ajusta os pinos RS e E como saída  */
		 P2DIR	|= D4+D5+D6+D7;			/* Ajusta os pinos D4...D7 como saída */
                 P2SEL = 0x00;                          /* Configura a saida do pino como output*/
                 P2SEL2= 0x00;                          /* Pois por default ele tem outra fun�ao P2.6 e P2.7*/
	}
	   

	/* PROGRAMA PRINCIPAL */
	void main(void)			      	
	{
		Configurar_uC(); 		
		__delay_cycles(15000);			/* Delay de 15ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = D4+D5;				/* Envia nibble(+) 0011, do comando 0x30 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		//P2OUT &= 0x00;				/* Envia nibble(-) 0000, do comando 0x30 */
		//P1OUT &= ~E;				/*\				*/		
		//P1OUT |= E;				/* Envia pulso de Enable de 1us */
		//P1OUT &= ~E;				/*/                             */
		
		__delay_cycles(5000);			/* Delay de 5ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = D4+D5;				/* Envia nibble(+) 0011, do comando 0x30 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		//P2OUT &= 0x00;				/* Envia nibble(-) 0000, do comando 0x30 */
		//P1OUT &= ~E;				/*\				*/		
		//P1OUT |= E;				/* Envia pulso de Enable de 1us */
		//P1OUT &= ~E;				/*/				*/
		
		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = D4+D5;				/* Envia nibble(+) 0011, do comando 0x30 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		//P2OUT &= 0x00;				/* Envia nibble(-) 0000, do comando 0x30 */
		//P1OUT &= ~E;				/*\				*/		
		//P1OUT |= E;				/* Envia pulso de Enable de 1us */
		//P1OUT &= ~E;				/*/				*/
		
		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = D5;				/* Envia nibble(+) 0010, do comando 0x20 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		//P2OUT &= 0x00;				/* Envia nibble(-) 0000, do comando 0x20 */
		//P1OUT &= ~E;				/*\				*/		
		//P1OUT |= E;				/* Envia pulso de Enable de 1us */
		//P1OUT &= ~E;				/*/				*/
		
		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = D5;				/* Envia nibble(+) 0010, do comando 0x28 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		P2OUT = D7;				/* Envia nibble(-) 1000, do comando 0x28 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
			
		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = 0x00;				/* Envia nibble(+) 0000, do comando 0x08 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		P2OUT = D7;				/* Envia nibble(-) 1000, do comando 0x08 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		
		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = 0x00;				/* Envia nibble(+) 0000, do comando 0x01 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		P2OUT = D4;				/* Envia nibble(-) 0001, do comando 0x01 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/

		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = 0x00;				/* Envia nibble(+) 0000, do comando 0x06 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		P2OUT = D5+D6;				/* Envia nibble(-) 0110, do comando 0x06 */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/

		__delay_cycles(1000);			/* Delay de 1ms */
		P1OUT &= ~RS;				/* RS = 0, enviar comando */
		P2OUT = 0x00;				/* Envia nibble(+) 0000, do comando 0x0F */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/
		P2OUT = D4+D5+D6+D7;			/* Envia nibble(-) 1111, do comando 0x0F */
		P1OUT &= ~E;				/*\				*/		
		P1OUT |= E;				/* Envia pulso de Enable de 1us */
		P1OUT &= ~E;				/*/				*/	 		 		    
    					
	}

	

	
  
