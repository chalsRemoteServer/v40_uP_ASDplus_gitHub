/*
 * SistemaOperativo.c
 *
 *  Created on: Oct 3, 2017
 *      Author: chals
 */

#include "maths.h"
#include "SistemaOperativo.h"
#include "VFDmenu.h"
#include "system.h"
#include "VFD.h"
#include "DSP.h"
#include "VFDisplay.h"
#include "Memoria.h"
#include "queue.h"
#include "VFDserial.h"
#include "delay.h"
#include "strings.h"
#include "IOcomms.h"
//#include "DDS_counter.h"
#include "errorController.h"




extern struct _Detection Deteccion;
extern struct _Comando_DDS dds;  //extern struct _DDS_ DDS1;
extern struct _Menu_ menu;
extern struct _Comando_DDS dds;
extern struct ZoomControl zoom;
//extern struct _Error323_ sysMon;
extern struct _Comando_DDS dds;
extern struct _GLOBAL_CONTEXTO g,*AjustVibr,*DDSpant;
extern struct _DISPLAY_VFD_ vfd;
extern struct ArbolMenu MenuActualScreen;//la estrucrura del menu actual en pantalla.
extern struct _Recursos recurso;
//extern struct _KeyPad_ keypad;



extern struct EntSyst{//variables 	de entradas de sistema 
		unsigned char taco;
		unsigned char fotocel;
        unsigned char fotocelBloqueado;//de 1 seg a 60 segundos
		unsigned char rechazo;
		unsigned char rechazoBloqueado;
		unsigned char rechazoPruebasist_seg;
		unsigned char rechazoPruebasist_mseg;
		unsigned char producto; 
		unsigned char productoBloqueado;
		unsigned char productoPruebasist_seg;
		unsigned char productoPruebasist_mseg;
		unsigned char aux1;// xxxx=rele
		unsigned char aux1Rele;//nombre del rele seleccionado
		unsigned char aux1Label;//label seleccinado
		unsigned char aux1_seg;
		unsigned char aux1_mseg;
		unsigned char aux2;
		unsigned char aux2Rele;
		unsigned char aux2_seg;
		unsigned char aux2_mseg;
		unsigned	 char aux2Label;
		unsigned char byten;	
		unsigned char PulsosPorSegundo;//pulsos medidos en tiempo real por segundo
}EntradasSistemaReg;

signed short int *head4;//debug
signed short int *head5;//debug

unsigned char semMenu2=DONE;//semaforo para el control de Menus con barra de deteccion
//extern unsigned char DDStx_flag_INT;
extern volatile unsigned char  TM1_IRQ1;

//extern unsigned char save_Pixels(signed short int x,signed short int y);
extern struct _Menu_ menu;

extern unsigned char semaforoDisplay;
extern unsigned char DDSplotflag;//bandera para ver si ya se mando un byte TX al VFD		
//extern unsigned char EventDDS;
extern unsigned char DDSplotTimer;//variable para generar un delay en mandar comandos para despliegue de los puntitos
extern struct _GLOBAL_CONTEXTO gxc,gxc2,gxc3,gxc4;//variable global por contexto





/*Controla la salida segura de los menu de modo Operativo para que termine de hacer la tarea el
 * menu actual y luego se pase al siguiente menu cuando acabe y se presione el escape "x"
 *   se mete el contexto Actual 
 * */

