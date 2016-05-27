/// PROVA 1 - EX2 ///

/* 
ALUNO: EDGAR DOS REIS
GRR: 20111758

Descrição

O Subintervalos foram inseridos com CMP (comparação) e JLO (salto se for menor) para cada subfunção
das quais ligavam e desligavam os LEDs necessarios

*/


#include <msp430.h>             ; Definições do MSP430

#define timer   R4               ; Valor do Timer


// Alocação de Memória
ORG     0xFFFE                  ; Alteração do PC
DC16    main                    ; Define uma constante de 16 bits (main)


// Vetor de Interrupção
ASEG 0xFFE4                     ;endereço do flag de interrupção da porta P1
DC16 interrup_p1                ; cria a variavel interrupção, ocupando 1 lote de 16 bits

RSEG    CODE                    ; coloca o programa no segmento CODE


main:   MOV.W 	#0x0400,SP              ; Inicializar a pilha
        CALL    #setup                  ; Chama Função de Configuração
        BIC.B 	#0x41,&P1OUT 	        ; desliga P1.0 e P1.3               
        
loop:   JMP     loop                    ; Loop Infinito

           
           
// Interrupção do pino P1.3
interrup_p1:
        BIC.B 	#0x41,&P1OUT 	     ; desliga P1.0 e P1.6 por prevenção
        MOV.W   TA1R,timer            ; copia o valor do Timer na interrupção
        
        // Obs. o meu erro esta aqui no CMP e JN, tenho que ajusta-los para a comparação, mas não deu tempo, Mas está funcionando 

        CMP.B   0x3FFF,timer            ; 1ª comparação
        JL      offleds                 ; Se menor pula
        JEQ     offleds                 ; ou se for igual
        
        CMP.B   0x7FFF,timer            ; caso tenha não sido maior que a anterior( 0x3FFF), novo sub intervalo até 0x7FFF
        JL      on_green                 ; pulo
        JEQ     on_green
        
        CMP.B   0xBFFF,timer            ; caso tenha não sido maior que a anterior( 7FFF), novo sub intervalo até 0xBFFF
        JL      on_red                  ; Se menor pula
        JEQ     on_green                ; ou se for igual
        
        CALL    #onleds                  ; caso tenha não sido maior que a anterior( 0xBFFF), será maior e acenderá os 2 leds
        
        
        
fim:
        MOV.B   #0x00,P1IFG         ; Limpar flag de Interrupção
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
      
        // Configuração dos Pinos
        MOV.B #0x08,&P1REN              ; resistor enable faz a porta I/O
        BIC.B #0x08,&P1DIR              ; seta input direction p/ o bit 3
        MOV.B #0x41,&P1DIR              ; seta output direction p/ o bit 6 e 0
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3
        
        // Setup de Interrupção
        BIS.B   #0x08,&P1IE             ; Habilita Interrupção do Pino
        BIC.B   #0x08,&P1IES            ; Define a Borda de Subida
        BIC.B   #0x00,&P1IFG            ; Resetar a Flag do Pino          
                  
               
        // Setup do Timer A1
        MOV.W   #TASSEL_2+ID_3+MC_2,&TA1CTL
        /* TASSEL_2 - Timer A clock source select: 2 - SMCLK */
        /* ID_3     - Timer A input divider: 3 - /8 */
        /* MC_2     - Timer A mode control: 2 - Continous up */
        
        
        // Configuração de Frequencia 1 MHz
        MOV.B   CALBC1_1MHZ_,BCSCTL1    /* BCSCTL1 Calibration Data for 1MHz */
        MOV.B   CALDCO_1MHZ_,DCOCTL    /* DCOCTL  Calibration Data for 1MHz */
        
        
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrupções

        RET                     ; Retorna a função Main

  END