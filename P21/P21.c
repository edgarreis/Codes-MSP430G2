/// EXERCICIO 2 ///
    
/* 
ALUNO: EDGAR DOS REIS
GRR: 20111758

Contadores:

Modo UP/DOWN
t = 1.5 s
f = 1M hz
div = 8
T' = 8u s
N = 46.875

Modo UP
t = 0.4 s
f = 1M hz
div = 8
T' = 8u s
N = 50.000

Modo UP/DOWN
t = 0.5 s
f = 1M hz
div = 8
T' = 8u s
N = 15.625

*/

#include <msp430g2553.h>

unsigned int x = 0;                     // variavel para DEBAUNCING
unsigned int flag_button = 0;           // flag para acionamento do botão
unsigned int count = 0;                 // Contador p/ 1.5s

/* Configurar uC */
void Configurar_uC(void)
{
    WDTCTL  = WDTPW + WDTHOLD;          //Desabilita o watchdog
    BCSCTL1 = CALBC1_1MHZ;       	// Configuração do clock para 1MHz
    DCOCTL  = CALDCO_1MHZ;        	// Configuração do clock para 1MHz
    P1DIR  |= 0x41;        	        // seta os leds como saida
    P1REN  |= 0x08;                	// Habilita resistor de pullup/down na chave s2
    P1OUT  |= 0x08;        		// Define resistor de pull up
                  
/* Configurar interrupção P1.3 */

    P1IE   = 0x08;		     	 // Habilita interrupção do boão p1.3
    P1IES  = 0x08;		      	 // Habilita interrupção na borda de descida
    P1IFG  = 0x00;		      	 // Zera a flag de interrupção da porta 1

/* Configurar Timer0_A */
    TA0CTL   |= TASSEL_2 + ID_3 + MC_3;  // clock= SMCLK, div= /8, MC_3 = Up/Down mode => TACCR0
    TA0CCR0   = 46875;	           	 // 46875 * 2 (UP/Down) * 2 * 8 us  -->  1.5 s em Modo UP/DOWN
    TA0CCTL0 |= CCIE;		   	 // Habilita captura/comparação com relação a IFG

/* Configurar Timer1_A */
    TA1CTL   |= TASSEL_2 + ID_3 + MC_1;  // clock= SMCLK, div= /8, MC_1 = Up mode => TACCR0
    TA1CCR0   = 50000;	           	 // 50.000  (UP) * 8 us  -->  0.4 s em Modo UP
    TA1CCTL0 |= CCIE;		   	 // Habilita captura/comparação com relação a IFG

    __enable_interrupt(); 	         // Habilitar Interrupções
}	   


/* PROGRAMA PRINCIPAL */
void main(void)			      	
{
    Configurar_uC();                     // Configurações
	
    while(1) 		    		 // Loop Infinito			
    {
       
    }
}

// Interrupção do P1.3 
#pragma vector = PORT1_VECTOR		
__interrupt void interr_P1(void)
{

 /* DEBAUNCING */
    __delay_cycles(5);			// delay de 5ms
    x = 1;				// garante que irá entrar no while
    while (x == 1)
    if ((P1IN & 0x08) == 0)		// testa se o botão p1.3 está pressionado
    {
	x = 0;			        // muda estatus de x para sair do while
    }
    else
    {				        // se não estiver pressionado volta a conferir
	x = 1;
    }
    __delay_cycles(5);			// delay de 5ms

        
        
    //P1OUT = ~0x41;
    flag_button ^= 1;			// Inverte flag_pisca
    if (flag_button == 1)
    {   
        TA0CTL |= MC_0; 		// Para Timer
        TA0CCR0 = 15625;                //  15625 * 2 (UP/Down) * 2 * 8 us  -->  0.5 s em Modo UP/DOWN
        TA0CTL |= MC_3; 		// Inicia timer no modo Up/Down
        P1OUT &= ~0x41;                 // Apaga os Leds para nova sequência
    }	
    else
    {
	TA0CTL |= MC_0; 		// Para Timer
        TA0CCR0 = 46875;                // 46875 * 2 (UP/Down) * 2 * 8 us  -->  1.5 s em Modo UP/DOWN
        TA0CTL |= MC_3; 		// Inicia timer no modo Up/Down
        
    }
    
    P1IFG = 0x00;                       // Zera a flag de interrupção da porta 1
                  
}


/* Interrupção do Timer0_A */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interrupt_0(void)
{    
    if (flag_button == 0)               // Verifica se foi pressionado
    {  
      count++;                          // Contador para gerar 1.5 s
      if (count == 2)
      {
        P1OUT ^= 0x01;                  // Inverte LED
        count = 0;                      // Zera contador
      }
      
    }

    else
    {  
      P1OUT ^= 0x41;                    // Inverte LED
      
    }
    
    TA0CCTL0 &=~ CCIFG;                 //Limpar flag de interrupção 
    P1IFG = 0x00;		      	// Zera a flag de interrupção da porta 0
      
}



/* Interrupção do Timer1_A */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void interrupt_1(void)
{
    if (flag_button == 0)               // Verifica se o botão foi pressionado
    {
      P1OUT ^= 0x40;                    // Inverte o LED
      
    }
    TA1CCTL0 &=~ CCIFG;                 //Limpar flag de interrupção 
    P1IFG = 0x00;		      	// Zera a flag de interrupção da porta 1
}
