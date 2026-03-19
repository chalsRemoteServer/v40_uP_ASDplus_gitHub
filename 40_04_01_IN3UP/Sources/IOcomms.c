/*
 * IOcomms.c
 *
 *  Created on: Mar 9, 2017
 *      Author: chals */
/*
 * IOcomms.c
 *
 *  Created on: Dec 5, 2016
 *      Author: chals
 *      
 *      
 *      PARIDAD DE COMUNICACION RS232 DE LA PROCESADORA A LA IO 
 *       ES DE  "EVEN" POR HARDWARE
 *       
 *       UART0 es para comunicarse con la tarjeta IO
 *       
 */



#include "IOcomms.h"
#include "PE_Types.h"
#include "IOUPserial.h"
#include "errorController.h"
#include "VFDmenu.h"
#include "I2C_.h"
#include "system.h"
#include "CTS_UART0.h"
#include "RTS_UART0.h"
#include "TI2.h"
#include "maths.h"
#include "queue.h"
#include "string.h"
#include "LED12_SYS.h"
#include "VFDkey.h"

//definiciones
//#define  SIZEIOBUFFRX   10
//#define  SIZEIOBUFFTX   10  //NO HACER MAYOR A 250 TAMAÑO HABRIA PROBLEMAS EN CODIGO uchar using compare del buffer de transmision


//variables globales internas
unsigned char *prx,*lastrx;
unsigned char RXdata[2];
extern unsigned  char buffer1[SIZE_BUFFER_TX];//del la transmision FIFO serial IOUP
extern unsigned char  buffer14[SIZE_BUFFER14];//buffer de la fifo de transmision de tx de serial a IOUP 
extern struct _SysMon sysmon,sysmon2;
extern struct _Menu_  menu;
extern struct _Recursos recurso;


//VARIABLES LIBRERIA PROCESADORA-IO
// version  1.0.3
//unsigned char isGotisEnable;//Software de recepcion de datos en la DeskTop
//unsigned char Requerimiento_en_Proceso;
//unsigned char EnableComunicationsTX;
unsigned char rxEnableFlag_IRQ;//hablita interrupcion de recepcion
//struct COMMUNICACION IO_TX;
extern unsigned char *popTX;//head y last de los array A y B
extern struct _Comando_DDS dds;
extern unsigned short int sizeof1(unsigned char *p,unsigned short int size);
extern struct _Error323_ e;//error controller data struct
extern struct COMUNICACIONES_ com;

//FIN DE VARIABLES PROCESADORA -IO

typedef union{
	signed short int sigshortint;
	unsigned char byte[2];
}SigShortInt;


//estructuras
//struct TX_BUFFER txbuff[SIZEIOBUFFTX]; //arreglo de estructuras para hacer la lista doblemente ligada 
//struct Tx_Control txCon; //control de transmision
//struct Rx_Control rxCon;//control de recepcion
//struct RX_BUFFER rxbuff[SIZEIOBUFFRX];
struct COMMs_IOUP *iuComms;
struct COMMs_IOUP FIFO_IUComms[SIZE_FIFO_IOUP];
//struct SALIDAS_UP  IOouts;
struct ENTRADAS_UP IOins;
extern struct _SERIAL_ serial;//extern struct _Vector_ serial;
struct _Error323_ sysMon;


void init_IOcomms(void){
//unsigned char i;
	      IOUPserial_TurnRxOff();//EnableComunicationsTX=FALSE;
		  rxEnableFlag_IRQ=FALSE;
		  serial.IO.Salidas.bitA.Gotis=FALSE;
		  //rxCon.paquete=RX_FREE;
		  //RTS_UART0_PutVal(TRUE);
		  RTS_UART0_ClrVal();//NO ready to send
		  //RTS_UART0_PutVal(FALSE);//al iniciarse debe estar apagado el RTS
		  init_FIFO_comms_IOUP(); //init fifo de communication con la board uproc
		  SemaforoTX(TRUE,'w');
		  //IOouts.state=BYTE_COMMAND;
		  //IOouts.ANS_DATOS=NONE;
//		  IO_TX.Flags.ByteFlags=0;//limpiamos bandera para transmitir
//		  IO_TX.Libres=SIZE_IO_TX_BUFFER;//nodos libres en la fifo
//		  IO_TX.Flags.bitFlag.freeTXx2=TRUE; //esta libre la transmision, HASTA QUE HAYAA UN PAQUETE
//          serial.appendDDS=push_FIFO_CMD_DDS;
/*          serial.popDDS=pop_FIFO_CMD_DDS;*/
          serial.tx.appendByte=FIFO_general_1byte_push;// push_FIFO_TRANSMISION_serial_IO;//agregar byte al buffer  de tx
          serial.tx.vaciarbuff=vaciarBuffer;
          serial.vars.Flags.bitF.FreeBuffTX=TRUE;
          dds.remove=xpop_FIFO_CMD_DDS;//apuntando a la funcion
          dds.append=push_FIFO_CMD_DDS;//apuntando a la funcion
          //serial.tx.resetFIFO=reset_FIFO_ser
}//fin init IO communications


void enable_Comms_TX(void){
	  IOUPserial_TurnRxOn();   
	  serial.vars.Flags.bitF.EnableComunicationsTX=TRUE;}
void enableIO_reciv(void){rxEnableFlag_IRQ=TRUE;}
void disableIO_reciv(void){rxEnableFlag_IRQ=FALSE;}


/* 
 *  version 300322-930     restructuracion completa*/
//void IOUP_BOARD_SERIAL_CONTROLLER(void){
	
//sysmon.i=3; IOUP_BOARD_TRANSMISSION_CONTROLLER();
//sysmon.i=4; pop_DDSpacket_to_FIFO_serial_TX_IOUP();//METER Comandos de dds al fifo de transmision
//}//fin del controlador principal de transmision de datos----------------------------------------





