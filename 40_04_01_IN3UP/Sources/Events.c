/* ###################################################################
**     Filename    : Events.c
**     Project     : 17_IN1UP
**     Processor   : MCF52233CAF60
**     Component   : Events
**     Compiler    : CodeWarrior MCF C Compiler
**     Date/Time   : 2015-07-13, 17:20, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnCoreWatchdogINT - void Cpu_OnCoreWatchdogINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "errorController.h"
#include "VFD.h"
#include "Reloj.h"
#include "ADC.h"
#include "DSP.h"
#include "VFDserial.h"
#include "IOcomms.h"
#include "VFDmenu.h"
#include "Memoria.h"
#include "system.h"
#include "AnalogCntrl.h"
#include "SistemaOperativo.h"
#include "queue.h"
#include "TFTmenu.h"
#include "keypad.h"
#include "CLK_AN_COUNT.h"
#include "WP.h"



extern struct _Comando_DDS dds; //extern struct _DDS_ DDS1;
extern struct _Detection Deteccion;
extern struct _SerialControl_ serialKeyPad2;
extern struct _DISPLAY_VFD_ vfd;
extern struct _KEYPAD_ keypad;
extern struct DDS_GEN ddsgen;
extern struct _SysMon sysmon,sysmon2;
extern struct _Error323_ e;//error controller data struct


//extern struct _KeyPad_ keypad;
unsigned int  delaycount;
//extern struct COMMUNICACION IO_TX;
extern struct _SERIAL_ serial;    //extern struct _Vector_ serial;
volatile unsigned char  TM1_IRQ1;
extern unsigned char DDStx_flag_INT;
extern unsigned char DDSplotflag;//bandera para ver si ya se mando un byte TX al VFD	
//unsigned char debug10;
extern unsigned char keypadIn[SIZE_KEYPAD_IN];
extern unsigned char statusDDS;
unsigned long int delaydebug;
unsigned long long int delay_General_ms;

//unsigned long int Reloj_Analogo_Monitor;


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnCoreWatchdogINT (module Events)
**
**     Component   :  Cpu [MCF52233_112_LQFP]
**     Description :
**         This event is called when the OnCoreWatchdog interrupt had
**         occurred. This event is automatically enabled when the <Mode>
**         is set to 'Interrupt'.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Cpu_OnCoreWatchdogINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI1_delays_OnInterrupt (module Events)
**
**     Component   :  TI1_delays [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_delays_OnInterrupt(void)
{
  /* Write your code here ... */
	delaycount++;
	
	
}// fin TI1 delays OnInterrupt fin---VFDserial_OnTxChar------------------------------------

/*
** ===================================================================
**     Event       :  VFDserial_OnError (module Events)
**
**     Component   :  VFDserial [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VFDserial_OnError(void)
{
  /* Write your code here ... */
	VFDserial_ClearRxBuf();
}

/*
** ===================================================================
**     Event       :  VFDserial_OnRxChar (module Events)
**
**     Component   :  VFDserial [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VFDserial_OnRxChar(void)
{
  /* Write your code here ... */
/*   señal del teclado    */
	 //serialKeyPad2.Byte1.bit.IRQ_rx=TRUE;
unsigned char c;	
	 
    VFDserial_RecvChar(&c);
    VFDkeypad_ISR(c);
    switch(c){
    	case 'O':
    	case 'A':
    	case 'L':
    	case 'E':
    	case 'R':keypad.v.c0=c;break;
    	default://keypad.v.c0=0;
    	        break;}	
}// fin VFDserial_OnRxChar-----------------------------------------------

/*
** ===================================================================
**     Event       :  VFDserial_OnTxChar (module Events)
**
**     Component   :  VFDserial [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VFDserial_OnTxChar(void)
{
  /* Write your code here ... */
//	DDStx_flag_INT=0xAA;
	DDSplotflag=0xAA;
	vfd.bits.b.TxBuffOFF=0;
	
}

