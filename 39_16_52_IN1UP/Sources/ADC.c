/*
  * ADC.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */


/*
 * ADC.c
 *
 *  Created on: Oct 9, 2015
 *      Author: chals
 *      
 *      
 *      Analog Digital Converter Main Controller Driver   
 */
#include "ADC.h"
#include "VFDmenu.h"
#include "system.h"
#include "RC.h"
#include "Busy.h"
//#include "frecqController.h"
#include "SM1.h"
#include "delay.h"
//#include "TI2.h"
#include "TI1.h"




#define  POSXADCU8  24
#define  POSXADCU7  120
#define  POSYADC     4

#define RC_OFF()    (setReg8(CLRTC, 0xFEU))
#define RC_ON()     (setReg8(SETTC, 0x01U))
#define CS0_ON()    (void)setReg8(SETLD, 0x04U)    //  //(setReg8(SETTC, 0x08U)) 
#define CS0_OFF()   (void)setReg8(CLRLD, 0xFBU)// (setReg8(CLRTC, 0xF7U))
#define CS1_ON()    (setReg8(SETAS, 0x08U)) 
#define CS1_OFF()   (setReg8(CLRAS, 0xF7U))

//constantes de la maquina de estados del control de adquisicion de datos de los ADC SPI
#define  ADC1   0x31
#define  ADC2   0x32
#define  ADC3   0x33
#define  ADC4   0x34
                      // cuando le subi el bufer de 20 a 200 dejo de hacer el glitch
#define  BUFFX_SIZE  5  //tamaño del buffer de procesamiento de datos para los filtros digitales
#define  MAX_5VOLTS  0x7FFF//numero a digital maximo cuanto hay +5 volts en ADCs  en COMPLEMENTO a 2

unsigned char displayADCvalores;// dice si se ejecuta desplegar los valores del adc en pruebas o no.
volatile  unsigned char ADCstatus;//maquina de estados del control del adc adquisicion de datos
unsigned short int ADCbuffer;//buffer para la interrupcion de los adc
signed short int *endpointer8,*endpointer7, *px8,*px7;   //pointers de la  pila circular 
signed short int  *initx8, *initx7;
signed short int x8[BUFFX_SIZE]; //buffer de datos de entrada para los filtros digitales
signed short int x7[BUFFX_SIZE]; //buffer de datos de entrada para los filtros digitales
extern unsigned char ErrorStatusLeds;
struct _ADC_ adc;

/*se ejecuta dentro del timer-IRQ  que se interrumpe varias veces por segundo
  despliga los valores de los ADC's cada vez que se interrumpe****************/
void displayADC_IRQ(void){
signed short int U8,U7;// pointers to  bring the ADC values variable de 16bits	
	if(displayADCvalores==ACTIVADO){
		//ADCsRead(&U8,&U7);DEPRECATED//obtiene los valores convertidos y los mete a un apuntador
		displayADCvalor(&U8,POSXADCU8,POSYADC);//despliega el valor de un ADC en la posicion que indican los parametros
		displayADCvalor(&U7,POSXADCU7,POSYADC);//lo mismo
	}//fin del primer if maestro++++++++++++++++++++++++++++++++
}//fin IRQ display ADC values---------------------------------------------------------------------------------

//obtiene los valores de conversion S0,S1=> 00:u8  01:u7   DEPRECATED, OBSOLETE
//se puede leer el dato anterior mientras se esta ejecutando una conversion 
//void ADCsRead(signed short int *adc8,signed short int *adc7){   DEPRECATED, OBSOLETE
//	   //se desabilita la conversion maestra del sistema operativo  DEPRECATED, OBSOLETE
////static unsigned char i;
////signed short int *p;            DEPRECATED, OBSOLETE
////signed short int a[]={-31321,-3239,-234,-1342, -11111,0,-32657};
////signed short int b[]={-21122,-1179,  3,-13342,1,7896,32456,};
//
// /* codigo para probar displayADCvalor en bit two complement despliega el array a y b en pantalla
//	 p=&a[0];  
//	 if(i>((sizeof(a)/2)-1)) i=0;//calcula el tamaño de bytes en short  int
//	 *adc8=*(p+i); //
//	 p=&b[0];
//	 *adc7=*(p+i++);
//  ************************************************************/	 	
//	
//	*adc8=*px8;
//	*adc7=*px7;
//	
//}//fin read ADC------------------DEPRECATED, OBSOLETE-----------------------------------------------------------------------------


