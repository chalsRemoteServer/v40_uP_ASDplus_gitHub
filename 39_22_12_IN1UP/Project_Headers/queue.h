/*
 * queue.h
 *
 *  Created on: Oct 3, 2017
 *      Author: chals
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "system.h" //"PE_Types.h"



#ifndef SIZE_FIFO_OFFSET //tamaño del array del filtro digital del offset
#define SIZE_FIFO_OFFSET 32
#endif


#if(SIZE_FIFO_OFFSET==32)
#define  DIV_OFFSET 5 //DIVISOR PARA offset  2^5=32
#else
#error "No coincide el tamaño matematico del filtro con el divisor"
#endif



#define nam1 "xInit_VFD"
#define nam2 "Driver_Tx_VFD_service"
#define nam3 "menus_service"

#define MEM3 2 //memoria 2 bytes de menu service

#define SIZE_BUF_TFT 5
#define SIZE_IO_TX_BUFFER 40 //ARRAY donde se guarda la direccion de la cabeza del paquete a enviar
#define SIZE_B 20//ARRAY donde se guarda los paquetes que se van a enviar

#define SIZE_FIFO_BOX 10
#define SIZE_FIFO SIZE_FIFO_BOX//30  //array size where the ADC's values are saved

#define SIZE_BARR_NUM 10//size fifo to displat numeros and position where menus has barra detection
#define SIZE_FIFO_CMD_DDS 15 //tamaño de la FIFO para transmitir comando DDS al touch

#define SIZE_BUFFER_TX 20//90 //BUFER DE TRANSMISION SERIAL ALA O
#define SIZE_BUFFER_DDS_FIFO 25//50
#define SIZE_TEMP_PIXEL  6//tamaño del buffer para guardar memoria de pixels del display DD
#define SIZE_DDS_PIXEL   40//para pintar el pixel en tiempo real
//#define SIZE_DDS_ZOOM   2000
#define SIZE_X  10//24//array zoom repaint char pantalla 0
#define SIZE_Y  20//129//array zoom repaint char pantalla0

#define SIZE_SER_KEYPAD 10 //TAMAÑO de la fifo del serial del keypad de entraada
#define SIZE_KEYPAD_IN 10 //buffer de entrada de chars de serial de keypad
#define SIZE_BUFFER1  10U //buffers de rx y tx de serial a IOUP
#define SIZE_BUFFER2  25U//tamaño de rx y tx de i2c fifo memorias
#define SIZE_BUFFER5  20U//kEYPAD FLIP FLOP save keysrokes tamaño de buffer char que entran keypad serial 
#define SIZE_BUFFER6  10//1954U//FIFO de graficos box,char,rayas,puntos,pos,con SO
#define SIZE_BUFFER9  50 //fifo GUARDA LOS pixeles que estan fuera de la pantalla DDS porque se salieton del zoom,estan mas alla del valor desplegable actual al zoom actual
#define SIZE_BUFFER13 10 //FIFO para gestionar recurso VFD de los hilos
#define SIZE_BUFFER14 20//20=No modif streaming-block size transmision a la IOUP  
#define SIZE_BUFFER15 20 //20 no modificar FIFO guarda deteccion, para transmitirla por serial IOUP
#define SIZE_BUFFER16 5  //buffer de recursos de vfd de gestion de
#define SIZE_BUFFER17 20//numero de procesos en la queue
#define SIZE_BUFFER22 60 //INPUT TUNEL AL driver EEPROM la, eeprom solo puede leer/write 64 bytes seguimos,
#define SIZE_BUFFER23 60//OUTPUT TUNNER del Driver EEPROM
#define SIZE_BUFFER26 8//historial/stacks de menus por los que hemos pasado

#define SIZE_BOXES    20
#define SIZE_SYS_MONITOR 30
#define SIZE_TBC 15
#define SIZE_CHAR 5
#define SIZE_FEW  3

#if(SIZE_BUFFER6>0xFFF1)
#error 'tamaño del buffer excede de 0xFFF1';
#endif

/*PROCESDIMIENTOS */
#define TIMERS_MAX     8 //NO CAMBIAR,NUMERO DE timer que maneja el sistema operativo pero se usan del 1 al 8 se tiene que convertir del 0 al 7
#define SCI_MAX        4//numero maximo de servicios en el systema
#define THREAD_MAX     7//PROC_MAX/2;
#define RECURSOS_MAX   2 //IO,VFD,No timer en sistema operativo manejo
#define BLOQUE50     50
#define PROC_NAME_SIZE 15
//#define REC_X_PROC     4 //numero de recursos que puede manejar un processo padre$
#define NUM_SON_X_PROC PROC_MAX-1//numero de hijos Max por proceso

/***************************************************************/
#define DATOS_SIZE 14U //tamaño del buffer de transmision al VFD

#define NUM_FILES_OPEN 5 //ARCHIVOS MAXIMOS ABIERTOS
#define MEMO_SIZE10 200//bloque de memoria para aplicaciones size de MEMORIA para procesos
#define MEMO_SIZE20 400//TAMAÑO de Los buffers de memoria
#define MEMO_SIZE30 300//del sistema operativo, memoria
#define MEMO_SIZE60 600//de 10,20,30,40,50 
#define MEMO_PROC_SIZE 1024//memoria para los programas
#define MEMO_FPBP_SIZE 8//MEMORIA DEL Free pROC BLOCK  POINTER
#define MEMO_NODO_MAX  20//nodos del manejo de memoria libre de proc
#define FMBC_SIZE 8//pointer free memory block control
#define _2   0//lugar de memoria libre de 2 bytes
#define _4   1//memoria de Proc de 4 bytes
#define _8   2
#define _16  3
#define _32  4
#define _64  5
#define _128 6
#define _255 7//lugar de pointer que apunta a bloques de memoria de 255



#if ((MEMO_SIZE10+MEMO_SIZE20+MEMO_SIZE30+MEMO_SIZE60)==1500)
   #define T_MEM_SIZE10 20//MEMO_SIZE10/10;//size de las tablas de asignacion de memoria para procesos
   #define T_MEM_SIZE20 40//MEMO_SIZE20/10;
   #define T_MEM_SIZE30 30//MEMO_SIZE30/10;
   #define T_MEM_SIZE60 60//MEMO_SIZE60/10;
   #define PROC_MEMO_NUM 4//numero de memorias para procesos
#else
   #error message("Errores en las definiciones")
#endif