/*
** ===================================================================
**     Event       :  VFDserial_OnFullRxBuf (module Events)
**
**     Component   :  VFDserial [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VFDserial_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  VFDserial_OnFreeTxBuf (module Events)
**
**     Component   :  VFDserial [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void VFDserial_OnFreeTxBuf(void)
{
  /* Write your code here ... */
	vfd.bits.b.TxBuffOFF=1;
	
	
}



/*
** ===================================================================
**     Event       :  TACHO_OnCapture (module Events)
**
**     Component   :  TACHO [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TACHO_OnCapture(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  RTC1_OnAlarm (module Events)
**
**     Component   :  RTC1 [RTC]
**     Description :
**         This event is called whenever an actual time is equal to the
**         alarm time. (only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent"> (event is
**         available only when the property Interrupt service/event is
**         set to - <"Enabled">)
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RTC1_OnAlarm(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  RTC1_OnSecond (module Events)
**
**     Component   :  RTC1 [RTC]
**     Description :
**         This event is called on every increment of the second
**         counter. (only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent"> (event is
**         available only when the property Interrupt service/event is
**         set to - <"Enabled">)
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RTC1_OnSecond(void)
{  /* Write your code here ... */
	displaySegundero_IRQ();
	if(serial.vars.var.timercomms<250)
		serial.vars.var.timercomms++;//serial.cntrl.timercomms++;
	IRQ_comunicacion_IOUP();
	
}

/*
** ===================================================================
**     Event       :  SM1_OnTxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SM1_OnTxChar(void)
{
  /* Write your code here ... */
	
	QSPI_OnTxChar_IRQ();//interrupcion de Maquina Automatica de DAQ

}

/*
** ===================================================================
**     Event       :  Busy_OnInterrupt (module Events)
**
**     Component   :  Busy [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing VFDserial_OnFreeTxBuf
** ===================================================================
*/
void Busy_OnInterrupt(void)
{
  /* place your Busy interrupt procedure body here */
	Busy_Interrupt_IRQ();//llego señal de busy vamos a leer los adc de t=0,

}


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
/*
** ===================================================================
**     Event       :  IOUPserial_OnError (module Events)
**
**     Component   :  IOUPserial [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing TI2_OnInterrupt
** ===================================================================
*/
void IOUPserial_OnError(void)
{
  /* Write your code here ... */
	__asm(nop);
}

/*
** ===================================================================
**     Event       :  IOUPserial_OnRxChar (module Events)
**
**     Component   :  IOUPserial [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void IOUPserial_OnRxChar(void)
{
  /* Write your code here ... */
unsigned char c;
static unsigned char i;
        if(IOUPserial_RecvChar(&c)==ERR_OK){
        	//menu_TFT_touch_ISR(c);
            IOUPserial_Recepcion_Procesamiento(c);}
		else IOrecv_char_ISR(0);
		
}//---------------------------------------------------------






/*
** ===================================================================
**     Event       :  IOUPserial_OnTxChar (module Events)
**
**     Component   :  IOUPserial [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void IOUPserial_OnTxChar(void)
{
  /* Write your code here ... */

	//IOUPserial_TransmitedChar_IRQ();
	//istxchar=TRUE;
	__asm(nop);
	//SemaforoTX(TRUE,'w');
}

/*
** ===================================================================
**     Event       :  IOUPserial_OnFullRxBuf (module Events)
**
**     Component   :  IOUPserial [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void IOUPserial_OnFullRxBuf(void)
{
  /* Write your code here ... */
	__asm(nop);
	
}



