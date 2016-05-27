/// EXERCICIO 08 ///

#include <msp430.h>             // Definições do MSP430

#define cont    R10

        ORG     0xFFFE          // Alteração do PC
        DC16    main            // Define uma constante de 16 bits (main)
        RSEG    CODE            // Realocação de Dados da memória 
 
        // Função Principal
main:   mov.w   #0x0400,SP              // Inicializar o Stack Pointer
        mov.w   #WDTPW + WDTHOLD,&WDTCTL// Desativa o WDT
        
        // Configuração dos Pinos
        mov.b   #0x41,&P1DIR    // Direção dos Pinos P1.6 E P1.0 OUT (#0x41 = 0100 0001)
                
        // Setar LEDs
        bis.b   #0x01,&P1OUT    // Liga P1.0 (#0x01 = 000 0001)
        bic.b   #0x040,&P1OUT   // Desliga P1.6 (#0x040 = 0100 0000)
        
loop:   call    #invert
        jmp     loop
        
        // Loop de Verificação
invert: call    #delay
        xor.b   #0x041,&P1OUT   // Inverte os LEDs 0->1 ... 1->0
        ret
        
        // Tempo Ocioso
delay:  mov.w   #62498,cont //62498
decr:   nop
        dec.w   cont
        jnz     decr
        ret
 end
          
          