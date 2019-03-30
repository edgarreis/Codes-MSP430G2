/// Exercise 26 //

// This program basically Received a Uart interrupt by USCI0RX_ISR, transfer Buffer in a loop until is full, then send it back to UART by UCA0TXBUF cyclically


#include "msp430g2553.h"            // My Launchpad

/* Config uC */
void config_uC(void)
{
    WDTCTL    = WDTPW + WDTHOLD;  // Disable watchdog
    BCSCTL1   = CALBC1_8MHZ;      // Configure clock to 8MHz 
    DCOCTL    = CALDCO_8MHZ;      // Configure clock to 8MHz 
    
    P1DIR     = 0x41;               // Directions of pins (Output)
    P1SEL     = (BIT1 + BIT2);        // Select Uart function to pins P1.1 e P1.2
    P1SEL2    = (BIT1 + BIT2);        // Select Uart function to pins P1.1 e P1.2
       
    UCA0CTL1 |= UCSWRST;        // Enable Uart to configure
    UCA0CTL0  = 0x00;           // USCI A0 Control Register 0
    UCA0CTL1 |= UCSSEL_2;       // USCI A0 Control Register 1 >> USCI 0 Clock Source: 2 (0x80)
    UCA0MCTL  = UCBRS_3;        // Division Fractional Part
    UCA0BR0   = 0x41;           // USCI A0 Baud Rate 1 - entire part  (0x341)
    UCA0BR1   = 0x03;           // USCI A0 Baud Rate 0 - entire part  (0x341)
    UCA0CTL1 &= ~UCSWRST;       // Disable  Uart to configure
    IE2      |= UCA0RXIE;       // Interrupt Enable 2 >> (0x01)
    
    __bis_SR_register (GIE);    // Enter LPM0, Enable Interrupts
}


/* Send Byte */
void send_byte( unsigned char byte_send )
{
    while(!(IFG2 & UCA0TXIFG));    // UCA0TXIFG set when buffer is full  
    UCA0TXBUF = byte_send;        // send Bytes to LCD
}

/* Send Text (Using Pointer) */
void send_text(const char *ptr)
{
    while (*ptr)
    {
        send_byte(*ptr);        // Run pointer in the byte to be sent
        ptr++;                  // Next letter of word
    }
}
       

unsigned char buffer;           // Variable of Buffer
int flag = 0;                   // Used to check if buffer is full

int main (void){
  
    config_uC();                // Function config uC
    P1OUT = 0x01;               // turn on the led 
    while(1);                   // Infinit Loop

}  
         
/* Interrupt Uart */
 #pragma vector = USCIAB0RX_VECTOR      // Received Interrupt
    __interrupt void USCI0RX_ISR (void) // Received Interrupt
{
    buffer = UCA0RXBUF;      // transfers register to Buffer
    P1OUT ^= 0x41;           // Blink led 
    
    if(flag == 0 )           // If the Buffer is empty wait load...
    {
        send_text("Serial Interface");  // Send text
        send_byte(0x0D);       // jump line
        send_byte(0x0A);       // start of line
        send_byte('>');        // Character
        send_byte(buffer);     // Resend what is in buffer
        
        flag = 1;              // Buffer is full

    }
    else
    {
        send_byte(0x0D);       // jump line
        send_byte(0x0A);       // start of line
        send_byte('>');        // Character
        send_byte(buffer);     // Resend what is in buffer
         
    }
       
}
       
       