void xControl_Principal_de_Menus_Operativo(void){
const unsigned short int DELAY_ESCAPE_BARRA_DETECCION=10000;//timepo de espera para desplegar el contexto que sigue
const unsigned short int DELAY_ESCAPE_BARRA_DONE=45;//timepo de espera para activar de nuevo el case DONE
const unsigned long int MINUTOS_10=700000; //INTERRUPCION DE 1miliseg
const unsigned long int MINUTOS_30=MINUTOS_10*3;
static unsigned char waitsDDS,timeWaitsDDS;//Esperar a que se limpie bien la pantalla antes de mandar llamar el contexto de portal
static unsigned char estado3;
const unsigned char SIZE_MEMO=4;
const unsigned char PID_menu_driver4=PID4;
//auto unsigned char i;
if(menu.ret.u.bit.fInit_Menu){
  if(menu.b.b.MenuPendiente){
		switch(estado3){//Maquina de Estados
			case 1:if(menu.contexto.Modificado==ESTAS_SEGURO){
						menu.contexto.Modificado=menu.contexto.final;
						menu.contexto.Actual=ESTAS_SEGURO;
						estado3=7;break;}
				   if(menu.contexto.control==TEXT_PROCESSOR){
					     menu.contexto.Anterior=menu.contexto.Actual;
						 menu.contexto.Actual=TEXT_PROCESSOR;
						 estado3=6;break;}
				   else {if(menu.contexto.final>0){estado3=7;}} 
				   break;
			case 6: switch(menu.contexto.final){
				         case AJUSTE_DE_PRODUCTO:
				         case AJUSTE_DE_SISTEMA:
				         case AJUSTE_PARAMETRICO_DE_PRODUCTO:
				        	                     menu.contexto.control=SERVICIO;
				        	                     estado3++;
				        	                     break;
				         case AJUSTE_DE_PRODUCTO_MODE:
				        	      if(menu.contexto.Anterior==NUEVO_PRODUCTO){
				        	    	    menu.contexto.control=SERVICIO;
				        	    	    estado3++;}
				        	  break;
				         case NOMBRE_PRODUCTO:
							   if((menu.contexto.padre==NUEVO_PRODUCTO)&&
								  (menu.contexto.Anterior==AJUSTES_AVANZADOS)&&
								  (menu.contexto.Modificado==AJUSTE_DE_PRODUCTO_MODE)&&
								  (menu.contexto.Actual==TEXT_PROCESSOR)){
							              menu.contexto.control=SERVICIO;
							              estado3++;}
				        	  break;//fin case NOMBRE PRODUCTO----------
				         default:break;}
			       break;
			case 7:if(vfd.bits.b.TxBuffOFF)//buffer de Tx vacio?
					 if(!menu.b.b.isBusy)//No se esta graficando  	  
					       estado3++;
				   break;
			case 8:if(delay_us_v3(100,&waitsDDS))estado3++;break;
			case 9: InitArbolMenu(menu.contexto.final);//menu que se va desplegar crear los pointers de la estructura de men
				    estado3=81;				    
				    break;
			case 81:if(recurso.solicitar(_VFD_,PID_menu_driver4)) estado3++; break;
			case 82:if(recurso.v.asignado==PID_menu_driver4){
				      if(MenuActualScreen.func1())//se despliega el Menu¡¡
			                estado3=89;}
					else{estado3=81;}
			        break;
			case 89:if(delay_ms_VFD(5)) estado3++;break;
			case 90:if(vfd.bits.b.TxBuffOFF){//buffer de Tx vacio?
				       if(menu.b.b.isMenu){menu.b.b.isMenu=0;//todavia no se despliega el menu
				    		 menu.b.b.MenuPendiente=0;
				    		 recurso.devolver(_VFD_,PID_menu_driver4);
				    		 menu.contexto.Actual=menu.contexto.final;
				    		 if(menu.contexto.Actual==PANTALLA_DDS)
				    			  vfd.bits.b.DDSon=1;
				    		 estado3=0;}}
			       break;
			default:estado3=1;break;}
	   }}//fin de bandera de Menu Pendiente-------------------------------------------
}//fin del control operativo del menu de escape-----------------------------------------
    
	  
	

  
  
  
//monitorea y autoriza el despliegue de una nueva pantalla, en base de delay de tiempo
//delay1: tiempo que espera para que se detenga la barra de deteccioon
//delay2: timepo que espera para construir el menu y liberar el teclado algo asi
//void monitorDelayDisplay(unsigned short int delay1,unsigned short int delay2){
//           switch(semMenu2){
//						 case DONE: //keypad.Status.bits.enable=FALSE; //disable_keyboard_IRQ();
//						            SemaforosTemporizadosController(SEM_CONTEXTO_CAMBIO,WAIT,0);//WAIT=10d=0Ah
//									semMenu2=POIN1;
//									break;
//						 case POIN1:if(SemaforosTemporizadosController(SEM_CONTEXTO_CAMBIO,READ,delay1)==DONE)//DONE=11d=0bh           
//										semMenu2=POIN2;
//									break;
//						 case POIN2:DisplayMenu();
//									semMenu2=FREE;
//									//Timer_enable_keyboard_IRQ
//									SemaforosTemporizadosController(SEM_CONTEXTO_FIN,WAIT,delay2);//WAIT=10d=0Ah
//									break;
//						 default:if(SemaforosTemporizadosController(SEM_CONTEXTO_FIN,READ,delay2)==DONE){
//								          semMenu2=DONE;
//						                  menu.debug43=0x0D;}
//           }//finn switch
//}//fin monitorDelayDisplay-------------------------------------------------------------