void SPIselect(unsigned char device){
	 switch(device){
	    case ADC:CS0_PutVal(FALSE);CS1_PutVal(FALSE);break;
	    case DDS: CS0_PutVal(TRUE);CS1_PutVal(TRUE);break;
	    default:// __asm{ halt   } buscar el asm de halt
	            break;
	 }//fin switch
}//fin seleccion de bus SPI para 3 diferentes dispositivos de que nesesitan transferir datos SPI


//QMR bits:7-0->  10h@1.252MHz, 08h@2.5Mhz  0A@2mhz  formula= 40Mhz/(2*2Mhz)=num binario del 7 al 0, 2Mhz:freec de QSPI deseada  
void init_ADC(void){//CPOL=0  CPHA=1,-> los ADC TRABAJaron BIEN¡¡ CON ESTOS VALORES
register unsigned char i;	
	 setReg16(PQSPAR,0x1515);//0001 0101 0001 0101=1515H
     setReg16(QMR, 0x810AU);//DDS->(1000 0001 0000 1010)  ADC:(1000 0000 0001 0000)  ->16bits tansfer
	 Busy_Disable();//Disable interrupt Busy
	 setReg16(QAR,0x0020);//set pointer to save data to transmit
	 setReg16(QDR,0x1239);//data to transmit
	 setReg16(QAR,0x0020); //pointer to comand raM
	 setReg16(QDR,0x7E00); //0111 1110 0000 0000
	 setReg16(QWR,0x1000); //0001 1000 0000 0000
	 SPIselect(ADC);//seleccionamos los ADC's para la comunicacion SPI 
	 Busy_Enable();//habilita la interrupcion  busy VA ANTES DEL READ CONVERT SINO  BORRALA BANDERA PEN..
	 endpointer8=&x8[BUFFX_SIZE-1];
	 endpointer7=&x7[BUFFX_SIZE-1]; //pointer de fin de cola circular
     initx8=&x8[0];
     initx7=&x7[0]; //pointer de inicio de cola	 	 
	 px8=initx8;
	 px7=initx7;
	 for(i=0;i<BUFFX_SIZE;i++){
	 		 x8[i]=0;x7[i]=0;}
//	 CS0_PutVal(OFF);//CS0_ON();//adc-x selected
//	 CS1_OFF();//apagar para seleccionar los ADC y no el DDS-gen
	 TI1_EnableEvent();
	 TI1_Enable();
	 Busy_Enable();
	 ADCstatus=0xC0 | ADC_1;//abxx xxxx a=anable ADC b=flag of RX SPI
	 RC_ON(); //no quitar de aqui	
	 CS0_OFF();
	 

}//inicizamos el ADC---------------------------------------------------------------------------------------


//char Busy_status(char s,char modo){//inicia la conversion de datos analogico digital
//static  char statusbusy;
//   if(modo=='w')
//      statusbusy=s;//guarda el estatus del busy del ADC
//   else
//      return statusbusy;
//return statusbusy;   
//}//fin busy status-------------------------------------------
// 


/*++++++Busy_Interrupt_IRQ+++++++++++++++++++ OJO-> Hay un inversor fisico
 * Busy Output. Goes LOW when a conversion is started, and remains LOW until the
conversion is completed and the data is latched into the on-chip shift register*/
//falling edge leemos el dato anterior guardado en el registro
//interrupcion externa pin-41  IRQ11, INTERRUPCIon directa de hardare la ejecuta una IRQ oN
//Interrupts when rising edge is trigger from busy adc's, despuede la conversion y guardado en los registros del valor de la conversion
void Busy_Interrupt_IRQ(void){//Interrumption on down edge, EXTERNAL PIN INTERRUPT SIGNAL.
       CS1_OFF();// select ADC8, inicimos lo leemos
       setReg16Bit(QDLYR,SPE);//start trasmit to get data from ADC7
       Busy_Disable();//PULSO 8.2us, MicroSegundos
           
}//fin busy interrupt  INTERRUPT REQUEST---------------------------------------------------------------------
 

