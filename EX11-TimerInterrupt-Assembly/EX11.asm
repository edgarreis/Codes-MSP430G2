/// EXERCICIO 11 ///

#include <msp430.h>             ; Defini��es do MSP430

#define flag_blink   R4         ; Registrador Para o Flag Pisca

// Aloca��o de Mem�ria
ORG     0xFFFE                  ; Altera��o do PC
DC16    main                    ; Define uma constante de 16 bits (main)

// Vetores de Interrup��o
ORG     0XFFE4                  ; (ASEG 0XFFE4)
DC16    interrup_P1             ; Vetor de Interrup��o Porta P1

ORG     0x0FFF2                 ; Endere�o do TA0 (0x0FFF2)
DC16    interrup_TA0            ; Vetor de Interrup��o TA0

RSEG    CODE                    ; coloca o programa no segmento CODE

main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha
        CALL    #setup                  ; Chama Fun��o de Configura��o
        MOV.B   #0x00,flag_blink        ; Flag pisca = 0x00
        BIC.B 	#0x41,&P1OUT 	        ; Apaga leds
loop:   JMP     loop                    ; Loop Infinito

        // Interrup��o Timer A0       
interrup_TA0:
        XOR.B   #0x41,&P1OUT            ; inverte os leds (XOR)
        BIC.W   #CCIFG,&TA0CCTL0        ; Limpar flag de interrup��o (bit clear)
        BIC.W   #BIT4,&TA0CTL           ; Para o Timer (TA0CTL = 0) *****************************************
        RETI

        // Interrup��o Port 1
interrup_P1:
        XOR.B   #0x01,flag_blink        ; inverte o flag pisca
        AND.B   #0x01,flag_blink        ; Verifica se flag_blink esta setado (ou Opera��o BIT)
        JNZ     set_timer               ; Se flag_blink = 1 Inicia Timer
        BIC.W   #BIT4,&TA0CTL           ; Para o Timer (TA0CTL = 0)
        BIC.B   #0x41,&P1OUT            ; Apaga os LEDs
        JMP     stop
        
set_timer:
        BIS.W   #BIT4,&TA0CTL           ; Inicia Timer modo UP
        BIS.B   #0x01,&P1OUT            ; acende o led vermelho
stop:   BIC.B   #BIT3,&P1IFG            ; Limpar flag de interrup��o (bit clear)
        RETI     

        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
      
        // Configura��o dos Pinos
        MOV.B #0x08,&P1REN              ; resistor enable P1.3
        BIC.B #0x08,&P1DIR              ; seta input direction P1.3
        MOV.B #0x41,&P1DIR              ; seta output direction P1.0 e P1.6
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3
        
        // Setup de Interrup��o
        BIS.B   #0x08,&P1IE             ; Habilita Interrup��o do Pino
        BIS.B   #0x08,&P1IES            ; Define a Borda de Decida
        BIC.B   #0x00,&P1IFG            ; Resetar a Flag do Pino
        
        // Setup do Timer A
        MOV.W   #TASSEL_2+ID_2+MC_0,&TA0CTL
        /* TASSEL_2 - Timer A clock source select: 2 - SMCLK */
        /* ID_2     - Timer A input divider: 2 - /4 */
        /* MC_0     - Timer A mode control: 0 - Stop */

        MOV.W   #CCIE,&TA0CCTL0
        /* CCIE     - Capture/compare interrupt enable */
  
        MOV.W   #62500,TA0CCR0  ; Registrador de compara��o setado em 62500 (25ms)
        
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrup��es

        RET                     ; Retorna a fun��o Main

  END