//vemos si el semaforo esta listo para liberarse o cambio de estado en base a qe
//se activa el timer y nosotros entramos a revisar en el hilo principal.
//cambio de contexto por esto es un delay operativo para dar tiempo para desactivar cosas y 
//leugo cambiar de contexto
unsigned char SemaforosTemporizadosController(unsigned char semaforo,unsigned char status,unsigned short int delay1){
static unsigned short int cambioContextoCounter;
//static unsigned short int contextoControlandoDelay;//contexto actual que estamos controlando
//const unsigned short int DELAY_ESCAPE_BARRA_DETECCION=10000;//timepo de espera para desplegar el contexto que sigue
//const unsigned short int DELAY_ESCAPE_BARRA_DONE=45;//timepo de espera para activar de nuevo el case DONE

	if((TM1_IRQ1 & 0x02)==0x02){//bandera que activa el timer para entrar aqui cada 1miliseg
		 TM1_IRQ1&=0xFD; //1111 1101
		  switch(semaforo){
			  case SEM_CONTEXTO_CAMBIO:
				       switch(status){//WAIT=10d
				           case WAIT:cambioContextoCounter=0;
				                    break;//fin de case WAIT  READ=24h=36d
				           case READ:if(cambioContextoCounter++>delay1)
				        	               return DONE;//DONE=11d se cumplia el tiempo
				                     else return BUSY;//BUSY=13me d
				                     break;//fin de case READ
				           default:break;}
 				       break;//FIN break SEM_CONTEXTO_CAMBIO
			  case SEM_CONTEXTO_FIN:
				        switch(status){
				           case WAIT:cambioContextoCounter=0;
				                     break;
				           case READ:if(cambioContextoCounter++>delay1)
	        	                           return DONE;//se cumplia el tiempo
	                    	        else return BUSY;
	                                break;//fin de case READ
				           default:break;}
	                    break;//fin brake   SEM_CONTEXTO_FIN   
			  default:break;
		  }//fin de switch semaforo 
		  
//    cambioContextoCounter++;		  
	}//fin de bndera de revision del timer si la activo y podemos entrar
}//fin controlador de semaforos temporizados


 
//despliega variables dinamicas en el menu actualmente seleccionado
void menuTemporizadorOperativo(void){
//static unsigned char menu.contexto.Actual0;
static short int counter;
static unsigned long int lcounter;
const unsigned long int timerN=10000000;//usado en ENTRADA_TACO
     	
	 switch(menu.contexto.Actual){
		 case ENTRADA_TACO: lcounter=SOmenuEntradaTacoMonitor(lcounter,timerN);break;
		 
	 }//fin contexto Actual----------------------------------------------------
}//fin de menuTemporizadorOperativo-------------------------------------------


//menu de entrda monitor desplegamos elos pulsos por segundo cuando el menu esta activo
//desplegamos los pulsos por minuto en timepo real que se muestran en pantalla
unsigned long int SOmenuEntradaTacoMonitor(unsigned long int counter,unsigned long int time){
unsigned char a[3];	  
unsigned long int c;
      c=counter;
      if(++c>time){
    	c=0;
    	getASCIIFromUChar1(EntradasSistemaReg.PulsosPorSegundo,&a[0]);
        displayUChar(&a[0],POSX_MENU_TACO,POSY10);
       }//fin menu.contexto.Actual==ENTRADA_TACO
return c;        
}//fin SOmenuEntradaTacoMonitor----------------------------------------------




/*  Controllador principal que despliegua los valores delos ADC's en la pantalla del DDS 
 	 Despliega los puntitos en el menu DDS, tomando en cuenta
 	 un tiempo dado en el timer tambien, se ejecuta
 	 cada ciclo principal del main operativo
 	 * DEBE ESTAR INIZIALIZADO EN DSP LA LISTA FIFO
 	 * */
void Controlador_Driver_Procesador_de_Señales_DDS_Operativo(void){
//unsigned char x,y;    			
static unsigned char stateDDSplot,init;    			
//unsigned char dat[13],i;			
static unsigned char delay,p2;//delay actual que se este ejecutando
//unsigned char regx,regy,lo;//el regitro del actual pixel a desplegar y el valor minimo del registro, de la memoria de pixels desplegados actualmente es el valor de x ya que se guarda en horisontal como las teles, la y solo ayuda para ver la altura del registro
//coordn16 coordenadas;
//static unsigned short int countLineSwitch;
static unsigned short int delay123;
static unsigned char ix,iy,init1,pen;
static unsigned long jj,h=1234567890;
static unsigned long penxy;
static unsigned long long int prueba;
unsigned char var=0;
//signed short int dx,dy;s

//head4=dds.SaveTempPix.headx;
if(1){//semaforoDDS(READ)==VERDE){
	//semaforoDDS(ROJO);
 if(menu.contexto.Actual==PANTALLA_DDS){//debe entrar aqui despeus de un tiempo depues que se inizialize las fifos.
	 if(dds.Bnderas.bit.DDS1_BORRAR==TRUE)
			      return;  //var=DDS_BORRAR;
	 else
		 if(dds.Bnderas.bit.EventDDS==TRUE) //DDS1.Banderas.bit.EventDDS==TRUE)
			 if(dds.Bnderas.bit.DDS_Reload)//se estan pintando los puntitos del nuevo zoom
				  return;//esperar que acabe el repintado
		     else var=EVENT_DDS_ON;
		
	 switch(var){
    	 case EVENT_DDS_ON:
					if((TM1_IRQ1&0x40)==0x40){              
							TM1_IRQ1&=0xBF;//1011 1111
							if(ix==Deteccion.signalDDSx){
								if(iy==Deteccion.signalDDSy){
//									 semaforoDDS(VERDE);								 
									 return;}}
							ix=Deteccion.signalDDSx;
							iy=Deteccion.signalDDSy;
					        pen=1;//bajo, para pintar los punto en tiempo real
//					        FIFO_Display_DDS_Pixel_push(ix,iy,pen);}//debug //_v2
//					        if(savePixel(ix,iy))
//					           vfd.f.append(ix,iy,_PUNTO_);
					}   
                    break;
    	
        
    	 default:break; }//fin switch		
   }//fin contexto pantala DDS arewe in
  //semaforoDDS(VERDE); 
    }//fin semaforo DDS

//head5=dds.SaveTempPix.headx;
}////display de la senal de los ADC'S en el DDS por interrupcion---------------------------------------