//CON  if(n++==1){n=0; se ejecuta la conversion cada 1000usegungos  //que el busy este en estado alto 
//timer 1 se interrumpe cada 520useg y la señal RC debe ser cada 1mseg.
void adcSample_IRQ(void){//se ejecuta cada 1ms y 14us lo lanza  el TI1@events.c
static unsigned char swap;	
	    //ADCstatus  abxx xxxx, b=bandera de ejecucion del adcSampleIRQ, tm1 IRQ=500ns, tons, entra cada 1000ns
	if((ADCstatus & 0xC0)==0xC0){//1xxx xxxx bit que habilita la ejecucion de los ADC's
			 //if(swap==0xAA){//como tm1 se ejecuta cada 520nseg esto hace que se ejecute cada 1mseg
				// swap=0;// quitamos la bnadera para hacer flip flop y generar 1ms de interrupcion
				 Busy_Enable();//este enable es importante si se quita se detiene la conversion.
				 RC_OFF();//640nseg, que se ejecute la conversion, se ordena la execucion con low
				 RC_ON();}//PULSO DE 230ns, nanosegundos
			 //else {swap=0xAA;}
        //}//encendemos bandera para hacer flipflop	
        
}// adc sample-----------------------------------------------------------------------------------------------


//
//void push8(unsigned short int data){
//	
//	*px8=(signed short int)data;//ESTE ES X
//	//if(px8++==endpointer8) //si lo ponemos antes  de x hay un glitch de 0 en los displays
//		//	   px8=initx8;
//}//fin meter a la pila los datos del ADC's--------------------------------------------------------------------
//
//void push7(unsigned short int data){
//	
//    *px7=(signed short int)data;
//    
//	//if(px7++==endpointer7) 
//		//		   px7=initx7;
//}//fin meter a la pila los datos del ADC's--------------------------------------------------------------------


void disable_SM1(void){ 		
//	SM1_Disable();		
	setReg16(QIR, 0x110DU); }	




/************************************************************************************************************
 * CONTROLADORES DE INTERRUPCIONES DEL QSPI++++SM1.c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * debajo de la instrucción  setReg16Bit(QIR,SPIF); 
 * en __declspec(interrupt) void SM1_Interrupt(void) 
 * poner:            
 * 
	 * if(ADCstatus!=0)
		  SerialPeriferialInterface_IRQ();
	   else{
 * agregar en el SM1.c extern volatile unsigned char ADCstatus;//maquina de estados del control del adc adquisicion
 * agregar #include "ADC.h"
   OJO  LEER ESTO DE ARRIBA SI HAY PROBLEMAS CON LOS ADC's
*/
//void SerialPeriferialInterface_IRQ(void){//LA BANDERA  ya se limpio en el driver que llama a esta funcion
//	  
//	 switch(ADCstatus){//maquina de estados del control de adquision de los adc's
//	   case ADC1: setReg16(QAR,0x0010); //IMPRTANTE ponemos el ADDRESS EN RECEPCION pointer to reg RX spi
//	              ADCbuffer=getReg16(QDR);//lo metemos a la cola
//	              CS1_ON();//seleccionamos el otro ADC
//		          ADCstatus++;//el estado que sigue
//		          setReg16Bit(QDLYR,SPE);//start trasmit to get data from ADC7 
//	              break;
//	   case ADC2: setReg16(QAR,0x0010); //IMPRTANTE ponemos el ADDRESS EN RECEPCION pointer to reg RX spi
//		          push7(getReg16(QDR));
//		          push8(ADCbuffer);
//		          Busy_status(NOBUSY,'w');
//		          Busy_Enable();
//	              break;
//	   default: break;
//	 }//FIN ADCstatus--------------------------------
//	   	
//}// fin SERIAL PERIFERIAL INTERFACE* CONTROLLER****************************************************************


