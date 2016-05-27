/// EXERCICIO 16 ///

#include <msp430.h>

int flag_blink, p13_ant;      		// Define variavel tipo int

void blink(void)               	// Função Pisca LEDs
{
    if (flag_blink == 1)     	        // se flag pisca == 1 blink
    {
        __delay_cycles(250000);         // delay de 250ms
       	P1OUT = P1OUT ^ 0x41;           // inverte o estado dos leds
    }
    else                     	
    {
      	P1OUT = P1OUT & ~0x41;          // se não, desliga os leds
    }
}

void main(void)			        // Main
{
    WDTCTL  = WDTPW + WDTHOLD;           // Desabilita o watchdog
    BCSCTL1 = CALBC1_1MHZ;              // Configuração do clock para 1MHz
    DCOCTL  = CALDCO_1MHZ;               // Configuração do clock para 1MHz
    
    P1DIR = P1DIR | 0x41;               // seta os leds como saida
    P1REN = P1REN | 0x08;               // Habilita resistor de pullup/down na chave s2
    P1OUT = P1OUT | 0x08;               // Define resistor de pull up
    
    p13_ant = 1;		        // Inicializa o indicador de borda
    flag_blink = 0;                     // Inicializa a flag pisca
    
    P1OUT = P1OUT & ~0x41;              // Inicializa os leds desligados
    
  while(1)
  {
      
      blink();           	      	// Chama função blink
                                   
      if (P1IN & 0x08)   	        // testa se o botão está em nivel alto
      {
          p13_ant = 1;     		//  se sim, mantem a var p13_ant em 1
      }
      else 				// se o botão esta em nivel baixo testa se a var p13_ant == 1
      {
          if (p13_ant == 1)    		// identifica borda de descida
          {	
              p13_ant = 0;              // se sim, reseta a var p13_int, para entrar no if apenas 1 vez se botao press
              flag_blink ^= 1;          // inverte o estado da flag
                          
              if (flag_blink == 1) 	// antes de sair do if, testa a flag_blink
              {
                  P1OUT |= 0x01;    	// se a flag == 1,  se APENAS o led Vermelho
              }
           }
        }     			   
  }
}


      
      