///*Despliega en Pantalla tanto datos, numeros, chars y 
// * barra de deteccion al mismo tiempo */
//void Display_Y_Barra_Deteccion_Controlador_Operativo(void){
//	 switch(menu.contexto.Actual){
//		 case PORTAL_INICIO:
//		 case AJUSTE_DE_VIBRACION:
//		 case AJUSTE_PARAMETRICO_DE_PRODUCTO:
//		 case PARAMETROS_DE_DETECCION:
//		 case AJUSTE_TIEMPO_RECHAZO: Display_Nums_Chars_BarraDeteccion_DriverController();break;
//		 default:
//			switch(menu.contexto.Anterior){
//				case PORTAL_INICIO:
//				case AJUSTE_DE_VIBRACION:
//				case AJUSTE_PARAMETRICO_DE_PRODUCTO:
//				case PARAMETROS_DE_DETECCION:
//					       if(menu.bytes.semMenuPendiente==TRUE)
//					    	   if(Deteccion.BarraDeteccionStatus==BUSY)
//					    		   Display_Nums_Chars_BarraDeteccion_DriverController();
//					    break;     
//				default:break;}				
//			break;}//fin default and primer switch	
//}//FIN DE  Controlador OPERATIVO de despliegue de datos y chars en pantalla con barra de deteccion

//
////regresa TRUE si el byte a evaluar que debe ser el primero es un comando
//void isCommand(unsigned char c){
//	switch(c){
//		case CMD_POS_CHAR:
//		case COMANDO_POS:
//		case COMANDO_CHAR:
//		case CMD_POS5CHARS:return TRUE;break;
//		default:return FALSE;break;}
//}//fin is Comando-------------------------------------------------------------
//