#define SIZE_CACHE_FILE_TABLE 10//2 son los archivos que vamos a guardar favoritos
#define SIZE_CACHE_BIT_BLOCKS 50//bytes para buscar bloques libres
#define O_FILE_MAX  5 //archivos abiertos Num Maximo

#define SIZE_MEM_EEPROM_BITS  1024000
#define FILE_NAME_SIZE 15//tamao del nombre de archivo maximo
#define SEEK_SET  0xA3
#define SEEK_CUR  0xA4
#define SEEK_END  0xA5
#if(SIZE_MEM_EEPROM_BITS==1024000)
	#define SIZE_DE_BLOCK         50 //50 bytes
	
	#define MAX_SIZE_FILE         12700 //bytes ,para que inode No. blocks sea de 1 byte tamao maximo del archivo
	#define SIZE_MEM_EEPROM_BYTES 128000 //1024000/8
	#define NUM_FILES             1280 //((128000/50(blocksSize))/2
	
	#define SIZE_TABLA_BITS_BLOCKS  314 //{[128k-(NUM_FILES*2)]/SIZE_DE_BLOCK}/8
	#define NUM_BLOCKS 2503 //[128k-314-(numfiles*2)]/50
    #define ADD_HEAP_CRC_FILENAME  0//DIRECCION DEL heap 
    #define SIZE_HEAP_CRC_FILENAME NUM_FILE*4 //numero de archivos maximos
    #define ADD_TABLA_ASIGNACION_FILES SIZE_HEAP_CRC_FILENAME+1//direccion donde empieza la tabla de asignacion de archivos
    #define SIZE_TABLA_ASIGNACION_FILES_BYTES (NUM_FILES*2) //
    #define ADD_TABLA_BITS_BLOCKS  ADD_TABLA_ASIGNACION_FILES+SIZE_TABLA_ASIGNACION_FILES+1


#define ADD_MEMO_DATOS SIZE_TABLA_ASIGNACION_FILES_BYTES+SIZE_TABLA_BITS_BLOCKS+1 
#else
    #error message(" Error de definiciones")
#endif



struct NodoBox{
	struct NodoBox *next;
//	struct NodoBox *prev;
	unsigned char box;
};

//struct TBC{//Task Block Control
//	unsigned char uc[SIZE_TBC];
//	char[SIZE_CHAR];
//	float f[SIZE_CHAR];
//	unsigned short int usint[2];
//	signed short int ssint[2];
//	unsigned long int ulint[2];
//	double d[2];
//};

//buffer de Processos 
struct _UCProc{//Unidad de control de Processors
  unsigned char (*proc)(unsigned char *m);//Tabla de Procesos, pointer to proceso actual que se guarda
  char name[PROC_NAME_SIZE];//nombre del proceso
  unsigned char size;//cantidad de memoria asinada a este proceso
  //ver si memoAddr se usa o no se usa para quitarlo
  //unsigned char *memoAddr;//direccion de la Tabla de memoria de PIDs asignada a este proceso
  unsigned char *memoPrAddr;//direccon del Bloque de memoria que usara el proceso
  unsigned char recurso;//[REC_X_PROC];//recursoS asignadoS
  //unsigned char pid;//id del proceso
  struct File *file;//archivos abiertos asignados al recurso
  unsigned char state;//RUNNING,WAITING,READY,
  unsigned char HijosPID[NUM_SON_X_PROC];
    unsigned char PadrePID; 
  unsigned char priority;//prioridad del proceso, 0 es alta prioridad  
};//processors control


struct FILE{
  word FileBlock;//pointer to block-file Logic, header block of the file
  word *filechar;//points to the byte of the file actual to read
  byte tipo;//tipo de dato que se esta leyendo, para el paso del puntador
  uint32 addressFile;//direccion fisica del archivo abierto head
  char name[FILE_NAME_SIZE];
  byte Modo;
  byte idFile;//guarda el indice al que pertenece en la tabla de files abiertos
  byte processPID;//el PID del proceso que solicito el archivo
  byte Return;
  byte status;//SEEKING,READING,WRITING,DELETING ETC
  byte FileDescriptor;//es el indice/estado de la tabla de Servicios/daemons  en Ready
};


struct _SysMon{
   unsigned short int t[SIZE_SYS_MONITOR];	
   unsigned char i;//index	
   unsigned char size; //=SIZE_SYS_MONITOR;
   void(*reset)(void);//funcion de reseNVITACIONt de los regs
};


// version-1
/* DEPRECATED
struct _Vector__{
	union _Byte_{
		unsigned char ByteFlags;
		struct{
			unsigned char FreeBuffTX:1;//bandera de que ya se envio el buffer completo
			unsigned char flipflopBuff:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char resetDDS:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char DDS_ACTIVA:1;
			unsigned char x5:1;
			unsigned char x6:1;
			unsigned char x7:1;
		}bitFlag;
	  }Flags;
   struct _Control_{
	  unsigned char timercomms;//timer de comunicaciones	  
      unsigned char ncountNopeAns;//contamos las no respuestas numero.
      unsigned char ledComunicaciones;
	  }cntrl;	  
	  
//	unsigned char (*appendDDS)(signed short int x,signed short int y);//agrega dato al final de la lista	
//	void (*popDDS)(signed short int *x,signed short int *y);//quitar el prmer elemento de la fifo DDS
    unsigned char buffer[SIZE_BUFFER_TX];//buffer de transmission
//    unsigned char buffer2[SIZE_BUFFER_TX];
    unsigned char *pop;//primer de la FIFO para salir
//    unsigned char *last;//primer de la FIFO para entrar
    unsigned char *head;//indice 0 del buffer
    unsigned char *tail;//ultimo indice del buffer
    unsigned char *push;//pointer buffer 2
//    unsigned char *tail2;//pointer to tail buff2
//    unsigned char *head2;//pointer to head buff2
    void (*appendByte)(unsigned char n);//agregar byte al buffer de transmision
    void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned char size);
    void (*resetFIFO)(void);
    unsigned char ncount;//cuenta los nodos llenos en la fifo
    unsigned char test1;
    //unsigned char test2[50];
};*/


/*  version 290322-1747  add EnableComunicationsTX 
 *  version 310322-1238  modify ledcomms control
 * */
struct Serial_Bandera{
    union _Byte1_{
		unsigned char ByteF;
		struct{
			unsigned char FreeBuffTX:1;//bandera de que ya se envio el buffer completo
			unsigned char flipflopBuff:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char resetDDS:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char DDS_ACTIVA:1;
			unsigned char x4:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char EnableComunicationsTX:1;
			unsigned char x6:1;
			unsigned char x7:1;//timercomms
		}bitF;
	  }Flags;
   struct _VAR_{
		   unsigned char ncountNopeAns;	 
		   unsigned char timercomms;
	  }var;    	  
};


