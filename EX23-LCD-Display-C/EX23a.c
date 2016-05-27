/// EXERCICIO 23 ///								    

#include <msp430.h>

#define	RS	0x40            // Mascara do pino P1.6
#define	E	0x80		// Mascara do pino P1.7
#define D7	0x80		// Mascara do pino P2.7
#define D6	0x40		// Mascara do pino P2.6 
#define D5	0x20		// Mascara do pino P2.5
#define D4	0x10		// Mascara do pino P2.4
	
/* ConfiguraÁ„o uC */
void config_uC(void)
{
    WDTCTL  = WDTPW + WDTHOLD;  // Desabilita o watchdog
    BCSCTL1 = CALBC1_1MHZ;      // Configura√ß√£o do clock para 1MHz 
    DCOCTL  = CALDCO_1MHZ;      // Configura√ß√£o do clock para 1MHz 
    P1DIR  |= 0xC0;        	// Ajusta os pinos RS e E como sa√≠da  
    P2DIR  |= 0xF0;	// Ajusta os pinos D4...D7 como sa√≠da ---------VERIFICAR
    P2SEL   = 0x00;             // Configura a saida do pino como output
    P2SEL2  = 0x00;             // Pois por default ele tem outra funÁao P2.6 e P2.7
}

/* Enviar pulso enable */
void enable(void)
{
    P1OUT &=~ 0x80;
    P1OUT |= 0x80;
    //__delay_cycles(1000);
    P1OUT &=~ 0x80;        
}

/* Enviar Dados/Comandos LCD */
void send(unsigned char byte)
{     
    __delay_cycles(1000);       // TemporizaÁ„o de 1ms 
    P2OUT = byte;               // Envia Nibble + signif.
    enable();                   // Pulso para envio
    P2OUT = byte << 4;          // Envia Nibble - signif.
    enable();                   // Pulso para envio
}

/* InicializaÁ„o LDC */
void startup(void)
{
        __delay_cycles(15000);          // Delay de 15ms
        P1OUT &= ~0x40;		        // RS = 0, enviar comando
        
          P2OUT = 0x30;	        // Envia nibble(+) 0011, do comando 0x30
          enable();                     // Pulso para envio
                      
          __delay_cycles(5000);	        // Delay de 5ms
          enable();                     // Pulso para envio
                  
          __delay_cycles(1000);		// Delay de 1ms 
          enable();                     // Pulso para envio
          
          __delay_cycles(1000);		// Delay de 1ms 
          P2OUT = 0x20; 
          enable();                     // Pulso para envio
                
                          
          send(0x28);                  // Envia Comando 0x28   
          send(0x08);                     // Envia Comando 0x08
          send(0x01);                     // Envia Comando 0x01
          send(0x06);                  // Envia Comando 0x06
          send(0x0F);            // Envia Comando 0x0F ---------VERIFICAR
          
}


/******************************************************************************/
/******************************************************************************/
/* Envia Byte */
void send_byte( unsigned char byte_send )
  {
    P1OUT |= RS;		// RS = 1, enviar envia dado
    send(byte_send);            // Enviar Bytes para LCD
  }

/* Enviar Texto (Utilizando Ponteiro) */
void send_text(const char *ptr)
{
    while (*ptr)
    {
	send_byte(*ptr);        // Correr ponteiro no byte a ser enviado
	ptr++;                  // Proxima letra da palavra
    }
}


/* PROGRAMA PRINCIPAL */
void main(void)			      	
{
        config_uC();
        startup();
        
        send_text( " TEST TEST TEST ");
        P1OUT &= ~RS;		        // RS = 0, enviar comando
        send(0xC0);                     // Modificar o endereco de escrita DDRAM (end. 40)
        send_text( " test test test ");
        while(1);                       // Loop Infinito
}


	
  