//CONTROLADOR DE DESPLIEGUE OPERATIVO DE NUMEROS CHARS Y BARRA DE DETECCION
//void Display_Nums_Chars_BarraDeteccion_DriverController(void){
//static unsigned char estado65,estado65wait;
//static unsigned char box0,box,box_builder,x,y;
////unsigned char c;
//    switch(estado65){
//    	case BDe1: if(menu.bytes.semMenuPendiente==TRUE)//nos vamos a cambiar de menu
//     	                  return;
//    		       if(Deteccion.CuadroMadreReady==FALSE)//esta construido el cuadro madre?
//    		                 return;
//    		       if(isFIFO_DDS_Display_Char_empty2())//  isFIFO_num_Not_Empty
//    		                 goto box654;
//    		       else{  Deteccion.BarraDeteccionStatus=BUSY;// aqui desplegamos numeros y chars 
//					      FIFO_Display_DDS_Char_pop2(&x,&y);//c=FIFO_barraNum_pop();
//					      estado65=BDe_wait5;
//					      Deteccion.flag_TX_box=0;
//					      if(y==0xFE){ //es 0xFE=es char
//					           estado65wait=BDC01;  //posicion de envio de char 
//								return;}
//						  if(y==0xFD){ //comando bold, fd=es comando bold,este comando es de otro menu, se quedo la fifo, por alguna razon
//							   //__asm(Halt);// debug estado65wait=DDSb1;//command bold
//							   return;}//debug pendiente en esste halt
//						  else estado65wait=DDSp1x;//estado para enviar posicion
//						  return;
////					      switch(c){//obtenemos a donde saltamos
////							  case CMD_POS_CHAR: estado65=BDPC01;break;
////							  case COMANDO_POS:  estado65=BDP01;break;
////							  case COMANDO_CHAR: estado65=BDC01;break;
////							  case CMD_POS5CHARS:estado65=BDN01;break;	  
////							  default:break;}        
////						 return;
//						 break;}    		       
//box654:	           if(Deteccion.CuadroMadreReady==NUMEROS)//en este menu solo desplegamos numeros
//						return;
//                   if(FIFO_displayBoxChars_pop(&box0,&box)>0){
//    	    	        	   Deteccion.BarraDeteccionStatus=BUSY;//estamos graficando chars, no podemos cambiar de menu
//    	    	               estado65=BDe4;
//    	    	               break;}
//    		       if(FIFO_displayBox_isEmpty())	  	 
//    	                return;
//    	           if(box0>MAX_BOXES)//inizializando la var box0
//    	        	    box0=0;
//       			   box=FIFO_displayBox_pop();
//    			   if(box0==box)//siguen los mismos cuadros pintados en el mismo estado
//    				   return;
//    			   Deteccion.BarraDeteccionStatus=BUSY;//estamos graficando cajas
//    	           if(box0<box)//actual es mayor a la anterior, hay que dibujar mas cajas                   
//    	              estado65=BDe2;//CONSTRUIR cajar
//    	           else estado65=BDe3;//borrar cajas
////    	           return;
//    	           break;
//    	case BDe2: if(box0<box){//construyendo una box, box0<box
//    		           box0=Display_Construir_Box(box0,box,PARRIBA);//CONSTRUyendo box, cuando acabe una box se vacia en la var num de box
//    		           if(box0<box)//terminamos de construir las boxes?
//    		                 estado65=BDe2;
//    		           else {estado65=BDe1;
//    		                 Deteccion.boxinit=0;}}//para init contruccion de arriba o bajao desde 0, evita errores
//    	           else estado65=BDe1;
//    	           break;
//    	case BDe3: if(box0>box){ //borrando boxes,
//    				   box0=Display_Construir_Box(box0,box,PABAJO);
//                       if(box0>box)
//                    	   estado65=BDe3;
//                       else {estado65=BDe1;
//                       	     Deteccion.boxinit=0;}}
//    				else estado65=BDe1;
//                    break;
//    	case BDe_wait:if(Deteccion.flag_TX_box>1)
//    		                 estado65=estado65wait;
//    		          break;
//    	case BDe_wait2:if(Deteccion.flag_TX_box>500)
//    	    		        estado65=estado65wait;
//    	    		   break;
//    	case BDe_wait3:if(Deteccion.flag_TX_box>2)
//    	    	    	    estado65=estado65wait;
//    	    	       break;   
//    	case DDSe_wait3x:    	       
//    	case BDe_wait4:if(Deteccion.flag_TX_box>21)
//      	    	    	    estado65=estado65wait;
//    	      	       break;    		
//    	case DDSe_wait5x:if(Deteccion.flag_TX_box>800)
//    		                estado65=estado65wait;
//    		    	      	    break;
//    	case BDe_wait5:if(Deteccion.flag_TX_box>40)
//    	      	    	   estado65=estado65wait;
//    	    	      	       break;    	    	      	    
//    	case BDe_wait6:if(Deteccion.flag_TX_box>90)
//    	    	      	    	   estado65=estado65wait;
//    	    	    	      	       break;     	    	       
//    	case BDe4: if(box==0xFE)//y=FEh? es char sino es posicion
//    		            estado65=BDe5;//Es un char, vamos a mandar un char
//    			   else estado65=BDe9;//vamos a mandar una posicion
//                   break;
//    	case BDe5: VFDserial_SendChar(box0);//desplegar un char
//				   estado65=BDe_wait3;
//				   estado65wait=BDe1;
//			 	   Deteccion.flag_TX_box=0;
//				   break;
//    	case BDe9: VFDserial_SendChar(0x1F);//comando 1 desplegar una posicion.
//    	           estado65=BDe_wait;
//    	           estado65wait=BDe10;
//				   Deteccion.flag_TX_box=0;
//    	           break;
//    	case BDe10: VFDserial_SendChar(0x24);//comando 1
//				   estado65=BDe_wait;
//				   estado65wait=BDe11;
//				   Deteccion.flag_TX_box=0;
//				   break;
//    	case BDe11: VFDserial_SendChar(box0);//comando 1
//				   estado65=BDe_wait;
//				   estado65wait=BDe13;
//				   Deteccion.flag_TX_box=0;
//				   break;
//    	case BDe13: VFDserial_SendChar(0x00);//comando 1
//				   estado65=BDe_wait;
//				   estado65wait=BDe14;
//				   Deteccion.flag_TX_box=0;
//				   break;    	         
//    	case BDe14: VFDserial_SendChar(box);//comando 1
//				   estado65=BDe_wait;
//				   estado65wait=BDe15;
//				   Deteccion.flag_TX_box=0;
//				   break;
//    	case BDe15:VFDserial_SendChar(0x00);//comando 1 COMANDO POSICION Y 5 CHARS
//				   estado65=BDe_wait2;
//				   estado65wait=BDe1;
//				   Deteccion.flag_TX_box=0;
//				   break; 
//    	
//       
//        case DDSp1x:VFDserial_SendChar(0x1FU);//POSICION MAQUINA DE ESTADOS INICIA
//                    Deteccion.flag_TX_box=0;
//                    estado65wait=DDSp2x;
//                    estado65=DDSe_wait3x;
//					break;			
//		 case DDSp2x:VFDserial_SendChar(0x24U);//POSICION MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//		            estado65wait=DDSp3x;
//					estado65=DDSe_wait3x;
//					break;			
//		 case DDSp3x:VFDserial_SendChar(x);//POSICION MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//		            estado65wait=DDSp4x;
//					estado65=DDSe_wait3x;
//					break;				
//		 case DDSp4x:VFDserial_SendChar(0x00);//POSICION MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//		            estado65wait=DDSp5x;
//					estado65=DDSe_wait3x;
//					break;	
//		 case DDSp5x:VFDserial_SendChar(y);//POSICION MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//		            estado65wait=DDSp6x;
//					estado65=DDSe_wait3x;
//					break;	
//		 case DDSp6x:VFDserial_SendChar(0x00);//POSICION MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//					if(dds.Bnderas.bit.Apagando==TRUE)//esta linea no va aqui, porque barra no tiene apagando
//						estado65wait=DDSe_wait5x;
//					else
//						estado65wait=BDe1;
//					estado65=BDe_wait6;
//					break;	
//					
//		 case BDC01:VFDserial_SendChar(x);//Char MAQUINA DE ESTADOS 
//		            Deteccion.flag_TX_box=0;
//					if(dds.Bnderas.bit.Apagando==TRUE)
//						estado65wait=DDSe_wait5x;
//					else 
//						estado65wait=BDe1;
//				    estado65=DDSe_wait3;
//					break;	
//        	        
//        	        
//				    
//    default:estado65=BDe1;break; }//FIN SWITCH principal 65
//
//}//FIN DE CONTROLADOR MAESTRO DE DESPLIEGUE DE NUMEROS DATOS Y BARRA DE DETECCION

