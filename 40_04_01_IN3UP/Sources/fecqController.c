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
#include "ADC.h"
#include "system.h"
#include "PIN_DRIVE_OUT.h"
#include "CLK_AN_COUNT.h"
#include "queue.h"
#include "maths.h"
#include "TI1.h"
#include "TI2.h"




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



extern unsigned char frecuenciaSeleccion;//SELECCIONA EL NIVEL de la frecuencia actual configurada y activada
extern unsigned char Alta; //variable que guarda el ultimo digito de la frecuencia alta
extern unsigned char Media;//variable que guarda el ultimo digito de la frecuencia media
extern unsigned char Baja;//variable que guarda el ultimo digito de la frecuencpia baja
extern  volatile unsigned char ADCstatus;//maquina de estados del control del adc adquisicion de datos
extern struct _I2C_CONTROL IIC;
unsigned char statusDDS;
extern struct DDS_GEN ddsgen;
extern unsigned char statusDDS;


unsigned long int frecuency0;//store the actual frecq to compare in case of any change of it




/* MUY IMPORTANTE CONFIGURACION
 * input buffer 0
 * output bufer 22
 * Wdth=8bits
 * Clock Edge=falling edge
 * shift clock rate 8.5useg
 * wait dealay=0.425us
 * empty char =0
 * ingnore empty char  no
 * Send MSB first yes
 * Shift clock idle polairty= Low
 * 
 * */
void init_Control_deFrecuencias(void ){	
const int SINE = 0x2000;                    // Define AD9833's waveform register value.
const int SQUARE = 0x2028;//0x2020
const int ATT_PIN =7;//Attenuation pin             // frecuencia a generar.
const unsigned char SIZE1=6;
unsigned short int w[SIZE1];
unsigned long frecuencia=1000000;//frecuencia deseada
float xtal = 24570000.0;// On-board crystal reference frequency
const unsigned short int waveform=0x2000; //forma de onda sinusoidal
unsigned char i;
const unsigned short int _SPE=0x1000; //iniciar Transferecia SPI  ejecutar todos los comandos [bit15 de QDLYR]
const unsigned short int _WREN=0x4000;//After executing command pointed to by QWR[ENDQP], wrap back to entry zero, or the entry pointed to by QWR[NEWQP] and continue execution. registro QWR
const unsigned short int _HALT=0x8000;//Halt transfers, QWR REG
const unsigned short int _CSIV=0x1000;//QWR QSPI chip select outputs return to one when not driven from the value in the current command RAM entry during a transfer (that is, inactive state is 1, chip selects are active low).
const unsigned short int _NEWQP=0x0020;//pointer commnad firs command  QWR
const unsigned short int _ENDQP=0x0600;//pointer last command to be transfer QWR
const unsigned short int _WRTO=0x2000;//Wraparound location. Determines where the QSPI wraps to in wraparound mode 1 Wrap to RAM entry pointed to by QWR[NEWQP].
const unsigned short int _QCR0=0x0020;//comando primero direccion
const unsigned short int _BAUD=0x000C; //400nseg      1/[49.7664Mhz/2[12]]=482nseg
const unsigned short int _MSTR=0x8000;//QMR 1 The QSPI is in master mode. Must be set for the QSPI module to operate correctly.
const unsigned short int _CPHA=0x0100;//QMR 1 Data changed on the leading edge of QSPI_CLK and captured on the following edge of QSPI_CLK.
const unsigned short int _CPOL=0x0200;//Clock phase. Defines the QSPI_CLK clock-phase.
const unsigned short int _DTL=0x0007;//delay=15 entre transferencias, [32*15/(fsys=49.7664Mhz)]=9useg ,bits de QDLYR
const unsigned short int _8BITS=0x2000;//0010 0000 0000 0000
const unsigned short int _16BITS=0x0000;//0010 0000 0000 0000
const unsigned short int _SPIF=0x0001;//QSPI finished flag. Asserted when the QSPI has completed all the commands in the queue. 

//union _i33{
//		int i;
//		struct b1{
//	      unsigned short int x;		 
//		  unsigned short int y;	
//		}v2;
//	}w5;

//0x00–0x0F Transmit RAM
//0x10–0x1F Receive RAM
//0x20–0x2F Command RAM
unsigned long int delay=0;
     TI1_Disable();
     Select_SPI_device(DDS);//seleccionamos los ADC's para la comunicacion SPI
     setReg16(PQSPAR,0x1515);//0001 0101 0001 0101=1515H
     setReg16(QMR, 0x0000);//se resetea el QSPI
     setReg16(QMR,_MSTR|_BAUD | _16BITS);//config QSPI, CPOL=0,CPHA=1
     setReg16(QWR,0x0000); //reset QWR
     setReg16(QWR,0x0500);//setup queue begin at 0 end to 6  
     setReg16(QDLYR,0x0707);//0000 0111 0000 01111   delays asignar de delay entre transferencias
     get_the_words_(&w[0],xtal,frecuencia,waveform);  
     //get_the_words_400(&w[0]);
    setReg16(QAR,0x0020);//transmit add, _QCR0);//direccin primer comando
    setReg16(QDR,0x7E00);//0111 palabra de reset, primer comando
     for(i=1;i<6;i++)
        setReg16(QDR,0x6000);//1000 0000 0000 0000, SPIF
     setReg16(QAR,0x0000);//direccion zero de Transmission
     for(i=0;i<6;i++)
    	 setReg16(QDR,w[i]);//datos a transmitir
     setReg16Bits(QIR,0x0010);//_SPIFE);QSPI finished (SPIF) interrupt enable.
l14: FSYNC_PutVal(FALSE);
     setReg16Bit(QDLYR,SPE);//start trasmit to get data from ADC7
     setReg16(QIR,_SPIF);//clear the int Flag
     for(delay=0;delay<210;delay++)__asm(nop);//Fsys=49.76Mhz 
     FSYNC_PutVal(TRUE);
     for(;;) if(getReg16(QIR)==_SPIF) break;
//     for(;;) if(delay++>(2357142*5)){delay=0;break; }
//     goto l14;
     //CLK_AN_COUNT_Enable();
     //CLK_AN_COUNT_EnableEvent();
     //CLK_AN_COUNT_Reset();//interupcion de captura habilitada
     TI2_EnableEvent();//monitor de error habilitado
}// fin de la inicializacion de control de frecuencias
 
