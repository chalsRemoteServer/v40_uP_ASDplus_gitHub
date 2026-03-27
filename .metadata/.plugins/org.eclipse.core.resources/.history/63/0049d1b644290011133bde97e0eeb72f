/*
 * system.h
 *
 *  Created on: Sep 15, 2015
 *      Author: chals
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_



#endif /* SYSTEM_H_ */

#define PHARMACEUTICO 0xAB

#ifndef _TIPO_DE_MAQUINA_
#define _TIPO_DE_MAQUINA_ PHARMACEUTICO //TRIPLE_FRECUENCIA
#endif

#define ON   1
#define YES  1

#ifndef FALSE
 #define  FALSE  0
#endif

#ifndef TRUE
  #define  TRUE   1
  #define  YES    1
  #define  SIMON  1
  #define  ON     1   
#endif


#define OFF  0
#define NOP  0
#define OK   0 //PARA EL regreso de una funcion
#define BOX_VACIA   0x01    
#define BOX_LLENA   0x02
#define POSITIVO    0	
#define NEGATIVO    1
#define POS         0xFF
#define NEG         0xAA

#define VERDE1      0xE1
#define HI  0x00
#define LO  0x01

#define POPx  0
#define PUSHx 1
#define HEADx 2
#define TAILx 3
#define POPy  4
#define PUSHy 5
#define HEADy 6
#define TAILy 7


#define WAIT        10	
#define DONE        11
#define FREE        12
#define _BOOT_      14  //estamos en el init antes del main booteando
#define SUBE        16
//#define STOP        17
#define RIGHT        'R'
#define CENTRAL      19
#define LEFT         'L'
#define MAYUSCULAS   21
#define MINUSCULAS   22
#define SYMBOL       23
#define GET          24
#define PABAJO        '!'
#define PARRIBA       '¡'
#define ACTIVADO      'A'
#define ACERCAR       'a'
#define ALEJAR        '@'
#define BUSY          'B'
#define BUSY_WAIT     '@'
#define EJECUTAR      'C'
#define _DRIVER_      'D'
#define DINAMICO      'D'
#define DESACTIVADO   'd'
#define EMPTY         'E'
#define ESTATICO      'E'
#define ERROR_PASS    'e'
#define FULL          'F'
#define INIT          'I'
#define EJECUTANDOSE  'J'
#define NUMEROS       'N'
#define _PICKUP_      'P'
#define ROJO          'R'
#define SET           'S'
#define CANAL_X       'X'
#define CANAL_Y       'Y'
#define VERDE         'V'//verde ya usado no cambiar
#define libre_1       '1'// lugar libre para usar
#define CMD_POS_CHAR  '2'//COMANDO_FASE  lugar libre para usar  comando display fase de 5 chars
#define COMANDO_POS   '3'//comando solo posicion se manda la posicion
#define COMANDO_CHAR  '4'//comando solo un char asi nomas 
#define CMD_POS5CHARS '5'//COMANDO ENVIAR POSICION Y luego 5 chars
#define _DELAY_US     'u'
#define _DELAY_MS     'm'


#define EVENT_DDS_ON   0x80  //axxx xxxx flag event DDS TRUE
#define EVENT_DDS_OFF  0x7E //0111 1111 put off flag event-DDS as FALSE
#define DDS_BORRAR     0x40 //xbxx xxxx flag event borrar, TRUE   
#define DDS_BORRAR_OFF 0xBF //1011 1111 apagar bandera de borrar DDS
#define DDS_TIMER      0x20 //xxcx xxxx flag de timer para desplegar DDS
#define DDS_TIMER_OFF  0xDF //1101 1111 flag apgar el timer de dispaly DDS
#define DDS_RELOAD     0xE0//to reoad dds on zoom change, or rezise product angle
#define BUSY_K         555 //timer veces, que se cicla todo el sistema para hacer delay 


#define FIRST_START  0x80 //1xxx xxxx el '1' indica que ya se prendido|inizializado antes