//struct _TBC{//Task Block Control
//	
//	unsigned char uc[15];
//	char c[5];
//	signed short int ssi[2];
//	unsigned short int usi[2];
//	double d[1];
//	float  f[1];
//	
//	
//};


struct DDS_GEN{
   union _Byte91_{
	  unsigned char status;
	  struct{
		unsigned char overflow:1;//flag de over flow
		unsigned char IRQcapture:1;//INTERRUPCION
		unsigned char contrlVar:1;//para ver cual capture vamos a usar
		unsigned char display_Cntrl_mens:1;//control de despliegue de mensaje de error
		unsigned char x3:1;
		unsigned char x2:1;
		unsigned char x1:1;
		unsigned char x0:1;
	  }Bits;
    }bits;
  struct _VAR145_ {
    unsigned long int capture;//conteo de pulsos en 10mseg 
    unsigned char estado;//estado de control de frecq
    unsigned short int delay;//delay de este algoritmo
    unsigned short int ndelay;//variable a comparar para el delay
    unsigned short int t3A; //valor de timestamp timer 3
    unsigned short int t3B;//valor de timerstamp timer 3
    unsigned short int countEvent;//cuenta el numero de veces que hay errores de frecuencia
    unsigned short int countRelojAnalogo;//monitor del reloj analogo muerto,
    unsigned short int countTimeMensajeError; //cuenta el tiempo que paso desde la ultima alerta de error si ya no aparece la alerta se quita el mensaje de errror  
  }var; 
};



/* version 110422-1119 */
struct IO_Bandera{
    union _Byte2_{    
		unsigned char Outs2;
		struct{
			unsigned char Rechazo:1;//bandera de que ya se envio el buffer completo
			unsigned char Warnning:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char Fail:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char Lock:1;
			unsigned char Aux_out:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char PFVE:1;
			unsigned char Gotis:1;
			unsigned char x7:1;//timercomms
		}bitA;
	  }Salidas;
	union _Byte3_{
	  		unsigned char IN2;
	  		struct{
	  			unsigned char PACK_CHK:1;//bandera de que ya se envio el buffer completo
	  			unsigned char GATING:1;//seleccionar el buffer que llenamos en tiempo t
	  			unsigned char AUX1:1;//indica si borramos registro de datos repetidos de DDS
	  			unsigned char AUX2:1;
	  			unsigned char SYS_CHK:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
	  			unsigned char LockSW:1;
	  			unsigned char ResetSW:1;
	  			unsigned char x7:1;//timercomms
	  		}bitB;
	  	  }Ent;
   unsigned char TACHO;	  	    
};






/*version 310322-1641 add reset genaral */
struct _FIFO_1byte_{//FIFO PARA UNA VARIABLE para un byte
    unsigned char *pop;//primer de la FIFO para salir
    unsigned char *head;//indice 0 del buffer
    unsigned char *tail;//ultimo indice del buffer
    unsigned char *push;//pointer buffer 2
	unsigned char (*popf)(unsigned char *n,struct _FIFO_1byte_ *s);//quitar el prmer elemento de la fifo DDS
	unsigned char (*appendByte)(unsigned char  n,struct _FIFO_1byte_ *s);//agregar byte al buffer de transmision
    void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned short int size);
    void (*resetFIFO)(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int size);
    unsigned short int ncount;//cuenta los nodos llenos en la fifo
    unsigned short int size; //size de la fifo
};

//manejo de archivos
struct File{
	unsigned char n;
	unsigned char M;

};

struct _EEPROM_D{//eeprom device 
	struct _FIFO_1byte_ inFIFO;
	struct _FIFO_1byte_ outFIFO;
	
};



/*version 310322-1641 add reset genaral */
struct _FIFO_1Proceso_{//FIFO PARA UNA VARIABLE para un byte
	
    struct _UCProc *pop;//apunta al primer fifo
    struct _UCProc *head;//indice 0 del buffer
    struct _UCProc *tail;//ultimo indice del buffer
    struct _UCProc *push;//Apunta al apuntador a funcion actual que representa el puch actual 
	unsigned char (*popf)(struct _UCProc *n,struct _FIFO_1Proceso_ *s);//quitar el prmer elemento de la fifo DDS
	unsigned char (*appendProc)(struct _UCProc n,struct _FIFO_1Proceso_ *s);//agregar byte al buffer de transmision
    void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned short int size);
    void (*resetFIFO)(struct _FIFO_1Proceso_ *s);
  
    unsigned short int ncount;//cuenta los nodos llenos en la fifo
    unsigned short int size; //size de la fifo
};

struct _Timer{
  union _Status{
	  unsigned char statusByte;
      struct _Bits_{
		  unsigned char enable:1;    
		  unsigned char autoreset:1;
		  unsigned char sleep:1;
		  unsigned char x3:1;
		  unsigned char x4:1;
		  unsigned char x5:1;
		  unsigned char x6:1;
		  unsigned char x7:1;
          }bits;}status;	
  int contador;//el timepo actual
  int interval;//el tiempo que se solicita
  unsigned char pid;//aque PID esta asignado
  void (*new)(unsigned char ntimer,void (*func)(unsigned char *c));
  void (*start)(int interval);
  void (*func)(void);//funcion a la que se interrumpe
  
};

struct _Recursos_{
	 //struct _FIFO_func_  f;//funciones de la fifo de VFDprocesos
	 struct _FIFO_1byte_ RFtmrs;//Recurso Fifo timers, los PID que quieren el recurso
	 struct _FIFO_1byte_ VFD;//fifo de VFD 
	 struct _FIFO_1byte_ IO;//fifo de IO
	 unsigned char (*insertVFDProcess)(unsigned char proceso);
	 unsigned char (*solicitar)(unsigned char recurso,unsigned char PID);
	 	 	  void (*devolver)(unsigned char,unsigned char);
   	 unsigned char (*get_inst)(void);//obtener instancia
   	          void (*dev_inst)(unsigned char inst);//devolver instancia
	 struct{
		unsigned short int control;//VFD numero de registros,que guarda quien esta en la FIFO   
		unsigned short int control_IO;//IO numero de procesos/hilos a manejar
		unsigned char VFD_pid;//pid que ocupa el VFD
		unsigned char AsignadoTimers[TIMERS_MAX];//PID que tienen asignado ese timer y lo posee
		//unsigned char AsignadoTimer;//PID que tiene asignado un timer y todavia no lo posee
		unsigned char timer[TIMERS_MAX];//PIDS que tienen asignado los timers
		unsigned char Asignado[RECURSOS_MAX];//a quien es asiganado el recurso IO
	 }v;//variable
	
};//fin recursos ------------------------------------------------------
	
