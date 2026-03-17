/* ###################################################################
**     Filename    : main.c
**     Project     : 17_IN1UP
**     Processor   : MCF52233CAF60
**     Version     : Driver 01.00
**     Compiler    : CodeWarrior MCF C Compiler
**     Date/Time   : 2015-07-13, 17:20, # CodeGen: 0
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
**  
**  PROGRAMA PARA LA TARJETA INSIGHT PROCESADORA VERSION-2
**  
**  version 34.-
**  version 35.- se intercambio el uart 1 y 0 porque el hardware estan intercambiados
**  
**  
/* 25-OCT-19
 *   Xtal: 25 Mhz, 
 *   MCF52233CAF60
 *   External clock: 60Mhz
 *   Bus  clock Frecuq: 30Mhz
 *   System Clock Frecq: xtal*12/1
 *   PLL: enabled
 *   loss of lock reset: disable, disable
 *   PLL multiplier: 12
 *   PLL divider: 1
 *   RTC  divider: 1hz
 *   Unhandler vectors: Own handle for every
 *   Cpu_D
 *   
 *  Busy:Exint,
 *     IRQ11_PGP3, falling edge, 
 *     level 7 priority 7,
 *  TI1:TimerInt
 *     GPATA_channel1
 *     leve1,priority1
 *     interrupt period:500us
 *     same period in mode: yes
 *     use entire timer: no
 *     init: no
 *     event enable in init:no
 *     high speed: enable only
 *  SM1:SynchroMaster
 *  
 *      SM1
 *      QSPI
 *      LEVEL 6,7
 *      LEVEL 6,7
 *      input buffer:0
 *      output buffer:0
 *      width:16bits
 *      clock edge: falling edge
 *      shift  clock rate: 0.1useg
 *      wait delay: 0.425useg
 *      empty char:0
 *      ignore::no
 *      send MSB first:yes
 *      shift clock idle polarity: High
 *      enble in init: yes
 *      events enabled in init: yes  sin
 *      METHODS: ALL OFF
 *      events:
 *    
 *     channel: QSPI
 *     int:Enabled
 *     level 6 priority 7
 *     level 3 priority 7
 *     
 *     width: 16 bits
 *     input pin: QSPI DIN CANRX URXD1 PQS1
 *     output pin: QSPI DOUT CANTX
 *     clock pin: QSPI CLK SCL
 *     slave disable
 *     clock edge: falling edge
 *     shift clock rate: 0.1useg
 *     wait delay: 0.425us
 *     empty char: 0
 *     ignore empty char no
 *     Send MSB FIRST:  yes
 *     shift clock idle polarity:  High
 *     enable in init: yes
 *     events enable in init: yes
 *     high speed: enable
 * CS0:BITio
 *     PIN: SPDLED_PLD2
 *     Dir: out
 *     init value: 1
 *     Optimizacion: speed
 * CS1: bITIO
 *    pin:SYNCA CANTX FEC MDIO
 *    Direction: output
 *    init value: 1
 *    optimization:speed
 * RC:bitIO
 *    pin:DTIN0 DTOUT0 PWM0 PTC0
 *    dir: output
 *    init value:1
 *    optimization: speed
 * QSPI_CS0:BitIO
 *    pin:QSPI CS0 SDA UCT1
 *    dir:output
 *    init:0
 *    optimization:speed
 * LED_BOOT:BitIO
 *    pin:DUPLE PLD3
 *    dir:output
 *    init:0
 *    optimization:SPEED
 * LED2:BitIO
 *    pin:AN0 PAN0
 *    dir:output
 *    init:0
 *    optimization: speed
 * LED3:BitIO
 *    pin:AN1 PAN1
 *    dir:output
 *    init:0
 *    optimizacion: speed
 * VFDserial:AsynchroSerial
 *    channel:UART1
 *    level2, priority 2
 *    level1, priority 2
 *    level1, priority 3
 *    input buffer: 200
 *    output buffer: 900
 *    CTS: disable
 *    RTS: disable
 *    parity:8
 *    width:1
 *    stop bit: Normal
 *    channel mode:Normal
 *    Receiver: URXD1 FEC TXD0
 *    Transmitter: UTXD1 FEC COL PUB0
 *    Baud rate: 9200
 *    Break: disable
 *    enable init code: yes
 *    enable event in init: yes
 *    high speed: enable
 *    
*/       
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "VFDserial.h"
#include "LED2.h"
#include "LED3.h"
#include "LED4_SYS_MON.h"
#include "SDATA.h"
#include "FSYNC.h"
#include "TI1.h"
#include "DB0.h"
#include "DB3.h"
#include "LED_BOOT.h"
#include "LED12_SYS.h"
#include "LED_BOOT.h"
#include "LEDERRCOMM.h"
#include "LED56_COMM_WARN_MON.h"
#include "LED56_COMM_WARN_MON.h"
#include "LED3_Process.h"
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
#include "CS0.h"
#include "I2C.h"
#include "RTC1.h"
#include "CS1.h"
#include "CTS_UART0.h"
#include "DB0.h"
#include "RC.h"
#include "LED3.h"
#include "WP.h"
#include "IOUPserial.h"
#include "CTS_UART0.h"
#include "RTS_UART0.h"
#include "LED12_SYS.h"
#include "FSYNC.h"
#include "TI2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "VFD.h"
#include "VFDmenu.h"
#include "errorController.h"
#include "Memoria.h"	
#include "frecqController.h"
//#include "LCD.h"
#include "I2C_.h"							
#include "ADC.h"
#include "BIOS.h"
#include "DSP.h"
#include "IOcomms.h"
#include "delay.h"
#include "AnalogCntrl.h"
#include "SistemaOperativo.h"
#include "seguridad.h"
#include "queue.h"
#include "TFTmenu.h"
#include "system.h"
#include "LED3_Process.h"
#include "keypad.h"