/* Transmite todo el buffer FIFO completo de espera de Serial, 
 *  debe ir antes de cada comado que mete a la fifo serial
 *  version  280322-1713     estructura de datos a serial.rx|tx|vars
 *  version  300322-919      nombre y reorganizacion general
 *  version  300322-924      cambia nombre del method
 *  version  310322-1012     cambia reenvio de ACK en vacio
 *  version  310322-1251     control de errores, led error. comms
 *  version  060422-1514     constante de aaCK, cambio de 14 a 2 seg
 *  version  070422-1042     estructiura de datos de error y control de ans
 *  p: pointer OCUPA 1 POINTERS  */ 
void xComunicaciones_Transmisor_a_IOUP_BOARD(void){
word n1;
static word count; //reset_FIFO_general_UChar
//static unsigned char ig;
const unsigned char TIME_ACK=3;//tiempo en segundos de mandar ACK
const unsigned char TIME_AWAIT=3;//TIMEPO DE espera antes de marcar ERROR, son vueltas,
static  unsigned char estado,dato;
static  unsigned char buff[SIZE_BUFFER14];//buffer14
static  unsigned char k;

  switch(estado){
   case 1:if(com.IOUP.tx.ncount>(SIZE_BUFFER14-5))estado++;break;
   case 2:estado++;break;
   case 3:if(IOUPserial_GetCharsInTxBuf()==0)estado++;else estado=0;break;
   case 4:estado++;k=0;count=com.IOUP.tx.ncount;break;
   case 5:if(com.IOUP.tx.popf(&dato,&com.IOUP.tx)){buff[k++]=dato;}else{estado++;}break;
   case 6:serial.vars.Flags.bitF.FreeBuffTX=FALSE;	      
   	   	  IOUPserial_SendBlock(&buff[0],count,&n1);
		  estado=0;break;
   default: estado=1;break;}
}//fin  IOUP_BOARD_TRANSMISSION_CONTROLLER------------------------------------




/*  Transmite comando de ggraficacion
 *  stx=03h,CMD=13h,xh,xl,yh,yl,ETX=02h
 *   S,C,1,2,3,4,E*
 *     version-1   */
//void pop_DDSpacket_to_FIFO_serial_TX_IOUP(void){
void xEmpaquetado_de_datos_Deteccion_Hacia_IOUP(void ){//6
static signed short int x,y;
const unsigned char SIZE_CMD=4; //tamao del comando 
static unsigned char bytes[8]={0x03,0x13,0x0A,0x0B,0x0C,0x0D,0xCC,0x02};
static unsigned char i;
static unsigned char estado;
const unsigned char SEMAFORO_EMP=0xC1;
static signed short int x0,y0;
const unsigned char PID_DDS_TX_2_IOUP9=PID9;
const unsigned char TAMAO_DE_COMANDO=4;
//static unsigned char buff[8];
  //com.IOUP.txBusy  deprecated
  if(com.gotis.DDS_activado){
   switch(estado){
	 case 1:if(com.gotis.dds.fifos.xl.ncount>5)estado++;break;
	 case 2:estado++;break;
	 case 3:if(xpop_FIFO_CMD_DDS(&x,&y))estado++;break;
	 case 4:if((x0==x)&&(y0==y))estado=0;else estado++;break;
	 case 5:if((x==0)&&(y==0))estado=0;else estado++;break;
	 case 6:estado++;x0=x;y0=y;break;
	 case 7:bytes[0]=STX;bytes[1]=CMD_DDS;
	 case 8:getBytes_from_SSInt(&bytes[2],&bytes[3],x);estado++; break;
	 case 9:getBytes_from_SSInt(&bytes[4],&bytes[5],y);estado++;break;
	 case 10:bytes[6]=getCheckSUM(CMD_IN3UP_GOTIS,&bytes[2],SIZE_CMD);
	 	 	 bytes[7]=ETX;estado++;i=0;break;
	 case 11:if(recurso.solicitar(_IO_,PID_DDS_TX_2_IOUP9))estado++;break;
	 case 12:if(com.IOUP.tx.appendByte(bytes[i],&com.IOUP.tx)){i++;}
	         if(i>7) estado++;
	         break; 
	 case 13:recurso.devolver(_IO_,PID_DDS_TX_2_IOUP9);estado=0;break;
	 default:estado=1;break;}}
}//fin comando  DDS meter a  buffer el paquete o los paquetes---------




/*  FIFO MANAGEMENT+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



void init_FIFO_comms_IOUP(void){
	
	    FIFO_IUComms[0].MaquinaEstados=NODO_FREE;
	    FIFO_IUComms[0].next=&FIFO_IUComms[1];
	    FIFO_IUComms[0].prev=&FIFO_IUComms[SIZE_FIFO_IOUP-1];
	  
	    //FIFO_IUComms[0].status=NODO_FREE;
	    FIFO_IUComms[SIZE_FIFO_IOUP-1].MaquinaEstados=NODO_FREE;
	    FIFO_IUComms[SIZE_FIFO_IOUP-1].next=&FIFO_IUComms[0];
	    FIFO_IUComms[SIZE_FIFO_IOUP-1].prev=&FIFO_IUComms[SIZE_FIFO_IOUP-2];
	    
	    //FIFO_IUComms[SIZE_FIFO-1].status=NODO_FREE;
	    for(iuComms=&FIFO_IUComms[1];iuComms<=&FIFO_IUComms[SIZE_FIFO_IOUP-2];iuComms++){
	    	//iuComms->status=NODO_FREE;
	        iuComms->MaquinaEstados=NODO_FREE;
	        iuComms->next=iuComms+1;
	        iuComms->prev=iuComms-1;
	        iuComms->MaquinaEstados=NODO_FREE;}//fin for
	        iuComms->next=&FIFO_IUComms[0];
	        iuComms->prev=&FIFO_IUComms[SIZE_FIFO_IOUP-2];
	        
	    
}//FIN de initzializatione de la FIFO comunication to processor



/* LIBRERIA  COMUNICACION IO-UP
 *  insertar un nodo en la cola de la FIFO
 * version  1.0.5 */
