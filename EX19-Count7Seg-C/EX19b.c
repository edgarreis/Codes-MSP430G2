
#include <msp430.h>
							/* dgfexcba */
#define zero	0x3F;					/* 1011x111,  para todos x=1 => P1.3, botão s-2 */
#define one	0x06;					/* 0000x110     */
#define two	0x5B;					/* 1101x011	*/
#define three	0x4F;					/* 1100x111	*/
#define four	0x66;					/* 0110x011	*/
#define five	0x6D;					/* 1110x101	*/
#define six	0x7D;					/* 1111x101	*/
#define seven	0x07;					/* 0000x111	*/
#define eight	0x7F;					/* 1111x111	*/
#define nine	0x6F;					/* 1110x111	*/ 

int flag_conta, x, uni, dez, cent, comut, display ;
	
/* Configurar uC */
	void Configurar_uC(void)
	{
   		 WDTCTL = WDTPW + WDTHOLD;    		/* Desabilita o watchdog */
    		 BCSCTL1 = CALBC1_1MHZ;       		/* Configuração do clock para 1MHz */
   		 DCOCTL = CALDCO_1MHZ;        		/* Configuração do clock para 1MHz */
    		 
                 P1DIR = P1DIR | 0xFF;        		/* seta todos os pinos menos o botão p13 como saida */
   		 P1OUT = P1OUT & ~0xFF;			/* Forca Zero nas saidas de P1 */
               
                 P2DIR = P2DIR | 0x07;			/* seta os pinos 0, 1 e 2 de p2, como saida */
		 P2OUT = P2OUT & ~0x07;			/* Forca Zero nas saidas de P2 */
               
	}

        
/* Configurar Timer0_A */
	void Configurar_Timer0_A(void)
	{
		TACTL |= TASSEL_2 + ID_1 + MC_0;  	/* clock= SMCLK, div= /2, contagem MC_0 = parado, (MC_1 = Up mode => TACCR0) */
		TACCR0 = 0xC350;	           	/* 0xC350 = 50000 decimal * 2us => 100ms */
		TACCTL0 |= CCIE;		   	/* Habilita captura/comparação com relação a IFG */
	}	   

/* PROGRAMA PRINCIPAL */
	void main(void)			      	
	{
		Configurar_uC();
		Configurar_Timer0_A();	
		 __enable_interrupt(); 			/* Habilitar Interrupções */  		 		    
    		
		flag_conta = 0;				/* Inicializa flag_conta em zero, não contar */	
		
		uni = 0;				/* inicializa variaveis de contagem */
		dez = 0;
		cent= 0;
		comut = 0;	
                
                TACTL |= MC_1;  /* Liga timer modo UP */
                
//	P1OUT = 0xFF;
//        P2OUT = 0X01;
//        
//        while(1);
//          
    		while(1) 		    		/* Loop Infinito */
		{
			display = 0;
			if(comut<3)
			{
				while(display<100)	/* 2500 ciclos de 2us = 5ms para cada display -> 66Hz ao todo */
				{
					if(comut == 0)
					{
						P2OUT = 0x01;	/* habilita APENAS display em P2.1 => UNIDADES */
						switch (uni)
						{
							case 0:
								P1OUT = zero;
								break;
							case 1:
								P1OUT = one;
								break;
							case 2:
								P1OUT = two;
								break;
							case 3:
								P1OUT = three;
								break;
							case 4:
								P1OUT = four;
								break;
							case 5:
								P1OUT = five;
								break;
							case 6:
								P1OUT = six;
								break;
							case 7:
								P1OUT = seven;
								break;
							case 8:
								P1OUT = eight;
								break;
							case 9:
								P1OUT = nine;
								break;
							//default: {};

						}
					}
						else if (comut == 1)
					  		{
								P2OUT = 0x02;	/* habilita APENAS display em P2.2 => DEZENAS */
								switch (dez)
								{
									case 0:
										P1OUT = zero;
										break;
									case 1:
										P1OUT = one;
										break;
									case 2:
										P1OUT = two;
										break;
									case 3:
										P1OUT = three;
										break;
									case 4:
										P1OUT = four;
										break;
									case 5:
										P1OUT = five;
										break;
									case 6:
										P1OUT = six;
										break;
									case 7:
										P1OUT = seven;
										break;
									case 8:
										P1OUT = eight;
										break;
									case 9:
										P1OUT = nine;
										break;
									//default: 
								}

					  		}
								else if(comut == 2)
								{
									P2OUT = 0x04;	/* habilita APENAS display em P2.3 => CENTENAS */
									switch (cent)
									{
									case 0:
										P1OUT = zero;
										break;
									case 1:
										P1OUT = one;
										break;
									case 2:
										P1OUT = two;
										break;
									case 3:
										P1OUT = three;
										break;
									case 4:
										P1OUT = four;
										break;
									case 5:
										P1OUT = five;
										break;
									case 6:
										P1OUT = six;
										break;
									case 7:
										P1OUT = seven;
										break;
									case 8:
										P1OUT = eight;
										break;
									case 9:
										P1OUT = nine;
										break;
									//default: 
									}
					  			}	
					display = display + 1;		/* incremento no final do while (display) */			
				}					/* final do while(display) */ 
			comut = comut + 1;			 	/* incremento no final do if (comut<3)*/
			}						/* final do if (comut<3)*/
			else  comut = 0;				
		}/*final do while(1)*/
			 
	   			
	}	/*final do main */
       

/* Interrupção do Timer0_A */				/* CONTADOR DE 10ms */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void interr_piscar(void)   			/* quando esta interr. é acionada, significa que 0.1s se passou */
	{
			if (uni < 10)	
			{		
				uni = uni + 1;		/* incremento da unidade */
			}
			else if (dez < 10)
				{
					uni = 0;
					dez = dez + 1; 		/* Incremento da dezena */	
				}
				else if (cent < 10)
					{
                                                uni = 0;
						dez = 0;
						cent = cent +1;		/* Incremento da centena */	
					}
					else
					{
						uni  = 0;
						dez  = 0;
                                                cent = 0;		/* O contador vai até 99.9 */			
					}
                        P1IFG = 0x00;   /* Zera flag de estouro da contagem */
	}
    
