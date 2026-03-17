/*
 * errorController.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */


/*
 * errorController.c
 *
 *  Created on: Jul 14, 2015
 *      Author: chals
 */
#include "errorController.h"
#include "LED3.h"
#include "LED2.h"
#include "TI1.h"
#include "delay.h"
#include "LEDERRCOMM.h"
#include "system.h"
#include "VFDmenu.h"
#include "LED_BOOT.h"
//#include "LED_COMM.h"
#include "LED12_SYS.h"
#include "LED3_Process.h"
#include "LED56_COMM_WARN_MON.h"
#include "queue.h"
#include "IOcomms.h"
#include "Reloj.h"
#include "FSYNC.h"
#include "CS0.h"
#include "CS1.h"
#include "LED4_SYS_MON.h"
#include "ADC.h"
#include "frecqController.h"






#define COMMI2CFAILS  100
#define COMMSPIFAILS  1
#define COMMI2COK     20
#define COMMSPIOK     30 

#define LEDERRTIME    300//tiempo que queda encendido el led de errores de communicacion cuando sale un error
#define VEL           2//constante, 
#define VEL1          3//velocidad de 3, primer ciclo no entra hasta el segundo, por tanto velocidad de 2
#define VEL2          4





unsigned long int ledloopvar; //variable para controlar el ledloop
unsigned char ledErrCommStatus;//varible que controla los led de errores de comunicaciones
unsigned short int  ledErrCommVar=0;//varible que controla los led de errores de comunicaciones
unsigned short int  ledvel,ledvel1=0; //velocidad de parpadeo del led de errores de comunicacion
//unsigned char ErrorHardware; //error de hardware se muestra en los led de flip-flop
unsigned char enableErrorControls;//habilitar el control de los errores
unsigned char realTimeError;//guarda el error que se despliega en pantalla en tiempo real
extern struct _SERIAL_ serial;//_Vector_ serial;
extern struct DDS_GEN ddsgen;


struct ErrorList_nodo eList[SIZE_LIST_ERRORS],*ehead;
struct ErrorHardware  eHw[SIZE_ERROR_HW];//lista de errores de hardware
//struct _Error323_ sysMon;
unsigned char ErrorStatusLeds;
struct _Error323_ e;//error controller data struct
extern  volatile unsigned char  TM1_IRQ1;


void Select_SPI_device(unsigned char device){
	switch(device){
		case DDS:     CS0_PutVal(TRUE); CS1_PutVal(TRUE);break;
		case ADC__CH0:                  CS1_PutVal(FALSE);break;
		case ADC_CH0: CS0_PutVal(FALSE);CS1_PutVal(FALSE);break;
		case ADC_CH1: CS0_PutVal(FALSE);CS1_PutVal(TRUE);break;
		case ADC__CH1:                  CS1_PutVal(TRUE);break;
		default:break;}
}//fin select SPI device----------------------------

void init_ErrorController(void){
	  init_Leds();	
	  ledloopvar=0;
	  init_ListaError();
	  init_Lista_errorHardware();
	  ErrorStatusLeds=0;	
//	  sysMon.debug1=verificar_head_FIFOs_;
}// fin del controllador maestro de errores-+++++++++++++++++++++++++++++++++++++++

void init_Leds(void){
	  //-----------------inicia  prueba de leds
	  LED3_PutVal(FALSE);
	  LED2_PutVal(FALSE);
//	  LEDACT_PutVal(FALSE);
	  LEDERRCOMM_PutVal(TRUE);
//	  delay_ms(100);//--------
	  LED3_PutVal(TRUE);
	  LED2_PutVal(TRUE);
//	  LEDACT_PutVal(FALSE);
	  LEDERRCOMM_PutVal(FALSE);
//	  delay_ms(250);
	 // termina prueba de leds e inicia estado de inicio
	  LED3_PutVal(TRUE);
	  LED2_PutVal(FALSE);
//	  LEDACT_PutVal(TRUE);
	  LEDERRCOMM_PutVal(TRUE);
	  TI1_Enable();
//	  sysMon.debug1=
	  ledErrCommStatus=OFF;
	 // TI1_EnableEvent();
	  LED56_COMM_WARN_MON_PutVal(TRUE);//apagado
	 
}//fin init leds error indicators, system health indicators++++++++++++++++++


