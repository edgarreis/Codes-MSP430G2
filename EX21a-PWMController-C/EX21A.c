
/// EXERCICIO 21A //

#include "msp430g2553.h"

/* Configuração uC */
void config_uC(void)
{
    WDTCTL  = WDTPW + WDTHOLD;  // Desabilita o watchdog
    BCSCTL1 = CALBC1_8MHZ;      // ConfiguraÃ§Ã£o do clock para 8MHz 
    DCOCTL  = CALDCO_8MHZ;      // ConfiguraÃ§Ã£o do clock para 8MHz 
    
    P1DIR  |=  0x40;             // Saidas P1.6
    P1OUT  &= ~0x40;            // Forçar saida em zero
    P1SEL  |=  0x40;             // Habilitar TA0.1 Saidas P1.6
    
/* Configuração Timer0_A */
    
    /* Timer A Control */
    TA0CTL |=  (TASSEL_2 + ID_1 + MC_1);
    /* Timer A clock source select: 2 - SMCLK */
    /* Timer A input divider: 1 - /2 */
    /* Timer A mode control: 1 - Up to CCR0 */
    
    /* Timer A Capture/Compare Control 1 */
    TA0CCTL1 |= (OUTMOD_6);
    /* PWM output mode: 6 - PWM toggle/set */
    
    TACCTL0 |= CCIE;		   	/* Habilita captura/comparação com relação a IFG */
         
    
/* Configuração ADC */
    
    /* ADC10 Control 0 */
    ADC10CTL0|= (SREF_0 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE);
    /* VR+ = AVCC and VR- = AVSS */
    /* 16 x ADC10CLKs */
    /* ADC10 Multiple SampleConversion */
    /* ADC10 On/Enable */
    /* ADC10 Interrupt Enalbe */
    
    /* ADC10 Control 1 */    
    ADC10CTL1 |= (INCH_5 + ADC10SSEL_0);
    /* Selects Channel 5 */
    /* ADC10OSC */       
    
    /* ADC10 Analog Enable 0 */
    ADC10AE0 |= BIT5;        // Pino P1.5
    
    __bis_SR_register (GIE);    // Enter LPM0, interrupções habilitado
    
    ADC10CTL0 |= (ENC + ADC10SC);
    /* ADC10 Enable Conversion */
    /* ADC10SC */
    
    /* Timer A Capture/Compare 0 */
    TA0CCR0 = 1023;            // Valor Máximo da Contagem
    
}

int buffer_ADC10;               // Variavel para o Buffer do Conversor ADC10

int main (void){
  
    config_uC();
    while(1)                    // Loop Infinito
    {
      
    }

}  
         
/* Interrupção ADC */
 #pragma vector = ADC10_VECTOR      // Interrupção de recebimento
 __interrupt void interrupt_ADC (void)
{
  
    //P1OUT |= 0x41;
    
    buffer_ADC10 =  (ADC10MEM);            // Buffer recebe byte recebido
    ADC10CTL0   &= ~ADC10IFG;             // Reset Flag Interrupt
    /* ADC10 Interrupt Flag */
    
    ADC10CTL0 |= (ENC + ADC10SC);         // Habilita e Inicializa conversão
    /* ADC10 Enable Conversion */
    /* ADC10SC */
     
}
       
/* Interrupção TIMER0_A */
 #pragma vector = TIMER0_A0_VECTOR       // Interrupção do Timer
 __interrupt void TIMER0_A (void) 
{
   TA0CCR1 = buffer_ADC10;              // tempo em up recebe valor do Buffer
   TA0CCTL0 &=~ CCIFG;                  //Limpar flag de interrupção 

}