/*
 * fecqController.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals
 */


/*
 * fecqController.c
 *
 *  Created on: Jul 27, 2015
 *      Author: chals
 *      
 *  Los parametros para configurar el puerto QSPI del procesador usando los
 *  chips multiplexores y los inversores del circuto IN1UP tal cual para multiplear la 
 *  señal de reloj hasta el DDS@AD9833 son los siguientes
 *  MSB-FIRST:YES++++++++++++++++++++++++++++++++
 *  CLOCK EDGE:FALLING EDGE+++++++++++++++++++++
  * SHIFT CLOCK IDLE POLARITY= LOW++++++++++++++++
  * SHIFT-CLOCK=12.7useg++++++++++++++++++++++++
  * QSPI_DOUT-17++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * QSPI_SCK_21+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * 
 */

#include "frecqController.h"
#include "Memoria.h"
#include "errorController.h"
#include "FSYNC.h"
#include "SDATA.h"
//#include "SCLK.h"
#include "VFD.h"
#include "SM1.h"
#include "delay.h"
#include "CS0.h"
#include "CS1.h"
#include "ADC.h"
#include "system.h"
#include "VFDmenu.h"

#ifndef  DDS_GENERADOR
  #define  ARDUINO_DDS 0x1A
  #define  AD9834_DDS  0x2A
  #define  DDS_GENERADOR AD9834_DDS  //ARDUINO_DDS
#endif


#define CUADRADA       12
#define SINUSOIDAL     33
#define XTAL_ANALOGA   24576000//FRECUENCIA DEL CRISTAL DE LA ANALOGA INSGHT
#define FRECUENCIA_ALTA  870000  //frecuencias base para sumarle la variable
#define FRECUENCIA_MEDIA 280000
#define FRECUENCIA_BAJA  100000 
#define FORMA_DE_ONDA    CUADRADA//CUADRADA O SINUSOIDAL



//extern unsigned char frecuenciaSeleccion;//SELECCIONA EL NIVEL de la frecuencia actual configurada y activada
extern unsigned char Alta; //variable que guarda el ultimo digito de la frecuencia alta
extern unsigned char Media;//variable que guarda el ultimo digito de la frecuencia media
extern unsigned char Baja;//variable que guarda el ultimo digito de la frecuencpia baja
extern  volatile unsigned char ADCstatus;//maquina de estados del control del adc adquisicion de datos
extern struct _I2C_CONTROL IIC;
extern struct _Productos_ prod;




unsigned long int frecuency0;//store the actual frecq to compare in case of any change of it

//extern unsigned long int frecuency; //guarda el valor actual de la frecuencia activada

 void init_Control_deFrecuencias(void ){
	
#if DDS_GENERADOR == AD9834_DDS	 
	
	 init_DDS(_BOOT_);
#elif DDS_GENERADOR == ARDUINO_DDS
     Cambio_de_Frecuencia_por_IIC_1(1000);//frecuencia en khz
#endif

 }// fin de la inicializacion de control de frecuencias
 
 