/*Monitorea que el generador de señal DDS de la analoga
 * sea generada en tiempo y forma y periodo.
 * */
void Monitor_Reloj_Analogo(void){
	
//const unsigned short int FREC_1M=0;
//const unsigned short int FREC_1M_TOL=0;//tolerancia
//const unsigned short int DELAY_MON=2566; //CADA cuando se monitorea
//const unsigned short int MON_INT=2200;//monitor de interrupcion
//unsigned short int c;	
//   switch(ddsgen.var.estado){
//	  case 0: ddsgen.var.delay=0;
//		      ddsgen.var.estado++;
//		      ddsgen.bits.Bits.IRQcapture=0;
//		      CLK_AN_COUNT_EnableEvent();
//	  	  	  CLK_AN_COUNT_Reset();
//	  	  	  break;
//	  case 1:if(ddsgen.var.delay++>100){
//		         ddsgen.var.estado++;ddsgen.var.delay=0;}
//	         break;
//	  case 2:if(ddsgen.bits.Bits.IRQcapture){//hubo interrupcion
//		          CLK_AN_COUNT_DisableEvent();
//		          if(CLK_AN_COUNT_GetCaptureValue(&c)==ERR_OK){//leemos capturas en tiempo timer configurado
//		        	  if((absusi(FREC_1M-c))<FREC_1M_TOL){//esta dentro del rango que corresponde a 1M?
//		        		   ddsgen.var.estado=45;}//todo esta bien
//		        	  else{eLog(ERROR_RELOJ_ANALOGO); 
//		        	       ddsgen.var.estado=45;
//		        	       ddsgen.var.ndelay=200;}}
//		          else{eLog(ERROR_RELOJ_ANALOGO);
//		               ddsgen.var.estado=45;
//		               ddsgen.var.ndelay=500;}} 
//	          if(ddsgen.var.delay++>MON_INT){
//	        	  eLog(ERROR_RELOJ_ANALOGO);
//	        	  ddsgen.var.estado=45;
//	              ddsgen.var.ndelay=2000;}
//	  	      break;  	  
//	  case 45:ddsgen.var.estado++;ddsgen.var.delay=0;break;
//	  case 46:if(ddsgen.var.delay++>ddsgen.var.ndelay){
//		             ddsgen.var.estado=0;}  
//	          break;           
//	  default:ddsgen.var.estado=0;break;}	
	   
}//fin----------------------------------------------------