#define CONFIRMAR    24
#define INCREMENTAR  25
#define RESET        26
#define READ_A_INC   28//read and increase
#define SUMAR         0x80  //No midificar ACcion de sumer en una funcion una variable, es un comando en una funcion 
#define RESTAR        0x40 //No modificar  ES UN comando en la funcion setUINT_varxDigito

#define SIZE_EEPROM 2000


#define GIRO_DE_PLANO_CARTESIANO    0x6B
#define GIRO_DE_COORDENADAS_POLARES 0x5A
//#define BAJA        17 redefined en memoria.h

//Configuracion de Vibracion
#define DISPLAY_FREE      0xF0 //Este valor es para la autoConfiguracion
#define DISPLAY_BUSY_BOX  0xF1//EL display esta ocupado desplegando boxes
#define DISPLAY_BUSY_NUM  0xF2 //El display esta ocupado desplegando numeros

//BOX BAR DISPLAY KOSTANT
#define SUBE_DONE   18
#define BAJA_DONE   19
#define LAST_BOX    20
#define MAX_BOXES   17 //NUMERO DE BOXES DINAMICAS del detector de barra.



//Constantes de control Operativo
#define DISPLAY_EEPROM       0x88 //despliegue la pantalla de contenido de eeprom
#define READ_EEPROM_DISPLAY  0x89

//Constantes Generales de memoria
#define RESTAURAR   0x20
#define SAVE        0x21

#define READ        0x24
#define BORRAR      0x25
#define CAMBIAR     0x26

//Menu MEDIDAS_DE_VOLTAJES
#define RAW_X       0x30 
#define RAW_Y       0x31
#define CENTRADO_X  0x32
#define CENTRADO_Y  0x33
#define ROT_X       0x34
#define ROT_Y       0x35
#define FILTRADO_X  0x36
#define FILTRADO_Y  0x37

//constantes para seleccionar el dispositivo SPI  a comunicar
#define ADC        0x31
#define DDS        0x33 //direct digital synthesis generator chip AD9833
#define ADC_CH0    0x34//cambio al ADC de otro dispositivo
#define ADC__CH0   0x35//para el ADC YA funcionando
#define ADC_CH1    0x36//cambio al ADC de otro device
#define ADC__CH1   0x37//el ADC en marcha constante

#define _A5_  0
#define _A4_  1
#define _A3_  2
#define _A2_  3
#define _A1_  4
#define _DM_  5
#define _M_   6
#define _C_   7
#define _D_   8 
#define _U_   9


//usuarios-----------------------------------------------------------------
#define UserDesarrollador  6  //clave    1974
#define UserIngeniero3     5  //clave    2904 
#define UserIngeniero2     4  //clave 5
#define UserIngeniero      3  //clave 4
#define UserAdministrador  2  //clave 2
#define UserOperador       1  //clave 1


//constantes IGUALES EN LA PROCESADORA Y LA IO
#define ACK   0xAA  //Byte de Acknowlege
#define NACK  0xAB  //Byte de no reconocimiento de paquete

//SEMAFOROS
#define SEM_CONTEXTO_CAMBIO 0xA1//semaforo para el cambio de contexto, 
#define SEM_CONTEXTO_FIN    0xA2//semaforo para esperar que se habilite el teclado y poder reactivar el controlador operativo escape

//-CONTROL DE PASSWORD
#define PASSCNTRL_SIZE  6

#define _ORIGEN_  1 //indice del array que controla la brrera de password
#define _DESTIN_  2	
#define _AUTHOR_  3
//--------------------------------------
#define ADMINISTRADOR  'A'
#define SERVICIO       'S'
#define OPERADOR       'O'



//display de Sensibilidad en el DDS y deteccion.
//limites de sensibilidad en cada zoom
#define ZOOM_LIM_99 30
#define ZOOM_LIM_98 30
#define ZOOM_LIM_97 31


#define STX 0x03
#define ETX 0x02


//SIZES-------------------------------------------------------
#define BUFF_X_CONTEXTO_SIZE  MAXIMO_NUMERO_PROD+1 //RELE



