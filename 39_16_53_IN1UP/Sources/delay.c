/*
 * delay.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */






#include "delay.h"



//50ns el paso-------------------------------------------------------------------
void delay1ms(void){
unsigned int i;
    for(i=0;i<6500;i++) { __asm{  nop; } }       
}//fin 1 mlisegundo--------------------------------------------------------------

void delay1us(void){
unsigned int i;
    for(i=0;i<24;i++) { __asm{  nop; } }       
}//fin 1 mlisegundo--------------------------------------------------------------



void  delay_ms(unsigned short int t){
int i;	
 if(t>0)
    for(i=0;i<t;i++)
    	delay1ms(); 
}//fin delay ms-----------------------------------------------------------------

	
void delay_us(unsigned short int t){
int i;
    for(i=0;i<t;i++)
	     delay1us();
}