/*CS0=0  CS1=1->ADC7 y CS1=0->ADC8
 * method to read both ADC's
 * Tm1  activate signal RC, it activate buy signal that it activate IRQ busy,
 *  and busy IRQ method trigger and sent the one read to QSPI which it
 *  trigger an IRQ about QSPI, it set on the flag ADSstatus,
 *  in the main loop this method Read_QSPI will read the QDR at address 10h
 *  then chance the CS1 to another adc,
 * */
unsigned char Read_QSPI2(signed short int *xx8,signed short int *xx7) {
static unsigned char i;
unsigned char n=0;
	
          if(i>(BUFFX_SIZE-1)) i=0; // evitar error de memoria allocation
QSPI21:   if(x8[i]==MAX_5VOLTS){//es EFFFh tons ta vacio
	    	  if(++i>(BUFFX_SIZE-1))
	    		     i=0;
	    	  if(n++>BUFFX_SIZE+1) //evita que nos quedemos en el loop en caso de falla de ADCs
	    		  goto QSPI22;
	          goto QSPI21;}
          else goto QSPI23;
QSPI22:   return FALSE;
QSPI23:	  *xx8=x8[i];
          *xx7=x7[i];
          x8[i++]=(signed short int)MAX_5VOLTS; //it shows the value has been used
	     return TRUE;

}//fin prueba---------------------------------------------------



//INTERRUPCION PRINCIPAL MAQUINA AUTIMATICA DE 1ms AQUI SE 
//RESTA EL OFFSET
//se ejecuta cuando hay una IRQ de envio o recepcion del  QSPI
void QSPI_OnTxChar_IRQ(void){//poner en evento de transmision
signed short int n;
    ErrorStatusLeds |=0x80; // axxx xxxx a=bandera de que funcionan los ADC,
	if((ADCstatus & 0x80)==0x80){//1xxx xxxx bit que habilita la ejecucion de los ADC's
    //n=ADCstatus & 0x3F; //dejamos solo el estado de la maquina de estados
    switch(ADCstatus & 0x3F){//abxx xxxx= a=ADC enable, b=ADC leidos-ambos, xxxxx=Estados del ADC
    	case ADC_1:setReg16(QAR,0x0010); //IMPRTANTE ponemos el ADDRESS EN RECEPCION pointer to reg RX spi
    	           if(px8>endpointer8)
    	        	   px8=initx8;//OFFSET PARA X8=+444
    	           n=(signed short int)getReg16(QDR);//moves the next position and return old content
     	           *(px8)++=n-444;//INCREMETA LA direccion pointer, Se le resta el OFFSET
    	           CS1_ON();//activamos el siguiente ADC
    	           ADCstatus= 0x80 | ADC_2;//nos vamos al siguiente estado
    	           setReg16Bit(QDLYR,SPE);//start trasmit to get data from ADC7
    	           //adc.ADCreads=0xAA;//Mandamos el ADC-1 dato
    	           break;
    	case ADC_2:setReg16(QAR,0x0010); //IMPRTANTE ponemos el ADDRESS EN RECEPCION pointer to reg RX spi
    	    	   if(px7>endpointer7)
    	    	      px7=initx7;//OFFSET PARA X7=+405
    	           n=(signed short int)getReg16(QDR);//moves the next position and return old content
    	           *(px7)++=n-405;//INCREMETA LA direccion pointer, Se le resta el OFFSET
    	           CS1_OFF();//activamos el siguiente ADC
    	    	   ADCstatus= 0x80 | ADC_1 | 0x40;//b=0100 0000 nos vamos al siguiente estado
//    	    	   Busy_Enable();desabilitado se activa solo con RC
    	    	   break;
    	default:  break;	}//fin switch    		           
    	           
	}//fin if
}//fin void QSPI_OnTxChar_IRQ(void){