void insertFIFOcommsIOUp(struct COMMs_IOUP datos){
  static struct COMMs_IOUP *p;//debug quitar el static
  unsigned char j=0;
	p=iuComms;
	if(serial.vars.Flags.bitF.EnableComunicationsTX){	
		if(p->MaquinaEstados==NODO_FREE){
			p->MaquinaEstados=datos.MaquinaEstados;
			//p->ndatos=datos.ndatos;
			//for(i=0;i<datos.ndatos;i++)
		        //p->datos[i]=datos.datos[i];}
			p->xh=datos.xh;
			p->xl=datos.xl;
			p->yh=datos.yh;
			p->yl=datos.yl;}
		else{
l62:        if((p->MaquinaEstados==NODO_FREE)&&(p->prev->MaquinaEstados!=NODO_FREE)){//buscamos el ultimo nodo de la FIFO
				 //p=p->prev;
				 p->MaquinaEstados=datos.MaquinaEstados;
				// p->ndatos=datos.ndatos;
				 //for(i=0;i<datos.ndatos;i++)
                     //p->datos[i]=datos.datos[i];}
				 p->xh=datos.xh;
				 p->xl=datos.xl;
				 p->yh=datos.yh;
				 p->yl=datos.yl;}
			 else {p=p->prev;
				   if(j++<SIZE_FIFO_IOUP+10) //debug
					   goto l62;}  
		}}//fin else
}//insert FIFO comunications uProcessor--------------------------------------------------


struct COMMs_IOUP popFIFOcommsIOUp(void){//POP from the FIFO top
static struct COMMs_IOUP r,*p,*ptail;//debug quitar el static	
       p=iuComms;
       ptail=iuComms->prev;
       if(serial.vars.Flags.bitF.EnableComunicationsTX){
l145:      if(p->MaquinaEstados==NODO_FREE){
	         if(p==ptail){
	        	  iuComms->MaquinaEstados=NODO_FREE;
	        	  r.xh=iuComms->xh;r.xl=iuComms->xl;
	        	  r.yh=iuComms->yh;r.yl=iuComms->yl;
	        	  return r;}
	         else { p=p->next;
    	            goto l145;}}
           else{if(p->MaquinaEstados==READY){
        	         p->MaquinaEstados=NODO_FREE;
        	         r.xh=p->xh;r.xl=p->xl;
        	         r.yh=p->yh;r.yl=p->yl;
        	        return r;}
                }//fin else
       }//fin enable------------------------
}//end fin de la pop del FIFO de transmision, datos del ADC


/*insertOnTopFIFO_IOUPcomms  inserta con prioridad
 *    LIBRERIA PROCESADORAA  IO
 *      version  2.0.0    */
void insertOnTopFIFO_IOUPcomms(struct COMMs_IOUP data){
	
	 if(iuComms->MaquinaEstados==NODO_FREE){
		 //iuComms->ndatos=data.ndatos;
		 iuComms->MaquinaEstados=READY;//data.MaquinaEstados;
		 //for(i=0;i<iuComms->ndatos;i++)
			// iuComms->datos[i]=data.datos[i];
		 iuComms->xh=data.xh;
		 iuComms->xl=data.xl;
		 iuComms->yh=data.yh;
		 iuComms->yl=data.yl;
	    }//endif 
	 else{
		 if(iuComms->prev->MaquinaEstados==NODO_FREE){
			 //iuComms->ndatos=data.ndatos;
			 iuComms->MaquinaEstados=READY;//data.MaquinaEstados;
			 //for(i=0;i<iuComms->ndatos;i++)
			 	//		 iuComms->datos[i]=data.datos[i];}
			 iuComms->xh=data.xh;
			 iuComms->xl=data.xl;
			 iuComms->yh=data.yh;
			 iuComms->yl=data.yl;}
		 else{ 
			  eLog(CODE_ERROR_FIFO_FULL);}
	 }//endelse
}//end insert on top of FIFO communications



/*  END  FIFO MANAGEMENT+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++END FIFO MANAGEMENT+++++++++++++++++++++++++++++++++++++++++++++++++*/


/*  CONTROLADRES MAESTROS DE COMUNICACIONES PROCESADORA-ENTRADAS/SALIDAS++++++++++++
 *   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*  CONTROLADRES MAESTROS DE COMUNICACIONES PROCESADORA-ENTRADAS/SALIDAS++++++++++++
 *   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */




/*  INTERRUPT SERVICE ROUTINE 
 *     cada vez que recibe un caracter
 *     GUARDA EL DATO EN  una FIFO            
 *          version  5.0.10         */
void IOrecv_char_ISR(unsigned char c){
//struct COMMs_IOUP RXdata[];
static unsigned char i;	
  if(rxEnableFlag_IRQ){
	           if((c&0x80)==0x80)
	        	     IOins.byteH=c;
	           else  IOins.byteL=c;
        }//end flag
}//fin de recepcion de char de la IO INTERRUPT SERVICE ROUTIN----------------------------------


void MonitorCentralEntradas(void){
static unsigned char byteH0,byteL0;

       if(byteH0!=IOins.byteH){
    	   byteH0=IOins.byteH;
    	   if((byteH0&0x80)==0x80){   //1000 0000
              if((byteH0&0x40)==0x40)
                   IOins.Tacho=TRUE;
              else IOins.Tacho=FALSE;
              if((byteH0&0x20)==0x20)
    		       IOins.Gatin=TRUE;
              else IOins.Gatin=FALSE;
              if((byteH0&0x10)==0x10)
            	  IOins.SysCHK=TRUE;
              else IOins.SysCHK=FALSE;
              if((byteH0&0x08)==0x08)
            	  IOins.PackCHK=TRUE;
              else IOins.PackCHK=FALSE;
              if((byteH0&0x02)==0x02)
            	  IOins.ResetSW=TRUE;
              else IOins.ResetSW=FALSE;
    		  if((byteH0&0x01)==TRUE)
    			  IOins.Aux1IN=TRUE;
    		  else IOins.Aux1IN=FALSE; }}
       if(byteL0!=IOins.byteL){
    	   byteL0=IOins.byteL;
    	   if((byteL0&0x00)==0x00){
    		 if((byteL0&0x40)==0x40)
    			 IOins.gotis=TRUE;
    		 else IOins.gotis=FALSE;
    		 if((byteL0&0x20)==0x20)
    			 IOins.askdat=TRUE;
    		 else IOins.askdat=FALSE;
    		 if((byteL0&0x10)==0x10)
    			 IOins.LockSW=TRUE;
    		 else IOins.LockSW=FALSE;
    		 if((byteL0&0x08)==0x08)
    			 IOins.Aux2IN=TRUE;   //aqui falta poner los 3 bits de requerimientos de datos
    		 else IOins.Aux2IN=FALSE;}}
       
 
	
	
}//fin del monitor central de las entradas centrales de entrada-----------------------------------

