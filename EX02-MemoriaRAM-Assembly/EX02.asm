/// EXERCICIO 02 ///

#define   var_1   0x0206        // Var_1 = 0x0206

          ORG     0xFFFE        // Altera��o do PC
          DC16    main          // Define uma constante de 16 bits (main)
           
          ASEG    0x0200        // Reservar espa�o na mem�ria de dados
var_2:    DS8     1             // Aloca um bloco de 8 bit para var_2   // 0x0200 ((((VERIFICAR))))
          align   1             // Alinha no prox. end. de Par (word)
var_3:    DS16    1             // Aloca um bloco de 16 bit para var_3  // 0x0202 ((((VERIFICAR 200 para 202 s�o 2 bits ????))))

          RSEG    CODE          // Realoca��o de Dados da mem�ria
          
          // Aloca��o de Endere�os em Mem�rias
main:     mov.w   #0x0240,R5    // R5  = End 0x0240
          mov.w   #0x0250,R10   // R10 = End 0x0250
          mov.b   #0x58,R12     // R12 = End 0x58
          mov.w   #0x8053,R13   // R13 = End 0x8053
          
          // Endere�amentos
          mov.w   #0x5678,&var_1  // Endere�amento Absoluto  #0x5678 -> &0x0206
          mov.b   R12,var_2       // Endere�amento Simb�lico R12     -> var_2 (0x0200)
          mov.w   #4359,var_3     // Endere�amento Simb�lico #4359   -> var_3 (0x0202)
          
          // Ponteiros                  ((((VERIFICAR SE UTILIZAR R5 AO INVES DE 0(R5) FUNCIONA???))))))
          mov.w   #0x9876,0(R5) // Endere�amento Indexado       End. 0x0240  (R5) = #0x9876 Na mem�ria(00 76 98 00)
          mov.w   R13,0(R10)    // Endere�amento Indexado       End. 0x0250 (R10) = #0x8053 Na mem�ria(00 53 80 00)
          
          // Adi��o na mem�ria apontada pelo registrador      ((((VERIFICAR POR QUE 2????))))))
          add.w   #2,R5         // 0x0240 + 2 = 0x0242 R5  agora aponta para end.0x0242
          add.w   #2,R10        // 0x0250 + 2 = 0x0252 R10 agora aponta para end.0x0252
          
          // Aloca��o de Conteudo       ((((VERIFICAR PORQUE VAR1 USA & E VAR 2 N�O???????))))
          mov.w   &var_1,0(R5)  // Endere�amento Indexado       End. 0x0242 (R5)  = #0x5678 (&var_1)
          mov.w   var_3,0(R10)  // Endere�amento Indexado       End. 0x0252 (R10) = #0x4359 (var_3)
          mov.b   var_2,R15     // Endere�amento Indexado       End. ?????? (R15) = # ????arrumar liha do var_2
                           
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
          
fim:      jmp     fim           // La�o infinito(jmp $)

          end
          
          