unsigned long int  getFrecuency(void){
#if FORMA_DE_ONDA == SINUSOIDAL	   
	      switch(prod.cntrl.frecuenciaSeleccion){
	       case ALTA:  return(FRECUENCIA_ALTA+Alta*(unsigned long int)1000);break;
	       case MEDIA: return(FRECUENCIA_MEDIA+Media*(unsigned long int)1000);break;
	       case BAJA:  return(FRECUENCIA_BAJA+Baja*(unsigned long int)1000);break;	   
	       default:prod.frecuenciaSeleccion=MEDIA;
	               return(getFrecuency());break;
	      }//fin switch
#elif FORMA_DE_ONDA == CUADRADA
	      switch(prod.cntrl.frecuenciaSeleccion){
	      	       case ALTA:  return((FRECUENCIA_ALTA+Alta*(unsigned long int)1000)*2);break; 
	      	       case MEDIA: return((FRECUENCIA_MEDIA+Media*(unsigned long int)1000)*2);break;
	      	       case BAJA:  return((FRECUENCIA_BAJA+Baja*(unsigned long int)1000)*2);break;	   
	      	       default:prod.cntrl. frecuenciaSeleccion=MEDIA;
	      	               return(getFrecuency());break;
	      	      }//fin switch
#endif	      
 }// fin de get frecuency---------------------------------------------------------------
 
 
 /*CLOCK EDGE->RISING EDGE+++++++++++++++++++PARAMETROS+PARA+EL+PUERTO+QSPI++CONECTADO+++
  * SHIFT CLOCK IDLE POLARITY= HIGH++++++++++DIRECTAMENTE+ EL+PROCESADOR+AL+DSS+++++++++
  * SHIFT-CLOCK=12.7useg+++++++++++++++++++++SIN+USAR+INVERSOR+Y+MULTIPLEXOR++++++++++++
  * QSPI_DOUT-17++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * QSPI_SCK_21+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * **********************INIT**DDS**************************************************
  * frecuency  calculada    real      error   %error   
  *   1786000   893000    909.1khz   16.1hz    +1.8%
  *   1980000   990000    990.1khz     0%      0%
  *   1982000   991000    990.1khz
  *   1998000   999000    990.1khz
  *   
  *   
  * */
 void init_DDS(char modo){//Direct Digital Synthesis  Generator chip
#if DDS_GENERADOR == AD9834_DDS		 
	unsigned long int frecuency; //=875000;//si es cadrada el reslado es la mitad de la frecencia
	unsigned short int MSB,LSB,phase=0; //2 bytes   
	unsigned long int calculated_freq_word; //4 bytes   
	float AD9833val=0.00000;
	
	frecuency=getFrecuency();
	if((frecuency0!=frecuency)||(modo==1)||(modo==_BOOT_)){//preguntamos si la frecuencia que tenemos activa es el mismo valor por el que queremos reconfigurar
		frecuency0=frecuency;
		frecuency=1000000*2;
		ADCstatus=0; //se ejecute la interrupcion para el DDS y no para el ADC
		SM1_Init();//se hicializa el QSPI porque lo modifcamos para el ADC
		delay_ms(400);
		SPIselect(DDS);//SELECCIONA el bus SPI para el device AD9833 direct digital synthesis generator chip

		
  // while(1){
        if(frecuency>1100000)
    	  frecuency=286000;
        else frecuency+=500;
		AD9833val=((float)(frecuency))/XTAL_ANALOGA;
		calculated_freq_word=(unsigned long int)(AD9833val*0x10000000);
		MSB=(unsigned short int)((calculated_freq_word & 0xFFFC000)>> 14); // 14 bits
		LSB=(unsigned short int)( calculated_freq_word & 0x3FFF);
		//set cntrl bits D15 & D14 to 0 & 1, repectivel, for frecq reg, 0
		LSB |=0x4000;
		MSB |=0x4000;
		phase &=0xC000;
		WriteRegisterAD9833(0x100);//reset //(0x2100);
		delay_ms(20);
		WriteRegisterAD9833(0x2100); //square
		WriteRegisterAD9833(LSB);// LOWER 14 bits
		WriteRegisterAD9833(MSB); // upper 14 bits
		WriteRegisterAD9833(0xC000);//phase);//mid-low
		WriteRegisterAD9833(0x2028); //waveform cuadrada
		
		delay_ms(1000);
		  //}
		//WriteRegisterAD9833(0x2000); //sin
		  //AD9837Write(0x2002); //triangle
		if(modo!=_BOOT_)
				      init_ADC();
	}//fin if is the same frecq?  	
	 delay_ms(350);
#endif	 
 }//fin initializae DDS--------------------------------------------------------------------
 

 void WriteRegisterAD9833(unsigned short int dat){
#if DDS_GENERADOR == AD9834_DDS		 
word16 buffer;
//unsigned int i;
     buffer.word16=dat;
	 FSYNC_PutVal(FALSE);
	 delay_us(1);
	   SM1_SendChar(buffer.byte[0]);
	   delay_us(1);//SIN ESTE DELAY NO FUNCIONA--------<--IMPORTANTE
	   SM1_SendChar(buffer.byte[1]);
	 delay_us(1);
	 FSYNC_PutVal(TRUE);
#endif	 
	 
 }// fin de write to register AD9833------------------------------------------
 
 
 