//    
//unsigned char Display_Construir_Box(unsigned char box2build,unsigned char metabox,unsigned char modo){
//static unsigned char estado66,estado66wait,box1;//,boxinit;
//const static unsigned char SIZE_DAT=14;
//static unsigned char dat[SIZE_DAT],ii;
//unsigned char mode,pen,ret=0,ret2=0;
//unsigned short int x1,y1,x2,y2;
//coordn16 coordenadas;
//
//      if(Deteccion.boxinit==0xAA)
//    	    goto DCB1;
//      else {Deteccion.boxinit=0xAA;
//            box1=0;
//            if(modo==PARRIBA)
//                estado66=BOXe1;
//            else {if(modo==PABAJO)
//                     estado66=BOXa1;
//            	  else __asm(Halt);}}//Debug meter a control de errores
//DCB1: if(modo==PARRIBA){//vamos a construir boxes
//		switch(estado66){
//			  case BOXe1:if(box2build==metabox)
//				                return metabox;
//			  	  	  	 else{estado66=BOXe_wait;//porque cuando acaba un ciclo ya no regresa a su ultimo delay por eso neceita un delay antes de un nuevo ciclo
//			  	  	  	      estado66wait=BOXe2;
//			  	  	  	      break;}
//			  	  	  	 break;                   
//			  case BOXe_wait:if(Deteccion.flag_TX_box>9)//Si:122,10,9   No:6,8
//				                   estado66=estado66wait;
//			                 break;   
//			  case BOXe2:getBoxPattern(box2build,&mode,&x1,&y1,&x2,&y2);
//			  	  	  	 dat[0]=0x1F;dat[1]=0x28;dat[2]=0x64;dat[3]=0x11;
//						 dat[4]=mode;
//						 dat[5]=pen=1;		
//						 coordenadas.coord16=x1;		
//						 dat[6]=coordenadas.byte[LO];		
//						 dat[7]=coordenadas.byte[HI];		
//						 coordenadas.coord16=y1;		
//						 dat[8]=coordenadas.byte[LO];		
//						 dat[9]=coordenadas.byte[HI];
//						 coordenadas.coord16=x2;		
//						 dat[10]=coordenadas.byte[LO];		
//						 dat[11]=coordenadas.byte[HI];		
//						 coordenadas.coord16=y2;		
//						 dat[12]=coordenadas.byte[LO];		
//						 dat[13]=coordenadas.byte[HI];
//						 ii=0;
//			             estado66=BOXe3;
//			             break;
//			  case BOXe3:VFDserial_SendChar(dat[ii++]);
//			             if(ii==SIZE_DAT){
//			            	 estado66=BOXe_wait;
//			            	 estado66wait=BOXe1;
//			            	 Deteccion.BarraDeteccionStatus=BUSY_WAIT;
//			            	 vfd.v.timer=
//			                 ret=1;//terminamos de incrementar el display de una BOX
//			                 break;}
//			             else{ 	 
//			            	 estado66=BOXe_wait;
//			            	 estado66wait=BOXe3;}
//			  			 Deteccion.flag_TX_box=0;
//			  			 break;
//			  default:__asm(Halt);//estado66=BOXe1;break;  
//			  	  	  __asm(nop);
//			      	  break;
//      }}	//fin de switch---PARRIBA--------------------------------
//   else{if(modo==PABAJO){
//    	  switch(estado66){ 
//    	    case BOXa1:if(box2build==metabox)
//    	    	                return metabox;
//    	    		   else{estado66=BOXa_wait1;//porque cuando acaba un ciclo ya no regresa a su ultimo delay por eso neceita un delay antes de un nuevo ciclo
//    	    			    estado66wait=BOXa2;
//    	    		        break;}
//    	    	       break;
//    	    case BOXa_wait1:if(Deteccion.flag_TX_box>9)//Si:122,10,9   No:6,8
//    	    				        estado66=estado66wait;
//    	    			   break; 
//    	    case BOXa2:getBoxPattern(box2build,&mode,&x1,&y1,&x2,&y2);
//    	    		     dat[0]=0x1F;dat[1]=0x28;dat[2]=0x64;dat[3]=0x11;
//						 dat[4]=mode;
//						 dat[5]=pen=0;//borrar		
//						 coordenadas.coord16=x1;		
//						 dat[6]=coordenadas.byte[LO];		
//						 dat[7]=coordenadas.byte[HI];		
//						 coordenadas.coord16=y1;		
//						 dat[8]=coordenadas.byte[LO];		
//						 dat[9]=coordenadas.byte[HI];
//						 coordenadas.coord16=x2;		
//						 dat[10]=coordenadas.byte[LO];		
//						 dat[11]=coordenadas.byte[HI];		
//						 coordenadas.coord16=y2;		
//						 dat[12]=coordenadas.byte[LO];		
//						 dat[13]=coordenadas.byte[HI];
//						 ii=0;
//						 estado66=BOXa3;
//						 break;
//    	    case BOXa3:VFDserial_SendChar(dat[ii++]);
//						 if(ii==SIZE_DAT){
//							 estado66=BOXa_wait1;
//							 estado66wait=BOXa1;
//							 Deteccion.BarraDeteccionStatus=BUSY_WAIT;
//							 ret2=1;//terminamos de incrementar el display de una BOX
//							 break;}
//						 else{ 	 
//							 estado66=BOXa_wait1;
//							 estado66wait=BOXa3;}
//						 Deteccion.flag_TX_box=0;
//						 break;		 
//    	    default:__asm(Halt); //Debug error de diseño de software   
//    	            __asm(nop);
//    	            break;
//          }}}//fin de modo  PABAJO, else, switch
//
//return box2build+ret-ret2;//regesamos el mismo porque todavia no terminamos de desplegar la box
//}    //fin de Display_Construir_Box---------------------