struct _Threading{
    unsigned char (*sleep)(unsigned short int time);
    unsigned short int contador[THREAD_MAX];//contadores que estan descontando el tiempo el indice corresponde al pid asignado por thread_sleep
    unsigned char pidRegistos[THREAD_MAX];//el pid guardado en el lugar indicado el index
    unsigned short int intervalo[THREAD_MAX];//valor que se desea dormir
    unsigned char enable[THREAD_MAX];//si esta habilitado u nu
    
	
};


struct  _MemNodo{
  unsigned char *next;//siguiente nodo
  unsigned char *pAddBlock;//direccion del block
  
};

struct _Nodo_ {
  byte id;//1xxx xxxx [libre|busy]id del nodo, para identificar nodo
  byte *addr;//direccion del bloque de memoria libro o ocupado por un proceso
  byte PID;//pid propertario del bloque de memoria
  struct _Nodo_ *next;
  byte size;//tamaño del bloque de memoria
  byte status;//libre,ready,0,
};


//https://opendsa-server.cs.vt.edu/embed
struct _Memoria{
	//unsigned char memo10[MEMO_SIZE10];//memoria en bloques de 10
	//unsigned char memo20[MEMO_SIZE20];
	//unsigned char memo30[MEMO_SIZE30];
	//unsigned char memo60[MEMO_SIZE60];
	//unsigned char ProcMemo[MEMO_PROC_SIZE];	//memoria de los programas
	//struct _Nodo_ *FMBC[FMBC_SIZE];//pointer to free memory block control
	//struct _Nodo_ nodo[PROC_MAX];//nodo de control de memoria de procesos libre y ocupada
    //struct _Nodo_ *(*getNodo)(void);
	
    
	unsigned char* (*malloc)(unsigned char *size,unsigned char pid);
	void (*free)(unsigned char *ptr);
	//unsigned char tablaSizera e10[T_MEM_SIZE10];//tablas para encontrar
	//unsigned char tablaSize20[T_MEM_SIZE20];//que pid tiene asignada
	//unsigned char tablaSize30[T_MEM_SIZE30];//que bloque de memoria
	//unsigned char tablaSize60[T_MEM_SIZE60];//
	unsigned char *FPBpSize[MEMO_FPBP_SIZE];//Free Process Block pointer size, guarda los pointer de cada size
    struct _MemNodo mn[MEMO_NODO_MAX];	
    
    unsigned short int freeSpace;//memoria libre de Procesos
    unsigned short int usedSpace;//mem usada de procesos
	
	unsigned char tablaPid[PROC_MAX];//numero de pid del process
	unsigned char *tablaAdd[PROC_MAX];//tabla de inicio de direccion de memo asignada al pid

	
	
};


//struct _Procsx_{
//   byte ncount;//cuantos procesos Ready hay	
//   byte Stack[PROC_MAX];//guarda el numero del PID de los procesos todos
//   byte (*insert)(byte pid);//insert pid en REady list
//   byte (*pop)(byte *pid);//sacar un pid de la lista ready
//   
//};


struct _KERNEL{
	struct _FIFO_1byte_ Ready;
	byte pid_debug;
	byte pid_debug2;
	void (*kill)(byte     pid);
	void (*suspend)(byte  pid);
	void (*resume)(byte   pid);
	void (*free)(byte pid);
	byte (*run)(byte pid);
	void (*insertRecurso)(byte recurso,byte pid);//lista de recursos de cada proceso
	void (*removeRecurso)(byte pid);
	void (*cleanRecursos)(byte pid);
//	byte (*buscarRecurso)(byte recurso,byte pid,byte *index);
	void (*addPID)(byte pid);//agregar pid hijo a un pid padre
	byte (*nextPID)(void);//busca el siguiente PID a ejecut
	struct SYSTEM_CALL{
		byte (*seekFile)(const char*,byte pid,byte *m);
	    struct FILE*(*getFileDescriptor)(void);
	    void (*createFile)(const char*,byte pid,byte *r,byte *m,byte s);
	    byte (*nextSYSCALL)(void);
	    void (*createSYSCALL)(byte (*process)(byte *m),char *name,byte size);//debug REvisar si se usa
	 }SYSCALL;
	struct _FIFOs_{
	   byte (*getNextPID)(void);
	   //struct _FIFO_1byte_ SCIsReady;//System CAll Interfaz fifo of ready processes
	   
	}fifos;
};// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


struct _HEAP_NODO{
   word index;//numero CRC del archivo en heap
   word *NumBlock1;//pointer al elemento del a tabla que apunta al bloque del archivo
};


struct _FileSystem{//LOGICAL FILE SYSTEM
   word Tabla_FCB[SIZE_CACHE_FILE_TABLE]; //FILE CONTROL BLOCK,guarda solo favoritos
   byte Mapa_bit_Block[SIZE_CACHE_BIT_BLOCKS];  //MbB FILE ORGANIZATION MODULE, guarda favoritos
   word Address_Table_Bit_Blocks;//direccion dela tabla de mapa de bits
   word Address_data;//direccon donde comienza los datos
   uint32 addresBitBlockMap;//direccion de inicio del mapa de bit guardado
   struct FILE files[O_FILE_MAX];//stack de archivo abiertos usandose
   struct _HEAP_NODO Heap[NUM_FILES];

};

struct _FileIO{
   struct FILE *(*fopen)(const char *s,const char *mode);	
   byte (*fclose)(struct FILE *f);
   byte (*fwrite)(const void *p,byte size,byte nelemts,struct FILE *f);//This functions write the specified amount of bytes to the binary file.
   byte (*fputs)(char *s,struct FILE *f);//
   char (*fputc)(char c,struct FILE *f); 
   byte (*fread)(void *p,byte size,byte nelemts,struct FILE *f);
   char (*fgetc)(struct FILE *f);
   char *(*fgets)(char *s,byte n,struct FILE *f);
   byte (*fseek)(struct FILE *f,word offset,byte origin);
   void (*rewind)(struct FILE *f);
};