/* desabilita el control de los errores para evitar fallas y errores en la inizializacion si se
 *  detecta un error antes de inizializar todo el sistema   */
void disable_ErrorControls(void){enableErrorControls=FALSE;}
void enable_ErrorControls(void){ enableErrorControls=TRUE;} //se habilita el contol de los errores


void init_ListaError(void){
unsigned short int i;    
	  for(i=0;i<=SIZE_LIST_ERRORS;i++)//debug pero codigo incompleto
	      eList[0].status=NONE;  //debug   
}// initialization de lista errores------------------------------------------------

/* Lista Doblemente Ligada que guarda los Errores de Hardware Registrados
 * */
void init_Lista_errorHardware(void){
struct ErrorHardware *head,*p;	

	       head=getheadeHw(); //&eHw[0];
		   for(p=head;p<=&eHw[SIZE_ERROR_HW-1];p++){	   
			   if(p==head){
				   p->next=&eHw[1];
				   p->prev=&eHw[SIZE_ERROR_HW-1];
				   p->error=CODE_ERROR_STATUS;//nos dice que no hay errores
				   p->prioridad=NONE;}
			   else{
			    if(p==&eHw[SIZE_ERROR_HW-1]){
				    p->next=&eHw[0];
				    p->prev=&eHw[SIZE_ERROR_HW-2];
				    p->error=NONE;
				    p->prioridad=NONE;}
			    else{
			         p->next=p+1;
			         p->prev=p-1;
			         p->error=NONE;
			         p->prioridad=NONE;}}}
	      
		
}//fin initializacione de lista error hardware------------------------------------------------


//obtenemos la direccion de la cabeza de la lista de errores de hardware
struct ErrorHardware *getheadeHw(void){return &eHw[0];} 
	


