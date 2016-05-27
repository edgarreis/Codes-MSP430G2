/// EXERCICIO 1 ///
    
/* 
ALUNO: EDGAR DOS REIS
GRR: 20111758

Contadores:

Modo UP/DOWN
t = 2 s
f = 1M hz
div = 8
T' = 8u s
N = 62.500

Obs.: Na minha placa esta funcionando somente quando eu forço (na simulação) 1 no P1.3 (pressionando manualmente não esta invertendo)
Revisei o programa e não há porque ele não fazer a inversão

*/

#include <msp430g2553.h>

unsigned int x = 0;                     // variavel para DEBAUNCING
unsigned int flag_button = 0;           // flag para acionamento do botão
unsigned int count = 0;                 // Contador p/ 1.5s
unsigned int flag_blink = 0;            // flag para piscar 
int flag = 0;                           // flag da sequencia os LEDS

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
    TA0CCR0   = 62500;	           	 // 62500 * 2 (UP/Down) * 2 * 8 us  -->  2 s em Modo UP/DOWN
    TA0CCTL0 |= CCIE;		   	 // Habilita captura/comparação com relação a IFG


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

    
    flag_button ^= 1;			// Inverte flag_button sinalizando que o botão foi pressionado    
    P1IFG = 0x00;                       // Zera a flag de interrupção da porta 1
                  
}


/* Interrupção do Timer0_A */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interrupt_0(void)
{    
   
      count++;                          // Contador para gerar 2 s
      if (count == 2)                   // em 2 s
      {
        count = 0;                      // Zera contador
        
        if( flag_button == 0)           // Se o botão não foi pressionado
        {  
            flag++;                     // Sequência ++
        }
       
        else                            // Se o botão foi pressionado
        {
            flag--;                     // Sequência --;
        }
          
    
        if(flag == 1 || flag == -4)     // 1ª da Sequência ++, 4 da Sequência --
        {
          P1OUT = 0x00;                 // LEDs apagados
        }
        
        if(flag == 2 || flag == -3)     // 2ª da Sequência ++, 3ª da Sequência --
        {
          P1OUT = 0x01;                 // LED vermelho ligado
        }
        
        if(flag == 3 || flag == -2)     // 3ª da Sequência ++, 2ª da Sequência --
        {
          P1OUT = 0x40;                 // LED verde ligado
        }
        
        if(flag == 4 || flag == -1)     // 4ª da Sequência ++, 1ª da Sequência --
        {
          P1OUT = 0x41;                 // LED vermelho e verde ligados
        }
        
        if(flag == 4 || flag == -4)     // No ultimo da sequência, reinicia a contagem
        {       
          flag = 0;                     // Reset flag
        }
        
        flag_blink = 0; 		// Inicializa flag_pisca em zero, não piscar
     }
     
     TA0CCTL0 &=~ CCIFG;                 //Limpar flag de interrupção 
     P1IFG = 0x00;		      	// Zera a flag de interrupção da porta 0
      
}
