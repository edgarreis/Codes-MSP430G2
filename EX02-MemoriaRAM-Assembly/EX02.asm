/// EXERCICIO 02 ///

#define   var_1   0x0206        // Var_1 = 0x0206

          ORG     0xFFFE        // Alteração do PC
          DC16    main          // Define uma constante de 16 bits (main)
           
          ASEG    0x0200        // Reservar espaço na memória de dados
var_2:    DS8     1             // Aloca um bloco de 8 bit para var_2   // 0x0200 ((((VERIFICAR))))
          align   1             // Alinha no prox. end. de Par (word)
var_3:    DS16    1             // Aloca um bloco de 16 bit para var_3  // 0x0202 ((((VERIFICAR 200 para 202 são 2 bits ????))))

          RSEG    CODE          // Realocação de Dados da memória
          
          // Alocação de Endereços em Memórias
main:     mov.w   #0x0240,R5    // R5  = End 0x0240
          mov.w   #0x0250,R10   // R10 = End 0x0250
          mov.b   #0x58,R12     // R12 = End 0x58
          mov.w   #0x8053,R13   // R13 = End 0x8053
          
          // Endereçamentos
          mov.w   #0x5678,&var_1  // Endereçamento Absoluto  #0x5678 -> &0x0206
          mov.b   R12,var_2       // Endereçamento Simbólico R12     -> var_2 (0x0200)
          mov.w   #4359,var_3     // Endereçamento Simbólico #4359   -> var_3 (0x0202)
          
          // Ponteiros                  ((((VERIFICAR SE UTILIZAR R5 AO INVES DE 0(R5) FUNCIONA???))))))
          mov.w   #0x9876,0(R5) // Endereçamento Indexado       End. 0x0240  (R5) = #0x9876 Na memória(00 76 98 00)
          mov.w   R13,0(R10)    // Endereçamento Indexado       End. 0x0250 (R10) = #0x8053 Na memória(00 53 80 00)
          
          // Adição na memória apontada pelo registrador      ((((VERIFICAR POR QUE 2????))))))
          add.w   #2,R5         // 0x0240 + 2 = 0x0242 R5  agora aponta para end.0x0242
          add.w   #2,R10        // 0x0250 + 2 = 0x0252 R10 agora aponta para end.0x0252
          
          // Alocação de Conteudo       ((((VERIFICAR PORQUE VAR1 USA & E VAR 2 NÃO???????))))
          mov.w   &var_1,0(R5)  // Endereçamento Indexado       End. 0x0242 (R5)  = #0x5678 (&var_1)
          mov.w   var_3,0(R10)  // Endereçamento Indexado       End. 0x0252 (R10) = #0x4359 (var_3)
          mov.b   var_2,R15     // Endereçamento Indexado       End. ?????? (R15) = # ????arrumar liha do var_2
                           
          // 
          mov.w   #0x0240,R5
          mov.w   #0x0250,R10
          mov.w   #0x0260,R7
          
          //
          mov.w   @R5,0(R7)
          add.w   @R10,0(R7)
          
          //
          add.w   #2,R5
          add.w   #2,R10
          add.w   #2,R7
          
fim:      jmp     fim           // Laço infinito(jmp $)

          end
          
          