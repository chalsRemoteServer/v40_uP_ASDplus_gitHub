/*
 * keypad.c
 *
 *  Created on: Mar 17, 2022
 *      Author: desarrollo-1
 */

#include "string.h"
#include "queue.h"
#include "keypad.h"
#include "VFDserial.h"
#include "PE_Types.h"
#include "VFD.h"
#include "VFDmenu.h"



extern struct _FIFO_1byte_ serialKeyPad;
struct _SerialControl_ serialKeyPad2;
extern struct _KEYPAD_ keypad;
extern struct _Menu_  menu;

extern unsigned char  buffer5[SIZE_BUFFER5];//FIFO de char keypad que entran del teclado from serial


/*LINK: Al automata de O modelo template
{"type":"DFA","dfa":{"transitions":{"start":{"*":"start","C":"start","I":"start","D":"s9"},"s1":{"O":"s1","D":"s2"},"s2":{"D":"s2","*":"s3"},"s4":{"*":"s11","D":"s4","C":"s10"},"s3":{"*":"s3","D":"s5"},"s5":{"D":"s5"},"s9":{"C":"s10","D":"s9"},"s10":{"*":"s11","C":"s10","D":"s4"},"s11":{"*":"s11","C":"s11"}},"startState":"start","acceptStates":["s5","s11"]},"states":{"start":{},"s9":{"top":273.92578125,"left":195.9375,"displayId":"S4"},"s1":{"top":-81.9921875,"left":188.984375,"displayId":"s1"},"s2":{"top":-85,"left":360,"displayId":"S7"},"s3":{"top":-86.9921875,"left":573.984375,"displayId":"S4"},"s4":{"top":278.1640625,"left":635.13671875,"displayId":"S6"},"s11":{"isAccept":true,"top":434.94140625,"left":474.94140625,"displayId":"S7"},"s10":{"top":280.68359375,"left":390.68359375,"displayId":"S5"},"s5":{"isAccept":true,"top":-81.9921875,"left":745,"displayId":"S17"}},"transitions":[{"stateA":"start","label":"*","stateB":"start"},{"stateA":"start","label":"C","stateB":"start"},{"stateA":"start","label":"I","stateB":"start"},{"stateA":"start","label":"D","stateB":"s9"},{"stateA":"s1","label":"O","stateB":"s1"},{"stateA":"s1","label":"D","stateB":"s2"},{"stateA":"s2","label":"D","stateB":"s2"},{"stateA":"s2","label":"*","stateB":"s3"},{"stateA":"s4","label":"*","stateB":"s11"},{"stateA":"s4","label":"D","stateB":"s4"},{"stateA":"s4","label":"C","stateB":"s10"},{"stateA":"s3","label":"*","stateB":"s3"},{"stateA":"s3","label":"D","stateB":"s5"},{"stateA":"s5","label":"D","stateB":"s5"},{"stateA":"s9","label":"C","stateB":"s10"},{"stateA":"s9","label":"D","stateB":"s9"},{"stateA":"s10","label":"*","stateB":"s11"},{"stateA":"s10","label":"C","stateB":"s10"},{"stateA":"s10","label":"D","stateB":"s4"},{"stateA":"s11","label":"*","stateB":"s11"},{"stateA":"s11","label":"C","stateB":"s11"}],"bulkTests":{"accept":"*****IIIIIIIICCIIIICCCIII****CCCDDDDCCCCDDDDCCCDDDCCC******CCCCC****","reject":""}}

https://automatonsimulator.com/#%7B%22type%22%3A%22DFA%22%2C%22dfa%22%3A%7B%22transitions%22%3A%7B%22start%22%3A%7B%22*%22%3A%22start%22%2C%22C%22%3A%22start%22%2C%22I%22%3A%22start%22%2C%22D%22%3A%22s9%22%7D%2C%22s1%22%3A%7B%22O%22%3A%22s1%22%2C%22D%22%3A%22s2%22%7D%2C%22s2%22%3A%7B%22D%22%3A%22s2%22%2C%22*%22%3A%22s3%22%7D%2C%22s4%22%3A%7B%22*%22%3A%22s11%22%2C%22D%22%3A%22s4%22%2C%22C%22%3A%22s10%22%7D%2C%22s3%22%3A%7B%22*%22%3A%22s3%22%2C%22D%22%3A%22s5%22%7D%2C%22s5%22%3A%7B%22D%22%3A%22s5%22%7D%2C%22s9%22%3A%7B%22C%22%3A%22s10%22%2C%22D%22%3A%22s9%22%7D%2C%22s10%22%3A%7B%22*%22%3A%22s11%22%2C%22C%22%3A%22s10%22%2C%22D%22%3A%22s4%22%7D%2C%22s11%22%3A%7B%22*%22%3A%22s11%22%2C%22C%22%3A%22s11%22%7D%7D%2C%22startState%22%3A%22start%22%2C%22acceptStates%22%3A%5B%22s5%22%2C%22s11%22%5D%7D%2C%22states%22%3A%7B%22start%22%3A%7B%7D%2C%22s9%22%3A%7B%22top%22%3A273.92578125%2C%22left%22%3A195.9375%2C%22displayId%22%3A%22S4%22%7D%2C%22s1%22%3A%7B%22top%22%3A-81.9921875%2C%22left%22%3A188.984375%2C%22displayId%22%3A%22s1%22%7D%2C%22s2%22%3A%7B%22top%22%3A-85%2C%22left%22%3A360%2C%22displayId%22%3A%22S7%22%7D%2C%22s3%22%3A%7B%22top%22%3A-86.9921875%2C%22left%22%3A573.984375%2C%22displayId%22%3A%22S4%22%7D%2C%22s4%22%3A%7B%22top%22%3A278.1640625%2C%22left%22%3A635.13671875%2C%22displayId%22%3A%22S6%22%7D%2C%22s11%22%3A%7B%22isAccept%22%3Atrue%2C%22top%22%3A434.94140625%2C%22left%22%3A474.94140625%2C%22displayId%22%3A%22S7%22%7D%2C%22s10%22%3A%7B%22top%22%3A280.68359375%2C%22left%22%3A390.68359375%2C%22displayId%22%3A%22S5%22%7D%2C%22s5%22%3A%7B%22isAccept%22%3Atrue%2C%22top%22%3A-81.9921875%2C%22left%22%3A745%2C%22displayId%22%3A%22S17%22%7D%7D%2C%22transitions%22%3A%5B%7B%22stateA%22%3A%22start%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22start%22%7D%2C%7B%22stateA%22%3A%22start%22%2C%22label%22%3A%22C%22%2C%22stateB%22%3A%22start%22%7D%2C%7B%22stateA%22%3A%22start%22%2C%22label%22%3A%22I%22%2C%22stateB%22%3A%22start%22%7D%2C%7B%22stateA%22%3A%22start%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s9%22%7D%2C%7B%22stateA%22%3A%22s1%22%2C%22label%22%3A%22O%22%2C%22stateB%22%3A%22s1%22%7D%2C%7B%22stateA%22%3A%22s1%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s2%22%7D%2C%7B%22stateA%22%3A%22s2%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s2%22%7D%2C%7B%22stateA%22%3A%22s2%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22s3%22%7D%2C%7B%22stateA%22%3A%22s4%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22s11%22%7D%2C%7B%22stateA%22%3A%22s4%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s4%22%7D%2C%7B%22stateA%22%3A%22s4%22%2C%22label%22%3A%22C%22%2C%22stateB%22%3A%22s10%22%7D%2C%7B%22stateA%22%3A%22s3%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22s3%22%7D%2C%7B%22stateA%22%3A%22s3%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s5%22%7D%2C%7B%22stateA%22%3A%22s5%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s5%22%7D%2C%7B%22stateA%22%3A%22s9%22%2C%22label%22%3A%22C%22%2C%22stateB%22%3A%22s10%22%7D%2C%7B%22stateA%22%3A%22s9%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s9%22%7D%2C%7B%22stateA%22%3A%22s10%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22s11%22%7D%2C%7B%22stateA%22%3A%22s10%22%2C%22label%22%3A%22C%22%2C%22stateB%22%3A%22s10%22%7D%2C%7B%22stateA%22%3A%22s10%22%2C%22label%22%3A%22D%22%2C%22stateB%22%3A%22s4%22%7D%2C%7B%22stateA%22%3A%22s11%22%2C%22label%22%3A%22*%22%2C%22stateB%22%3A%22s11%22%7D%2C%7B%22stateA%22%3A%22s11%22%2C%22label%22%3A%22C%22%2C%22stateB%22%3A%22s11%22%7D%5D%2C%22bulkTests%22%3A%7B%22accept%22%3A%22*****IIIIIIIICCIIIICCCIII****CCCDDDDCCCCDDDDCCCDDDCCC******CCCCC****%22%2C%22reject%22%3A%22%22%7D%7D 

*/
void Controlador_Driver_de_Keypad_Operativo(void){//control del teclado por Automatas
unsigned char *s,c,*s2,k;
unsigned char *i,*c0,*n,filtro;    
 if(menu.bytes.semMenuPendiente)
	 return;
 if(keypad.b.enable){//bit que indica si esta habilitado el keypad
	s2=&keypad.v.ss;//estados de la maquina que vacia los array ala FIFO
	i=&keypad.v.i;//indice actual del array actual que se esta vaciando
 	switch(*s2){//guardar los chars del keypad que se apreto de los arrays rapidos a la FIFO
	  case 0:if(keypad.b.swap)//cual array se  esta usando
		            *s2=1;//se esta usando el array 1 vamos a vaciar el 2
	  	  	 else *s2=2; //se esta usando el 2 vamos a vaciar el uno
	         *i=0;
	         break;
	  case 1:if(*i>SIZE_BUFFER5-1) 
		             *s2=0;
	         else {c=keypad.v.a0[*i];
	               if(c>0){
	        	      keypad.F.appendByte(c,&keypad.F);
	                  keypad.v.a0[*i]=0;}
	               (*i)++;}
	         break;
	  case 2:if(*i>SIZE_BUFFER5-1) 
					  *s2=0;
			 else {c=keypad.v.a1[*i];
					if(c>0){
					  keypad.F.appendByte(c,&keypad.F);
					   keypad.v.a1[*i]=0;}
					(*i)++;}
			 break;       
	  default:break;}//fin s2
	if(keypad.F.ncount>10){//hay datos en la FIFO?
	   c0=&keypad.v.c0;//guarda el char anterior aceptado del keypad 
	   n=&keypad.v.ncount;//numero de veces que el char igual al anterior es leido,	
       s=&keypad.v.s;//variable que guarda los estados
       keypad.F.popf(&c,&keypad.F);//sacar dato la FIFO, del char que se apreto hace un momento
       switch(c){//automata: char anterior es C  <<-----manejo del char anterior
			case 'O'://automata: cialquiercosa es D
			case 'A': //entradas y manejo de Entradas del automata 
			case 'L':
			case 'E':
			case 'R':switch(menu.contexto.Actual){        //Sin Anomalias      Con Anomalias
				        case TEXT_PROCESSOR:filtro=125;break;//135,
				        case MENU_INSIGHT:  filtro=100;break;
				        default:filtro=95;break; }//                                95
				     keypad.v.c=c;
				     if(*c0==c){//El char	apretado es igual al anterior 
				       if((*n)++>filtro)
				    	    k='D';//se sigue apretando el mismo char que se apreto la ultimavez
				       else k='I';}//si es igual, tendremos que contar hasa estar seguros que no vendra otro
			         else k='D';//el anterior es diferente al actual apretado 
			         break;
			case '*':k='*';break;         
			default:k='C';break;}//cualquier otro dato diferente al asterisco o los keypad chars         
	   switch(*s){//Inicia el AUTOMATA
		   case 0:switch(k){//D:diferente, I:igual, C:cualquiera otro, *=* 
			        case 'D': *s=4;break;
			        case 'I':
			        case 'C':
			        case '*':
			        default:break;}
		          break;        
		  case 4:switch(k){
			         case 'C':*s=5;break;
			         case 'D':
			         default:break;} 	 
		          break;   
		   case 5:switch(k){
			        case '*':*s=7;break;
			        case 'D':*s=6;break;
			        case 'C':
			        default:break;}
		          break;
		   case 6:switch(k){
			        case '*':*s=7;break;
			        case 'C':*s=5;break;
			        case 'D':
			        default:break;}
		          break;
		   case 7:switch(k){
				   case '*':
				   case 'C':
				   default:pre_menus(keypad.v.c); break;}
		          break;
	       default:*s=0;break;}// fin switch automata
	}//ncount fin
  }//fin if keyboard_IRQ
}//fin de Controlador_Driver_de_Keypad_Operativo------------------------------------------------------


