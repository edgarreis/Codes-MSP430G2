//CHANAS Damien
//     |= ~
//           {           
//           }
#include <msp430.h>

void pulso_E(void)      //Enviar pulso (E)
{
    P1OUT &=~ 0x80;
    P1OUT |= 0x80;
    __delay_cycles(1000);             //Temporização de 1ms   // precisa ?????
    P1OUT &=~ 0x80;        
}

void enviar(unsigned char byte)
{     
    __delay_cycles(1000);             //Temporização de 1ms 
    P1OUT &=~ 0x40;       //setar RS = 0 -> comando
    P2OUT = byte;
    pulso_E();
    P2OUT = byte<<4;
    pulso_E();
}

void inicializar(void)
{           
    
    __delay_cycles(15000);             //Temporização de 15ms
    P1OUT &=~ 0x40;       //setar RS = 0 -> comando
    P2OUT = 0x30;
    pulso_E();  
    __delay_cycles(5000);             //Temporização de 5ms
    pulso_E();
    __delay_cycles(1000);             //Temporização de 1ms
    pulso_E();
    __delay_cycles(1000);             //Temporização de 1ms
    P2OUT = 0x20;
    pulso_E();
   
    enviar(0x28);
    enviar(0x08);
    enviar(0x01);
    enviar(0x06);
    enviar(0x0F);          

}

//----------------------------------------------------------------
//	Rotina para escrever um byte no LCD
//----------------------------------------------------------------
//void escrever_byte(unsigned char byte_escr)
//  {
//      __delay_cycles(1000);     // Temporizar 1ms
//      P1OUT     |= 0x40;          // RS = 1
//      P2OUT     = byte_escr;
//      pulso_E();
//      P2OUT     = (byte_escr << 4);
//      pulso_E();
//   }
//---------------------------------------------------------------------------
//	Rotina para enviar um byte para o LCD 
//---------------------------------------------------------------------------
//void enviar_byte( unsigned char byte_env )
//  {
//    escrever_byte(byte_env);        // escrever byte no LCD
//  }

//---------------------------------------------------------------------------
//	Rotina para enviar um texto 
//---------------------------------------------------------------------------
//void enviar_texto(const char *ptr)
//	{
//	while (*ptr)
//		{
//		enviar_byte(*ptr);
//		ptr++;
//		}
//	}


void   main(void)
{        
    //Configurar µC
    WDTCTL = WDTPW + WDTHOLD;           // Desativa o WDT watchdog
    P1DIR =  0xC0;                      //seta output direction para os bits  6 e 7 do P1
    P2DIR =  0xF0;                      //seta output direction para os bits  4 a 7 do P2
    P2SEL = 0x00; 
    P2SEL2 = 0x00;
    
    //Conf. DC0
    BCSCTL1 = CALBC1_1MHZ;             //Ajuste do DCO para 1MHz
    DCOCTL = CALDCO_1MHZ;              //Ajuste do DCO para 1MHz
    
    inicializar();  
    
    //enviar_texto("TE124 - 2014/2");
    //P2OUT = 0xC0 // = pulso_E, delays ...
   
    while(1)                              //infinite loop
    {
            
    }      
         
}








  
 