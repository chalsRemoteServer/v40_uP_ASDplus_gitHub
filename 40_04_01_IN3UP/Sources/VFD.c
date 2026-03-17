/*
 * VFD.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */


/*
 * VFD.c
 *
 *  Created on: Jul 13, 2015
 *      Author: chals
 */



#include "VFD.h"
#include "VFDserial.h"
#include "queue.h"
#include "VFDmenu.h"
#include "delay.h"
#include "system.h"
#include "DSP.h"
#include "errorController.h"
#include "SistemaOperativo.h"



typedef union{//access word: 
	unsigned  short int wordx;   //   	0xaabb   
	unsigned char byteH;      //         aa
	unsigned char byte[2];        //byte[0]=aa,byte[1]=bb
}uword1;

extern struct _GLOBAL_CONTEXTO g,*AjustVibr,*DDSpant,*AjParamProd,*ParamDet;
extern struct _KEYPAD_ keypad;

extern unsigned int delaycount;
//unsigned char keyboard_IRQ,keyboard_IRQ2;
unsigned char timerEnableKeypad;//PARA activar el teclado despues de desactivarlo y esperar que se acabe de llegar todos los caracteres al display y esperar, porque tarda en desplegar y le estan apriete y aprite al teclado

extern struct _Menu_  menu;//contexto Actual que se despliega
extern struct _DISPLAY_VFD_ vfd;
extern struct _Recursos recurso;

void init_VFD_BIOS(void){
	   // delay_ms(200);//<--prueba para quitar error de Fnt size
	   // TI1_delays_Disable();
//	    //TI1_delays_DisableEvent();
//	    VFDcommand(0x1BU); // init display  ESC@= 1BH,40H (Initialize Display)
//        VFDcommand(0x40U); // Setting return to default values(Initialize Display)
//	//******************************CUrsosr display *****************************
//		VFDcommand(0x1FU);//CURSOR display, default=off
//		VFDcommand(0x43U);  // comando  1Fh,43h,n,  n=1->display cursor ON
//		VFDcommand(0x01U);// n=01h,-> O
//    
//
//		FontSizeVFD(FONTSIZE1);//TAMAÑO DE fuente 2->8x16
		/*VFDcommand(0x1FU);// font size select of a character command
		VFDcommand(0x28U);//  comando 1fh,28h,67h,01h,m
		VFDcommand(0x67U);//m= 01h  6x8 font,   m=02h->8x16 font
		VFDcommand(0x01U);//m=03h-> 12x24   m=04h->16x32 font
		VFDcommand(0x02U);*/

		/*VFDcommand(0x1FU); //comando de FONT WIDTH
		VFDcommand(0x28U); // COMANDO 1fh,28h,67h,04h,m
		VFDcommand(0x67U);//m=00h, fixed width,, m=02h-> proportional1
		VFDcommand(0x04U);//m=03->proportional-2  m=03h->proportional-3
		VFDcommand(0x00U);*/

		/*VFDcommand(0x1FU); //comando de FONT BOLD
	    VFDcommand(0x28U); // COMANDO 1fh,28h,67h,41,m
		VFDcommand(0x67U);//m=00h, fixed width,, m=02h-> proportional1
	    VFDcommand(0x41U);//m=03->proportional-2  m=03h->proportional-3
		VFDcommand(0x00U);//m bold=01H*/
	
}//fin init VFD -------------------------------------------------------------------





// pusimos estos delay y el tamaño de la Font no obedecia
	//y se ponia chica