/*Traduce el char que se acepto con el 
 * char del sistema operativo*/
void pre_menus(unsigned char c){
	flushBuffers();
	switch(c){
		case 'E':menus(keyEN);break;
		case 'R':menus(keyRT);break;
		case 'L':menus(keyLF);break;
		case 'O':menus(keyDN);break;
		case 'A':menus(keyUP);break;
		default:break;}
	keypad.b.enable=1;//habilitar teclado
}//fin -------------------------------------


void flushBuffers(void){
	 keypad.b.enable=FALSE;//Desabilitar teclado
	 keypad.v.delay=0;
	 cleanArray2(&keypad.v.a0[0],SIZE_BUFFER5,0);
	 cleanArray2(&keypad.v.a1[0],SIZE_BUFFER5,0);
	 keypad.F.resetFIFO(&keypad.F,&buffer5[0],SIZE_BUFFER5);//queda como nueva
	 keypad.v.s=0;
	 keypad.v.ss=0;
	 keypad.v.ncount=0;
	 keypad.v.i=0;
	 keypad.v.c0=keypad.v.c;
	 keypad.v.c=0;
     VFDserial_ClearRxBuf();

}//fin --------------------------------------------------------------------------



/* arr: es el array a limpiar, size es el tamaño del array, k es la kosntante
 * que se va allenar el array  */
void cleanArray2(unsigned char *arr,unsigned char size,unsigned char k){
unsigned char i;        
	for(i=0;i<size;i++)
	     *(arr+i)=k;
}//fin de clean array----------------------------------------------------------------
