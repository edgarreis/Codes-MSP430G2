/// EXERCICIO 13 ///

#include <msp430g2533.h>             ; Defini��es do MSP430G2533

// Aloca��o de Mem�ria
ORG     0xFFFE                  ; Altera��o do PC para o Final da mem�ria
DC16    main                    ; Define uma constante de 16 bits (main)

// Vetores de Interrup��o

ORG     0XFFE4                  ; (ASEG 0XFFE4)
DC16    interrup_P1             ; Vetor de Interrup��o Porta P1

RSEG    CODE                    ; coloca o programa no segmento CODE

main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha (Stack pointer)
        CALL    #setup                  ; Chama Fun��o de Configura��o Geral
        
//loop:   JMP     loop                  ; Loop Infinito
        MOV.B   #LPM4+GIE,SR            ; Chave geral de interrup��es
        
        // Interrup��o Port 1
interrup_P1:
        XOR.B   #0x08,&P1IES            ; Define a Borda
        stop:   BIC.B   #BIT3,&P1IFG    ; Limpar flag de interrup��o (bit clear no P1.3)
        
        BIT.B   #0x08,P1IN              ; Verifica o estado do P1.3 (FUN��O AND)
        JNZ     on                      ; Se for 1 - Desligar Leds
        BIS.B   #0x41,P1OUT             ; Liga Leds
        JMP     exit
on      BIC.B   #0x41,P1OUT             ; Se for 0 - Liga Leds


exit        RETI          
        
            
        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; Desabilita o watchdog
      
        // Configura��o dos Pinos
        MOV.B #0x08,&P1REN              ; resistor enable faz a porta I/O
        BIC.B #0x08,&P1DIR              ; Seta input direction P1.3
        MOV.B #0x41,&P1DIR              ; seta output direction P1.0 e P1.6
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3

        // Setup de Interrup��o
        BIS.B   #0x08,&P1IE             ; Habilita Interrup��o do Pino
        BIS.B   #0x08,&P1IES            ; Define a Borda de Decida
        BIC.B   #0x00,&P1IFG            ; Resetar a Flag do Pino
        
                
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrup��es

        RET                     ; Retorna a fun��o Main

  END