//extern struct _Error323_ sysMon;
extern struct _Comando_DDS dds;


/* User includes (#include below this line is not maintained by Processor Expert) */
void main(void){  //MCF52233CAF60    256Kbytes/RAM=32kbytes.
	
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert inte	rnal initialization.                    ***/
  /* Write your code here */
  /* For example: for(;;) {} */
  controlador_LED12(INIT);
  disable_ErrorControls();
  TI1_DisableEvent();
  Busy_Disable();
  disableIO_reciv();
  TACHO_Disable();
  CLK_AN_COUNT_Disable();
    //init_VFD(); la mandamos para abajo
    
    init_I2C();
    init_Menu();
    init_ErrorController();
  
    init_MemoVars(); //sacar variables de la EEPROM no se pudo poner el preprocesador #error
    init_Products();
    init_Control_deFrecuencias();
    
    init_DSP();
    
    //init_BIOS(); //ejecuta el BIOS 
    init_VFD();
    run_Menu();
    init_ADC();//al final se debe ejecutar
    init_IOcomms();
    enable_ErrorControls();//va al final de todos
    enable_Comms_TX();
    enableIO_reciv();	
    init_Semaforos();
    init_Analoga();
    init_queues();
    init_menuTFT();
    controlador_LED12(WAIT);
    controlador_LED3_Detect(ON);//encedered2
    setPassword_Debug();//simula que estan grabadas las passwords en LA eeprom
  for(;;){
	 //          inicia codigo de pruebas
//	             SignalProcessor_IRQ();//Procesamiento de datos digitales del ADC
	  
	    DDSsignalProcessor_Operativo();// se tiene que inizializar el dsp //despliega los puntitos en pantalla DDS
	    DDS_repaint_Zoom();//aqui	    
	    Display_DDS_TRANSMISSOR_CONTROLLER();//transmite los datos al menu DDS
	    IOUP_BOARD_SERIAL_CONTROLLER();//CONTROLADOR PRINCIPAL DE transmision a la tarjeta io
	    Signal_Processor_Controller();//Procesador Central de las Señales analogas digitalizadas
	    MonitorCentralEntradas();//monitorea las ntradas y decide 
	    displayPruebasEntradas();  
	    //ControladorOperativoPrincipal()
	    Display_de_Graficos_Operativo();//displayOperativoBarraDet_Numeros();//deprecated Display&Cntrl de datos del Menu de Configuracion de angulo de Vibracion 
              //Display_Y_Barra_Deteccion_Controlador_Operativo();//despliegue de barra de deteccion y caracteres
	    Control_Operativo_de_Escape_de_Menus();//Cambio de Menu  
	    menuTemporizadorOperativo();//monitor de menus,
	    ErrorMonitor();//monitor de errores
//	    ControladorOperativoComunicacionI2C();//controlador de comunicaciones I2C hacia memorias,driver de vibracion,y generador de frecuencia
//	    Procesador_Operativo_de_Graphicos();EN CONSTRUCCION
	    ControladorOperativoTemporizado_IRQ();//controlador de tiempos del sistema operativo
	    displayTimer_IRQ();
	    IN3UP_test();
	    DebugSimulationKeyboard();//teclado virtual remoto
	    Monitor_Maestro_de_Voltajes();//monitorea los voltajes de la analoga.
	    //validatePassword1();
	    Controlador_Principal_Central_Communicacion_IIC();
	    //Controlador_de_Teclado_Keypad();
	    Testing_SO_Debug();
	    Control_Driver_Keypad();
      }// fin codigo de pruebas
  	  

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