void LedController_IRQ(void){//interrupcion proveniente de la IRQ principal del TIMER
	                  //entra aqui cada 200milisegundos
static unsigned short int  i,j;	
static unsigned char ErrorHardware0; //ver que el ErrorHardware no pase de 255 OJO  --> debug 
//struct ErrorHardware *p;
static unsigned char errorADC;
unsigned char modoerror=0;//modo que van a desplegar los leds el error leds 3 y 2	
static unsigned char errorVFD, errorIO;//error de comunicaciones
//const unsigned char ERROR=0xAA;
const unsigned char LED_COMM_MAX_ON=300;//tiempo encendido el LED
const unsigned char LED_COMM_MAX_OFF=30;//tiempo apagado el LE


if((TM1_IRQ1&0x40)==0x40){
	TM1_IRQ1&=0xBF; //1011 1111
 if(enableErrorControls==TRUE){	 
	  //LEDs FLIP-FLOP status del sistema
	         if((ErrorStatusLeds & 0x80)==0){ // axxx xxxx a=bandera de que funcionan los ADC,
	               if(errorADC++>250){//no detectamos ADC desde hace mucho
	            	   modoerror=CODE_ERROR_0001_ADC;//contamos las veces que detecta no haber ADCs
	            	   ErrorHardware0=CODE_ERROR_0001_ADC;
	                   errorADC=0;}}
	         else {errorADC=0;//se limpia porque detecto funcionando los ADC
	               if(ErrorHardware0==CODE_ERROR_0001_ADC){//Si ya no hay error
	            	   ErrorHardware0=0; 
	                   LED2_PutVal(TRUE);
	                   LED3_PutVal(FALSE);}}
	         //LEDs STATUS DE SISTEMA PRINCIPALES BOOT o bios
	         if((ErrorStatusLeds & 0x40)==0x40)// xbxx xxxx b= LED BOOT
	        	  ErrorHardware0=CODE_ERROR_0002_BOOT;
	         else {ErrorHardware0=0;
	               LED_BOOT_PutVal(FALSE);}
	        //LED status error de COMMS comunicaciones VFD  e IO         
	         if((ErrorStatusLeds & 0x20)==0x20){//xxcx xxxx c=LED COMM error de Comm VFD = blink slow,fast error IO, conter blink
	               if(errorVFD++>250){
	            	   errorVFD=0; 
	            	   ErrorHardware0=CODE_ERROR_0003_VFD;}}
	         else  {errorVFD=0;
	                if(ErrorHardware0==CODE_ERROR_0003_VFD){
	                   ErrorHardware0=0;
	                   
	                   errorVFD=LED_COMM_MAX_OFF;}}//when no error, it blinks, duty on=90%
	         //LED status error de COMMS comunicaciones  IO         
	         if((ErrorStatusLeds & 0x10)==0x10){//xxxD xxxx c=LED COMM error de Comm IO = blink slow,fast error IO, conter blink
	               if(errorIO++>250){
	                   errorIO=0; 
	                   ErrorHardware0=CODE_ERROR_0004_IO;}}
	         else{errorVFD=0;
	               if(ErrorHardware0==CODE_ERROR_0004_IO){
	                   ErrorHardware0=0;
	                   errorVFD=LED_COMM_MAX_ON;}}//when no error, it blinks, duty on=90%
	                         
	         
	         //Leds FLIP-FLOP Controller
	         switch(ErrorHardware0){
	           case CODE_ERROR_0001_ADC: if(i++>2200){i=0;LED2_NegVal();LED3_PutVal(FALSE);}break; 
	           case CODE_ERROR_0002_BOOT:if(i++>2200){i=0; LED_BOOT_PutVal(TRUE); break;}	
//	           case CODE_ERROR_0003_VFD: if(i++>2200){i=0; LED_COMM_NegVal(); break;}
//	           case CODE_ERROR_0004_IO:  if(i++>1500){i=0; LED_COMM_NegVal(); break;}
	                      
	           default:if(i++>2200){i=0;
	        	             LED2_NegVal();
	        	             LED3_NegVal();}//si no hay error se ejecutan normal
//	                   if(errorIO++>LED_COMM_MAX_ON){
//	                	   if(errorVFD++>LED_COMM_MAX_OFF){
//	                		      errorVFD=0; errorIO=0;
//	                        	  LED_COMM_PutVal(TRUE);}
//	                	   else{LED_COMM_PutVal(FALSE); }}

	                   break;//fin default
	           }//fin del switch-----------------------------------------------------------------
	         
	        	 
	     ErrorStatusLeds=0; //se resetea el estatus para volver a revisar el estado de todo
	         
	
   }//fin enable control de errores
    controlador_LED12(EJECUTAR);  
    controlador_LED_COMM_SYS_MON();
 
 
}//fin flag tmier1
}// fin Interrupt Request Querry OF lED cONTROLLER


/*led 3 Amarillo ON: init signal Process
 *  Flash: Active Process
 *  OFF: Signal process inactive  */
void controlador_LED3_Detect(unsigned char n){
	static unsigned char estado = 0;
	    static unsigned short int cont = 0;
	    const unsigned short int BLINK_1750MS = 1750; // 1.75 segundos
	    
	    switch(estado){
	        case 0: // estado ON
					if(cont++ > BLINK_1750MS){
	                cont = 0;LED3_Process_PutVal(FALSE); // apagar LED
	                estado = 1;}
	            	break;
	        case 1: // estado OFF
	                if(cont++ > BLINK_1750MS){cont = 0;
	                LED3_Process_PutVal(TRUE); // encender LED
	                estado = 0;}
	                break;}//fin switch
}//fin controlador_LED3_Detect----------------------------------

//cuarto led AMARILLO MUESTRA
/* STATUS DE COMUNICACION CON LA IO Y EL ESTATUS
 * DEL MONITOR DEL LOS WARNINGS*/
void  controlador_LED_COMM_SYS_MON(void){
static unsigned char estado;
static unsigned short int ncount;
const unsigned short int DELAY_NO_ACK=500;

//    
//	if(e.ERROR.LEDS==0)
//		 estado=0;
//	else{if(e.ERROR.bits.aLed4_Comm_Warnning_Sys){//ERROR_RECEPCION_ACK_IO
//		       estado=10;}} 
//	switch(estado){
//		case 0 :LED56_COMM_WARN_MON_PutVal(1);//apagar
//		        break; 	   
//	    case 10:if(ncount++>DELAY_NO_ACK){
//	    	        LED56_COMM_WARN_MON_NegVal();
//	    	        ncount=0;}
//	            break;
//	            
//		default:estado=0;break;}//fin switch-----------
//	
}//fin controlador_LED_COMM_SYS_MON-------------------------------