//CONFIGURACION DEL dds
void get_the_words_(unsigned short int *w,float xtal,unsigned long int frecuencia,unsigned short int waveform){
	
	unsigned long FreqWord;
	int MSB1;
	int LSB1;
	union _i32{
		int i;
		struct b{
	      unsigned short int x;		 
		  unsigned short int y;	
		}v;
	}w4;
	unsigned short int MSB=0,LSB=0;
	double frec1,d1;
	  
	  frec1=frecuencia;
	  d1=(frec1*0x10000000)/((double)xtal);
	  FreqWord = (unsigned long)d1;  
	  MSB1 = (int)((FreqWord & 0xFFFC000) >> 14);    //Only lower 14 bits are used for data
	  LSB1 = (int)(FreqWord & 0x3FFF);
	  //Set control bits 15 ande 14 to 0 and 1, respectively, for frequency register 0
	  w4.i=MSB1;
	  MSB=w4.v.y;
	  w4.i=LSB1;
	  LSB=w4.v.y;
	  LSB |= 0x4000;
	  MSB |= 0x4000; 
	  *(w+0)=0x0100;//comando del reset
	  *(w+1)=0x2100;   
	  *(w+2)=LSB;                  // Write lower 16 bits to AD9833 registers
	  *(w+3)=MSB;                  // Write upper 16 bits to AD9833 registers.
	  *(w+4)=0xC000;               // Phase register
	  *(w+5)=waveform;             // Exit & Reset to SINE, SQUARE or TRIANGLE
}//fin de ogbtener las palabras para configurar el DDS


void get_the_words_400(unsigned short int *w){
	  *(w+0)=0x0100;//comando del reset
	  *(w+1)=0x2100;   
	  *(w+2)=0x50C7;                  // Write lower 16 bits to AD9833 registers
	  *(w+3)=0x4000;                  // Write upper 16 bits to AD9833 registers.
	  *(w+4)=0xC000;               // Phase register
	  *(w+5)=0x2000; 
}//-----------------------------------------------------------------------



/*genera la frecuecia en el DDS, manda por SPI los
 comandos al DDS AD9813*/
void Generador_de_Frecuencia_DDS(unsigned long frec){
	AD9833reset(); // Reset AD9833 module after power-up.
	delay_us(10);
	AD9833setFrequency(frec,0x2000,25000000); 
}// FIN generador de frecuencia de DDS------------------------



// AD9833 documentation advises a 'Reset' on first applying power.
void AD9833reset(void) {
  WriteRegister(0x100);   // Write '1' to AD9833 Control register bit D8.
  delay_us(1);
}//-----------------------------------------------


// Set the frequency and waveform registers in the AD9833.
void AD9833setFrequency(unsigned long frequency, unsigned short Waveform,float xtal) {
unsigned long FreqWord;
int MSB1;
int LSB1;
union _i32{
	int i;
	struct b{
      unsigned short int x;		 
	  unsigned short int y;	
	}v;
}w4;
unsigned short int MSB=0,LSB=0;
double frec1,d1;
  
  frec1=frequency;
  d1=(frec1*0x10000000)/((double)xtal);
  FreqWord = (unsigned long)d1;  
  
  MSB1 = (int)((FreqWord & 0xFFFC000) >> 14);    //Only lower 14 bits are used for data
  LSB1 = (int)(FreqWord & 0x3FFF);
  //Set control bits 15 ande 14 to 0 and 1, respectively, for frequency register 0
  w4.i=MSB1;
  MSB=w4.v.y;
  w4.i=LSB1;
  LSB=w4.v.y;
  LSB |= 0x4000;
  MSB |= 0x4000; 

   WriteRegister(0x2100);   
   WriteRegister(LSB);                  // Write lower 16 bits to AD9833 registers
   WriteRegister(MSB);                  // Write upper 16 bits to AD9833 registers.
   WriteRegister(0xC000);               // Phase register
   WriteRegister(Waveform);             // Exit & Reset to SINE, SQUARE or TRIANGLE
   
        
         
}//fin-----------------------------------------------



