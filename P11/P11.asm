/// PROVA 1 - EX1 ///

/* 
ALUNO: EDGAR DOS REIS
GRR: 20111758

Descri��o

Como a frequencia de 12Mhz � muito alta para contar 0.5 S On/off
foi necessario ter 12 ciclos de timer:

f = (div*N)/(t)

dividindo a frequencia f = 12M Hz /12 = 1 Mhz (nova frequencia), com N = 62500
para se obter a nova frequencia � necessario uma flag para contar regressivamente
(flag_freq) = 12 (divisor), ou seja somente quando o flag chega em Zero � que os leds ir�o 
inverter, conforme abaixo:

*/


#include <msp430.h>             ; Defini��es do MSP430

#define flag_freq   R4         ; Registrador Para o Flag Pisca


// Aloca��o de Mem�ria
ORG     0xFFFE                  ; Altera��o do PC
DC16    main                    ; Define uma constante de 16 bits (main)

// Vetores de Interrup��o
ORG    0x0FFF2                  ; Endere�o do TA0 (0x0FFF2) / TA1 (0x0FFFA)
DC16    interrup_TA0            ; Vetor de Interrup��o TIMER A

RSEG    CODE                    ; coloca o programa no segmento CODE

main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha
        CALL    #setup                  ; Chama Fun��o de Configura��o
        BIC.B 	#0x41,&P1OUT 	        ; desliga P1.0 e P1.3
        MOV.W   #0xC,flag_freq           ; Contador inicia em 12 (1100)                 
        
loop:   JMP     loop                    ; Loop Infinito

        // Interrup��o TA0     
interrup_TA0:
        SUB.B   #0x01,flag_freq        ; Decremento
        JNZ     decremento
        MOV.W   #0xC,flag_freq           ; reinicia  em 12 (1100)
        XOR.B   #0x41,&P1OUT            ; inverte os leds (XOR)
               
decremento:
        BIC.W   #CCIFG,&TA0CCTL0        ; Limpar flag de interrup��o (bit clear)
        RETI
                      
                
        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
      
        // Configura��o dos Pinos
        MOV.B #0x41,&P1DIR              ; seta output direction p/ o bit 6 e 0
                  
                  
               
        // Setup do Timer A0
        MOV.W   #TASSEL_2+ID_3+MC_1,&TA0CTL
        /* TASSEL_2 - Timer A clock source select: 2 - SMCLK */
        /* ID_3     - Timer A input divider: 3 - /8 */
        /* MC_1     - Timer A mode control: 1 - Up to CCR0 */
        
        
        
        MOV.W   #CCIE,&TA0CCTL0
        /* CCIE     - Capture/compare interrupt enable */
        
        MOV.W   #62500,TA0CCR0  ; Registrador de compara��o setado em 62500 (0.5 s)
  
        
        // Configura��o de Frequencia 12 MHz
        MOV.B   CALBC1_12MHZ_,BCSCTL1    /* BCSCTL1 Calibration Data for 12MHz */
        MOV.B   CALDCO_12MHZ_,DCOCTL    /* DCOCTL  Calibration Data for 12MHz */
        
        
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrup��es

        RET                     ; Retorna a fun��o Main

  END