//con estos delay ya obedece pero no sabemos si todos o cual
// instancia s1,s2
//METODO  mono-Padre Con-Instancia No almismo Tiempo
unsigned char FontSizeVFD(unsigned char m,unsigned char *p){
auto unsigned char estado;
auto unsigned char ret=0;
auto unsigned char inst[2];
auto unsigned short int n=10000; 

   estado=*p;
   inst[0]=*(p+1);
   inst[1]=*(p+2);
   switch(estado){
	   case 1:inst[0]=0;inst[1]=0;
		      estado++;
		      break;
	   case 2:if(VFDcommand_init(0x1FU,p+3))// font size select of a character command
	             estado++;        
		      break;
	   case 3:if(delay_us_VFD(n))//<--pusimos estos delay
		         estado++;
	          break;
	   case 4:if(VFDcommand_init(0x28U,p+3))//  comando 1fh,28h,67h,01h,m
		         estado++;
	          break;
	   case 5:if(delay_us_VFD(n))//<--pusimos estos delay
		         estado++;
	          break;
	   case 6:if(VFDcommand_init(0x67U,p+3))//m= 01h  6x8 font,   m=02h->8x16 font
		         estado++;
	          break;
	   case 7:if(delay_us_VFD(n))//<--pusimos estos delay
		         estado++;
	          break;
	   case 8:if(VFDcommand_init(0x01U,p+3))//m=03h-> 12x24   m=04h->16x32 font
		          estado++;
	          break;
	   case 9:if(delay_us_VFD(n))//<--pusimos estos delay
		          estado++;
	          break;
	   case 10:if(VFDcommand_init(m,p+3))
		          estado++;
	          break;
	   case 11:ret=TRUE;estado=0;break;
	   default:estado=1;break;}
*p=estado;
*(p+1)=inst[0];
*(p+2)=inst[1];
return ret;
}// fin font size for the VFD----------------------------------------------------


//VFD comando en MODO operativo----------------------------------
//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
//instancia 1 y dos 
// funcion mono-Padre || Solo puede tener un padre a la vez
unsigned char VFDcommand(unsigned char cmd){ //unsigned char *p){
//auto unsigned char estado11;
auto unsigned char ret=FALSE;
static unsigned char Semaforo;//SI el semaforo esta en cero se puede usar o si el semaforo vale lo que el padre
//static unsigned char inst[2];//variables que usan los procesos hijos multipadre
	//TI1_delays_Init();
	//delay1us();
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us
 

     return VFDserial_SendChar1(cmd);

//  estado11=*p; 
//  switch(estado11){
//	    case 1:*(p+1)=0;*(p+2)=0;	              
//	    	   estado11++;
//	           break;
//    	case 2:if(delay_us_VFD(225))
//    		          estado11++;
//    	       break;
//    	case 3:if(VFDserial_SendChar1(cmd))
//    		          estado11++;
//    	       break;
//    	case 4:if(delay_us_VFD(225))
//    		         estado11++;
//    	       break;
//    	case 5:ret=TRUE;estado11=0;
//    	       break;
//    	case 6:ret=0;break;
//    	default:estado11=1;break;}
//*p=estado11;
//return ret;	
}//fin vfd command----------------------------------------------------




//VFD comando en MODO operativo----------------------------------
//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
//instancia 1 y dos 
// funcion mono-Padre || Solo puede tener un padre a la vez
unsigned char VFDcommand_init(unsigned char cmd,unsigned char *p){
auto unsigned char estado11;
auto unsigned char ret=FALSE;
static unsigned char Semaforo;//SI el semaforo esta en cero se puede usar o si el semaforo vale lo que el padre
//static unsigned char inst[2];//variables que usan los procesos hijos multipadre
	//TI1_delays_Init();
	//delay1us();
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us
 
  estado11=*p; 
  switch(estado11){
	    case 1:*(p+1)=0;*(p+2)=0;	              
	    	   estado11++;
	           break;
    	case 2:if(delay_us_VFD(225))
    		          estado11++;
    	       break;
    	case 3:if(VFDserial_SendChar1(cmd))
    		          estado11++;
    	       break;
    	case 4:if(delay_us_VFD(225))
    		         estado11++;
    	       break;
    	case 5:ret=TRUE;estado11=0;
    	       break;
    	case 6:ret=0;break;
    	default:estado11=1;break;}
*p=estado11;
return ret;	
}//fin vfd command----------------------------------------------------





/* instancia que se esta usando este metodo
 * parametros son las instancias del metodo por si se manda
 * llamar en otro hilo en otro lado  */