//metodo para enviar los datos para graficas el Adc
/* Se manda llamar del Procesador central Digital de Seales y son los datos de 
 *  deteccion  */
unsigned char Comm_DDS_GOTIS_FIFO(signed short int datax,signed short int datay){
//const signed short int x[]={-11, 33,  55,  -7,  -9, 1111, 1313,-1515};
//const signed short int y[]={ 22,-44,   6,   8, -10, -12,  1414,   160};
//static signed short int j,h,v;
//static unsigned char i;	// FFF5 0016 0023 FFF8 FFF7 00FE 0463 F2A3
                        // 0016 FFA3 0004 0003 FFDE FFFB 00EA 00D5	
	// if(i>7) i=0;
    // datax=x[i];datay=y[i++];
    if(com.gotis.DDS_activado){
	    dds.append(datax,datay);//com.gotis.dds.f.fpush(); FIFO que envia los datos al serial IO-SERIAL 
	                            //para se derplegados en GOTIS APP.
	    
  }//------------------------------------------------------------------
}//fin del metodo de envio de datos para graficar--union------------------------------


//busca si esta repetido el valor que se quiere transmitir
unsigned char isRepeated(signed short int x,signed short int y){
const unsigned char SIZE11=200;
static signed short int xx[SIZE11];
static signed short int yy[SIZE11];
unsigned char i;
static unsigned char ii;

        for(i=0;i<SIZE11;i++)
        	 if(yy[i]==x)
        		 goto is339;
        goto is345;
is339:  for(i=0;i<SIZE11;i++)
         	 if(yy[i]==x)
         		 return TRUE;
is345:  if(ii>(SIZE11-1))//hace una cola circular
        	ii=0;
        xx[ii]=x; yy[ii++]=y;
        return FALSE; //la y no hay igual
}//fin is repeated 




unsigned char swapNibbles(unsigned char x){
	 return((x&0x0F)<<4 | (x&0xF0)>>4);
}//fin de swap nibbles--------------------------------------------------------

unsigned char  SemaforoTX(unsigned char s, char modo){
static 	unsigned char Semaforo_TX;
   if(modo=='w'){
	  if(s==TRUE) 
	    Semaforo_TX=TRANSMITED; 
	  else 
		Semaforo_TX=NO_TRANSMITED;
      return 0;}
   else
	   return Semaforo_TX;
}//


unsigned char isGotisEnabled(void){

	 return TRUE;//DEBUG
     if((RXdata[1]&0x80)==0x00){//es el byte correcto?
    	 if((RXdata[1]&0x40)==0x40)//is enabled?
    	         return TRUE;
    	 else return FALSE;}         
     else{ 
    	 if((RXdata[0]&0x80)==0x00){//entons se voltearon los bytes 
    		 if((RXdata[0]&0x40)==0x40)//is enabled?
    			 return TRUE;
    	     else return FALSE;}
    	 else{
    		  eLog(CODE_ERROR_COMM_IO_RX);}
         }//fin else	
return 0;     
}//is gotis enabed? -------------------------------------------------



/*bytes de Recepcion UP:  A,B
*                MSN(7)       6    5     4      3       2       1       0
*      bits de A:  --1-- ,TACHO,  Gatin, SysChk,PackCHk,ResetEn,ResetSW,AuXin
*      bits de B:  --0-- ,GOTIS,ASK-Dat,Lock-SW, Aux2IN,datReq0,datReq1,n/a
* 
*      datReq0   datReq1:
*         0           0:Solo valores ADC
*         0           1:parametros de display, sensibilidad, phase, altura, ganancia
*         1           0:Reporte,
*         1           1:Respaldo de Productos
*/
unsigned char  getANS_DATOS(void){
unsigned char c;
         if((RXdata[0]&0x80)==0x00){
              c=RXdata[0]&0x06;  //0000 0110
        	  goto swloop;} 
         else{if((RXdata[1]&0x80)==0x00){
        	     c=RXdata[0]&0x06;  //0000 0110
        	     goto swloop;} 
              else{
            	  eLog(CODE_ERROR_COMM_IO_RX);
                  return 0; }}
swloop:   switch(c){
              case 0x00:return 0x00; //IOouts.ANS_DATOS=0x00;  //0000 0000
            	        break;
              case 0x02:return 0x02; //IOouts.ANS_DATOS=0x02;	  //0000 0010
            	        break;
              case 0x04:return 0x04; //IOouts.ANS_DATOS=0x04;	  //0000 0100
            	        break;
              case 0x06:return 0x06; //IOouts.ANS_DATOS=0x06; 	  //0000 0110
                        break;
              default:eLog(CODE_ERROR_0006);
            	      break;
           }//endswithc         
	
}//fin get answer datos--------------------------------------------



/*  fin CONTROLADRES MAESTROS DE COMUNICACIONES PROCESADORA-ENTRADAS/SALIDAS+++++++++++++++++++++++++++++++++
 *   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  fin CONTROLADRES MAESTROS DE COMUNICACIONES PROCESADORA-ENTRADAS/SALIDAS+++++++++++++++++++++++++++++++++
 *   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  fin CONTROLADRES MAESTROS DE COMUNICACIONES PROCESADORA-ENTRADAS/SALIDAS+++++++++++++++++++++++++++++++++
 *   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */





/*Insercion de un numero proveniente del procesamiento del
 * ADC para ser transmitido al  Goptix    
 * lo mete a unas fifos y una maquina de de interrupciones los va transmitiendo
 *  con su control de protocolo, claro  
 *  transmite la informacion del DSP ala IO */
void Tx_DSP2IO(signed short int x1,signed short int y1){   
struct COMMs_IOUP nodo;	
sword16 sWord; 
	//signed short int x2=(signed short int)0x1234 ,y2=(signed short int)0x5678;//debug	
 if(serial.vars.Flags.bitF.EnableComunicationsTX){	 
	if(serial.IO.Salidas.bitA.Gotis==TRUE){ //las transmisiones al IO estan activadas? 
			nodo.MaquinaEstados=READY;//esta listo el nodo para ser transitido
			//nodo.datos[0]=READY;
			sWord.coord16=x1;
			nodo.xh=sWord.byte[_HI_];
			nodo.xl=sWord.byte[_LO_];
			sWord.coord16=y1;
		    nodo.yh=sWord.byte[_HI_];
			nodo.yl=sWord.byte[_LO_];
			//nodo.ndatos=5;
		    insertFIFOcommsIOUp(nodo);}}
	        
}// transmision to IO tarjeta------------------------------------------------------------






/*************************++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     CONTROLES DE SALIDAS Y ENTRADAS DE HARDWARE A NIVEL NUCLEO++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

unsigned char PIN_CTS_UART0(void){
        //if(((unsigned char)(getReg8(SETUA) & 0x08))==0x08)
	    if((CTS_UART0_GetVal() & 0x08)==0x08)
        	return TRUE;
        else return FALSE;
        
}//fin  read CTS  port from the UART0 belonging to UP-IO  control comunications


//Description	Resource	Path	Location	Type
//fined : "Rechazo_Salida" Referenced from "detectionAnalisis" in	39_16_49_IN1UP		 	C/C++ Problem


/* deteccion es la deteccion en tiemo real en cada instante
 * y con timers y controladores activa el tren de 
 * rechaos que se guardan en la fifo, aqui se van 
 * guardar en la fifo, y en tiempo real el 
 * sistema operativo los va a ir sacando.
 * */
void ReleRechazo(unsigned char status){
unsigned char dato[7];//={STX,0x05,CMD_RELE_FALLA_FLASH,1,status&0x7F};	
unsigned char crc;

	if((status&0x80)==0x80){//comando flash de testing prueba menu
		 RTS_UART0_PutVal(FALSE);
		 serial.IO.Salidas.bitA.Rechazo=0;
		 dato[0]=STX;dato[1]=0x05;//lenght
		 dato[2]=CMD_RELE_RECHAZO_FLASH;//comando de flash falla
		 dato[3]=2;// Numero de pulsos flash
		 dato[4]=status&0x7F; //sacamos la duracion delos pulsos
		 serial.tx.appendByte(dato[0],&serial.tx);
		 serial.tx.appendByte(dato[1],&serial.tx);//len
		 serial.tx.appendByte(dato[2],&serial.tx);//COMANDO RELLE ALARMA ON
		 serial.tx.appendByte(dato[3],&serial.tx);//solo 1 flash
		 serial.tx.appendByte(dato[4],&serial.tx);//duracion 1500mseg
		 crc=getCheckSUM(dato[2],&dato[3],2);
		 serial.tx.appendByte(crc,&serial.tx);//CRC
		 serial.tx.appendByte(ETX,&serial.tx);
	     RTS_UART0_PutVal(TRUE);}
	else{serial.IO.Salidas.bitA.Rechazo=status&0x01;//filtramos y vaciamos estado
		 RTS_UART0_PutVal(serial.IO.Salidas.bitA.Rechazo);}
}//funcione de encapsulacion para de manejo de salidas de la Tarjeta IO




        
void ReleFalla(unsigned char status){
unsigned char dato[7];//={STX,0x05,CMD_RELE_FALLA_FLASH,1,status&0x7F};
unsigned char crc;
     dato[0]=STX;dato[1]=0x05;//lenght
     dato[2]=CMD_RELE_FALLA__FLASH;//comando de flash falla
     dato[3]=1;// Numero de pulsos flash
     dato[4]=0x32; //duracion delos pulsos
	 if((status&0x80)==0x80){
		 serial.tx.appendByte(dato[0],&serial.tx);
	  	 serial.tx.appendByte(dato[1],&serial.tx);//len
	 	 serial.tx.appendByte(dato[2],&serial.tx);//COMANDO RELLE ALARMA ON
		 serial.tx.appendByte(dato[3],&serial.tx);//solo 1 flash
		 serial.tx.appendByte(dato[4],&serial.tx);//duracion 1500mseg
		 crc=getCheckSUM(dato[2],&dato[3],2);
		 serial.tx.appendByte(crc,&serial.tx);//CRC
		 serial.tx.appendByte(ETX,&serial.tx);
		 serial.IO.Salidas.bitA.Fail=0;}
	 else{		   
		 serial.IO.Salidas.bitA.Fail=status;
		 if(status){serial.tx.appendByte(STX,&serial.tx);
					serial.tx.appendByte(0x03,&serial.tx);//len
					serial.tx.appendByte(CMD_RELE_FALLA_ON,&serial.tx);//COMANDO RELLE ALARMA ON
					serial.tx.appendByte(CMD_RELE_FALLA_ON_CRC,&serial.tx);
					serial.tx.appendByte(ETX,&serial.tx);}
		 else{serial.tx.appendByte(STX,&serial.tx);
			  serial.tx.appendByte(0x03,&serial.tx);//len
			  serial.tx.appendByte(CMD_RELE_FALLA_OFF,&serial.tx);//COMANDO RELLE ALARMA ON
			  serial.tx.appendByte(CMD_RELE_FALLA_OFF_CRC,&serial.tx);
			  serial.tx.appendByte(ETX,&serial.tx);  }}
}//fin ReleFalla-------------------------------------------

void ReleAlarma(unsigned char status){
	    serial.IO.Salidas.bitA.Warnning=status;}
