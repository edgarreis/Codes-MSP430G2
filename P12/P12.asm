/// PROVA 1 - EX2 ///

/* 
ALUNO: EDGAR DOS REIS
GRR: 20111758

Descri��o

O Subintervalos foram inseridos com CMP (compara��o) e JLO (salto se for menor) para cada subfun��o
das quais ligavam e desligavam os LEDs necessarios

*/


#include <msp430.h>             ; Defini��es do MSP430

#define timer   R4               ; Valor do Timer


// Aloca��o de Mem�ria
ORG     0xFFFE                  ; Altera��o do PC
DC16    main                    ; Define uma constante de 16 bits (main)


// Vetor de Interrup��o
ASEG 0xFFE4                     ;endere�o do flag de interrup��o da porta P1
DC16 interrup_p1                ; cria a variavel interrup��o, ocupando 1 lote de 16 bits

RSEG    CODE                    ; coloca o programa no segmento CODE


main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha
        CALL    #setup                  ; Chama Fun��o de Configura��o
        BIC.B 	#0x41,&P1OUT 	        ; desliga P1.0 e P1.3               
        
loop:   JMP     loop                    ; Loop Infinito

           
           
// Interrup��o do pino P1.3
interrup_p1:
        BIC.B 	#0x41,&P1OUT 	     ; desliga P1.0 e P1.6 por preven��o
        MOV.W   TA1R,timer            ; copia o valor do Timer na interrup��o
        
        // Obs. o meu erro esta aqui no CMP e JN, tenho que ajusta-los para a compara��o, mas n�o deu tempo, Mas est� funcionando 

        CMP.B   0x3FFF,timer            ; 1� compara��o
        JL      offleds                 ; Se menor pula
        JEQ     offleds                 ; ou se for igual
        
        CMP.B   0x7FFF,timer            ; caso tenha n�o sido maior que a anterior( 0x3FFF), novo sub intervalo at� 0x7FFF
        JL      on_green                 ; pulo
        JEQ     on_green
        
        CMP.B   0xBFFF,timer            ; caso tenha n�o sido maior que a anterior( 7FFF), novo sub intervalo at� 0xBFFF
        JL      on_red                  ; Se menor pula
        JEQ     on_green                ; ou se for igual
        
        CALL    #onleds                  ; caso tenha n�o sido maior que a anterior( 0xBFFF), ser� maior e acender� os 2 leds
        
        
        
fim:
        MOV.B   #0x00,P1IFG         ; Limpar flag de Interrup��o
        RETI
     
     
/////////     
offleds:
       BIC.B 	#0x41,&P1OUT 	        ; desliga P1.0 e P1.6
       JMP      fim
       
/////////
on_green:
       BIS.B 	#0x40,&P1OUT 	        ; liga P1.6 (verde)
       BIC.B 	#0x01,&P1OUT 	        ; desliga P1.0 (vermelho)
       JMP      fim
       
/////////
on_red:
      BIC.B 	#0x40,&P1OUT 	        ; desliga P1.6 (verde)
      BIS.B 	#0x01,&P1OUT 	        ; liga P1.0 (vermelho)  
      JMP      fim
 
 ///////
 onleds:
      BIS.B 	#0x41,&P1OUT 	        ; liga P1.0 e P1.6 
      RET


        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
      
        // Configura��o dos Pinos
        MOV.B #0x08,&P1REN              ; resistor enable faz a porta I/O
        BIC.B #0x08,&P1DIR              ; seta input direction p/ o bit 3
        MOV.B #0x41,&P1DIR              ; seta output direction p/ o bit 6 e 0
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3
        
        // Setup de Interrup��o
        BIS.B   #0x08,&P1IE             ; Habilita Interrup��o do Pino
        BIC.B   #0x08,&P1IES            ; Define a Borda de Subida
        BIC.B   #0x00,&P1IFG            ; Resetar a Flag do Pino          
                  
               
        // Setup do Timer A1
        MOV.W   #TASSEL_2+ID_3+MC_2,&TA1CTL
        /* TASSEL_2 - Timer A clock source select: 2 - SMCLK */
        /* ID_3     - Timer A input divider: 3 - /8 */
        /* MC_2     - Timer A mode control: 2 - Continous up */
        
        
        // Configura��o de Frequencia 1 MHz
        MOV.B   CALBC1_1MHZ_,BCSCTL1    /* BCSCTL1 Calibration Data for 1MHz */
        MOV.B   CALDCO_1MHZ_,DCOCTL    /* DCOCTL  Calibration Data for 1MHz */
        
        
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrup��es

        RET                     ; Retorna a fun��o Main

  END