unsigned char VFDclrscr1(unsigned char *instancia,unsigned char *instancia2){
//static unsigned char estado4;
auto unsigned char ret=0;
//unsigned char e;
//	monitorDDS_Halt();//debug, quitar un dia que la version este super probada 
auto unsigned char estado;
auto unsigned char c;
  estado=*instancia;
  c=*instancia2;
  switch(estado){//estado4){
	 case 1:if(VFDserial_SendChar1(0x0CU))//(Display Clear)   Display screen is cleared and cursor moves to home position.
		         estado++;// estado4+=0x10;
	        c=0;
	        break;
	 case 2:if(delay_ms_VFD(1)){//con 50 sigue sin borrar el anterior
	            ret=TRUE;estado=1;}
	         break;
	 default:estado=1;break;}
  *instancia=estado;
  *instancia2=c;
return ret;    
}//fin clear screen VFD-------------------------------------------------------------

/* esta funcion esta vacia porque no esta
 *  lista para ser inpuesta en un sistema operativo como este
 *  esta esperando que se desarrolle su codigo */
void VFDclrscr(void){
     __asm(nop);
     __asm(Halt);
}//fin clear screen VFD-------------------------------------------------------------


void VFDclrscr_DDS(void){
//	FIFO_DDS_push(0x0CU,10);  // VFDserial_SendChar(0x0CU);//(Display Clear)   Display screen is cleared and cursor moves to home position.
}//fin clear screen VFD-------------------------------------------------------------


//No se ejecuta bien el comando de limpiar cuando hay barra de deteccion. Description	Resource	Path	Location	Type

void VFDclrscrForce(void){
unsigned short int i;	
   
	VFDposicion(0,0);
	for(i=0;i<512;i++)
		VFDserial_SendChar1(' ');
}//limpiar pantalla a la fuerza-------------------------


//para evitar Usar estas funciones cuando estamos en contexto de DDS
//DDSpant->igxc0 esta variable desabilita este monitor, solo para usarse en el display maestro inicial
//void monitorDDS_Halt(void){
////unsigned char c;
//     //c=AjustVibr->igxc0;//DDSpant->igxc0;
////	 if(menu.contexto.Actual==PANTALLA_DDS)
////		 if(c==0xAA)
////			  return;
////		 else
////	       __asm(Halt);//Debug error de diseño de software, no se debe usar esta funcion
//}//fin monitorDDS-------------------------------------------------------------



/*   posicion de x es de 0000h a 1ffh  ó  a 511 
  *    posicion de y es de 0000h a 0Fh  * */
//void VFDposicion(unsigned short int x,unsigned short int y){ //MANDA DEL COMANDO DE POSICION AL vfd
//uword1 v; 
//	// unsigned char  byteL=(unsigned char)x;//toma el lowbyte
//	 //unsigned char  byteH=(unsigned char)(x<<)
//     monitorDDS_Halt();//debug, quitar un dia que la version este super probada
//	 v.wordx=x;
//	 VFDserial_SendChar(0x1FU);//comando de posicion 
//	 VFDserial_SendChar(0x24U); //comando 1fh,24h,xL,xH,yL,yH
//	 VFDserial_SendChar(v.byte[LO]);//v.byte[LO]);
//	 VFDserial_SendChar(v.byte[HI]);//v.byteH);
//	 v.wordx=y;
//	 VFDserial_SendChar(v.byte[LO]);
//	 VFDserial_SendChar(v.byte[HI]);
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
//     delay_us(100);//con 50 sigue sin borrar el anterior
// }// fin posicionVFD-------------------------------------------------------------

void BarraDet_VFDposicion(unsigned short int x,unsigned short int y){
uword1 v;	
	 v.wordx=x; 
	 FIFO_barraNum_push(COMANDO_POS);
	 FIFO_barraNum_push(v.byte[LO]);
	 v.wordx=y;
	 FIFO_barraNum_push(v.byte[LO]);
}//FIN ------------------------------------------------------------------------


void BarraDet_VFDserial_SendChar(unsigned char c){
	 FIFO_barraNum_push(COMANDO_CHAR);
	 FIFO_barraNum_push(c);
}//fin BarraDet_VFDserial_SendChar--------------------------------------------------