//en so.timer[n].recursos[n]//cada timer tiene 4 recursos y hay 8 timers
//y el timer tiene un pide para saber a que pid esta asignado.
struct _SO_{ 
  struct _Recursos_ recurso;	 
  struct _FIFO_1byte_ TI1;	
  struct _FIFO_1Proceso_ fifoProc;//procesos FIFO	
  struct _UCProc BCP[PROC_MAX];//Bloque de control de Processors
 // struct _UCProc SCI[SCI_MAX];//Bloque de control de servicios/daemons
  struct _Timer timer[TIMERS_MAX];//timers que maneja el sistema operativo
  struct _Threading thread;
  struct _Memoria memo;
  struct _KERNEL kernel;
 //struct _FileSystem fs;//file system
  struct _EEPROM_D e2prom;
  byte Pstatus[PROC_MAX];//estatus de los procesos
  unsigned char TimerCntrl;//controla cual timer esta asignado
  byte currPID;//pid del proceso en ejecucion actualmente
  byte currPID_SYSCALL;//PID del process en execution del sistema daemon/service
 // unsigned short int BCPbits;//bits que determinan si estan en ready
  union _Status_{
  	  unsigned char statusByte;
        struct _Bit_{
  		  unsigned char inBooting:1;//estamos en Proceso de boot    
  		  unsigned char ProductChanging:1;//Se esta cambiando el Producto
  		  unsigned char TeachingProcess:1;//estamos en proceso de Teaching product
  		  unsigned char MonitoringSystem:1;//si se esta monitoreando el sistema es true
  		  unsigned char init_Signal_Proc:1;//initializar signal processing
  		  unsigned char activ_Signal_Proc:1;//activo el procesamisnto de la señal
  		  unsigned char warning_sys_mon:1;//hay un warning en el sys monitoring
  		  unsigned char comms_error:1;//error de comunicacion con tarjeta IO
            }bits;}status;
  union _Stats_{
	  unsigned char statusByte2;
		struct _Bt2_{
		  unsigned char Metal_Detection:1;//Se ha detectado un Metal   
		  unsigned char error_Monitoring:1;//for system monitoring errors(1.75 s)
		  unsigned char Serious_Error:1;// On: for serious errors 
		  unsigned char System_Call:1;//System Call Pendiente
		  unsigned char x4:1;//
		  unsigned char x5:1;//
		  unsigned char x6:1;//h
		  unsigned char x7:1;//
			}bit;}stat2;
                                  
};//fin de estructura de ssitema operativo--------------------------





/* version 110422-1119 */
struct  _SERIAL_{
	struct _FIFO_1byte_ rx;
	struct _FIFO_1byte_ tx;
	struct Serial_Bandera vars;
	struct IO_Bandera IO;
};//serial-------------------------------------------------------------

//struct FIFO_S{
//  signed short int *pos;//indice dela posicion actual de la FIFO
//  signed short int *pop;//indice de la posicion del primer elemento a sacar
//  signed short int *tail;//indice dela ultima posciion del array de la FIFO
//  unsigned char ncount; //cuanta cuantos libres hay
//  unsigned char isCenter;//esta centrado??
//};



struct _FIFO_func_{
	  unsigned char (*append)(unsigned char x,unsigned char y, unsigned char p);
	  unsigned char (*pop)(unsigned char *x,unsigned char *y, unsigned char *p);
	  unsigned char (*resetFIFOS)(void);//resetear todas las FIFOs Y arrays y registros
};//fin _FIFO_func_----------------------------------------

struct _FIFO_func2_{
	  unsigned char (*append)(signed short int x,signed short int  y);
	  unsigned char (*pop)(signed short int *x,signed short int  *y);
	  unsigned char (*resetFIFOS)(void);//resetear todas las FIFOs Y arrays y registros
};//fin _FIFO_func_----------------------------------------


struct _Recursos{
	 //struct _FIFO_func_  f;//funciones de la fifo de VFDprocesos
	 struct _FIFO_1byte_ VFDprocesos;//procesos que quieren el recurso
	 struct _FIFO_1byte_ VFD;//fifo de VFD 
	 struct _FIFO_1byte_ IO;//fifo de IO
	 unsigned char (*insertVFDProcess)(unsigned char proceso);
	 unsigned char (*solicitar)(unsigned char recurso,unsigned char PID);
	 	 	  void (*devolver)(unsigned char,unsigned char);
   	 unsigned char (*get_inst)(void);//obtener instancia
   	          void (*dev_inst)(unsigned char inst);//devolver instancia
	 struct{
		unsigned short int control;//VFD numero de registros,que guarda quien esta en la FIFO   
		unsigned short int control_IO;//IO numero de procesos/hilos a manejar
		unsigned char VFD_pid;//pid que ocupa el VFD
		unsigned char control_uchar;
		unsigned char char1[8];
		unsigned char asignado;//a quien es asigando el recurso de VFD
		unsigned char asignadoIO;//a quien es asiganado el recurso IO
	 }v;//variable
	
};//fin recursos ------------------------------------------
	

struct COMUNICACIONES_{//-----------------------------------------------
	struct IOUP_{   	
	unsigned char estado;//estado actual de maquina de estado de comando
   unsigned char buffer[SIZE_BUF_TFT];
//    unsigned char commandoActual;//comando procesando actualmente
//    unsigned char contextoActual; //contexto,Tab,Pantalla Actual controlando
//    void (*fTabActual)(unsigned char c);//funcion actual de la tabactual
   void (*fAck)(unsigned char c);//mandar acknowledge
   unsigned char aux1;
   struct _FIFO_1byte_ tx;
   unsigned char txBusy;//ocupado el Tx fifo
   
	    }IOUP;
	struct GOTIS_{
		unsigned char DDS_activado;//si entonces se manda stream de deteccion al DDS de Desktop	
   	    struct DDS_{
   	    	struct _Func_{
  	    	  unsigned char (*fpush)(signed short int x,signed short int);
   	    	  unsigned char (*fpop)(signed short int *x,signed short int *y);
   	    	   }f;
   	    	struct _FIFOS_{
   	    	  struct _FIFO_1byte_ xl;
   	    	  struct _FIFO_1byte_ xh;
   	    	  struct _FIFO_1byte_ yl;
   	    	  struct _FIFO_1byte_ yh;}fifos;}dds;
	    }gotis;
};//-------------------------------------------------------------