void ReleBloqueo(unsigned char status){
	    serial.IO.Salidas.bitA.Lock=status;}
void SalidaPHVF(unsigned char status){
	    serial.IO.Salidas.bitA.PFVE=status;}
void SalidaAux(unsigned char status){
	    serial.IO.Salidas.bitA.Aux_out=status;}


unsigned char getTacho(void){return serial.IO.TACHO;  }
unsigned char getGatin(void){return serial.IO.Ent.bitB.GATING;  }
unsigned char sysCHK(void)  {return serial.IO.Ent.bitB.SYS_CHK; }
unsigned char packCHK(void) {return serial.IO.Ent.bitB.PACK_CHK;}
unsigned char Aux1IN(void)  {return serial.IO.Ent.bitB.AUX1; }
unsigned char Aux2IN(void)  {return serial.IO.Ent.bitB.AUX2; }
unsigned char LockSW(void)  {return serial.IO.Ent.bitB.LockSW; }
unsigned char ResetSW(void) {return serial.IO.Ent.bitB.ResetSW;}



void IN3UP_test(void){
//static unsigned long int count;
//const unsigned char STX=0x03,ETX=0x02;
//const unsigned char CMD_DDS=0x13;
//const unsigned char SIZE_CO=14;
//word n;
//unsigned char a[]=" Hola ely ";
//unsigned char b[]={STX,CMD_DDS,0x32,0x00,0x32,0x00,0x88,ETX};
//unsigned char c[]={"123456789"};
//signed short int x[SIZE_CO]={-301,500,32,15,-25,99,-88,192,102,-56,-43,45,22,-66};
//signed short int y[SIZE_CO]={ 22,13,-123,44,-12,-34,78,-73,22, 43, -22,34,12,56};
//unsigned char d[8];
//unsigned char h,l;
//static unsigned char i;
//    if(++count>4825){//con 50000 se envian cada 2.5seg
//    	count=0;
//    	if(i>SIZE_CO)
//    		i=0;
//    	d[0]=STX;d[1]=CMD_DDS;
//    	convert_SI_byte(&h,&l,x[i]);
//    	d[2]=h;d[3]=l;
//    	convert_SI_byte(&h,&l,y[i++]);
//    	d[4]=h;d[5]=l;
//    	d[6]=getCRC3(&d[1],6);
//    	d[7]=ETX;
//    	IOUPserial_SendBlock(&d[0],sizeof(d),&n);
//    }
}//fin IN3UP test ------------------------------------------------

/* cnversion de signed int a byte dvidiendo el valor a 2 bytes */
void convert_SI_byte(unsigned char *h,unsigned char *l,signed short int a){
union _valor_{
  signed short int signo;
  unsigned char   byte[2];
}v;
        v.signo=a;
        *h=v.byte[0];
        *l=v.byte[1];
}//fin conversion de SIGNED INT  a bytes--------------------------

/* obtener el CRC, parameter pointer de los parametros
 * y comando, n: es e numero de bytes desde el primero*/
unsigned char getCRC3(unsigned char *p,unsigned char n){
unsigned char i,suma=0,ret;
	for(i=0;i<n;i++){
		 suma+=*(p+i);}
	ret=((unsigned char)~suma);	
return ret;
}//fin get CRC ----------------------------



/// nos regresa el numero de byet a enviar en funcion del comando contando el comando
unsigned char getLenghtCMD(unsigned char comando){
unsigned char ret;	
      comando&=Mask_CMD;  
      switch(comando){
    	  case CMD_GOTIS_XY: ret=5;//<stx><cmd><xl,xh,yl,yh,><etx>
    	                     break;
    	  default:ret=0;break;
      }	//fin de switch-------------------------
	      
return ret;
}//fin de obtener numero de bytes de un comando a enviar.





/*prcesar los bytes que llegron en la recepcion serial para
//ejecutar comandos los procesamos de manera operativa
//protocolo
//  STX,LEN,CMD,PARAM0,..PARMn,CRC,ETX
//   version 3 */
//void IOUP_RECEPTION_COMUNICATIONS_CONTROLLER(void){
//static unsigned char estado,estado0,cmd,len;	
//unsigned char a;	
//static unsigned char (*fp)(unsigned char,unsigned char,unsigned short int);//comando a procesar
//static unsigned short int sum;//para el crc
//static unsigned char i;
//word n;
//IOUPserial_TError e;
//const BUFF_SIZE =50;
//static unsigned char  buff[BUFF_SIZE];
////static unsigned char  buff2[BUFF_SIZE];
//
//#if (SIZE_BUFFER1>30)
//  #define LIMIT 15
//#else
//  #define LIMIT SIZE_BUFFER1-(SIZE_BUFFER1/2)
//#endif
//	
//   if(!estado){  
//    if(IOUPserial_GetCharsInRxBuf()>15){
//       switch(IOUPserial_RecvBlock(&buff[0],sizeof(buff),&n)){
//    	  case ERR_OK:estado=1;break;
//    	  case ERR_SPEED://This device does not work in the active speed mode
//    	  case ERR_RXEMPTY://The receive buffer didn't contain the requested number of data. Only available data has been returned.
//    	  case ERR_COMMON://common error occurred (the GetError method can be used for error specification)
//    		              IOUPserial_GetError(&e);
//        		          if(e.errName.OverRun){
//    		            	        IOUPserial_TurnRxOff();
//    		            	        estado=1;}
//						  else{if(!(e.err)){
//    		            	        estado=1;}//__asm(nop);} 
//    		                   else{if(e.errName.OverRun){
//   		            	                  IOUPserial_TurnRxOff();
//   		            	                  estado=1;}
//    		                        else{__asm(nop); //revisar porque se para aqui
//										__asm(Halt);//cuando grafica lineas en DDS
//										__asm(nop);}}}//PONER ESTE ERROR EN LOS LEDS
//    	                  break;}}}//fin if------------------------------------
//   
//    switch(estado){
//      case 0:enable_Comms_TX();break;
//	  case 1:i=0;estado++;break;
//lx4c: case 2:if(i<BUFF_SIZE){
//				 if(buff[i]!=STX){
//					  if(++i<BUFF_SIZE)
//							  goto lx4c;
//					  else {estado=0;}}
//				 else{cmd=buff[i];
//					  estado++;}}
//			 else{estado=0;}
//             break;
//	  case 3:if(++i<BUFF_SIZE){
//				  len=buff[i];
//				  if(len>0){
//					   cmd=0;sum=0;
//					   estado++;}
//				  else{if(++i<BUFF_SIZE)
//				    	   estado=0;
//				       else estado=2;}}
//			 break;
//	  case 4:if(++i<BUFF_SIZE){
//		           a=buff[i];
//	               if(cmd==0){//estmos en byte comando
//	            	   sum=cmd=a;//para guardar el tercer byte del paquete
//	            	   fp=Buqueda_de_Comandos_RX(cmd);//comndo a procesar,regresa apuntador a funcion de comando
//	            	   if(is_Longitud_wrong(cmd,len)){
//	            	            estado=2;}
//	            	   len--;
//	            	   break;}
//	               else{sum+=a;
//	                    fp(len,a,sum);}
//	               if(!(--len))
//	                    estado=2;}
//	  	  	  else estado=2;
//	          break;
//	  default:estado=0;break;}
//}//fin IOUP_RECEPTION_COMUNICATIONS_CONTROLLER----------------------------------



