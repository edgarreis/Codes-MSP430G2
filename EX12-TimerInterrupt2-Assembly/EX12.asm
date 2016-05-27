/// EXERCICIO 12 ///

#include <msp430.h>             ; Defini��es do MSP430


// Aloca��o de Mem�ria
ORG     0xFFFE                  ; Altera��o do PC
DC16    main                    ; Define uma constante de 16 bits (main)

// Vetores de Interrup��o
ORG    0x0FFFA                  ; Endere�o do TA1 (0x0FFFA)
DC16    interrup_TA1            ; Vetor de Interrup��o TIMER A

RSEG    CODE                    ; coloca o programa no segmento CODE

main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha
        CALL    #setup                  ; Chama Fun��o de Configura��o
        BIC.B 	#0x41,&P1OUT 	        ; desliga P1.0 e P1.3
        BIS.B 	#0x01,&P1OUT 	        ; liga P1.0
loop:   JMP     loop                    ; Loop Infinito

        // Interrup��o Timer A1       
interrup_TA1:
        XOR.B   #0x41,&P1OUT            ; inverte os leds (XOR)
        BIC.W   #CCIFG,&TA1CCTL0        ; Limpar flag de interrup��o (bit clear)
        RETI
        
        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
      
        // Configura��o dos Pinos
        MOV.B #0x41,&P1DIR              ; seta output direction p/ o bit 6 e 0
                  
        // Setup do Timer A1
        MOV.W   #TASSEL_2+ID_3+MC_1,&TA1CTL
        /* TASSEL_2 - Timer A clock source select: 2 - SMCLK */
        /* ID_3     - Timer A input divider: /8    */
        /* MC_1     - Timer A mode control: 1 - UP */
        
        MOV.W   #CCIE,&TA1CCTL0
        /* CCIE     - Capture/compare interrupt enable */
        
        MOV.W   #62500,TA1CCR0  ; Registrador de compara��o setado em 62500 (25ms)
        
     
        //MOV.B   #DCO0,&DCOCTL
        //MOV.B   #RSEL2+RSEL1+RSEL0,&BCSCTL1
        
        //MOV.B   #DCO1+DCO0,&DCOCTL
        //MOV.B   #RSEL3+RSEL1+RSEL0,&BCSCTL1
        
        //MOV.B   #DCO2,&DCOCTL
        //MOV.B   #RSEL3+RSEL2+RSEL0,&BCSCTL1
        
        MOV.B   #DCO2,&DCOCTL
        MOV.B   #RSEL3+RSEL2+RSEL1,&BCSCTL1
        
        
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrup��es

        RET                     ; Retorna a fun��o Main

  END