void WriteRegister(unsigned short int dat) { 
word16 buffer;
buffer.word16=dat;  
  // Display and AD9833 use different SPI MODES so it has to be set for the AD9833 here.
  //SPI.setDataMode(SPI_MODE2);       
  FSYNC_PutVal(FALSE);// digitalWrite(FSYNC, LOW);           // Set FSYNC low before writing to AD9833 registers
  delay_us(1);              // Give AD9833 time to get ready to receive data.
  SM1_SendChar(buffer.byte[0]);//SPI.transfer(highByte(dat));        // Each AD9833 register is 32 bits wide and each 16
n0:if(statusDDS==0xAA)
	    goto n1;
   else goto n0;
n1:statusDDS=0;
   delay_us(1);//SIN ESTE DELAY NO FUNCIONA--------<--IMPORTANTE
   SM1_SendChar(buffer.byte[1]);//SPI.transfer(lowByte(dat));         // bits has to be transferred as 2 x 8-bit bytes.
n2:if(statusDDS==0xAA)
  	    goto n3;
   else goto n2;
n3:statusDDS=0;
  FSYNC_PutVal(TRUE);//digitalWrite(FSYNC, HIGH);          //Write done. Set FSYNC high
}//fin writeRegister+++++++++++++++++++++++++++++++++++++++++++++++++++++













/************DEPRECATED+++++++++++++++++++++++++++++++++++++++++++++++++*/


unsigned long int  getFrecuency(void){
#if FORMA_DE_ONDA == SINUSOIDAL	   
	      switch(frecuenciaSeleccion){
	       case ALTA:  return(FRECUENCIA_ALTA+Alta*(unsigned long int)1000);break;
	       case MEDIA: return(FRECUENCIA_MEDIA+Media*(unsigned long int)1000);break;
	       case BAJA:  return(FRECUENCIA_BAJA+Baja*(unsigned long int)1000);break;	   
	       default:frecuenciaSeleccion=MEDIA;
	               return(getFrecuency());break;
	      }//fin switch
#elif FORMA_DE_ONDA == CUADRADA
	      switch(frecuenciaSeleccion){
	      	       case ALTA:  return((FRECUENCIA_ALTA+Alta*(unsigned long int)1000)*2);break; 
	      	       case MEDIA: return((FRECUENCIA_MEDIA+Media*(unsigned long int)1000)*2);break;
	      	       case BAJA:  return((FRECUENCIA_BAJA+Baja*(unsigned long int)1000)*2);break;	   
	      	       default:frecuenciaSeleccion=MEDIA;
	      	               return(getFrecuency());break;
	      	      }//fin switch
#endif	      
 }// fin de get frecuency---------------------------------------------------------------
 
 
 /*CLOCK EDGE->RISING EDGE+++++++++++++++++++PARAMETROS+PARA+EL+PUERTO+QSPI++CONECTADO+++
  * SHIFT CLOCK IDLE POLARITY= HIGH++++++++++DIRECTAMENTE+ EL+PROCESADOR+AL+DSS+++++++++
  * SHIFT-CLOCK=12.7useg+++++++++++++++++++++SIN+USAR+INVERSOR+Y+MULTIPLEXOR++++++++++++
  * QSPI_DOUT-17++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * QSPI_SCK_21+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * **********************INIT**DDS**************      ************************************
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
//		delay_ms(400);
		//SPIselect(DDS);//SELECCIONA el bus SPI para el device AD9833 direct digital synthesis generator chip
		Select_SPI_device(DDS);
		
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
//		delay_ms(20);
		WriteRegisterAD9833(0x2100); //square
		WriteRegisterAD9833(LSB);// LOWER 14 bits
		WriteRegisterAD9833(MSB); // upper 14 bits
		WriteRegisterAD9833(0xC000);//phase);//mid-low
		WriteRegisterAD9833(0x2028); //waveform cuadrada
		
//		delay_ms(1000);
		  //}
		//WriteRegisterAD9833(0x2000); //sin
		  //AD9837Write(0x2002); //triangle
		if(modo!=_BOOT_)
				      init_ADC();
	}//fin if is the same frecq?  	
//	 delay_ms(350);
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
 
 
 