// version:28-03-22-1448
unsigned char is_Longitud_wrong(unsigned char cmd,unsigned char len){
	
	switch(cmd){	 
	case CMD_RELE_FALLA_ON:      //      0x14//03 03 14 13 02
	case CMD_RELE_FALLA_OFF:     //0x15//03 03 15 E4 02
	case CMD_RELE_ALARMA_ON:     //0x16//03 03 16 E3 02
	case CMD_RELE_ALARMA_OFF:    //0x17//03 03 17 E2 02
	case CMD_RELE_LOCK_ON:      // 0x18//03 03 18 E1 02
	case CMD_RELE_LOCK_OFF:      //0x19//03 03 19 E0 02
	case CMD_TX_ACK_UPIO:
		                   if(len==0x03)
		                	    return FALSE;//isnt wrong
		                   break;
            //cc: CRC cuenta apertir del comando  //3,5,1A,32,1E,2 Delay:5seg,duration:3seg
	case CMD_RELE_FALLA__FLASH:
	case CMD_RELE_ALARMA_FLASH:
    case CMD_RELE_LOCK_FLASH:
	case CMD_RELE_REJECT_CONFIG: // 0x1A//03 05 1A aa bb cc 02  aa:rechazo en 255*100=25.5segundos  bb:delay
                                if(len==0x05)
                                	 return FALSE;//nothing is wrong
                                break;
	
	default:return TRUE; break;	}//fin switch, true somthing is wrong
return TRUE;	
}//



/*busqued de comandos de recepcion, 
 param c. numero de comando num hexadecimal
*  regresa un appuntador a una funcion con parametro uchar
*  // version:28-03-22-900 */
unsigned char (*Buqueda_de_Comandos_RX(unsigned char c))(unsigned char,unsigned char,unsigned short int){
	
	  switch(c){//				    return Commando_Rele_Falla_Flash; break;
	   case CMD_RELE_FALLA_ON:  //return Comando_Rele_Falla_ON;  break;
	   case CMD_RELE_FALLA_OFF: //return Comando_Rele_Falla_OFF; break;
	   case CMD_RELE_ALARMA_ON: //return Comando_Rele_Alarma_ON; break;
	   case CMD_RELE_ALARMA_OFF://return Comando_Rele_Alarma_OFF;break; 
	   case CMD_RELE_LOCK_ON:   //return Comando_Rele_Lock_ON;   break;
	   case CMD_RELE_LOCK_OFF:  //return Comando_Rele_Lock_OFF;  break; 
	   case CMD_RELE_REJECT_CONFIG:// return Comando_Rele_Reject_Conf;  break;
	   case CMD_RELE_ALARMA_FLASH: // return Commando_Rele_Alarm_Flash; break;
	   case CMD_RELE_LOCK_FLASH:   // return Commando_Rele_Lock_Flash;  break;
	   case CMD_RELE_FALLA__FLASH://return Commando_Rele_Falla_Flash; break;
	   case CMD_TX_ACK_UPIO: return Commando_TX_ACK_UPIO;break;
                                  break;
	   default: return 0;} 
}//fin Buqueda_de_Comandos_RX------------------------------


/*    3   3-> 1   2   3
 *    3h  3h 14h 13h  2h
 *   STX,LEN,CMD,CRC,ETX 
 *   para c. indica que posicion de byte parametro es
 *   d: es el byte del parametro valor
 *    version 070422-1201*/
unsigned char Commando_TX_ACK_UPIO(unsigned char c,unsigned char d,unsigned short int s){
	s=0; 
	switch(c){
	   case 2:if(d==CMD_TX_ACK_UPIO_CRC){
		                serial.vars.var.ncountNopeAns=0;//hay respuesta
		                //serial.vars.Flags.bitF.ledComunicaciones=0;
//		                e.ERROR.bits.aLed4_Comm_Warnning_Sys=0;
	                 return TRUE;}//COMAndo ejecutado just fine
	          else return FALSE;         
		      break;
	   case 1:return TRUE;
	   default:break;}
}//fin Comando_Rele_Falla_ON---------------------------------


/*se interrupe cada segundo*/
void IRQ_comunicacion_IOUP(void){
//static unsigned char ncount1;	
//const unsigned char TIEMPO_MONITOR=5;//TIEMPO para declarar no comunicaciones
       
//      if(ncount1++>TIEMPO_MONITOR){
//    	   ncount1=0;
//    	   serial.vars.Flags.bitF.ledComunicaciones=ERROR_RECEPCION_ACK_IO;}
//      
	
}//fin IRQ_comunicacion_IOUP------------------------------------



/*Transmision de acknowledge hacia la IO la IO manda el akw
 * y en la UP  se pone la bandera en cero y vuelve a mandar el Ack
 *  si no recive ack, un led es mas lento
 *   verison 280322-1520 */