/* led 1 y 2  ON:indica el que se esta cargando el sistema
 * OFF: ocnfiguracion de sistema no valida
 * flash(1.75) system monitor OK,
 * los led se encienden con ZERO*/
void controlador_LED12(unsigned char n){
static unsigned char estado,semaforo,estado0;	
static unsigned short int cont;	
const unsigned char INIT2=0x55,STOP1=0xFF;
    switch(n){
    	case INIT: estado0=estado=INIT;break;
    	case WAIT: estado0=estado=WAIT;break;
    	case EJECUTAR:if(semaforo==STOP1)
    		              estado=estado0;
    				  else estado=n;
    	              break;
    	default:break;}//fin first switch
	switch(estado){
		case INIT: LED12_SYS_PutVal(FALSE);//se prende el sistem
		           semaforo=STOP1;//nose ejecuta EJEUTAR
		           break;
		case WAIT: cont=0;semaforo=STOP1;
		           estado0=estado=INIT2;
		           break;
		case INIT2:if(cont++>350){//Un rato y se apaga
					  LED12_SYS_PutVal(TRUE);//APAGAR LED
					  estado=EJECUTAR;
		              semaforo=0;cont=0;
		              break;}//liberams la maquina de estados al ejecutor
		           semaforo=STOP1; 
			       break;
		case EJECUTAR:if(cont++>3200){
			               LED12_SYS_NegVal();
		                   cont=0;}
		            break;
		default:break;}//fin switch--------------------------		
}//fin controlador de led1 1 y 2




/*Aqui acomulamos los errores de hardware en una pila y los vamos a sacar uno por uno 
 *  para limpiar todos los errores tiene que apagar y prender el sistema
 *  
 *  Se manda llamar para decirle que hay un error de hardware, lo revisa y
 *  busca si ya lo tiene registrado sino lo registra
 * */
struct ErrorHardware *ErrorController(unsigned char e){//aqui se acomulan la pila de errores solo de hardware 
struct ErrorHardware *p,*p2;
unsigned char prioridad;
 if(enableErrorControls==TRUE){
	 
	 
	switch(e){
	     case CODE_ERROR_STATUS:return getheadeHw();// en head en errores debe estar este mismo constante si no ha habido algun error
	                            break;
	     case CODE_ERROR_GET://buscamos el error con mayor prioridad  en la lista y regresa el pointer del error
	    	                 prioridad=0;
	    	                 p=getheadeHw();p2=p;
	    	                 do{if(p->error&&CODE_ERROR_HARDWARE==CODE_ERROR_HARDWARE){
	    	                	   if(p->prioridad>prioridad){
	    	                	        prioridad=p->prioridad;
	    	                	        p2=p;}
                                   p=p->next;}
                                else
                                	 p=p->next;
	    	                        }while(p!=getheadeHw());
	    	                 return (p2);
	    	                 break;
	     case CODE_ERROR_COMM_IO:prioridad=10;//error con mayor prioridad porque es el unico orita
	    	                     break;
	     default:break;
	  }//fin switch
	 
	  //insertErrorHw(e);
	  p=getheadeHw();  //controlador que guarda los errores y con su prioridad
loopec:if(p->error==NONE) {//busca un lugar libre para meter el error en la lista
		  p->error=e;
		  p->prioridad=prioridad;}
	   else{ 
		   p=p->next;
	       if(p!=getheadeHw())
	           goto loopec;}  
       }//fin enable control de errores	 
return 0;
}// fin error controller--------------------------------------------



// esta funcion ya quedo obsoleta, tiempo de vida:3.5 horas
void LedLoop(void){//nos indica si el programa esta un loop que puede ser infinito
	  
	  
}//fin de led en loop----------------------------------------------

/*If comm I2C comm error then LED=ON
 * if  comm  SPI comm error then LED FLASH fast
 * if comm  SPI & I2C fails then LED FLASHing slow
 * */
