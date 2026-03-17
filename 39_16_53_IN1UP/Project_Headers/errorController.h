/*
 * errorController.h
 *
 *  Created on: Jul 14, 2015
 *      Author: chals
 */

#ifndef ERRORCONTROLLER_H_
#define ERRORCONTROLLER_H_



#endif /* ERRORCONTROLLER_H_ */


#define ERROR01                    0x01 //error en el switch de init_Control_deFrecuencias
#define ERROR02_I2CCOMMFAILS       0xFF //error I2C comm fails
#define COMMI2CERR                 0xFE //error I2C comm error no ack
#define SIZE_LIST_ERRORS           100 //cantidad de errorres guardados
#define SAVED                      0xFD//ha sido salvado el error?




#define Fault_Save_Prod_EEPROM     0x02 //para abrebiar se pone el codigo
#define CODE_ERROR_DIGITAL_FILTER1 0x03 //error de codigo en filtro digital1

#define CODE_ERROR_0000            0xFE //NO HAY NINGUN ERROR
//ERRORES DE SOFWARE   00XX XXXX  <--RESPETAR MASCARA MASCARA
#define CODE_ERROR_0001_ADC        0x01//no funcionan los ADCs
#define CODE_ERROR_0002_BOOT       0x02// error no funciona el BIOS algo no initizalizo
#define CODE_ERROR_0003_VFD        0x03// error de ccomunicacion de keypad recepcion o transmision de VFD
#define CODE_ERROR_0004_IO         0x04// error de comunicacion con la IO inea
#define CODE_ERROR_0005            0x05//
#define CODE_ERROR_0006            0X06//getANS_DATOS
#define CODE_ERROR_BUFFER_RX_FULL  0x76
//ERRORES COMANDOS     01XX XXXX  <--RESPETAR MASCARA
#define CODE_ERROR_STATUS          0x77//se guarda en el head y nos dice que no hay errores
#define CODE_ERROR_GET             0x78//comando para obtener el error con mayor prioridad si lo hay
//ERRORES DE HARDWARE  10XX XXXX  <--RESPETAR MASCARA       0x60U //aqui inician los errores por hardware que se indican en los LED's
#define CODE_ERROR_HARDWARE        0x80//INICIo de errores de harware
#define CODE_ERROR_COMM_IO         0x81//NO DETECTAMOS comunicacion de recepcion con la IO
#define CODE_ERROR_COMM_IO_RX      0x82
#define CODE_ERROR_ANSWER_DATOS    0x83 //ERROR DE RESPUESTA DE DATOS  
#define CODE_ERROR_FIFO_FULL       0x84
#define CODE_ERROR_EEPROM_BLOQUE1  0x85 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE2  0x86 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE3  0x87 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE4  0x88 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE5  0x89 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE6  0x8A //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE7  0x8B //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE8  0x8C //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE9  0x8D //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE10 0x8E //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE11 0x8F //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE12 0x90 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE13 0x91 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE14 0x92 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE15 0x93 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE16 0x94 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE17 0x95 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE18 0x96 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE19 0x97 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE20 0x98 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE21 0x99 //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE22 0x9A //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE23 0x9B //No se escribio o leyo el bloque en la EEPROM
#define CODE_ERROR_EEPROM_BLOQUE24 0x9C //No se escribio o leyo el bloque en la EEPROM

#define CODE_ERROR_EEPROM_BYTE1    0xB0//No se escribio o leyo un byte de memoria



// ver que esta lista no se mayor a SIZE_LIST_ERRORS
#define VEL_LED_NORMAL            120 //VELOCIDAD DEL LED NORMAL
#define VEL_LED_ERR_IO            60  //velocidad de error comm con la IO
#define SIZE_ERROR_HW             20//TAMAÑO DE LA lista que guarda los errores de hardware

/* DEFINICIONES COMUNES DE IO Y PROCESADORA
 * 
     version 1.0.0 */
#define ERROR_DE_RTS_DE_UP                   0x30
#define ERROR_DE_COMUNICACION_DE_RECEPCION   0x31
#define ERROR_RECEPCION_DE_ACK               0x32 //deprecated
#define ERROR_RECEPCION_EN_ESPERA_TIMEOUT    0x33 //no mandaron nada de la otra tareta como habian dicho
#define ERROR_CTS_CERRO_SIN_ECIBIR_ACK       0x34//la otra tarjeta cerro el puerto de escucha sin recibir ACK 
#define ERROR_RECEPCION_ACK_IO               0x01
#define ERROR_SW_FIFO_KEYPAD_LLENA           0x40


/* fin de definiciones comunnes de IO  y UP  */




/* version 310322-1430    add aLed
 * */


/*version 070422-1210  add union bit bytes error leds*/
struct _Error323_{
//	unsigned char status;
//	void (*debug1)(signed short int*,unsigned char);
//	signed short int *head[100];
//	unsigned char position[100];
	union _LEDS{
	    unsigned char LEDS;
		struct {
	    unsigned char aLed4_Comm_Warnning_Sys:1; 
		unsigned char i:1;
		unsigned char Led4_Comm:1;//status
		unsigned char Led4_Comm_sem:1;//semaforo
		unsigned char x5:1;
		unsigned char x6:1;
		unsigned char x7:1;
		unsigned char x8:1; 
	   }bits;
     };
};

struct ErrorList_nodo{
	   
	 //  unsigned short int errorNum;//numero de eror  ,, ver arriba
	   unsigned char status;// status salvado, borrado, listo o que
	 //  struct ErrorList_nodo *next;
};

struct ErrorHardware{//guarda los errores de hardware en una lista
	  struct ErrorHardware *next;
	  struct ErrorHardware *prev;
	  unsigned char error;
	  unsigned char prioridad;
};


struct _REPORTE_{
	  unsigned char Tipo;//es el tipo de aviso, warning, error,info,debug,
	  unsigned char Aviso;//aviso numero, el numero del aviso que despliega
	  unsigned long int timestamp;  //timesptamp son 4 bytes
      unsigned short int param1;
      unsigned short int param2;
};




//procedimientos y funciones++++++++++++++++++++++++++++++++++++++++
void init_ErrorController(void); // controlador maestro de errores
void init_Leds(void);
void LedController_IRQ(void);
struct ErrorHardware *ErrorController(unsigned char e);
void LedLoop(void);
void LedErrorCommunicaciones(unsigned char status);
void eLog(unsigned char log);
void init_ErrorController(void);
void init_ListaError(void);
void init_Lista_errorHardware(void);
void disable_ErrorControls(void);
void enable_ErrorControls(void);
struct ErrorHardware *getheadeHw(void);
void setLEDerrorHardware(void);
void ErrorMonitor(void);
void verificar_head_FIFOs_(signed short int *p,unsigned char n);
void controlador_LED12(unsigned char n);
void controlador_LED3_Detect(unsigned char n);
void  controlador_LED_COMM_SYS_MON(void);




