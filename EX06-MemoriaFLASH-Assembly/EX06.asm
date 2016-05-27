/// EXERCICIO 06 ///

#include <msp430.h>             // Defini��es do MSP430

//#define nome    

        ORG     0xFFFE          // Altera��o do PC
        DC16    main            // Define uma constante de 16 bits (main)
        RSEG    CODE            // Realoca��o de Dados da mem�ria 
 
         // Nome a ser escrito
nome:    DB      "Edgar dos Reis - "                    // DB(Define Byte)
         DB      "Universidade Federal do Parana",0x01  // 0x01 Crit�rio de Parada

        // Fun��o Principal
main:   mov.w   #0x0400,SP              // Inicializar o Stack Pointer
        mov.w   #WDTPW + WDTHOLD,&WDTCTL// Desativa o WDT
        
        // Aloca��o  em Mem�rias
        mov.w   #nome,R10       // R10 Aponta para conteudo de Nome
        mov.w   #0x200,R12      // R12 Aponta para o endere�o RAM (0x200)
             
        // Loop de Verifica��o
verif:  mov.b   @R10,0(R12)     // Move o Conteudo de R10 ('E','D'...) no endere�o de R12 (apartir de 0x200)
        
        // Verifica��o de igualdade
        cmp.b   #0x01,0(R10)    // Compara #0x01 (1) com o conteudo apontado por R10
        jz      fim             // Finaliza se for igual (#0x01) = (#0x01)
        
        // Incrementos
        add.w   #1,R10          // Incremento
        add.w   #1,R12          // Incremento  
        
        //Loop main        
        jmp     verif           // Retorna no inicio do loop verif
        
        //Finaliza com o conteudo apontado de R10 = (0x01)
fim:    jmp     fim             // La�o infinito(jmp $)

        end
          
          