unsigned char VFDposicion(unsigned short int x,unsigned short int y){ //MANDA DEL COMANDO DE POSICION AL vfd
	return vfd.f1.append((unsigned char)x,(unsigned char)y,_POS_);//FIFO_Display_DDS_Char_push((unsigned char)x,(unsigned char)y);          
}// fin posicionVFD-------------------------------------------------------------

/* Metodo Multi-Padre pero solo una Estancia ala Vez      */
unsigned char VFDserial_SendBlock1(unsigned char *Ptr,unsigned short Size,unsigned short *Snd,unsigned char *inst)
{//static unsigned char estado5;//111x xxxx
auto unsigned char ret=0; 
auto unsigned char estado;
auto unsigned char count;
 
estado=*inst; //estado5&0xE0; //111x xxxx
count =*(inst+1);// estado5&0x1F; //xxx1 1111
  switch(estado){//1110 0000
	  case 1: if(Size>31){__asm(nop);//001x xxxx
	                     __asm(Halt);}
	          count=0;
	          estado++;//0010 0000
	          break;
	  case 2: if(count<Size){   //010x xxxx
		          if(*(Ptr+count)==0){
		      	          count++;}
		          else{if(VFDserial_SendChar1(*(Ptr+count))){
	                	  count++;}}}
	  	  	  else{ret=TRUE;
	  	          estado=0;break;}
	  	  	  break;
	  default:estado=1;break;}           	
*Snd=0;
*inst=estado;
*(inst+1)=count;
return ret;                       /* Return error code */
}//fin insertar en la FIFO un comando para graficar varios carateres.------------------------

/* inst: es la instancia que la esta mandando llamar */
unsigned char VFDserial_SendBlock2(const unsigned char *Ptr,unsigned short Size,unsigned short *Snd,unsigned char *inst)
{//static unsigned char estado5;//111x xxxx
 //static unsigned char count;
 register unsigned char ret=0; 
 //register unsigned char estadox;
 //register unsigned char count;
 //register unsigned char init;
 auto unsigned char estado;
 auto unsigned char count;
   if(Size==0){
	    __asm(nop);
        __asm(Halt);}
  estado=menu.subMenu.estado[*inst];//estado5&0xC0; //11xx xxxx
  count =menu.subMenu.count[*inst];// estado5&0x1F; //xxx1 1111
  switch(estado){//1110 0000
	  case 1:if(Size>31){__asm(nop);//001x xxxx
	                     __asm(Halt);}
	         count=0;
	         estado++;//esta inizializado
	         break;
	  case 2:if(count<Size){   //010x xxxx   
	           if(VFDserial_SendChar1(*(Ptr+count))){
	              	    count++;}}
		     else{ret=TRUE;
	  	  	      estado=0;}
	         break;
	 default:estado=1;break;}           	
*Snd=0;
menu.subMenu.estado[*inst]=estado;
menu.subMenu.count[*inst]=count;
return ret;                       /* Return error code */
}//fin insertar en la FIFO un comando para graficar varios carateres.------------------------




//void VFDserial_SendChar1(unsigned char c){
//	vfd.f1.append(c,0,_CHAR_);// FIFO_Display_DDS_Char_push(c,0xFE);//0xFE means that is just a char display          
//}//fin VFDserial_SendChar_DDS---------------------------------
unsigned char VFDserial_SendChar1(unsigned char c){
	return vfd.f1.append(c,0,_CHAR_);
}//------------------------------------------------------------------


/* comando para mandar en el dds MENU si queremos las letras en bold  o no
 * bold: TRUE|FALSE*/
void VFDcommand_Bold_DDS(unsigned char bold){
unsigned char c=1,x;
	 x=bold+c;
	 vfd.f1.append(0,0,_BOLD_);// FIFO_Display_DDS_Char_push(bold,0xFD);//FDh means that we'll send a command of kind bold
}//fin VFDcomando DDS-------------------------------------

/* comando para mandar en el dds MENU si queremos las letras en bold  o no
 * bold: TRUE|FALSE*/