void LedErrorCommunicaciones(unsigned char status){
	  
	 switch(status){
	   case COMMI2CERR:if(ledvel1==0) ledvel1=VEL1;//es el primer fallo?
	                     else ledvel1=VEL2+1; //hay dos fallos de comunicacion
	                     ledErrCommStatus=ON;//activar alerta de fallo
	                     break;
	   case COMMSPIFAILS:if(ledvel1==0) ledvel1=VEL2;
                         else ledvel1=VEL2+1;
                         ledErrCommStatus=ON;
                         break;
	   case ERROR02_I2CCOMMFAILS:
		                 ledErrCommStatus=ON;//activar alerta de fallo
	                     ledloopvar=0;
                          break;                
	   default: eLog(CODE_ERROR_0001_ADC);  break;
	 }
	 
}//fin control de leds de Errores de Comunicaciones------------------------------------




/* FUNCION PRINCIPAL DEL CONTROL MAESTRO QUE ORIGINA LA DISTRIBUCION CENTRAL  
 *  EN EL MANEJADOR PADRE DE LOS ERRORES DEL SISTEMA 
 *  
 *  
 *  
 *  
 *  
 *  
 *  Distribuye los errores de hardware y los errores de software 
 *  HAY ERRORES QUE SE INDICAN POR
 *    PANTALLLA: ->Error_Control_VFDisplay(*p="Leyenda",[ms:blinck|0:no blink],    
 *                           [var3:[cuanto tiempo activo: [ms:time|0:permanente]]]);
 *    LEDS:    ->Error_Control_Leds(Cual Led=?,[ms:blinck|0:no blink],
 *                          [var3:[cuanto tiempo activo: [ms:time|0:permanente]]]);
 *                si se dispara el error otra vez se actializa el timer de la var3
 *                
 *                LED1:[VERDE],On:boot
 *                             OFF:sys config No valid
 *                             Blink: ??
 *                LED2:[Yellow]:On:inizializa proceso  de señales deteccion
 *                              OFF:Procesamiento de señal inactivo.
 *                              Blink:(1.2seg): Procesando Señal de deteccion
 *                LED3:[Yellow]:ON:Error de comunicaciones con IO board
 *                              OFF: ??? todavia no determinado
 *                              Blink:(0.8seg):system monitor activo
 *                LED4:[ROJO]:  ON: ??No determinado
 *                              OFF: Sin Errores, Sin detecciones
 *                              Blink:(1.75seg): Error de sistema Serio
 *                                    (300mseg): Deteccion de metal ejecutada
 *                
 *    LOGBOOK->Error_LogBook(Numero de string de Leyenda de error,
 *             timestamp,[tipo:Warnning|Error|info|Bug],param1:Conteo,
 *             param2:nivel bdeteccion);<--Se van a guardar en la NVRAM
 * */
void eLog(unsigned char log){
unsigned char error_relojA[]="ERROR_RELOJ_ANALOGO";
struct _REPORTE_ r;	
	r.Aviso=log;
	switch(r.Aviso){
	  case CODE_ERROR_0001_ADC: 
	  case CODE_ERROR_COMM_IO:r.Tipo=ERROR;
	                          r.timestamp=get_TimeStamp();
	                          //control de bounce yo creo por timer
	                          //base de datos de errores permanentes
	                          /* para guardar los errores monitoreables*/
	                          break;
	  case CODE_ERROR_BUFFER_RX_FULL:
	  case ERROR_DE_RTS_DE_UP:
      case ERROR_DE_COMUNICACION_DE_RECEPCION:
      case ERROR_RECEPCION_DE_ACK:
	  case ERROR_RECEPCION_EN_ESPERA_TIMEOUT:
	  case ERROR_CTS_CERRO_SIN_ECIBIR_ACK:
	  case CODE_ERROR_COMM_IO_RX:
	  case CODE_ERROR_ANSWER_DATOS:
	  case CODE_ERROR_FIFO_FULL: 
	  case ERROR_SW_FIFO_KEYPAD_LLENA://Error_LogBook(ERROR_SW_FIFO_KEYPAD_LLENA,timestamp(),Error,0,0);
		                              break;
	  case ERROR_RELOJ_ANALOGO:__asm(nop);
	                           e.Error13.bits.DDS_=TRUE; 
	                           break;
	           
	            
	            ddsgen.bits.Bits.display_Cntrl_mens=TRUE;
	           if(ddsgen.var.countTimeMensajeError>20000){
	             ddsgen.var.countTimeMensajeError=0;
	             ddsgen.bits.Bits.display_Cntrl_mens=0;
	             display_Error(0,0,&error_relojA[0],19);}
	           break;
	  default:break;                      
	
	
	}//fin switch-----------------------------
	
}//fin de error log---------------------------------------------------------


