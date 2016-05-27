/// EXERCICIO 07 ///

#include <msp430.h>             // Definições do MSP430

        ORG     0xFFFE          // Alteração do PC
        DC16    main            // Define uma constante de 16 bits (main)
        RSEG    CODE            // Realocação de Dados da memória 
 
        // Função Principal
main:   mov.w   #0x0400,SP              // Inicializar o Stack Pointer
        mov.w   #WDTPW + WDTHOLD,&WDTCTL// Desativa o WDT
        
        // Configuração dos Pinos
        mov.b   #0x41,&P1DIR    // Direção dos Pinos P1.6 E P1.0 OUT (#0x41 = 0100 0001)
        bis.b   #0x08,&P1REN    // Habilita Resistor pullup P1.3
        bis.b   #0x08,&P1OUT    // Seta Resistor pullup P1.3
        
        // Setar LEDs
        bis.b   #0x01,&P1OUT    // Liga P1.0 (#0x01 = 000 0001)
        bic.b   #0x040,&P1OUT   // Desliga P1.6 (#0x040 = 0100 0000)
                
        // Loop de Verificação
loop:   bit.b   #0x08,&P1IN     // Testa 0/1 no P1.3
        JZ      on              // Botão Apertado
        JNZ     off             // Botão Solto                    
   
        //Loop main        
        jmp     loop           // Retorna no inicio do loop
       
        // Botão Apertado
 on:    bis.b   #0x040,&P1OUT  // Liga P1.6 (#0x040 = 0100 0000)
        bic.b   #0x01,&P1OUT   // Desliga P1.0 (#0x01 = 000 0001)
        
        //Loop main        
        jmp     loop           // Retorna no inicio do loop
 
        // Botão Solto
 off:   bis.b   #0x01,&P1OUT    // Liga P1.0 (#0x01 = 000 0001)
        bic.b   #0x040,&P1OUT   // Desliga P1.6 (#0x040 = 0100 0000)

        //Loop main        
        jmp     loop           // Retorna no inicio do loop
         
 end
          
          