struct _DISPLAY_VFD_{
	struct _FIFO_1byte_ x;//parametro 1
	struct _FIFO_1byte_ y;//parametro 2
	struct _FIFO_1byte_ p;//parametro 3
	struct _FIFO_func_  f1;//funciones para guardar lo que se grafica
	struct _box_control{
		 unsigned char boxs[SIZE_BOXES];
		 unsigned char box0;
		 unsigned char box; 
		 unsigned short int timer;//se activa  por timer y resetea el array
	   }box;
	union _Byte5_{
	   	  		unsigned char bytes1;
	   	  		struct{
	   	  			unsigned char FIFOonReset:1;//Las FIFOS estan reseteadas?? osea que esan en ceros y desbilitadas, esto para cambiar de contexto
	   	  			unsigned char DDSon:1;//indica si borramos registro de datos repetidos de DDS
	   	  			unsigned char TxBuffOFF:1;//buffer de TX vacio, para saber que ya se transmitio todo
	   	  			unsigned char finit_VFD:1;//flag init VFD indica si ya se init el VFD comandos de inizializacion
	   	  			unsigned char finit_Menu:1;//flag init Menu, enciende e inicializa los menus y el primer menu en pantalla
	   	  			unsigned char BOX_enable:1;
	   	  			unsigned char VDF_busy:1;//se estan mandando comandos  o posiciones
	   	  		    unsigned char ADC_DATO:1;
	   	  		}b;
	   	  	  }bits;
	struct _Vars_{
		unsigned char nbytes;//bytes a emitir
		unsigned char dat[DATOS_SIZE];
		unsigned short int timer;
		unsigned short int timer_us;
		unsigned long  int timer_ms;
		unsigned char index;
		unsigned char recurso;//quien ocupa este recurso.
		
//		unsigned char estado;//estado de: init_VFD,
	   }v;
	
    		
};//fin display VFD----------------------------------------------




struct _CONTROL_IIC_{
    union _Byte11_{
		unsigned char ByteF;
		struct{
			unsigned char x0:1;//bandera de que ya se envio el buffer completo
			unsigned char x1:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char x2:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char x3:1;
			unsigned char x4:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char x5:1;
			unsigned char x6:1;
			unsigned char x7:1;//timercomms
		}bitG;
	  }Flags;
   struct _VAR19_{
		   unsigned char y0;	 
		   unsigned char y1;
	  }var;    	  
};

/* Estruectura de DAtos PRINCIPAL DE IIC*/
struct  _IIC_{
	struct _FIFO_1byte_ rx;
	struct _FIFO_1byte_ tx;
	struct  _CONTROL_IIC_ cntrl;
};//serial-------------------------------------------------------------

/* estructura de datos del manejo de los datos de entrada del
 * teclado-------------------------------------------------------------*/
struct _KEYPAD_ {
	struct _FIFO_1byte_ F;//fifo managment
 	struct __vars_{
		  unsigned char s;//estado dela maquina de estados de procesamiento;
		  unsigned char a0[SIZE_BUFFER5];
		  unsigned char a1[SIZE_BUFFER5];
 		  unsigned char ss;//estado dela maquina que controla que array se vacia ala fifo 
 		  unsigned char delay;//cuando se activa  el teclado char desechados antes de empezar a capturar 
 		  unsigned char i;//indice delos arrays que guardan los chars rapidos del keypad por int
 		  unsigned char c0;//keypad.v.c0 es el char anterior aceptado que se apreto
 		  unsigned char ncount;//es el numero de veces que vemos el char anterior en este actual ocacion
 		  unsigned char c;//guarda el ultimo char que representa la tecla presionada en el actual preocesamiento
 		  
 		  //unsigned short int timerkeypad;//running time after last rx byte
 		}v;//variables
 	struct{
		  unsigned char enable:1;//keyboard enable IRQ
		  unsigned char swap:1;//eleccion para escoger el arraya a llenar
		  unsigned char x2:1;//indica que uno de los dos array todavia no se vacia a la fifo y la interrupcion ya quiere cambiar de array, al que estamos usando, interrupcion mas rapida que la FIFO
		  unsigned char x3:1;
		  unsigned char x4:1;
		  unsigned char x5:1;
		  unsigned char x6:1;
		  unsigned char x7:1;
 		}b;//bits
};//fin --------------------------------------------------------------


/*para manejo de las fifos que guardan los pix fuera
 *  de rango para el zoom actual seleccionado en DDS menu*/
struct _PIX_{
	union{
	  signed short int usint;
	  struct _XH_{
		  unsigned char H;
		  unsigned char L;
	  }b;
	}u;
	
	struct _FIFO_1byte_ xL;//parametro 1
	struct _FIFO_1byte_ xH;//parametro 1
	struct _FIFO_1byte_ yL;//parametro 1
	struct _FIFO_1byte_ yH;//parametro 1
	struct _FIFO_func2_ f; 
    unsigned char pix[24][128];//192/8=24, x=24 registros hor, 128 lineas  o rows 
};


	
struct _Comando_DDS{//comando DDS para graficar en touch
	struct _PIX_ pix;
	union Bytex{
	   unsigned char Bnderas;
	   struct{
		   unsigned char DDS_Reload:1;//cuando se recarga el DDS por un zoom CON LOS valores anteriores guardadso de las detecciones
		   unsigned char Apagando:1;//le dice que nos vamos a salir del menu DDS, para que no mande datos del DDS en otro menu
	       unsigned char EventDDS:1;//nos dice que tenemos autorizacion para ejecutar graficacion en DDS		 
		   unsigned char DDS1_BORRAR:1;//Borrando de el DDS
		   unsigned char DDS1_TIMER:1;
		   unsigned char clean_Buffers:1;//para decirle cuando limpiar los buffers que guardan los datos de los puntos pintados en el DDS para repintrlos
		   unsigned char Aspect_Zoom:1;
		   unsigned char debug:1;
		   unsigned char Gain_AB:1;
	   }bit;
	}Bnderas;
	
	union Bytex2{
		   unsigned char Registro1;
		   struct{
			   unsigned char rePaintLines:1;//repintar lineas de ganacia
			   unsigned char isGraficable0:1;//la grafica anterior se grafico
		       unsigned char x2:1;//nos dice que tenemos autorizacion para ejecutar graficacion en DDS		 
			   unsigned char x3:1;//Borrando de el DDS
			   unsigned char x4:1;
			   unsigned char x5:1;//para decirle cuando limpiar los buffers que guardan los datos de los puntos pintados en el DDS para repintrlos
			   unsigned char x6:1;
			   unsigned char x7:1;
		   }b;//bit
		}b;
		
	
	
	struct _Display_{
		unsigned char flag_TX_Pixel; //timer para desplegar en DDS
		unsigned short int delay;//para manejo de delay por comando y timer de display VFD
	}display;
	
	struct _Cursores_Contrl{
		unsigned char status;
	}cursor;
	