void Transmission_Acknowledge(void){
	 

	    serial.tx.appendByte(STX,&serial.tx);
	    serial.tx.appendByte(0x03,&serial.tx);
	    serial.tx.appendByte(CMD_TX_ACK_UPIO,&serial.tx);
	    serial.tx.appendByte(CMD_TX_ACK_UPIO_CRC,&serial.tx);
	    serial.tx.appendByte(ETX,&serial.tx);
	
}//fin Transmission_Acknowledge-------------------------------


void init_IOUP_comms(void){//---------------------------------
	
//	com.gotis.TabActual=0;
//	com.IOUP.fTabActual=Master_Monitor;//monitorea el comando que se inicia remotamente para darle ejecucion en la master tarjeta
//	com.IOUP.commandoActual=0;
	IOUPserial_EnableEvent();
}//fin init recepcion IOUP -----------------------------------



//PROCESAMIENTO EN VIVO DE LA RECEPCIO DE LA IO++++++++++++++++++++
void IOUPserial_Recepcion_Procesamiento(unsigned char c){
auto unsigned char estado;	
const unsigned char _RESET_=0x80U;   

    estado=com.IOUP.estado;
    switch(estado){
	   case COMANDO:if(c==STX)estado=PARAM1;break; //STX,'
  	   case PARAM1:com.IOUP.buffer[0]=c;
		           switch(c){
			            	 case CMD_GOTIS_INICIO_APP://03h,44h,CEh,02h
			            		       com.IOUP.aux1=CMD_GOTIS_INICIO_APP_CRC;
			            		       estado=CRC_DDS;break;
			            	 case CMD_GOTIS_SEND_DDS://03h,45h,CFh,02h   START
			            		       com.IOUP.aux1=CMD_GOTIS_SEND_DDS_CRC;
			            		       estado=CRC_DDS;break;                                                                                         ;
			            	 case CMD_GOTIS_STOP_DDS://03h,46h,D0h,02h
			            		       com.IOUP.aux1=CMD_GOTIS_STOP_DDS_CRC;
			            		       estado=CRC_DDS;break;
			            	 case CMD_KEYPAD_CNTRL_UP://03,47,B5,02
			            		 	  com.IOUP.aux1=CMD_KEYPAD_CNTRL_UP_CRC;
			            		 	  estado=CRC_DDS;break;
			            	 case CMD_KEYPAD_CNTRL_DN://03,48,B4,02
			            		      com.IOUP.aux1=CMD_KEYPAD_CNTRL_DN_CRC;
			            		      estado=CRC_DDS;break;
			            	 case CMD_KEYPAD_CNTRL_RT://03,49,B3,02
			            		      com.IOUP.aux1=CMD_KEYPAD_CNTRL_RT_CRC;
			            		 	  estado=CRC_DDS;break;
			            	 case CMD_KEYPAD_CNTRL_LF://03,4A,B2,02
			            		 	  com.IOUP.aux1=CMD_KEYPAD_CNTRL_LF_CRC;
									  estado=CRC_DDS;break;
			            	 case CMD_KEYPAD_CNTRL_EN://03,4B,B1,02
									  com.IOUP.aux1=CMD_KEYPAD_CNTRL_EN_CRC;
									  estado=CRC_DDS;break;
			            	 case CMD_SYSTEM_RESET:   //03,4C,B0,02
			            		      com.IOUP.aux1=CMD_SYSTEM_RESET_CRC;
			            		      estado=CRC_DDS;break;
			            	 default:estado=COMANDO;break;}
			         break;//param1
	    case CRC_DDS:switch(c){ //este comando no responde porque responde con graficacion masiva
				            case CMD_GOTIS_INICIO_APP_CRC:
				            	     if(c==com.IOUP.aux1)
				            	          com.IOUP.fAck(CMD_GOTIS_INICIO_APP_CRC);break;
				            case CMD_GOTIS_SEND_DDS_CRC:
				            	     if(c==com.IOUP.aux1){com.gotis.DDS_activado=TRUE;}break;
				            case CMD_GOTIS_STOP_DDS_CRC:
				            	     if(c==com.IOUP.aux1){com.gotis.DDS_activado=FALSE;}break;
				            case CMD_KEYPAD_CNTRL_UP_CRC:
				            		 if(c==com.IOUP.aux1){PANTALLA_DDSkeyUP_v2();}break;
				            case CMD_KEYPAD_CNTRL_DN_CRC:
				            		 if(c==com.IOUP.aux1){PANTALLA_DDSkeyDN_v2();}break;
				            case CMD_KEYPAD_CNTRL_RT_CRC:
				            		 if(c==com.IOUP.aux1){PANTALLA_DDSkeyRT_v2();}break;
				            case CMD_KEYPAD_CNTRL_LF_CRC:
				            		 if(c==com.IOUP.aux1){PANTALLA_DDSkeyLF_v2();}break;
				            case CMD_KEYPAD_CNTRL_EN_CRC:
				            	     if(c==com.IOUP.aux1){PANTALLA_DDSkeyEN_v2();}break;
				            case CMD_SYSTEM_RESET_CRC:
				            	     if(c==com.IOUP.aux1){setReg8(ResetController_RCR, _RESET_);}break;
				            default: estado=COMANDO;
				                     break;}    
			           estado=COMANDO;
			           break;//fin CRC DDS--------------------------------------
			 default:estado=COMANDO; 
			         break;}//fin switch 
	com.IOUP.estado=estado;	 
}//fin de menu de TFT touch isr--------------------



/*  CONTROLADOR PRINCIPAL DE LA COMUNICACION CENTRAL MAESTRA
 *  DE LA TRANSMISION DE DATOS HACIA LA TARJETA IOUP **/
void Transmision_Consola(void){
	 xComunicaciones_Transmisor_a_IOUP_BOARD();
     xEmpaquetado_de_datos_Deteccion_Hacia_IOUP();
}//fin de la comunicacion serial al la tarjeta IOUP board--------------



