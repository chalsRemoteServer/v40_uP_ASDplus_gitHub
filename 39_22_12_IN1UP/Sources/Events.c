/* ###################################################################
**     Filename    : Events.c
**     Project     : 39_22_12_IN1UP
**     Processor   : MCF52233CAF60
**     Component   : Events
**     Compiler    : CodeWarrior MCF C Compiler
**     Date/Time   : 2026-03-10, 10:20, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         EInt1_OnInterrupt       - void EInt1_OnInterrupt(void);
**         Busy_OnInterrupt        - void Busy_OnInterrupt(void);
**         CLK_AN_COUNT_OnCapture  - void CLK_AN_COUNT_OnCapture(void);
**         CLK_AN_COUNT_OnOverflow - void CLK_AN_COUNT_OnOverflow(void);
**         TACHO_OnCapture         - void TACHO_OnCapture(void);
**         RTC1_OnAlarm            - void RTC1_OnAlarm(void);
**         RTC1_OnSecond           - void RTC1_OnSecond(void);
**         TI1_OnInterrupt         - void TI1_OnInterrupt(void);
**         TI2_OnInterrupt         - void TI2_OnInterrupt(void);
**         IOUPserial_OnError      - void IOUPserial_OnError(void);
**         IOUPserial_OnRxChar     - void IOUPserial_OnRxChar(void);
**         IOUPserial_OnTxChar     - void IOUPserial_OnTxChar(void);
**         IOUPserial_OnFullRxBuf  - void IOUPserial_OnFullRxBuf(void);
**         IOUPserial_OnFreeTxBuf  - void IOUPserial_OnFreeTxBuf(void);
**         VFDserial_OnError       - void VFDserial_OnError(void);
**         VFDserial_OnRxChar      - void VFDserial_OnRxChar(void);
**         VFDserial_OnTxChar      - void VFDserial_OnTxChar(void);
**         VFDserial_OnFullRxBuf   - void VFDserial_OnFullRxBuf(void);
**         VFDserial_OnFreeTxBuf   - void VFDserial_OnFreeTxBuf(void);
**         Cpu_OnIntRelatedEXP     - void Cpu_OnIntRelatedEXP(byte ExceptionType);
**         Cpu_OnCoreWatchdogINT   - void Cpu_OnCoreWatchdogINT(void);
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

/* User includes (#include below this line is not maintained by Processor Expert) */

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
void Busy_OnInterrupt(void)
{
  /* place your Busy interrupt procedure body here */
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
{
  /* Write your code here ... */
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
{
  /* Write your code here ... */
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
{
  /* Write your code here ... */
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
void TI2_OnInterrupt(void)
{
  /* Write your code here ... */
}

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
**     Returns     : Nothing
** ===================================================================
*/
void IOUPserial_OnError(void)
{
  /* Write your code here ... */
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
}

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
}

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
}

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
}

/*
** ===================================================================
**     Event       :  Cpu_OnCoreWatchdogINT (module Events)
**
**     Component   :  Cpu [MCF52233_80_LQFP]
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

/* END Events */

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
*/
