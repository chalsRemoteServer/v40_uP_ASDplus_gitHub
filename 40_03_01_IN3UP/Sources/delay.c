/*
 * delay.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */






#include "delay.h"
#include "queue.h"
#include "system.h"
#include "VFDmenu.h"
#include "FC322.h"

extern struct _DISPLAY_VFD_ vfd;
extern struct _Menu_  menu;
extern unsigned long long int delay_General_ms;


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

//delay con sistema operativo------------------------------------
//regresa true cuando se cumpla todo el methodo hasta el final
unsigned char delay_us_VFD(unsigned short int t){
//auto unsigned char  estado;	
//auto unsigned char c;
//auto unsigned short int s;
auto unsigned char ret=0;
union W7{//access word: 
	unsigned  short int wordx;   //   	0xaabb        //         aa
	unsigned char byte[2];        //byte[0]=aa,byte[1]=bb
}w16;
//auto unsigned short int usint; 

  w16.wordx=t;
  if(vfd.f1.append(w16.byte[0],w16.byte[1],_DELAY_US))
        ret=TRUE;
return ret;    
}//--------------------------------------------------


//delay con sistema operativo------------------------------------
//regresa true cuando se cumpla todo el methodo hasta el final
//Este metodo se ejecutan muchas instancias 
unsigned char delay_ms_VFD(unsigned short int t){
auto unsigned char ret=0;
union W17{//access word: 
	unsigned  short int wordx;   //   	0xaabb        //         aa
	unsigned char byte[2];        //byte[0]=aa,byte[1]=bb
}w16;
//auto unsigned short int usint; 

  w16.wordx=t;
  if(vfd.f1.append(w16.byte[0],w16.byte[1],_DELAY_MS))
		    ret=TRUE;//fue acepado en la fifo
return ret;    
}//--------------------------------------------------

unsigned char delay_us_v3(unsigned short int t,unsigned char *p){
auto unsigned char ret=0;
auto unsigned short int usint; 
auto unsigned char estado1; 
  estado1=*p;
  switch(estado1){
    case 1: estado1++;
   	case 2:if(FC322_Reset()==ERR_OK)
   	  		      estado1++;
   	       break;
   	case 3:switch(FC322_GetTimeUS(&usint)){//useg since the reset
			   case ERR_OVERFLOW:break;
			   case ERR_MATH:break;
			   case ERR_OK:if(usint>t){
			     			   estado1++;
			     			   estado1=0;
			     			   ret=TRUE;}
						   break;
			   default:break;}
            break;//break-3 
	 default:estado1=1;break;}
*p=estado1;
return ret;
}//-------------------------------------------------------------

/* estea funcion toma la variable que cuenta el tiempo que 
 * esta encendido el equipo cada milisegundo y guarda el valor en ese momneto
 * y despues pregunta si ya transcurrio el tiempo deseado para regresar
 * un True,
 * delay1 guarda el tiempo zero, tiempo deseado es el tiempo que 
 * queremos de delay y inst: es la memoria para el funcionamiento*/
unsigned char delay_general_1ms(unsigned long long int *delay1,unsigned long long int t_deseado,unsigned char *inst){
auto unsigned char estado,ret=0;
estado=*inst;
   switch(estado){
	   case 1:*delay1=delay_General_ms+t_deseado;//tiempo actual;
	          estado++;break;
	   case 2:if(delay_General_ms>*delay1)estado++;break;
	   case 3:ret=TRUE;estado=0;break;
	   default:estado=1;break;}
*inst=estado;	
return ret;
}//--fin delay genral 1mseg





/* Exclusivo para usarse en el envio de datos al VFD
 *  solo por un proceso:
 *  xDriver_de_Transmision_al_VFD*/
unsigned char delay_us_VFD_exclusivo(unsigned short int t){
auto unsigned char ret=0;
//auto unsigned short int usint; 
static unsigned char estado1; 
  
  switch(estado1){
    case 1:vfd.v.timer_us=1;
   	  	   estado1++;
   	  	   break;
   	case 2:if(vfd.v.timer_us>t){ret=TRUE;
   	 	 	    vfd.v.timer_us=0;//para detener el decremento en timer
   		        estado1=0;}
   	       break;
	 default:estado1=1;break;}
return ret;
}//-----------------------------------------------------------




