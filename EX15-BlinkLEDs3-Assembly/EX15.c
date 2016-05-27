/// EXERCICIO 15 ///

#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    // Desabilita o watchdog
    BCSCTL1 = CALBC1_1MHZ;       // Configuração do clock para 1MHz
    DCOCTL = CALDCO_1MHZ;        // Configuração do clock para 1MHz
    
    P1DIR = P1DIR | 0x41;        // seta os leds como saida
    P1OUT = P1OUT & ~0x41;       // Inicializa os leds desligados    
    P1OUT = 0x01;                // Seta só o led vermelho/
    
    while(1)
    {
        __delay_cycles(500000);
        P1OUT = P1OUT ^ 0x41;
    
    }
}


    
    