unsigned char VFDcommand_Bold_DDS_v2(unsigned char bold){
unsigned char c=1,x;
	 x=bold+c;
	 return(vfd.f1.append(0,0,_BOLD_));// FIFO_Display_DDS_Char_push(bold,0xFD);//FDh means that we'll send a command of kind bold
}//fin VFDcomando DDS-------------------------------------


unsigned char Basura(unsigned char c){
	switch(c){
		case 'A':
		case 'O':
		case 'L':
		case 'R':
		case 'E':return FALSE;
		default:return TRUE;}
}//fin Basura--------------------------------





/* copiar y pegar la url en un browser alli se vera
 * el automata 
 * 
 * */
void VFDkeypad_ISR(unsigned char c){
static unsigned char i,c0;
if(keypad.b.enable){
  if(c>0){
		 if(++i>SIZE_BUFFER5-1){
			 i=0;
			 if(keypad.b.swap)
					keypad.b.swap=0;
			 else keypad.b.swap=1;}
		  if(keypad.b.swap)
			 keypad.v.a0[i]=c;
		  else keypad.v.a1[i]=c;}}
		
}// fin VFD keypad Interrupt Service Routine++++++++++++++++++++++++++++++++++++++++
 	
    	



void VFDdrawLine(unsigned char pen,unsigned short int x1,unsigned short y1,unsigned short int x2,unsigned short y2){
coordn16 coordenadas;
unsigned short int delayhere=10;     
	    
        /*if(y1>255) y1=255;  //seguro por si hay un bug de rango
          if(y2>255) y2=255; //seguro por si hay un bug de rango codigo mete error no dibuja la linea
          if(x1>127) x1=127; //seguro por si hay un bug de rango segun coordenadas 
          if(x2>127) x2=127; //seguro por si hay un bug de rango*/
        VFDserial_SendChar1(0x1FU);delay_us(delayhere);
        VFDserial_SendChar1(0x28U);delay_us(delayhere);
        VFDserial_SendChar1(0x64U);delay_us(delayhere);
        VFDserial_SendChar1(0x11U);delay_us(delayhere);
        VFDserial_SendChar1(0x00U);delay_us(delayhere);//comando para dibujar una linea
        VFDserial_SendChar1(pen);delay_us(delayhere);// comando pen
        coordenadas.coord16=x1;
        VFDserial_SendChar1(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=y1;
        VFDserial_SendChar1(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=x2;
        VFDserial_SendChar1(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=y2;
        VFDserial_SendChar1(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);delay_us(delayhere);
        
}// fin draw line -----------------------------------------------------------------------------------------
     
void VFDdrawLine1(unsigned char pen,unsigned short int x1,unsigned short y1,unsigned short int x2,unsigned short y2){
coordn16 coordenadas;
unsigned short int delayhere=10;     
	    
        /*if(y1>255) y1=255;  //seguro por si hay un bug de rango
          if(y2>255) y2=255; //seguro por si hay un bug de rango codigo mete error no dibuja la linea
          if(x1>127) x1=127; //seguro por si hay un bug de rango segun coordenadas 
          if(x2>127) x2=127; //seguro por si hay un bug de rango*/
        VFDserial_SendChar1(0x1FU);//delay_us(delayhere);
        VFDserial_SendChar1(0x28U);//delay_us(delayhere);
        VFDserial_SendChar1(0x64U);//delay_us(delayhere);
        VFDserial_SendChar1(0x11U);//delay_us(delayhere);
        VFDserial_SendChar1(0x00U);//delay_us(delayhere);//comando para dibujar una linea
        VFDserial_SendChar1(pen);//delay_us(delayhere);// comando pen
        coordenadas.coord16=x1;
        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
        coordenadas.coord16=y1;
        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
        coordenadas.coord16=x2;
        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
        coordenadas.coord16=y2;
        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
        
}// fin draw line -----------------------------------------------------------------------------------------


//void VFDdrawLine_v3(unsigned char pen,unsigned short int x1,unsigned short y1,
//		 unsigned short int x2,unsigned short y2,unsigned char *inst){
//coordn16 coordenadas;
//unsigned short int delayhere=10;     
//auto unsigned char estado51;	    
//        /*if(y1>255) y1=255;  //seguro por si hay un bug de rango
//          if(y2>255) y2=255; //seguro por si hay un bug de rango codigo mete error no dibuja la linea
//          if(x1>127) x1=127; //seguro por si hay un bug de rango segun coordenadas 
//          if(x2>127) x2=127; //seguro por si hay un bug de rango*/
// switch(estado51){
//   case 1:if(VFDserial_SendChar1(0x1FU)) estado51++;break;
//   case 2:if(VFDserial_SendChar1(0x28U)) estado51++;break;
//   case 3:if(VFDserial_SendChar1(0x64U)) estado51++;break;
//   case 4:if(VFDserial_SendChar1(0x11U)) estado51++;break;
//   case 5:if(VFDserial_SendChar1(0x00U)) estado51++;break;
//   case 6:if(VFDserial_SendChar1(pen))   estado51++;break;
//   case 7:if(coordenadas.coord16=x1) estado51++;break;
//   case 8:if(VFDserial_SendChar1(coordenadas.byte[LO])) estado51;break;
//   case 9:if(VFDserial_SendChar1(coordenadas.byte[HI])) estado51;break;
//   case 10:coordenadas.coord16=y1; estado51;break;
//   case 11:
//        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
//        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
//        coordenadas.coord16=x2;
//        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
//        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
//        coordenadas.coord16=y2;
//        VFDserial_SendChar1(coordenadas.byte[LO]);//delay_us(delayhere);
//        VFDserial_SendChar1(coordenadas.byte[HI]);//delay_us(delayhere);
//        
//}// fin draw line -----------------------------------------------------------------------------------------



/*Requiere 15+2 bytes y 4 words de memoria RAM*/
unsigned char VFDdrawLine_v4(unsigned char pen,unsigned short int x1,
		                     unsigned short int y1,unsigned short int x2,
		                     unsigned short int y2,unsigned short int delay1,
		                     unsigned char *inst,unsigned short int *usi){
coordn16 coordenadas;
auto unsigned char estado5,ret=0;
const unsigned char byts[]={0x1F,0x28,0x64,0x11,0x00};
auto unsigned short int *p;
auto unsigned char i,j,*v;
//const unsigned char PIDdl=PID17+PIDpadre;
estado5=*inst;//memoria
i=*(inst+1);
v=inst+2;//15 de memoria
p=usi;//0,1,2,3

  switch(estado5){
	  case 1:*(p+0)=x1;*(p+1)=y1;*(p+2)=x2;*(p+3)=y2;estado5++;break;
	  case 2:for(i=0;i<5;i++) *(v+i)=byts[i];estado5++;break;
	  case 3:*(v+5)=pen;estado5++;break;
	  case 4:for(i=0,j=6;i<4;i++){
		        coordenadas.coord16=*(p+i);
	  	  	    *(v+j++)=coordenadas.byte[LO];
	  	  	    *(v+j++)=coordenadas.byte[HI];}
	  	  	 i=0;estado5++;
	         break;       
	  case 5:if(VFDserial_SendChar1(*(v+i)))estado5++;break;
	  case 6:if(delay_us_VFD(delay1))estado5++;break;
	  case 7:if(i<13){ i++; estado5=5;}
	  	  	 else{ret=1;estado5=0;estado5++;}
	         break;
	  default:estado5=1;break;}
*inst=estado5;
*(inst+1)=i;
return ret;  
}// fin draw line -----------------------------------------------------------------------------------------


/*Requiere 15+2 bytes y 4 words de memoria RAM*/
unsigned char VFDdrawLine_v5(unsigned char pen,unsigned short int x1,
		                     unsigned short int y1,unsigned short int x2,
		                     unsigned short int y2,unsigned short int delay1,
		                     unsigned char *inst,unsigned short int *usi,
		                     unsigned char PIDpadre){
coordn16 coordenadas;
auto unsigned char estado5,ret=0;
const unsigned char byts[]={0x1F,0x28,0x64,0x11,0x00};
auto unsigned short int *p;
auto unsigned char i,j,*v;
const unsigned char PIDdl=PID17+PIDpadre;
estado5=*inst;//memoria
i=*(inst+1);
v=inst+2;//15 de memoria
p=usi;//0,1,2,3

  switch(estado5){
	  case 1:*(p+0)=x1;*(p+1)=y1;*(p+2)=x2;*(p+3)=y2;estado5++;break;
	  case 2:for(i=0;i<5;i++) *(v+i)=byts[i];estado5++;break;
	  case 3:*(v+5)=pen;estado5++;break;
	  case 4:for(i=0,j=6;i<4;i++){
		        coordenadas.coord16=*(p+i);
	  	  	    *(v+j++)=coordenadas.byte[LO];
	  	  	    *(v+j++)=coordenadas.byte[HI];}
	  	  	 i=0;estado5++;
	         break;       
	  case 5:if(recurso.solicitar(_VFD_,PIDdl))estado5++;break;
	  case 6:if(VFDserial_SendChar1(*(v+i)))estado5++;break;
	  case 7:if(delay_us_VFD(delay1))estado5++;break;
	  case 8:if(i<14){ i++; estado5=6;}
	  	  	 else{estado5++;}
	  	  	 break;
	  case 9:recurso.devolver(_VFD_,PIDdl);
	         ret=1;estado5=0;break;
	  default:estado5=1;break;}
*inst=estado5;
*(inst+1)=i;
return ret;  
}// fin draw line -----------------------------------------------------------------------------------------






//void VFDdotDrawing(unsigned short int x,unsigned short int y){
//coordn16 coordenadas;
//unsigned short int n=40;  //<<--NO DISMINUIR Menos de 40 Nos da basura en Pantalla
//unsigned short int b=30;  //<<--NO DISMINUR PORQUE DESPLIEGA BASURA
//
//        //320usegundos de delays
//        VFDserial_SendChar(0x1FU);
//        delay_us(n);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(0x28U);
//        delay_us(n);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(0x64U);
//        delay_us(n);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(0x10U);
//        delay_us(n);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(0x01U);
//        delay_us(n);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        coordenadas.coord16=x;
//        VFDserial_SendChar(coordenadas.byte[LO]);
//        delay_us(b);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(coordenadas.byte[HI]);
//        delay_us(b);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        coordenadas.coord16=y;
//        VFDserial_SendChar(coordenadas.byte[LO]);
//        delay_us(b);//<<----NO MOVER O DESPLIEGA BASURA EN PANTALLA
//        VFDserial_SendChar(coordenadas.byte[HI]);
//        delay_us(30);//<<----NO DISMINUR O DESPLIEGA BASURA EN PANTALLA
//        
//}// fin de VFD dot drawing--------------------------------------------------------------------------------


void VFDboxLine(unsigned char pen,unsigned char mode,unsigned short int x1,unsigned short y1,unsigned short int x2,unsigned short y2){
coordn16 coordenadas;
unsigned short int delayhere=10;     
	    
        VFDserial_SendChar(0x1FU);delay_us(delayhere);
        VFDserial_SendChar(0x28U);delay_us(delayhere);
        VFDserial_SendChar(0x64U);delay_us(delayhere);
        VFDserial_SendChar(0x11U);delay_us(delayhere);
        if(mode!=BOX_VACIA){
        	if(mode!=BOX_LLENA){
        	      mode=BOX_VACIA;}}
        VFDserial_SendChar(mode);delay_us(delayhere);//comando para dibujar una linea
        if(pen!=0x01){
        	if(pen!=0x00){
        		pen=0x01;}}
        VFDserial_SendChar(pen);delay_us(delayhere);// comando pen
        coordenadas.coord16=x1;
        VFDserial_SendChar(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=y1;
        VFDserial_SendChar(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=x2;
        VFDserial_SendChar(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar(coordenadas.byte[HI]);delay_us(delayhere);
        coordenadas.coord16=y2;
        VFDserial_SendChar(coordenadas.byte[LO]);delay_us(delayhere);
        VFDserial_SendChar(coordenadas.byte[HI]);delay_us(delayhere);
        
}// fin draw line -----------------------------------------------------------------------------------------

unsigned char VFDboxLine1(unsigned char pen,unsigned char mode,unsigned short int x1,unsigned short y1,unsigned short int x2,unsigned short y2){
coordn16 coordenadas;
//unsigned short int delayhere=10;     
static unsigned char estado;
//static unsigned char delay=10;
unsigned char ret=0;
unsigned short int delay1=1000;//delay en pruebas
	    
  switch(estado){
     case 1:if(VFDserial_SendChar1(0x1FU)){//delay_us(delayhere);
    	             estado++;}
            break;
     case 2:if(delay_us_VFD(delay1))//delay en pruebas
    	          estado++;
            break;
     case 3:if(VFDserial_SendChar1(0x28U))//);delay_us(delayhere);
    	        {estado++;}
            break;
     case 4:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 5:if(VFDserial_SendChar1(0x64U))//;delay_us(delayhere);
    	 	 	 {estado++;}
      	  	 break;
     case 6:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 7:if(VFDserial_SendChar1(0x11U))///;delay_us(delayhere);
    	 	 	 {estado++;}
     	 	 break;
     case 8:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 9:if(mode!=BOX_VACIA){
        	   if(mode!=BOX_LLENA){
        	        mode=BOX_VACIA;}}
            estado++;
            break;
     case 10:if(VFDserial_SendChar1(mode))//;delay_us(delayhere);//comando para dibujar una linea
    	 	 	 {estado++;}
     	 	 break;
     case 11:if(delay_us_VFD(delay1))//delay en pruebas)
         	          estado++;
                 break;
     case 12:if(pen!=0x01){
        	   if(pen!=0x00){
        		   pen=0x01;}}
             estado++;
             break;
     case 13:if(VFDserial_SendChar1(pen))//;delay_us(delayhere);// comando pen
    	       {estado++;}
             break;
     case 14:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 15:coordenadas.coord16=x1;
            if(VFDserial_SendChar1(coordenadas.byte[LO]))//;delay_us(delayhere);
            	 {estado++;}
            break;
     case 16:if(delay_us_VFD(delay1))//delay en pruebas
         	      estado++;
                 break;
     case 17:coordenadas.coord16=x1;
    	     if(VFDserial_SendChar1(coordenadas.byte[HI]))//;delay_us(delayhere);
    	        {estado++;}
            break;
     case 18:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 19:coordenadas.coord16=y1;
			  if(VFDserial_SendChar1(coordenadas.byte[LO]))//;delay_us(delayhere);
				 {estado++;}
			  break;
     case 20:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 21:coordenadas.coord16=y1;
    	     if(VFDserial_SendChar1(coordenadas.byte[HI]))//;delay_us(delayhere);
    	  	  {estado++;}
          break;
     case 22:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 23:coordenadas.coord16=x2;
             if(VFDserial_SendChar1(coordenadas.byte[LO]))//;delay_us(delayhere);
            	{estado++;}
             break;
     case 24:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 25:coordenadas.coord16=x2;
    	     if(VFDserial_SendChar1(coordenadas.byte[HI]))//;delay_us(delayhere);
    	 	 	 {estado++;}
     	 	 break;
     case 26:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 27:coordenadas.coord16=y2;
        	 if(VFDserial_SendChar1(coordenadas.byte[LO]))//;delay_us(delayhere);
        		 {estado++;}
        	 break;
     case 28:if(delay_us_VFD(delay1))//delay en pruebas
         	          estado++;
                 break;
     case 29:coordenadas.coord16=y2;
    	     if(VFDserial_SendChar1(coordenadas.byte[HI])){//;delay_us(delayhere);
    	           estado++;}
             break;
     case 30:if(delay_us_VFD(delay1)){//delay en pruebas
         	          estado++;ret=TRUE;}
                 break;
     default:estado=1;break;}
return ret;
}// fin draw line -----------------------------------------------------------------------------------------







