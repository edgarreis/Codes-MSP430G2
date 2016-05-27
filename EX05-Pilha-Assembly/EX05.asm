/// EXERCICIO 05 ///

#include <msp430.h>             // Definições do MSP430

        ORG     0xFFFE          // Alteração do PC
        DC16    main            // Define uma constante de 16 bits (main)
        RSEG    CODE            // Realocação de Dados da memória 
       
main:   mov.w   #0x0400,SP              // Inicializar o Stack Pointer
        mov.w   #WDTPW + WDTHOLD,&WDTCTL// Desativa o WDT
        
        // Alocação  em Memórias
        mov.w   #0x41,R10       // R10 Contém o byte a ser armazenado ('A')
        mov.w   #0x200,R12      // R12 Contém o endereço (0x200)
        
        // Loop de Verificação
verif:  mov.b   R10,0(R12)      // Move o Conteudo de R10 ('A','B'...) no endereço de R12 (apartir de 0x200)
        
        // Verificação de igualdade
        cmp.b   #0x5A,R10       // Compara #5A ('Z') com o conteudo de R10
        jz      fim             // Finaliza se for igual ('Z') = ('Z')
        
        // Incrementos
        add.w   #1,R10          // Incremento
        add.w   #1,R12          // Incremento  
        
        //Loop main        
        jmp     verif           // Retorna no inicio do loop verif
        
        //Finaliza com R10 = ('Z')
fim:    jmp     fim             // Laço infinito(jmp $)

        end
          
          