/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{static unsigned short int status;
 static unsigned short int delayRechazo;
 static unsigned short int delayProd;
 static unsigned short int delaySens;
 static unsigned short int delayPhase;
 static unsigned short int SensTest[]={10325,34,100,22942,34,1243,11,22521,19,8,233,12341};
 static unsigned char iTestSens,iTestPhase;
 static float phaseTest[]={0.1,123.9,5.6,10.8,112.1,8.9,179.9,11.2,3.2,156.3,0.2,1.2};
 /* Write your code here ... */
	//500useg confirmado 9-Mar-20
	//500usegundos el intervalo de tiempo
		
	 // version 39.15.5.x++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 adcSample_IRQ();//manda orden de coversion a  los ADCs y enable IRQ input BUSY
	 TM1_IRQ1=0xFF;
	 //version 39.13.21.xx ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/* if(i++>1000){i=0; 
	    led1_NegVal();}
	 */
	 
	 if(status++>400){status=0;
		  WP_NegVal();}
	 if(delayRechazo++>100){delayRechazo=0;//SIMULACION DE DATOS
	     Deteccion.rechazo.NumRech++;}
	 
	 if(delayProd++>1000){delayProd=0;//SIMULACION DE DATOS
	 	     Deteccion.prod.NumProd++;}
	 	 
	 if(delaySens++>2000){delaySens=0;//SIMULACION DE DATOS
	      if(++iTestSens>11) iTestSens=0;
		  Deteccion.prod.Sens=SensTest[iTestSens];}
	 
	if(delayPhase++>1000){delayPhase=0;
	      if(++iTestPhase>11) iTestPhase=0;
	      Deteccion.prod.phase=phaseTest[iTestPhase];} 
	 	 
	delay_General_ms++;
	 
//FIN TIMER-1 IRQ-------------------------------------------------------------------------------------------------
}

/*
** ===================================================================
**     Event       :  IOUPserial_OnFreeTxBuf (module Events)
**
**     Component   :  IOUPserial [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void IOUPserial_OnFreeTxBuf(void)
{
  /* Write your code here ... */
//	IO_TX.Flags.bitFlag.freeTXx2=TRUE;//esta libre la transmision.
	serial.vars.Flags.bitF.FreeBuffTX=TRUE;//serial.Flags.bitFlag.FreeBuffTX=TRUE;
	
	
}

/*
** ===================================================================
**     Event       :  CLK_AN_COUNT_OnCapture (module Events)
**
**     Component   :  CLK_AN_COUNT [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CLK_AN_COUNT_OnCapture(void)
{//word c;																																																																																																																																																																																																																																					
static CLK_AN_COUNT_TCapturedValue CntrState,v; /* Content of counter */
word CntrStat1,b;

  /* Write your code here ... */
  ddsgen.var.countRelojAnalogo=0;//RESET señal reloj analogo muerto
  if(ddsgen.bits.Bits.contrlVar){
	    ddsgen.bits.Bits.contrlVar=FALSE;
        ddsgen.var.t3A=getReg16(GPTC3);}
  else {ddsgen.bits.Bits.contrlVar=TRUE;
        ddsgen.var.t3B=getReg16(GPTC3);
        CntrStat1 = getReg16(GPTCNT);
        b=ddsgen.var.t3B-ddsgen.var.t3A;
        if((b<13)||(b>8)){ddsgen.var.countEvent=0;}
        else{if(++ddsgen.var.countEvent>2000){
        	       ddsgen.var.countEvent=0;
        	       eLog(ERROR_RELOJ_ANALOGO);}
            else{}
          }
        
        }
  

   //a = getReg16(GPTC3);/* Return captured value of counter register */
   //d=  getReg16(GPTCNT); 
   
   
   //b=a0-a;    
  //CLK_AN_COUNT_Reset();
  //Reloj_Analogo_Monitor++;
}

