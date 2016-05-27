/// EXERCICIO 09 ///

#include "msp430.h" 

ORG     0xFFFE 
DC16    main                    ; seta o vetor de reset na etiqueta main
ASEG 0x0200
temporizador: DS16  1 ; cria a variavel temporizador, ocupando 1 lote de 16 bits
RSEG    CODE                    ; coloca o programa no segmento CODE
        
main:   MOV.W #0x0400,SP ; inicializar a pilha
        CALL  #setup
        MOV.B #0x01,R5 ; Estado anterior do botao
        MOV.B #0x00,R4 ; Flag pisca
        BIC.B #0x41,&P1OUT ; reseta o bit 6 e o bit 1

botao:  BIT.B #0x08,&P1IN    ; BOTAO PRESSIONADO?
        JNZ   bsolto           ; NAO - segue para verificar borda de descida
        CALL  #verifica_borda
        JMP   pula
bsolto: BIS.B #0x01,R5 ; seta o estado anterior do botao
pula:   BIT.B #0x01,R4
        JZ    botao
        CALL  #piscar
        JMP   botao

        //se o botao estiver pressionado
verifica_borda:  BIT.B #0x01,R5 ; ESTADO ANTERIOR DO BOTAO ERA ALTO?
                 JZ    volta    ; NAO - nao e borda de descida
                 BIC.B #0x01,R5 ; reseta o estado anterior do botao
                 XOR.B #0x01,R4 ; inverte o flag pisca
                 BIT.B #0x01,R4 ; PISCA LIGADO? - verifica se o flag pisca esta setado
                 JNZ   liga
                 BIC.B #0x41,&P1OUT
                 JMP   volta
liga:            BIS.B #0x01,&P1OUT
volta:           RET

        //inverte os leds
piscar:    CALL  #tempo 
           XOR.B #0x41,&P1OUT ; inverte os leds
           RET
        //contador de tempo
tempo:      MOV.W #0xF422,temporizador  ; valor necessário que multiplicado por 4 ciclos de máquina contabiliza 250 milisegundos
decremento: DEC.W temporizador          ; 1 ciclo de máquina -> quando tivermos 1-1=0 o flag de zero sera setado
            NOP                         ; 1 ciclo de máquina
            JNZ   decremento            ; 2 ciclos de máquina
            RET

        //setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
        MOV.B #0x08,&P1REN              ; resistor enable faz a porta I/O
        BIC.B #0x08,&P1DIR              ; seta o input no bit 3
        MOV.B #0x41,&P1DIR              ; seta output direction para o bit 6 e 0
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3
        
        RET           
  END 