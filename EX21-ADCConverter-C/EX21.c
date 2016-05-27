/// EXERCICIO 21 //

#include "msp430g2553.h"

/* Configurar uC */
void config_uC(void)
{
    WDTCTL  = WDTPW + WDTHOLD;  // Desabilita o watchdog
    BCSCTL1 = CALBC1_8MHZ;      // ConfiguraÃ§Ã£o do clock para 8MHz 
    DCOCTL  = CALDCO_8MHZ;      // ConfiguraÃ§Ã£o do clock para 8MHz 
    
    P1DIR  |= 0x41;           // Saidas P1.0 e P1.6
    P1OUT  &= ~0x41;
    
    /* Configurar ADC */
    
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
    
}

int buffer_ADC10;               // Variavel para o Buffer do Conversor ADC10

int main (void){
  
    config_uC();
    while(1)                    // Loop Infinito
    {
      if( buffer_ADC10 < 0x00FF)
      {
        P1OUT &= ~(0x41);       // OFF P1.0 e P1.6
      }
      
      if( (buffer_ADC10 >= 0x00FF) && (buffer_ADC10 < 0x01FF) )
      {
        P1OUT &= ~(0x01);       // OFF P1.0 
        P1OUT |=  (0x40);       // ON  P1.6
      }
      
      if( (buffer_ADC10 >= 0x01FF) && (buffer_ADC10 < 0x02FF) )
      {
        P1OUT &= ~(0x40);       // OFF P1.0 
        P1OUT |=  (0x01);       // ON  P1.6
      }
      
      if( buffer_ADC10 >= 0x02FF )
      {
        P1OUT |= (0x41);       // ON  P1.0 e P1.6
      }
      
    }

}  
         
/* Interrupção ADC */
 #pragma vector = ADC10_VECTOR      // Interrupção de recebimento
    __interrupt void interrupt_ADC (void)
{
  
    //P1OUT |= 0x41;
    
    buffer_ADC10 = ADC10MEM;            // Buffer recebe byte recebido
    ADC10CTL0 &= ~ADC10IFG;              // Reset Flag Interrupt
    /* ADC10 Interrupt Flag */
    
    ADC10CTL0 |= (ENC + ADC10SC);         // Habilita e Inicializa conversão
    /* ADC10 Enable Conversion */
    /* ADC10SC */
       
}
       
       


