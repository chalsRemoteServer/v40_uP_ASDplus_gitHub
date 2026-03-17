/* ###################################################################
**     Filename    : main.c
**     Project     : 39_22_12_IN1UP
**     Processor   : MCF52233CAF60
**     Version     : Driver 01.00
**     Compiler    : CodeWarrior MCF C Compiler
**     Date/Time   : 2026-03-10, 10:20, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "RTS_UART0.h"
#include "VFDserial.h"
#include "IOUPserial.h"
#include "LED4_SYS_MON.h"
#include "LED56_COMM_WARN_MON.h"
#include "LED3_Process.h"
#include "LED12_SYS.h"
#include "CS1.h"
#include "CTS_UART0.h"
#include "DB3.h"
#include "FSYNC.h"
#include "I2C.h"
#include "LED_BOOT.h"
#include "TI2.h"
#include "TI1.h"
#include "WP.h"
#include "RC.h"
#include "LED3.h"
#include "LED2.h"
#include "SDATA.h"
#include "LEDERRCOMM.h"
#include "RTC1.h"
#include "CS0.h"
#include "TACHO.h"
#include "CLK_AN_COUNT.h"
#include "CNTR_G_AMP1.h"
#include "CNTR_G_AMP2.h"
#include "CNTR_G_AMP3.h"
#include "PIN_DRIVE_OUT.h"
#include "SM1.h"
#include "FREC_SEL0_PIN.h"
#include "FREC_SEL1_PIN.h"
#include "ADC_BAL.h"
#include "Busy.h"
#include "EInt1.h"
#include "FC322.h"
#include "FC1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