	struct _TOUCHSCREEN_{
		signed short int bufferX[SIZE_BUFFER_DDS_FIFO];//buffer DDS fifo TRansmision Serial
		signed short int bufferY[SIZE_BUFFER_DDS_FIFO];//buffer y DDS FIFO Transmision Serial
		signed short int *headx; //head de buffer x
		signed short int *tailx; //head de buffer y
		signed short int *popx;  //pointer to pop from fifo
		signed short int *pushx; //
		signed short int *heady; //head de buffer x
		signed short int *taily; //head de buffer y
		signed short int *popy;  //
		signed short int *pushy;
	    unsigned char ncount;//numero de nodos ocupados en la fifo
	}touch;
	
	struct _SAVE_PIXELS_{
		signed short int xPixel[10];//SIZE_TEMP_PIXEL];
		signed short int yPixel[10];//SIZE_TEMP_PIXEL];
		signed short int *p[8];
#if(SIZE_TEMP_PIXEL<254)		
		unsigned char ncount;
#else   
	    unsigned short int ncount;
#endif	    
	}SaveTempPix; 	
		
//	unsigned char (*pop)(signed short int *x,signed short int *y);//pop function
//    unsigned char (*push)(signed short int x,signed short int y);
    unsigned char (*remove)(signed short int *x,signed short int *y);//quita el nodo primer en indicce 0
    unsigned char (*append)(signed short int x,signed short int y);//pone un nodo al final dela cola
    
    struct _DispayPixel_{//para pintar el pixel
    	unsigned char *pop,*popx,*popy;
    	unsigned char *push,*pushx,*pushy;
    	unsigned char *tail,*tailx,*taily;
    	unsigned char *head,*headx,*heady;
    	unsigned char buffX[2];//SIZE_DDS_PIXEL];
    	unsigned char buffY[2];//SIZE_DDS_PIXEL];
    	unsigned char pen[2];//SIZE_DDS_PIXEL];
#if(SIZE_DDS_PIXEL<254)
    	unsigned char ncount;
#else 
    	unsigned short int ncount;
#endif    	
    }pixel;
        
    struct _Repaint_ZOOM{
    	//signed short int xx[SIZE_DDS_ZOOM];
        //signed short int yy[SIZE_DDS_ZOOM];
    	unsigned char  xy[2][2];//SIZE_Y][SIZE_X];
    	unsigned char xy0[2][2];//SIZE_Y][SIZE_X];
    	unsigned short int ii;
    	unsigned char j,k,uy;
    	unsigned char ix,iy,b;
    	unsigned char x1[4],y1[41];//depurar
    	unsigned short int debug5;
    	unsigned short int debug6;
    }rePaint;    
}; //FIN ESTRUCTURA COMANDO DDS-----------------------------------------
/*  FIN DDS ESTRUCURA ********************************************++++*/
/*  FIN DDS ESTRUCURA ********************************************++++*/
/*  FIN DDS ESTRUCURA ********************************************++++*/

