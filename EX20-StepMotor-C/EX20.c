/// EXERCICIO 20 //

#include "msp430g2553.h"
    

/* Configurar uC */
void config_uC(void)
{
    WDTCTL  = WDTPW + WDTHOLD;  // Desabilita o watchdog
    BCSCTL1 = CALBC1_1MHZ;      // Configura��o do clock para 1MHz 
    DCOCTL  = CALDCO_1MHZ;      // Configura��o do clock para 1MHz 
   
    P1DIR  &= ~(BIT3);          // Entrada P1.3
        
    P2DIR  |=  (BIT1+BIT2+BIT3+BIT4);   // Saidas P2.1, P2.2, P2.3, P2.4
    P2OUT  &= ~(BIT1+BIT2+BIT3+BIT4);   // For�ar saidas em zero
    
/* Configurar interrup��o P1.3 */
    P1IE  |= 0x08;               // Habilita interrup��o do bo�o p1.3
    P1IES |= 0x08;		// Habilita interrup��o na borda de descida
    P1IFG  = 0x00;		// Zera a flag de interrup��o da porta 1
    
/* Configurar Timer0_A */
    TACTL    = TASSEL_2 + ID_1 + MC_1;  // clock= SMCLK, div= /2 contagem MC_1 = Up mode => TACCR0)
    TACCR0   = 5000;	           	// 5000 decimal * 1us => 5ms
    TACCTL0  = CCIE;		   	// Habilita captura/compara��o com rela��o a IFG
                 
    __bis_SR_register (GIE);    // Enter LPM0, interrup��es habilitado
    
}

int flag_sense    = 0;
int flag_posicion = 0;

int main (void){
  
    config_uC();
    while(1)                    // Loop Infinito
    {
      
    }

}  
         
/* Interrup��o ADC */
#pragma vector = TIMER0_A0_VECTOR      // Interrup��o de recebimento
__interrupt void interr_timer_A (void)
{
    if (flag_sense == 0)
    {
      
      if (flag_posicion == 0)
      {
        P2OUT |=  BIT1;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT3;
        P2OUT &= ~BIT4;
      }
      
      if (flag_posicion == 1)
      {
        P2OUT |=  BIT2;
        P2OUT &= ~BIT1;
        P2OUT &= ~BIT3;
        P2OUT &= ~BIT4;
      }
      
      if (flag_posicion == 2)
      {
        P2OUT |=  BIT3;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT1;
        P2OUT &= ~BIT4;
      }
      
      if (flag_posicion == 3)
      {
        P2OUT |=  BIT4;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT3;
        P2OUT &= ~BIT1;
      }
    }  
/******************************************************************************/    
    
    if (flag_sense == 1)
    {
      
      if (flag_posicion == 0)
      {
        P2OUT |=  BIT1;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT3;
        P2OUT &= ~BIT4;
      }
      
      if (flag_posicion == 1)
      {
        P2OUT |=  BIT4;
        P2OUT &= ~BIT1;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT3;
      }
      
      if (flag_posicion == 2)
      {
        P2OUT |=  BIT3;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT1;
        P2OUT &= ~BIT4;
      }
      
      if (flag_posicion == 3)
      {
        P2OUT |=  BIT2;
        P2OUT &= ~BIT1;
        P2OUT &= ~BIT2;
        P2OUT &= ~BIT3;
      }
    }  
    
/******************************************************************************/    
        
    flag_posicion++;
      
    if (flag_posicion == 4)
    {
      flag_posicion = 0;
    }
    
    P1IFG = 0x00;               // Zera a flag de interrup��o da porta 1
}


/* Interrup��o do P1.3 */
#pragma vector = PORT1_VECTOR		
__interrupt void interr_P1(void)
{
    /* DEBAUNCING */
    __delay_cycles(5000);               // delay de 5ms
	flag_sense ^= 1;		// garante que ir� entrar no while
	while (flag_sense == 1)
          if ((P1IN & 0x08) == 0)	// testa se o bot�o p1.3 est� pressionado
          {
            flag_sense = 0;		// muda estatus de x para sair do while	
          }
	else
	{				// se n�o estiver pressionado volta a conferir
          flag_sense = 1;
	}
	__delay_cycles(5000);		// delay de 5ms
		
    P1IFG = 0x00;       // Zera a flag de interrup��o da porta 1
}

       
       