//typedef union{//access word: 
//	unsigned  short int wordx;   //   	0xaabb   
//	unsigned char byteH;      //         aa
//	unsigned char byte[2];        //byte[0]=aa,byte[1]=bb
//}uword;
//
//typedef union{//access word: 
//	unsigned  long int Dwordx;   //   	0xffxxaabb    capacidad: 131071 bytes=1FFFFH=
//	//unsigned char byteH;      //         ff
//	unsigned char byte[4];        //byte[0]=ff,byte[1]=xx,byte[2]=aa,byte[3]=bb;
//}Dword;


/* REPORTES  +++++++++++++++++++++++++++++++++++++++++++++*/
//dominios
#define D_ERROR     0x22
#define D_FALLA     0x23
#define D_WARNING   0x24
/* tarjeta de evento  */
#define SUB_EV_UP   0x25
/* subdominios de eventos-.........................*/
#define  IIC_NO_ACK 0x26//I2C error no comm error de no responder ACK 


/*  VFD sistema operativo  KONSTANTES */
#define  _CHAR_     'c' //se despliega un char
#define  _PUNTO_    'p' //se despleiga un punto
#define  _RAYA_     'r'  //se despliega una raya
#define  _POS_      'P' //comando de posicion en la grafica
#define  _BOX_      'B' //se despliega una box de barra de deteccion
#define  _BOLD_     'D' //char bold display
#define  _NORM_     'N' //display char normal
#define  _BLINK_    'L' //display char blinking
#define  _INK_OFF   'F'//puntito apagado
#define _INK_ON     'O' //PUNTITO prendido
#define _DDS_BORRAR 'R'
#define _DDS_reZOOM 'z'
#define _DELAY_     'E'//delay de espera para mandar algo al VFD
//#define _FEM_       0x54 //estado FFlag End MEnu, describe que ya se desplego el menu seleccionado- para poder empezar a trabajar en ese menu
#define DELAY_TIME 1 //numero de IntERRUPCIONES DE TIMER1 para crear delay
#define DATOS_SIZE 14U //tamaño del buffer de transmision al VFD

#define XROW        128//buffer para guardar los pixeles  que se estan
#define YCOL         24//desplegando en el DDS para se usados por el zoom despues

/*******ERRORES CONTROL ****************************************/
#define WARNING 'W'
#define ERROR   'E'
#define INFO    'I'
#define BUG     'B'

/**MANEJADOR DE RECURSOS********************************************/
#define _VFD_       0xA1
#define _IO_        0xA2
//PID  DE MANEJADOR DE RECURSOS
#define PID_INIT_VFD            0xB1
#define PID_DISPLAY_RECHAZO_VFD 0xB2
#define PID_DISPLAY_PROD        0xB3
//PARAMETROS
#define SIZE_INST       4

/****************************************************************/
//PID  o Paternidad
#define PID1  0x01 //xinit_VFD
#define PID2  0x02 //VFDcommand
#define PID3  0x03 //FontSizeVFD
#define PID4  0x04//xControl_Principal_de_Menus_Operativo
#define PID5  0x05 //Display_Monitor_de_Phase_VFD
#define PID6  0x06//Display_Monitor_de_Rechazos_VFD
#define PID7  0x07//Display_Monitor_de_Producto_VFD
#define PID8  0x08 //Display_Monitor_de_Sensibilidad_VFD
#define PID9  0x09 //xEmpaquetado_de_datos_Deteccion_Hacia_IOUP
#define PID10 0x0A //xDisplay_Cursores_DDS_VFD
#define PID11 0x0B//xDisplay_Punto_DDS_VFD
#define PID12 0x0C //xDisplay_Sens_DDS_VFD
#define PID13 0x0D//cursorUpDown 
#define PID14 0x0E//digitoUpDown
#define PID15 0x0F//enterCursorDDS
#define PID16 0x10//digitoRtLeft
#define PID17 0x11//VFDdrawLine_v4

#define CMD_DDS 0xF1 //comando de transmision datos de deteccion raw X&Y


