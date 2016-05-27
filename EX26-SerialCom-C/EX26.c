/// EXERCICIO 26 //

#include "msp430g2553.h"

/* Configurar uC */
void config_uC(void)
{
    WDTCTL    = WDTPW + WDTHOLD;  // Desabilita o watchdog
    BCSCTL1   = CALBC1_8MHZ;      // Configuração do clock para 8MHz 
    DCOCTL    = CALDCO_8MHZ;      // Configuração do clock para 8MHz 
    
    P1DIR     = 0x41;
    P1SEL     = (BIT1 + BIT2);        // Selecionar Fun��o Uart nos pinos P1.1 e P1.2
    P1SEL2    = (BIT1 + BIT2);        // Selecionar Fun��o Uart nos pinos P1.1 e P1.2
       
    UCA0CTL1 |= UCSWRST;        // habilitar Uart para configura��o
    UCA0CTL0  = 0x00;           // USCI A0 Control Register 0
    UCA0CTL1 |= UCSSEL_2;       // USCI A0 Control Register 1 >> USCI 0 Clock Source: 2 (0x80)
    UCA0MCTL  = UCBRS_3;        // Parte Fracionaria da divis�o
    UCA0BR0   = 0x41;           // USCI A0 Baud Rate 1 - Parte inteira  (0x341)
    UCA0BR1   = 0x03;           // USCI A0 Baud Rate 0 - Parte inteira  (0x341)
    UCA0CTL1 &= ~UCSWRST;       // Desabilitar Uart para configura��o
    IE2      |= UCA0RXIE;       // Interrupt Enable 2 >> (0x01)
    
    __bis_SR_register (GIE);    // Enter LPM0, interrup��es habilitado
}


/* Envia Byte */
void send_byte( unsigned char byte_send )
{
    while(!(IFG2 & UCA0TXIFG));    // UCA0TXIFG seta quando o buffer esta cheio  
    UCA0TXBUF = byte_send;        // Enviar Bytes para LCD
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
       

unsigned char buffer;           // Variavel para o Buffer
int flag = 0;                   // Usado para verificar se o buffer esta cheio


int main (void){
  
    config_uC();
    P1OUT = 0x01;
    while(1);                       // Loop Infinito

}  
         
/* Interrup��o Uart */
 #pragma vector = USCIAB0RX_VECTOR      // Interrup��o de recebimento
    __interrupt void USCI0RX_ISR (void)
{
    buffer = UCA0RXBUF;      // Buffer recebe byte recebido      
    P1OUT ^= 0x41;
    
    if(flag == 0 )           // Se o Buffer estiver vazio aguarda carregar
    {
        send_text("Interface Serial");
        send_byte(0x0D);       // pula linha
        send_byte(0x0A);       // inicio da linha
        send_byte('>');        // Caracter
        send_byte(buffer);     // Reenvia o que esta no buffer
        
        flag = 1;              // 

    }
    else
    {
        send_byte(0x0D);       // pula linha
        send_byte(0x0A);       // inicio da linha
        send_byte('>');        // Caracter
        send_byte(buffer);     // Reenvia o que esta no buffer
         
    }
       
}
       
       

