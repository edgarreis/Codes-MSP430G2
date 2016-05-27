/// EXERCICIO 04 ///

#define   var_1   0x0206        // Var_1 = 0x0206

          ORG     0xFFFE        // Alteração do PC
          DC16    main          // Define uma constante de 16 bits (main)
                   
                  
          ASEG    0x0200        // Reservar espaço na memória de dados
var_2:    DS8     1             // Aloca um bloco de 8 bit para var_2   // 0x0200 (VERIFICAR)
          align   1             // Alinha no prox. end. de Par (word)
var_3:    DS16    1             // Aloca um bloco de 16 bit para var_3  // 0x0202 (VERIFICAR)

          RSEG    CODE          // Realocação de Dados da memória 
           
          // Alocação de Endereços em Memórias
main:     mov.w   #0x0240,R5    // R5  = End 0x0240
          mov.w   #0x0250,R10   // R10 = End 0x0250
          mov.b   #0x58,R12     // R12 = End 0x58
          mov.w   #0x8053,R13   // R13 = End 0x8053
          
          // 
          mov.w   #0x5678,&var_1  // 
          mov.b   R12,var_2       // 
          mov.w   #4359,var_3     // 
          
          // Ponteiros
          mov.w   #0x9876,0(R5) // End. 0x0240 (R5) = #0x9876 Na menória(00 76 98 00)
          mov.w   R13,0(R10)    // Valor de R13 (0x08053) em memória 0x0250 (Apontado por R10)
          
          // Adição no registrador
          add.w   #2,R5
          add.w   #2,R10
          
          // Alocação de Conteudo
          mov.w   &var_1,0(R5)  // Move o Endereço de var_1 para o Conteudo de R5
          mov.w   var_3,0(R10)  // Move o Conteudo de var_3 para o Conteudo de R10
          mov.b   var_2,R15     // Move o Conteudo de var_2 para o Conteudo de R15
          
          /////////
          
          
          mov.w   #0x0240,R5    // Move o End.0x0240 para o registrador R5
          mov.w   #0x0250,R10   // Move o End.0x0250 para o registrador R10
          mov.w   #0x0260,R7    // Move o End.0x0260 para o registrador R7
          
          //
          mov.w   @R10+,0(R7)   // Modo Indireto com Auto-Incremento do ponteiro
          bic.w   #0x0001,SR    // Bit Clear com mascara
          dadd.w  @R5+,0(R7)    // Adição em decimal (código BCD)
          mov.w   @R10,2(R7)    // (R7+2)
          addc.w  @R5,2(R7)     // 
                  
      
fim:      jmp     fim           // Laço infinito(jmp $)

          end
          
          