/* procesador central de datos al display
 *  manda todo lo que hay que deplegar a una FIFO individul para chars-posicion
 *  puntos, lineas o cajas
 * */
//void  Procesador_Operativo_de_Graphicos(void){
//static unsigned char estado;
//	
//	switch(estado){
//	  case GRAFICANDO:
//		              break;
//	  default:if(menu.b.b.graficando==TRUE)
//		            return;
//	          if(menu.b.b.semMenuPendiente==TRUE)//hay un menu pendiente para salir de alli
//	                return;
//              break;}	
//}//fin de Procesador_Operativo_de_Graphicos-------------------------------------
//    
/*procesamiento de GUARDADO de pixeles para cambio de cuaquier
	 *  ZOOM  manual o automatico en autoajuste*/
void DDS_repaint_Zoom(void){
struct Coordf P;
signed short int x,y;	
unsigned char ucx=0,ucy=0,n;	
static unsigned short int debug1;
unsigned short int *countPoints;
static unsigned char i;
 if(dds.Bnderas.bit.EventDDS){
    if(dds.Bnderas.bit.DDS_Reload){
    	 countPoints=&debug1;
		if((TM1_IRQ1&0x40)==0x40){
				TM1_IRQ1&=0xBF;//1011 111
				if(isFIFO_DDS_Display_Pixel_llena2())
					return;
		        if(get_Pixels_Saved(&x,&y,&ucx,&ucy,&n)){
		          if(n){	P.x=x;P.y=y;
//		        	 Escalar_al_ZoomSelected(P,&ucx,&ucy);
		          }
		          else{if((ucx==0)&&(ucy==0))
		                     return;}	
		         
//				  FIFO_Display_DDS_Pixel_push(ucx,ucy,1);
				  vfd.f1.append(ucx,ucy,_PUNTO_);
		          ++*countPoints;
		          }
		        else{dds.Bnderas.bit.DDS_Reload=FALSE;
		             dds.Bnderas.bit.debug=TRUE;
		     //        semaforoDDS(VERDE); 
		             dds.rePaint.debug6=0xAA;
		             dds.rePaint.ii=0;}}}
    else{if((ucx==32)&&(ucy==1)){
    	       __asm(nop);}
    	 if(is_Empty_FIFO_save_Temp_pixel_DDS())
		 				   return;
		 pop_FIFO_save_Temp_pixel_DDS(&x,&y);
		 P.x=x;P.y=y;
//		 Escalar_al_ZoomSelected(P,&ucx,&ucy);  	   
		 save_Pixels(x,y,ucx,ucy);
		 if((ucx==32)&&(ucy==1)){
		      __asm(nop);} 
         }}
}//DDS_repaint_Zoom----------------------------------------------------


/*Driver principal de transmision al display 
 * por serial* */
