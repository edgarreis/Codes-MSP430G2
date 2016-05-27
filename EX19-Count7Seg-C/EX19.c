/// EXERCICIO 19 ///

#include <msp430.h>

int uni  = 9;
int dez  = 9;
int cent = 9;
int display[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; 
                 /* 0  , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  */

/* Interrupção Timer0_A */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interr_timer_A (void)
{ 
  
  uni--;
  if(uni < 1 )
  {
      uni = 9;
      dez--;
  }
  if(dez < 1 )
  {
    dez = 9;
    cent--;
  }    
  if(cent < 1 )
  {
    cent = 9;  
  }  
  
  TA0CCTL0 &=~ CCIFG;   //Limpar flag de interrupção
  
  /* Capture/compare interrupt flag     */
  /* Timer A Capture/Compare Control 0  */
}

void   main(void)
       {     
          /* Configuração uC */
          WDTCTL = WDTPW + WDTHOLD;     // Desativa o WDT watchdog
          P1DIR =  0x7F;                // P1.0 ... P1.6 Saída
          P2DIR =  0x07;                // P2.0 ... P2.2 Saída
          P1OUT =  0x00;                // Forçar Zero
          P2OUT =  0x00;                // Forçar Zero

          /* Configuração Timer0_A */
          TA0CTL = TASSEL_2+ID_1+MC_0;
          /* Timer A clock source select: 2 - SMCLK */
          /* Timer A input divider: 1 - /2 */
          /* Timer A mode control: 0 - Stop */
          
          TA0CCTL0 = CCIE ; 
          /* Capture/compare interrupt enable */
          
          TA0CCR0 = 50000;
          TA0CTL  |= BIT4;        // =  MC_1;
          /* Timer A mode control: 1 - Up to CCR0 */
            
                   
          /* Configuração DC0 */
          BCSCTL1 = CALBC1_1MHZ; 
          /* BCSCTL1 Calibration Data for 1MHz */
          
          DCOCTL = CALDCO_1MHZ;
          /* DCOCTL  Calibration Data for 1MHz */
          
          __enable_interrupt();
          /*__bis_SR_register(GIE); _BIS_SR(GIE); */

          
          while(1)
           {
             
             
             P2OUT = 0x01;                      // Conf. Anodos - unidade
             P1OUT =  display[uni];            // Seta bits do valor da unidade
             __delay_cycles(5000);              // Delay 5ms
             
             P2OUT = 0x02;                      // Conf. Anodos - dezena
             P1OUT =  display[dez];             // Seta bits do valor da dezena
             __delay_cycles(5000);              // Delay 5ms
             
             P2OUT = 0x04;                      // Conf. Anodos - centena
             P1OUT =  display[cent];            // Seta bits do valor da centena
             __delay_cycles(5000);              // Delay 5ms             
             
           }      
         
        }