unsigned char FIFO_DDS_pop(unsigned char *dato);
void init_FIFO_DDS(void);
unsigned char isFIFO_DDS_empty(void);
unsigned char isFIFO_DDS_pixel_llena(void);
unsigned char isFIFO_DDS_pixel_empty(void);	
unsigned char FIFO_DDS_pixel_pop(unsigned char *x,unsigned char *y,unsigned char *pen);
void display_DDS_transmitter_Controller(void);
void init_FIFO_DDS_Display_Pixel2(void);
void init_FIFO_DDS_Display_Pixel2_v2(void);
unsigned char isFIFO_DDS_Display_Pixel_llena2(void);
unsigned char isFIFO_DDS_Display_Pixel_empty2(void);
unsigned char FIFO_DDS_Display_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char FIFO_DDS_Display_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char isFIFO_DDS_Display_Pixel_empty2_v2(void);
unsigned char *FIFO_DDS_next_X(unsigned char *p);
unsigned char isFIFO_DDS_Display_Pixel_empty2(void);
void init_FIFO_DDS_pixel(void);
void FIFO_DDS_pixels_push(unsigned char x,unsigned char y,unsigned char pen);
unsigned char FIFO_DDS_Display_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char isFIFO_DDS_Display_Pixel_llena2_v2(void);
unsigned char isFIFO_DDS_pixel_llena(void);
unsigned char isFIFO_DDS_Display_Pixel_empty_v2(void);
unsigned char isFIFO_DDS_pixel_empty(void);
unsigned char FIFO_DDS_Display_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);	
unsigned char FIFO_DDS_pixel_pop(unsigned char *x,unsigned char *y,unsigned char *pen);
void init_queues(void);
unsigned char FIFO_Display_DDS_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);
//unsigned char FIFO_Display_DDS_Pixel_push_v2(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char FIFO_Display_DDS_Pixel_pop2_v2(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char FIFO_Display_DDS_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char *FIFO_DDS_next_X(unsigned char *p);	
unsigned char isFIFO_DDS_Display_Char_empty2(void);
void init_FIFO_DDS_Display_Char2(void);
unsigned char FIFO_Display_DDS_Char_pop2(unsigned char *x,unsigned char *y);
unsigned char FIFO_Display_DDS_Char_push(unsigned char datox,unsigned char datoy);
unsigned char *gotonext(unsigned char *last,unsigned char *first,unsigned char *tail);
unsigned char FIFO_displayBox_pop(void);
void BarraDet_displayUINT_var(unsigned char posx,unsigned char posy,unsigned short int *usint);
unsigned char FIFO_displayBox_push(unsigned char box);
unsigned char FIFO_displayBox_isEmpty(void);
void FIFO_Display_DDS_Char_clean(void);
void FIFO_Display_DDS_Pixel_clean(void);
unsigned char FIFObox_nextLast(void);
void malloc_display_Box(void);
unsigned char FIFOboxChars_nextLast(void);
unsigned char FIFO_displayBoxChars_pop(unsigned char *x,unsigned char *y);
unsigned char FIFO_displayBoxChar_push(unsigned char x,unsigned char y);
void malloc_display_Box_Chars(void);
signed short int popPushFIFO_OFFSET(signed short int *ry,signed short int xn,signed short int yn);
void init_FIFO_OFFSET(void);
void init_FIFOs_TX_IOUP(void);
unsigned char getComand_FIFO_B(unsigned char n);
unsigned char pushFIFO_TX(unsigned char cmd,unsigned char *dato);
signed short int **next2(signed short int *inicio,signed short int *final,signed short int **pos);
//unsigned char pushFIFO_IO_TX(unsigned char *data,unsigned char size);
unsigned char isLoad_next(unsigned char **p);
void reset_pointer(void);
void pushFIFO_average_Offset(signed short int **pos,signed short int **pop,signed short int *tail,signed short int *ini,signed short int dato);
signed short int popFIFO_average_Offset(signed short int **pos,signed short int **pop,signed short int *tail,signed short int *ini);
unsigned char *next(unsigned char *inicio,unsigned char *final,unsigned char *pos,unsigned char *pop);
void init_FIFO_BARRA_NUMERO(void);
unsigned char FIFO_barraNum_push(unsigned char c);
void FIFO_barraNum_VFDposicion(unsigned char posx,unsigned char posy);
unsigned char  FIFO_barraNum_pop(void);
unsigned char isFIFO_num_Not_Empty(void);
void FIFO_barraNum_VFDserial_SendChar(unsigned char c);
void init_FIFO_CMD_DDS(void);
void init_FIFO_TX_serial_General(void);
void vaciarBuffer(unsigned char *p,unsigned char *p2,unsigned short int size);
void push_FIFO_TRANSMISION_serial_IO(unsigned char byte);
unsigned char xpop_FIFO_CMD_DDS(signed short int *x,signed short int *y);
unsigned char push_FIFO_CMD_DDS(signed short int x,signed short int y);
unsigned char pop_FIFO_TRANSMISION_serial_IO(unsigned char *status);
void reset_FIFO_serial_TX(void);
unsigned char search_NO_Repetido(signed short int x,signed short int y);
void init_FIFO_SaveTemp_pixel_DDS(void);
unsigned char pop_FIFO_save_Temp_pixel_DDS(signed short int *x,signed short int *y);
unsigned char push_FIFO__save_Temp_pixel_DDS(signed short int x,signed short int y);
unsigned char is_FULL_FIFO_save_Temp_pixel_DDS(void);
unsigned char is_Empty_FIFO_save_Temp_pixel_DDS(void);
void clean_repaint_Pixels_DDS(void);
unsigned char IIC_FIFO_push_RX(unsigned char);
//unsigned char IIC_FIFO_push_TX(unsigned char);
unsigned char IIC_FIFO_pop_RX(void);
//unsigned char IIC_FIFO_pop_TX(void);
unsigned char get_Paquete(unsigned char *p,unsigned char (*pop)(void));
unsigned char is_device(unsigned char (*pop)(void),unsigned char *n);
unsigned char is_Lenght(unsigned char(*pop)(void),unsigned char *n);
unsigned char is_App(unsigned char(*pop)(void),unsigned char *n);
unsigned char is_CMD(unsigned char(*pop)(void),unsigned char *n,unsigned char app);
void get_parametros(unsigned char (*pop)(void),unsigned char *p);
//void init_FIFO_IIC(void);
void init_FIFO_RX_serial_Keypad(struct _FIFO_1byte_ *s);	
unsigned char FIFO_general_1byte_push(unsigned char dato,struct _FIFO_1byte_ *s);
//unsigned char get_case_FIFO_general(struct _FIFO_1byte_ *s);
unsigned char FIFO_general_1byte_pop(unsigned char *dato,struct _FIFO_1byte_ *s);
void reset_FIFO_general_UChar(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int size);
void Testing_SO_Debug(void);
unsigned char vfd_FIFO_push(unsigned char x,unsigned char y,unsigned char p);
unsigned char vfd_FIFO_pop(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char vfd_FIFOs_RESET(void);
//void init_FIFO_General_1byte(struct _FIFO_1byte_ *s,unsigned char *h,unsigned char *t,unsigned short int size);
unsigned char dds_pix_pop(signed short int *x,signed short int *y);
unsigned char dds_pix_append(signed short int x,signed short int y);
unsigned char dds_pix_reset(void);
void init_System_Monitor(void);
void reset_sys_mon(void);
void reset_sys_mon2(void);
unsigned char test_FIFOS_VFD(void);
//unsigned char Recursos_Solicitud(unsigned char recurso,unsigned char pid);
//void Recursos_Devolver(unsigned char recurso1);
void insertVFDProcess(unsigned char process);
//unsigned char insertVFDProcess_(unsigned char process);
unsigned char Solicitar_Recurso(unsigned char Recurso,unsigned char pid);
//unsigned char ya_esta_en_la_FIFO_VFD(unsigned char pid);
//void get_parameters_to_process_PID(unsigned char *pid1,unsigned char *index,unsigned char pid);
//unsigned char sacar_de_la_FIFO_vfd(unsigned char pid);
//unsigned char insertarlo_en_Fifo_VFD(unsigned char pid);
//void Devolver_Recurso_VFD(unsigned char pid);
//unsigned char sacar_de_la_FIFO_IO(unsigned char pid);
//unsigned char insertarlo_en_Fifo_IO(unsigned char pid);
//unsigned char ya_esta_en_la_FIFO_IO(unsigned char pid);
void Devolver_Recurso(unsigned char recurso1,unsigned char pid);
//void init_FIFO_General_1Proc(struct _FIFO_1Proceso_ *s,struct _UCProc *h,struct _UCProc *t,unsigned char size);
//unsigned char FIFO_general_1Process_pop(struct _UCProc *dato,struct _FIFO_1Proceso_ *s);
//unsigned char FIFO_general_1Proc_push(struct _UCProc dato,struct _FIFO_1Proceso_ *s);
unsigned char buscar_timer_libre(void);
unsigned char  agregar_recurso_a_PID(unsigned char pid,unsigned char recurso);
unsigned char is_already_asigned(unsigned char pid);
unsigned char gestionar_de_Recursos_TIMER(unsigned char pid);
byte gestionar_Recursos_VFD(byte pid);
byte Seek_File_service(byte *memo);
byte exist(struct FILE *f);
struct FILE *Buscar_file_descriptor(byte status);
byte isvalidDescriptor(struct FILE *f);
byte get_CRC_from_name(const char *name,word *crc,byte *mem);
byte search_Heap_Index_CRC_Nombres(byte idFile,byte *m);
byte crearHeap(byte *memo);
byte Gestionar_Recurso_General(struct _FIFO_1byte_ *recurso,byte irec,byte pid);
void init_FIFO_General_1byte(struct _FIFO_1byte_ *s,unsigned char *h,unsigned char size);
byte proceso1(byte *mem);
byte proceso2(byte *mem);
//struct _Nodo_ *get_Nodo_Libre(void);

#endif /* QUEUE_H_ */
