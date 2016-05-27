/// EXERCICIO 10 ///

#include "msp430.h"     ; Definições do MSP430

#define flag_blink   R4         ; Registrador Para o Flag Pisca

// Alocação de Memória
ORG     0xFFFE                  ; Alteração do PC
DC16    main                    ; Define uma constante de 16 bits (main)

// Alocação para Variável
ASEG 0x0200
timer: DS16  1 ; cria a variavel temporizador, ocupando 1 lote de 16 bits

// Vetor de Interrupção
ASEG 0xFFE4          ;endereço do flag de interrupção da porta P1
DC16 interrup_p1  ; cria a variavel interrupção, ocupando 1 lote de 16 bits

RSEG    CODE                    ; insere o programa no segmento CODE

      //Rotina Principal
main:   MOV.W   #0x0400,SP              ; Inicialização do apontador da pilha
        CALL    #setup                  ; Call p/ setup microcontrolador
        MOV.B   #0x00,flag_blink        ; Flag pisca = 0x00
        BIC.B 	#0x41,&P1OUT 	        ; Apaga leds
verif:  BIT.B   #0x01,flag_blink        ; verifica se flag_blink esta setado
        JZ      verif                   ; Se não estiver, continua testando
        CALL    #blink_led              ; Se flag_blink estiver setado chama função blink_led
        JMP     verif                   ; Retorna a função verif


        // Rotina Piscar Led's
blink_led:
        XOR.B   #0x41,&P1OUT    ; Inverte os leds (XOR)
        CALL    #temp           ; Chama temporização
        RET
        
        // Contador Pisca Led's
temp:   MOV.W   #62498,timer    ; valor necessário que multiplicado por 4 ciclos de máquina contabiliza 250 milisegundos
decrement:
        DEC.W   timer           ; decremento de 1 no timer (1 machine-cycle)
        NOP                     ; Not Operation(1 machine-cycle)
        JNZ     decrement       ; Se não for zero decrementa(2 machine-cycle)
        RET                     ; se for zero retorna a função blink_led
              
        // Interrupção do pino P1.3
interrup_p1: 
        XOR.B   #0x01,flag_blink        ; inverte o flag pisca
        AND.B   #0x01,flag_blink        ; Verifica se flag_blink esta setado (ou Operação BIT)
        JZ      clear_leds              ; Se o flag_blink estiver em 0 clear_leds
        BIS.B   #0x01,&P1OUT            ; Senão acende led vermelho 
        JMP     clear_flag              ; Limpa a flag de interrupção ao final

clear_leds:
        BIC.B       #0x41,&P1OUT        ; Apaga os LEDs
clear_flag:
        MOV.B       #0x00,P1IFG         ; Limpar flag de Interrupção 
        RETI                            ; Retorna a função main

        // Setup do microcontrolador
setup:  MOV.W #WDTPW+WDTHOLD,&WDTCTL    ; desabilita o watchdog
      
        // Configuração dos Pinos
        MOV.B #0x08,&P1REN              ; resistor enable faz a porta I/O
        BIC.B #0x08,&P1DIR              ; seta input direction p/ o bit 3
        MOV.B #0x41,&P1DIR              ; seta output direction p/ o bit 6 e 0
        BIS.B #0x08,&P1OUT              ; Seta Resistor pullup P1.3
        
        // Setup de Interrupção
        BIS.B   #0x08,&P1IE             ; Habilita Interrupção do Pino
        BIS.B   #0x08,&P1IES            ; Define a Borda de Decida
        BIC.B   #0x00,&P1IFG            ; Resetar a Flag do Pino
       
        // Setup GIE
        BIS.W   #GIE,SR         ; Habilita Interrupções

        RET                     ; Retorna a função Main

end      
       