void setLEDerrorHardware(void){
	  
	
	
}//fin set LED error de hardware para debugear, en teoria este led nunca deberia de prender si prende hay un error de software

/* CONTROLADOR PRINCIPAL CENTRAL OPERATIVO DEL MONITOREO DE ERRORES PARA
 * DESPLEGARLOS EN PANTALLA HASTA QUE SE APRIETE EL SELET*/
void Monitor_Maestro_de_ERRORES(void){
	Manejador_de_LED_DE_STATUS();
    Manejador_de_Errores_por_Prioridad();	
}//FIN  DE ERROR MONITOR+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void Manejador_de_LED_DE_STATUS(void){
const unsigned short int LED_STATUP_DELAY=3200;		
const unsigned short int LED_FREC_NORMAL=2200;	
const unsigned short int LED_ERROR_SPEED=1800;
	   switch(e.status.estado){
		   case 1:LED12_SYS_SetVal();  //init tarjeta los LEDS
			      LED3_Process_SetVal();
			      LED4_SYS_MON_SetVal();
			      LED56_COMM_WARN_MON_SetVal();
			      e.Error13.LEDS=0;//limpieza banderas de error
		          e.status.delay=0;
		          e.status.timer=0;
		          e.status.estado++;
		          break;
		   case 2:if(e.status.timer>LED_STATUP_DELAY)
			              e.status.estado++;
		          break;
		   case 3: LED12_SYS_ClrVal();  //init tarjeta los LEDS
		   	   	   LED3_Process_ClrVal();
		   	   	   LED4_SYS_MON_ClrVal();
		   	   	   LED56_COMM_WARN_MON_ClrVal();  
		   	   	   e.status.estado++;
		           break;
		   case 4:if(e.Error13.LEDS>0)
			              e.status.estado=22;
		          else{ e.status.estado=5;
		                e.status.timer=0;
		                LED2_PutVal(TRUE);	                   
					    LED3_PutVal(FALSE);}
		          break;
		   case 5:if(e.status.timer>LED_FREC_NORMAL){
			                LED2_PutVal(FALSE);	                   
			   				LED3_PutVal(TRUE);
			   			    e.status.estado=6;
			   				e.status.timer=0;}
		          break;  
		   case 6:if(e.status.timer>LED_FREC_NORMAL)
			                e.status.estado=4;
			      break; 
		   
		   case 22:if(e.Error13.bits.DDS_){//error de DDS prioridad 1
			               e.status.estado=23;
			               LED2_PutVal(TRUE);	                   
			               LED3_PutVal(TRUE);
			               e.status.timer=0;
			               break;}//Abajo abra mas ifs
		            break;
		   case 23:if(e.status.timer>LED_ERROR_SPEED)    {
			               e.status.estado=24;
			               e.status.timer=0;
		                   LED2_PutVal(FALSE);	                   
		   		           LED3_PutVal(FALSE);}
		           break;
		   case 24:if(e.status.timer>LED_ERROR_SPEED)        
			   	   	   	   e.status.estado=4;//un ciclo vuelve a ver si hay error
		            break;  
		   default: e.status.estado=1;break;
	     }//switch-----------------------------------------
}//fin -----------------------------------------------------

void  Manejador_de_Errores_por_Prioridad(void){
const unsigned short int TIME_PRIOR=1000;//TIEMPO que manda la señal SPI al DDS
	
	if((e.Error13.bits.DDS_)&&(e.priority.timer>TIME_PRIOR)){//error de seña DDS
		init_Control_deFrecuencias();
		init_ADC();  
		e.priority.timer=0;//inicia Timer
		e.Error13.bits.DDS_=0;}
}//----------------------------------------------------------