/*
** ===================================================================
**     Event       :  Busy_OnInterrupt (module Events)
**
**     Component   :  Busy [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  Cpu_OnIntRelatedEXP (module Events)
**
**     Component   :  Cpu [MCF52233_80_LQFP]
**     Description :
**         This event is called when one of the interrupt related
**         exceptions occurs. This event is automatically enabled when
**         the <Interrupt related reset> property is disabled and at
**         least one of the <Interrupt related exceptions> is enabled.
**         Disabling this event also disables all the <Interrupt
**         related exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which exception caused this event.
**                           Possible values are:
**                           - SPURIOUS_INTERRUPT - Spurious Interrupt
**                           Exception 
**                           - UNINITIALIZED_INTERRUPT - Uninitialized
**                           Interrupt Exception
**     Returns     : Nothing
** ===================================================================
*/
void Cpu_OnIntRelatedEXP(byte ExceptionType)
{byte n;
  /* Write your code here ... */
	n=ExceptionType;
}

/*
** ===================================================================
**     Event       :  SM1_OnFreeTxBuf (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SM1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
	
	statusDDS=0xAA;
	
}

/*
** ===================================================================
**     Event       :  CLK_AN_COUNT_OnOverflow (module Events)
**
**     Component   :  CLK_AN_COUNT [Capture]
**     Description :
**         This event is called if counter overflows (only when the
**         component is enabled - <Enable> and the events are enabled -
**         <EnableEvent>.This event is available only if a <interrupt
**         service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CLK_AN_COUNT_OnOverflow(void)
{
  /* Write your code here ... */
   ddsgen.bits.Bits.overflow=TRUE;
//   CLK_AN_COUNT_DisableEvent();
   eLog(ERROR_RELOJ_ANALOGO);//señal muy lenta, baja frecuencia
}

/*
** ===================================================================
**     Event       :  EInt1_OnInterrupt (module Events)
**
**     Component   :  EInt1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void EInt1_OnInterrupt(void)
{
  /* place your EInt1 interrupt procedure body here */
}


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale MCF series of microcontrollers.
**
** ###################################################################
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
//void TI2_OnInterrupt(void)//50useg de interrupcion
//{static unsigned short int i;
//// static uns
//  /* Write your code here ... */
////    LED12_SYS_NegVal();
//	/*if(dds.display.flag_TX_Pixel<250)
//		dds.display.flag_TX_Pixel++;*///contamos porque queremos 60useg apartir de cualquier instante
////    Deteccion.flag_TX_box++;
////    if(++ddsgen.var.countRelojAnalogo>10000){
////    	eLog(ERROR_RELOJ_ANALOGO);}
//    
//    if(ddsgen.bits.Bits.display_Cntrl_mens)
//    	 ddsgen.var.countTimeMensajeError++;
//     
//   e.status.timer++;
//   e.priority.timer++;//timer parar prioridad
//   if(vfd.v.timer>0)
//	   vfd.v.timer--;//para mandar dato al VFD
//   delaydebug++;
//  
//}

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
			
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI2_OnInterrupt(void)
{
  /* Write your code here ... */
	static unsigned short int i;
	// static uns  INTERRUPT PERIOD 50US
	  /* Write your code here ... */
	//    LED12_SYS_NegVal();
		/*if(dds.display.flag_TX_Pixel<250)
			dds.display.flag_TX_Pixel++;*///contamos porque queremos 60useg apartir de cualquier instante
	//    Deteccion.flag_TX_box++;
	//    if(++ddsgen.var.countRelojAnalogo>10000){
	//    	eLog(ERROR_RELOJ_ANALOGO);}
	    
	    if(ddsgen.bits.Bits.display_Cntrl_mens)
	    	 ddsgen.var.countTimeMensajeError++;
	     
	   e.status.timer++;
	   e.priority.timer++;//timer parar prioridad
	   if(vfd.v.timer_us>0)
		   vfd.v.timer_us+=50;//para mandar dato al VFD
	   delaydebug++;
	   
	
	   
}  //--------------------------------------

/*
** ===================================================================
**     Event       :  SM1_OnRxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SM1_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SM1_OnError (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SM1_OnError(void)
{
  /* Write your code here ... */
}