void Transmision_al_Display_TFT(void){
static unsigned char estado;
static uint8_t dato,cmd,len;
//const unsigned char FIN1=0xF9U;
unsigned char *x;
	x=&dato;
    switch(estado){
    	case 1:if(vfd.TxDisp.ncount>0){estado++;}break;
    	case 2:if(vfd.fifo.pop(x)){
    		      if(*x==STX){estado++;}else{estado=FIN;}}
    		   else{estado=FIN;}break;
    	case 3:if(vfd.fifo.pop(x)){
    			  cmd=*x;
    			  if(buscarComando(cmd)){estado++;}
    			  else{estado=FIN;}}
    			break;
    	case 4:if(VFDserial_SendChar(STX)==0){estado++;}break;
    	case 5:if(VFDserial_SendChar(cmd)==0){estado++;}break;
    	case 6:if(vfd.fifo.pop(x)){
    		       len=*x;
    		       if(len>0){estado++;}}break;
    	case 7:if(VFDserial_SendChar(len)==0){estado++;}break;
    	case 8:if(len==0){estado=11;}else{estado++;}break;
    	case 9:if(vfd.fifo.pop(x)){dato=*x;estado++;len--;}break;
    	case 10:if(VFDserial_SendChar(dato)==0){estado=8;}break;
    	case 11:if(VFDserial_SendChar(ETX)==0){estado=FIN;}break;
    	case FIN:
    	default:len=0;cmd=0;dato=0;estado=1;break;}//fin de estado
}//fin de transmision del display tft

//buscar comando de transmision disponible
//regresa TRUE si encuntra comando
uint8_t buscarComando(uint8_t cmd){
uint8_t ret;	
	switch(cmd){
		case CMD_DDS:
			         ret=TRUE;break;
		default:ret=0;}
return ret;
}//fin buscar comando+++++++++++++++++++++++++++


/* obtener una instancia libre  para manipular hilos
 * de metodos al mismo tiempo del mismo metodo
 * */
unsigned char get_instancia(void){
unsigned char i;
unsigned char ret=0xAA;
unsigned char control;  	
auto unsigned char byte1[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

   control=menu.subMenu.control;  
   for(i=0;i<8;i++){//SIZE_PID
	  if((control&byte1[i])==0){
              ret=i;
              control|=byte1[i];
              break;}   		 
      }//fin for-----------------------
   if(ret==0xAA){
	    __asm(nop);
        __asm(Halt);}
menu.subMenu.control=control;
return ret;
}//fin get instancia----------------------------------------
//
//void reset_Instancia(unsigned char instancia){
////	menu.subMenu.estado[instancia]=0;
////	//menu.subMenu.control[instancia]=0;
////	menu.subMenu.aux1[instancia]=0;
////	menu.subMenu.aux2[instancia]=0;
////	menu.subMenu.aux2[instancia]=0;
//}//-----------------------------------------------------------
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
void return_instancia(unsigned char instancia){
auto unsigned char byte1[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//auto unsigned char byte1[]={};
auto unsigned char c;
auto unsigned char control;
     control=menu.subMenu.control;
     c=byte1[instancia]^0xFF;//invertir valores
     control&=c;
     menu.subMenu.estado[instancia]=0;
     menu.subMenu.control=control;     
}//fin regresar la instancia que se esta ocupando-------------

//p: pointers, n: numero de pointers
//void xControlador_Principal_de_Comunicaciones_Perifericas(void){ 
//	
//     Transmision_Consola();
//    // comunicacion_IIC_con_Memoria_EEPROM();
//    // comunicacion_IIC_con_NVRAM();
//    // comunicacion_serial_al_VFD_display_Transmission();
//}//_fin de controlador principal de comunicaciones perifericas---------



void Pruebas_Y_Debugeo_Testeo_del_Systema(void){
static unsigned char estado;	
static unsigned long long int delayvar;
const unsigned long long int DELAY_CHANGE_2_DDS=29000;//9segundos
static unsigned char inst;
const unsigned char _RESET_=0x80U;   
static unsigned  char resetControl=0;

static unsigned short int s;
static struct Ec_Recta f;
struct Coordf P;
struct Coordsf P58;
   P.x=96;P.y=-64;
   switch(estado){
	   case 1:if(delay_general_1ms(&delayvar,DELAY_CHANGE_2_DDS,&inst)){
		         menu.contexto.Anterior4=0;
				 menu.contexto.Anterior3=0;
				 menu.contexto.Anterior2=0;
				 menu.contexto.Anterior1=0;
				 menu.contexto.Anterior0=PORTAL_INICIO;
				 menu.contexto.Anterior =MENU_INSIGHT;
		         cambio_de_contexto_v2(PANTALLA_DDS);
		      s=100;f.m=-2.13477;f.b=140.938;
		      estado++;}
	          break;
	   case 2:if(resetControl==0xFF)
		           setReg8(ResetController_RCR, _RESET_);
//	          estado++;
		      break;
//	   case 3: if(s==240){
//		              __asm(nop);}
//		       P58=get_x2r(s,f,P);
//	           estado++;break;
//	   case 4: s+=10;estado=3;break;
	     

	   default:estado=1;break;}//fin switch--------------------------------------


}//fin de pruebas de debugeo testeo del Sistema-------------------

