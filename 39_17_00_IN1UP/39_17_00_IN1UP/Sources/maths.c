/*
 * maths.c
 *
 *  Created on: Mar 5, 2020
 *      Author: desarrollo-1
 */


#include "maths.h"
#include "system.h"
#include "queue.h"
#include "DSP.h"
#include "VFDisplay.h"


//extern unsigned char phase,phasefrac;//phase de ajustes de deteccion la variable fraccionaria es de la misma vriable
extern struct _GLOBAL_CONTEXTO gxc,gxc2,gxc3,gxc4;
extern struct _Comando_DDS dds;
extern struct ZoomControl zoom;
extern struct _Detection Deteccion;

/* se va aprocesar la variable phase sumar o restar
 *  el valor de 0.1 grados */
void procesarPhase(unsigned op){
    
     if(op==SUMAR){//sumar 0.1 a la phase
    	 if(++Deteccion.phasefrac>9){
    		 Deteccion.phasefrac=0;
    		 if(++Deteccion.phase>180){
    		        Deteccion.phase=0;
    		        Deteccion.phasefrac=0;}}}
     else{if(--Deteccion.phasefrac==0xFF){
    	    Deteccion.phasefrac=9;
    	    if(--Deteccion.phase==0xFF){
    	    	Deteccion.phase=180;
    	        Deteccion.phasefrac=0;}}}
}//fin procesarPhase----------------------------------------



/*op: SUMAR | RESTAR  operacion de las variables
 * digito es el digito que se le va hacer la operacion   000.0
 * digito 5 es el milenio                                ^^^^^
 *                                              digito-->54321
 * */
void operacionPhase(unsigned char digito,unsigned char op){
unsigned char phase,phasefrac;
	//getcharFromUChar(phase,&a[0]);
    phase=Deteccion.phase;
    phasefrac=Deteccion.phasefrac;
	switch(op){
		case RESTAR:if((phase==0)&&(phasefrac==0)){
			            	  phase=179;
			            	  phasefrac=9;
		                      break;}
					if(phase==0){
							  phase=179;
							  phasefrac=9;
							  break;}
		            if(digito==1){
			            if(phasefrac==0){
		                       phase--;
			                   phasefrac=9;}
			            else phasefrac--;
		                break;}
		            if(digito==3){
		            	 phase--;
		            	 break;}
		            if(digito==4){
		                if(phase<10){
		                	phase=179;
		                	phasefrac=9;
		                	break;}	
		                else phase-=10;
		                break;}
			        if(digito==5){
			        	if(phase<100){
			        		phase=179;
			        		phasefrac=9;
			        		break;}
			        	else phase-=100;
			            break;}
			        break;
		case SUMAR:if(digito==1){
			             if(phasefrac==9){
			            	 phase++;
			            	 phasefrac=0;
			            	 break;}}
		           if(digito==3){
		        	     if(phase<179){
		        	    	 phase++;
		        	         break;}
		        	     else {phase=0;
		        	           phasefrac=0;
		        	           break;}}
		        	if(digito==4){
		        		 if(phase==179){
		        			  phase=0;
		        			  phasefrac=0;
		        			  break;}
		        		 else{phase+=10;
		                      break;}}
		            if(digito==5){
		            	 if(phase==179){
		            		  phase=0;
		            		  phasefrac=0;
		            		  break;}
		            	 if(phase>99){
		            		 phase=0;
		            		 phasefrac=0;
		            		 break;}}
			       break;
		default:break;
	}//fin switch
	
}//fin operacion phase--------------------------------------------------


/* CONVERTIR NUMEROS PARTIDOS DE PHASE A NUMERO REAL flotante  */
float convert_Phase_a_Real(void){
float ang;	  
	  ang =convert_uchar_a_Float(Deteccion.phase);
	  ang+=convert_uchar_a_frac_float(Deteccion.phasefrac);
}//fin convert_Phase_a_Real---------------------------------------

/*convierte un numero uchar a decimal de 0.1 a 0.9*/
float convert_uchar_a_frac_float(unsigned char n){	
	   if(n>9)
		   return 0;
	   return(((float)n)/10);
}//fin convert_uchar_a_frac_float--------------------------------


//convierte un numero uchar a flotante
float convert_uchar_a_Float(unsigned char n){
	return ((float)n);
}//fin convert_uchar_a_Float--------------------------------------------


/*  *n: numero a ser procesado del tipo short int    
 *  digito: digito que se va ha hacer la operacion   digito-> 12345  <--posicion, 5->unidad
 *  op: operacion SUMAR | RESTAR    
 *  Funcion que resta o suma  en un digito en particular  */
void operacionUShortInt(unsigned short int *n,unsigned char digito,unsigned char op){//opera en el digito indicado
unsigned short int i,j;
const unsigned short int num[]={0,0,10,100,1000,10000};
	switch(op){
		case RESTAR:if(*n==1)
			             break;
		             j=num[digito];
		             //if(digito==5)
		            	//  j=32765;
					 switch(digito){
						 case 1: (*n)--; return;
						 case 2:
						 case 3:
						 case 4:
						 case 5: i=*n;
								 if(i<j)
									 *n=1;    	 
								else{i-=num[digito];
									 *n=i;}
							    break;
						default:break;}
				   break;//case restar
		case SUMAR:if(*n<32767){
			    	  if(digito==1){
			    		  (*n)++;
			    		  break;}
			    	  if(digito==2){
			    		  i=*n;
			    		  i+=10;
			    		  if(i>32767)
			    			  i=32767;
			    		  *n=i;
			    	      break;}
			    	  if(digito==3){
			    		  i=*n;
			    		  i+=100;
			    		  if(i>32767)
			    		     i=32767;
			    		  *n=i;
			    		  break;}
			    	  if(digito==4){
			    		  i=*n;
			    		  i+=1000;
			    		  if(i>32767)
			    		  	  i=32767;
			    		  *n=i;
			    		  break;}
			    	  if(digito==4){
						  i=*n;
						  i+=1000;
						  if(i>32767)
							  i=32767;
						  *n=i;
						  break;}
			    	  if(digito==5){
			    		 i=*n;
			    		 i+=10000;
			    		 if(i>32767)
			    			 i=32767;
			    		 *n=i;
			    	     break;}
			       }else break; 
		default:break;}
}//fin operacion de  numero short int -------------------------------------------------------


/* GEnerate a ecuation linea from
 *   Genera el maximo basado en la ecacion generada con los puntos
 * *   
 *     (99,32000)(1,64)  -----> ecuacion generada con estos puntos
 */
signed short int getMaximun(unsigned char zoom){ //Maximo valor por pantalla en determinado zoom
	const float x1=99;      //maz zoom  hacemos la ecuacion de la recta para cada maximo vs cada zoom
	const float y1=32000;  //cambiar este valor para aujustar el maximo maximus
	const float x2=1;     //zoom minimo
	const float y2=64;  //maximo menor
	float m,b;
//f(x)=mx+b  Aqui sacamos la ecuacion de la recta general para todos los Zoom,  valor ADC's max-min vs Zooms max-min
	     m=(y2-y1)/(x2-x1); //la pendiente
	     b=m*(-1)*x1+y1;
	     return ((signed short int)(m*((float)zoom)+b)); //aqui sacamos el valor f(x) que es el zoom y la x es el valor maximo del ADC para ese Zoom
}//final from get maximun  value on function on zoom



//pixel Analog up or down, e.g. 32.64 became 33, or 32.12 became 32
float redondearFloat(float f){
unsigned short int s;
float ff,g;	
unsigned char signo=POSITIVO;
	
     if(f<0){ //es negativo
    	 ff=f*-1;//convertir a positivo
    	 signo=NEGATIVO;}//Procesamos un num negativo
     else {ff=f;
           signo=POSITIVO;}//procesamiento de num positivo
     for(s=0;s<10000;s++){
    	   if((ff-(float)s)<1)//buscar el numero entero
    	       break; }
     ff=ff-s;//vaciamos solo la fraccion
	 for(g=0.0;g<=0.9;g+=0.1){
		   if((ff-g)<=0.09)//buscamos la fraccion en un digito 
			   break;}
	 if(g>0.5)
		 return(s+1);
	 else return(s);
	    
}//fin redondearFloat-----------------------------------------------------------------------------------


/*convierte el numero WORD  ADCVAL: es un numero del adc que es el voltaje de 0 a 65520 representa
 * 0v a 1.3volts    regresa float el numero miliamper  en float
 * formula es  y=19.834x10^-6(x)+0.03826       y=mx+b*/
double convertir_ADC_word_A_miliVolt(unsigned short int ADCval,unsigned char channel){
double r;
	  
    switch(channel){
    	case _PICKUP_://r=0.000019834*((float)ADCval)+0.03826;//Pickup formula con multimetro y generador
                      r=(0.000015367421067*((float)ADCval))+0.11468063; //con osciloscopio y señal real
    		          break;
    	case _DRIVER_://r=0.000722366*((float)ADCval)+0.177825;//DRiver formula con multimetro y generador
                      r=((0.000692321)*((float)ADCval))+3.880113079; //con osciloscopio y señal real
    		          break;
    	case CANAL_Y:              
    	case CANAL_X://r=m(x-a)+b; ecuacion de la recta (10,0),(17709,2.71)
    	             r=0.00015*((float)ADCval)-0.00153; //voltaje canal X
    	             break;}
        
return r;
}//fin convertir_ADC_word_A_miliVolt-----------------------------------------------


double absd(double m){//get absolute from double
	     if(m<0)
		   return(m*(-1));
		 else return(m);  //absoluto de la pendiente      
}//fin de obtener el absoluto de un numero float---------------------------------------



float absf(float m){//get absolute from float
	     if(m<0)
		   return(m*(-1));
		 else return(m);  //absoluto de la pendiente      
}//fin de obtener el absoluto de un numero float---------------------------------------


signed short int absSSInt(signed short int a){//get the absolute from signed short int
	if(a<0)
	  return(a*(-1));
	else return (a);
}//fin de signed short int absolute--------------------------------------------------



/*Obtener el check sum del pquete de datos a enviar
 *  cmd es el comando a enviar como cabeza, data es el apuntador a los datos a enviar
 *  len: es el numero de bytes a enviar sin contar comando ni checksum., b
 *  version-1*/
unsigned char getCheckSUM(unsigned char cmd, unsigned char *data, unsigned char len){
unsigned char i;
union _ussint_{
	unsigned short int Suma;
	unsigned char bytes[2];
}suma1;
           suma1.Suma=0;
           suma1.Suma=cmd;
           for(i=0;i<len;i++)
	           suma1.Suma+=*(data+i);
           i=suma1.bytes[0]+suma1.bytes[1];
           i^=0xFF;//nvertir valores;
return(i);                                 //inversor
}//fin de get checksum del paquete de datos que pretendemos enviar----------------------


//cuenta o descueta la variable que cuenta cuantos nodos quedan libres
unsigned char FIFOcount(unsigned char var,unsigned char op){
	 if(op==RESTAR){
		  if(var==0){
			  __asm(Halt);
			  return (0);}// __asm(Halt); //debug error de ingenieria de software
		  else 
			  return (var-1);}
	 else
		 return(var+1);
return 0;
}//fin de FIFO counte------------------------------------------------------

/*obtener los bytes rovenientes de un numero
 *   signed short int  
 *    version-1   */
void getBytes_from_SSInt(unsigned char *byteMSB,unsigned char *byteLSB,signed short int ssi){
union gr{
  signed short int ssi;
  unsigned char byte[2];
}number;	
	          number.ssi=ssi;
	          *byteMSB=number.byte[0];
	          *byteLSB=number.byte[1];
}//fin getBytes_from_SSInt-------------------------------------------


/* convierte las coordenadas de 0 a 4000 y de 0 a -4000
  en coordenadas de 0 a 1000 en 0 a 1000 bit 1
  en  0 a -1000  en array 0 a 1000 bit 5
  en coordenadas de 0 a 2000 en 0 a 1000 bit 2
   en 0 a -2000 en 0 a 1000 bit 6 
   regresa en numero del bit 
   y asi sucesivamente  hasta 4000 y -4000 mas de eso lo ignora*/
//unsigned char save_Pixels(signed short int x,signed short int y){
//unsigned char signox=0,signoy=0;
////signed short int xx,yy;
//unsigned short int i;    
//const unsigned char byte[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//const unsigned char inv[]= {0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};
//	signox=getSigno_Convert(&x);
//	signoy=getSigno_Convert(&y);
//	i=get_index_bit(&x,signox);
//	dds.rePaint.x[x]|=byte[i];
//	i=get_index_bit(&y,signoy);
//	dds.rePaint.y[y]|=byte[i];
//	
//}//fin getConvertXY---------------------------------




//convierte un numero negativo en positivo y regresa
//que signo era
unsigned char getSigno_Convert(signed short int *n){
signed short int m;
    m=*n;
	if(m<0){
		*n=m*-1;
		return NEG;}
	else return POS;
}//fin  getSigno_Convert-------------------------




/*Return FALSE if it finish till the last coord,*/
unsigned char is_get_number_from_pixel(unsigned char *x,unsigned char  *y){
//unsigned char i;
const unsigned char byte[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char xx,b;
static unsigned char bit,num,init,reg,var;
static unsigned char debugx[20],debugy[20],i;
static unsigned char row,col;

      row=dds.rePaint.iy;
      reg=dds.rePaint.ix;
      b=dds.rePaint.b;
      if(init!=0xAA){
    	   init=0xAA;
    	   dds.rePaint.iy=0;
    	   dds.rePaint.ix=0;
    	   num=0;
    	   bit=0;
    	  b=0x00;
    	   *x=0;*y=0;
    	   i=0;
    	   row=0;
    	   col=0;
    	   reg=0;}
      if(b>7){
    	   b=0;
    	   if((++reg)>(SIZE_X-1)){
    		    reg=0;
    		    if((++row)>(SIZE_Y-1)){
    		    	init=0;
    		    	return FALSE;}}}
     xx=dds.rePaint.xy[row][reg];
     if(xx==0){*x=0;*y=0;}
     else{if((xx&byte[b])==byte[b]){
    	      *x=get_Coord_Num(reg,b);
    	      *y=row;}
          else{*x=0;*y=0;}}
      
      
	dds.rePaint.iy=row;
	dds.rePaint.ix=reg;
	dds.rePaint.b=b+1;
return TRUE; //SEGUIMOS SACANDO NUMEROS DE LA MATRIZ
}//in is_get_number_from_pixel

/* se recalcula con el buffer que tenemos gurdado todos los puntoos
 * actuales del dds y algunos valores fuera del dds, el nuevo zoom
 * basado en el que hay, se reacomodan los puntos en base el nuevo zoom
 * */
void re_Calcular_Buffers_DDS(unsigned char zoom_wished){
	 
	 if(zoom_wished==zoom.Zoom)
		 return;
	 if(zoom_wished<zoom.Zoom){//se vamos a restar?
		 if((zoom.Zoom-zoom_wished)==1)//solo le sera uno, la resta
			 re_Calcular_Buffers_DDS_Modificar_Uno(zoom.Zoom-1);}
	 else{//aumentar el zoom ACERCAR
		  if((zoom_wished-zoom.Zoom)==1)
			  re_Calcular_Buffers_DDS_Modificar_Uno(zoom.Zoom+1);}
		   
	 
	
	
	
}//fin de recalculr el bufer que esta guardado en el DDS----------





void re_Calcular_Buffers_DDS_Modificar_Uno(unsigned char zoom1){
#if(SIZE_X>254)
	unsigned short int i,j;
#else
	unsigned char i,j;
#endif

unsigned char xn,b,xr,yr,cuadrante,bit,reg;
//unsigned char buff[2][SIZE_X];
//unsigned char xy[SIZE_Y][SIZE_X];
const unsigned char bbyte[]={1,2,4,8,16,32,64,0x80};
unsigned char zoomPalanca;//aleja o acerca?
    if(zoom.Zoom>zoom1) {
    	 zoomPalanca=ALEJAR;
    	 zoom.Zoom--;}
    else {if(zoom.Zoom<zoom1){
    	    zoomPalanca=ACERCAR;
            zoom.Zoom++;}
    	  else{__asm(nop);__asm(Halt);__asm(nop);}}
	for(j=0;j<SIZE_Y;j++)
	 for(i=0;i<SIZE_X;i++)
		 dds.rePaint.xy0[j][i]=0;
	for(j=0;j<SIZE_Y;j++)
	  for(i=0;i<SIZE_X;i++)
		  for(b=0;b<8;b++){//sacar el numero de x uno por uno del regitro x
             if((dds.rePaint.xy[j][i]&bbyte[b])==bbyte[b]){
			       xn=get_Coord_Num(i,b);	//OBTENER EL numero que corresponde al bit activado que es un pixel de 0 a 192
			       cuadrante=get_cuadrante(xn,j);//a que cuadrante pertenecen esta coordenada
                   reconvertir_coord_reducir_uno(xn,j,cuadrante,&xr,&yr,zoomPalanca);//reconvertir coordenada actual y redcirla a escala uno menor
                   bit=getRegistro_X_bit(&reg,xr);//obtener el registro y el bit de esta nueva coordenada
                   dds.rePaint.xy0[yr][reg]|= bit;
                   __asm(nop);
                   }}//Guradar nueva coordenada modificada de zoom buf temporal 
	//para fines de debug se combinan ambos buffers
	for(j=0;j<SIZE_Y;j++) //hacer un OR con los pixeles que tenemos con
		  for(i=0;i<SIZE_X;i++)//los reducidos a otro zoom, Debug solo
		        dds.rePaint.xy[j][i]=dds.rePaint.xy0[j][i];
		       
		        	
}//fin re_Calcular_Buffers_DDS_Alejar_Uno--------------

/*//reconvertir coordenada actual y redcirla a escala uno menor 
 * palanca: para saber si REDUCE  o aumenta el zoom a uno */
void reconvertir_coord_reducir_uno(unsigned char xn,unsigned char yn,unsigned char cuadrante,unsigned char *xr,unsigned char *yr,unsigned char palanca){
unsigned char a,b,c,d;
  if(palanca==ALEJAR){
          	 a=xn+1;b=yn+1;
          	 c=xn-1;d=yn-1;}
  else {a=xn-1;b=yn-1;
	    c=xn+1;d=yn+1;}
	switch(cuadrante){
		case 1:if(xn>0)
			       *xr=a;//xn+1;
		       if(yn>0)
		    	   *yr=b;//yn+1;
		       break;
		case 2:if(xn>0)
		        *xr=c;//xn-1;
	          if(yn>0)
	    	    *yr=b;//yn+1;
	           break;
		case 3:if(xn>0)
	              *xr=a;//xn+1;
               if(yn>0)
    	          *yr=d;//yn-1;
               break;
		case 4:if(xn>0)
	             *xr=c;//xn-1;
               if(yn>0)
    	         *yr=d;//yn-1;
              break;
			   __asm(nop);
			   __asm(Halt);
			   __asm(nop);
		default:break;}
	
	
	
}//fin reconvertir_coord_reducir_uno





/* obtenemos el cuadrante donde debe estar esta coordenada que va desde
 *  el numero 1 al 4 
 * cuadrante donde se encuentra el numero que va del 0 al 192  ydel 0 al 128 pero viendo el centro en 92,64*/
unsigned char get_cuadrante(unsigned char xi,unsigned char yi){
	
  if(xi<96){
	  if(yi<64)
	       return 1;
	  else return 3;}
  else{if(yi<64)
           return 2;
       else return 4;}
}//fin de la funcion get el cuadrante del dds donde esta esta coordenada


/* tenemos el registro de los  un registro de las lineas x del despliegue en pantalla de los pixeles
 *  el registro guarda los pixeles que se encienden o no en valor absoluto para se pintados,
 *  param tro bit indica cuales bits del registro estan encendidos
 *  este metodo converte el numero del registro y el bit en un numero
 *  que representa la coordenada la coordenada absoluta a de 1 a 192 ya convertia a dds absoluto.
 * */
unsigned char get_Coord_Num(unsigned char xn,unsigned char bit){
const unsigned char byte[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

   switch(xn){//rx
	   case 0:switch(byte[bit]){//b
		         case 1:return 192;break;
		         case 2:return 1;break;
		         case 4:return 2;break;
		         case 8:return 3;break;
		         case 16:return 4;break;
		         case 32:return 5;break;
		         case 64:return 6;break;
		         case 128:return 7;break;
		         default:break;}
	          break;//fin de 0
	   case 1:switch(byte[bit]){
		         case 1:return 8;break;
		         case 2:return 9;break;
		         case 4:return 10;break;
		         case 8:return 11;break;
		         case 16:return 12;break;
		         case 32:return 13;break;
		         case 64:return 14;break;
		         case 128:return 15;break;
		         default:break;}
		      break;//fin de 1
       case 2:switch(byte[bit]){
    	         case 1: return 16;break;
    	         case 2: return 17;break;
    	         case 4: return 18;break;
    	         case 8: return 19;break;
    	 		 case 16:return 20;break;
    	 		 case 32:return 21;break;
    	 		 case 64:return 22;break;
    	 		 case 128:return 23;break;
    	 		 default:break;}
    	      break;//fin de 2
    	   case 3:switch(byte[bit]){
    	       	 case 1: return 24;break;
    	       	 case 2:return 25;break;
    	       	 case 4:return 26;break;
    	       	 case 8:return 27;break;
    	       	 case 16:return 28;break;
    	       	 case 32:return 29;break;
    	       	 case 64:return 30;break;
    	       	 case 128:return 31;break;
    	       	 default:break;}
    	       break;// fin de 3
    	  case 4:switch(byte[bit]){
    		     case 1: return 32;break;
    		     case 2:return 33;break;
    		     case 4:return 34;break;
    		     case 8:return 35;break;
    		     case 16:return 36;break;
    		     case 32:return 37;break;
    		     case 64:return 38;break;
    		     case 128:return 39;break;
    		     default:break;}
    		   break;// fin de 4      
    	  case 5:switch(byte[bit]){
    		     case 1: return 40;break;
    		     case 2:return 41;break;
    		     case 4:return 42;break;
    		     case 8:return 43;break;
    		     case 16:return 44;break;
    		     case 32:return 45;break;
    		     case 64:return 46;break;
    		     case 128:return 47;break;
    		     default:break;}
    		     break;// fin de 5
         case 6:switch(byte[bit]){
    		     case 1: return 48;break;
    		     case 2:return 49;break;
    		     case 4:return 50;break;
    		     case 8:return 51;break;
    		     case 16:return 52;break;
    		     case 32:return 53;break;
    		     case 64:return 54;break;
    		     case 128:return 55;break;
    		     default:break;}
    		     break;// fin de 6 		     
         case 7:switch(byte[bit]){
        	     case 1: return 56;break;
        	     case 2:return 57;break;
        	     case 4:return 58;break;
        	     case 8:return 59;break;
        	     case 16:return 60;break;
        	     case 32:return 61;break;
        	     case 64:return 62;break;
        	     case 128:return 63;break;
        	     default:break;}
        	     break;// fin de 7 	
         case 8:switch(byte[bit]){
        	    case 1: return 64;break;
        	    case 2:return 65;break;
        	    case 4:return 66;break;
        	    case 8:return 67;break;
        	    case 16:return 68;break;
        	    case 32:return 69;break;
        	    case 64:return 70;break;
        	    case 128:return 71;break;
        	    default:break;}
        	    break;// fin de 8 		     
        case 9:switch(byte[bit]){
        	    case 1: return 72;break;
        	    case 2:return 73;break;
        	    case 4:return 74;break;
        	    case 8:return 75;break;
        	    case 16:return 76;break;
        	    case 32:return 77;break;
        	    case 64:return 78;break;
        	    case 128:return 79;break;
        	    default:break;}
        	    break;// fin de 9 	
       case 10:switch(byte[bit]){
        	    case 1: return 80;break;
        	    case 2:return 81;break;
        	    case 4:return 82;break;
        	    case 8:return 83;break;
        	    case 16:return 84;break;
        	    case 32:return 85;break;
        	    case 64:return 86;break;
        	    case 128:return 87;break;
        	    default:break;}
        	    break;// fin de 10 		     
       case 11:switch(byte[bit]){
        	    case 1: return 88;break;
        	    case 2:return 89;break;
        	    case 4:return 90;break;
        	    case 8:return 91;break;
        	    case 16:return 92;break;
        	    case 32:return 93;break;
        	    case 64:return 94;break;
        	    case 128:return 95;break;
        	    default:break;}
        	    break;// fin de 11	
       case 12:switch(byte[bit]){
    	        case 1: return 96;break;
    	        case 2:return 97;break;
    	        case 4:return 98;break;
    	        case 8:return 99;break;
    	        case 16:return 100;break;
    	        case 32:return 101;break;
    	        case 64:return 102;break;
    	        case 128:return 103;break;
    	        default:break;}
    	        break;// fin de 12		     
        case 13:switch(byte[bit]){
    	        case 1: return 104;break;
    	        case 2:return 105;break;
    	        case 4:return 106;break;
    	        case 8:return 107;break;
    	        case 16:return 108;break;
    	        case 32:return 109;break;
    	        case 64:return 110;break;
    	        case 128:return 111;break;
    	        default:break;}
    	        break;// fin de 13 	
       case 14:switch(byte[bit]){
    	        case 1: return 112;break;
    	        case 2:return 113;break;
    	        case 4:return 114;break;
    	        case 8:return 115;break;
    	        case 16:return 116;break;
    	        case 32:return 117;break;
    	        case 64:return 118;break;
    	        case 128:return 119;break;
    	        default:break;}
    	        break;// fin de 14 		     
       case 15:switch(byte[bit]){
    	        case 1: return 120;break;
    	        case 2:return 121;break;
    	        case 4:return 122;break;
    	        case 8:return 123;break;
    	        case 16:return 124;break;
    	        case 32:return 125;break;
    	        case 64:return 126;break;
    	        case 128:return 127;break;
    	        default:break;}
    	        break;// fin de 15       	        	        	     	       	        	     		            	     		     
       case 16:switch(byte[bit]){
    	        case 1: return 128;break;
    	        case 2:return 129;break;
    	        case 4:return 130;break;
    	        case 8:return 131;break;
    	        case 16:return 132;break;
    	        case 32:return 133;break;
    	        case 64:return 134;break;
    	        case 128:return 135;break;
    	        default:break;}
    	        break;// fin de 16	
       case 17:switch(byte[bit]){
    	        case 1: return 136;break;
    	       	case 2:return 137;break;
    	       	case 4:return 138;break;
    	       	case 8:return 139;break;
    	        case 16:return 140;break;
    	        case 32:return 141;break;
    	       	case 64:return 142;break;
    	       	case 128:return 143;break;
    	        default:break;}
    	       	break;// fin de 17		     
    	case 18:switch(byte[bit]){
    	       	case 1: return 144;break;
    	       	case 2:return 145;break;
    	       	case 4:return 146;break;
    	       	case 8:return 147;break;
    	        case 16:return 148;break;
    	       	case 32:return 149;break;
    	       	case 64:return 150;break;
    	       	case 128:return 151;break;
    	       	default:break;}
    	        break;// fin de 18 	
       case 19:switch(byte[bit]){
    	        case 1: return 152;break;
    	        case 2:return 153;break;
    	       	case 4:return 154;break;
    	        case 8:return 155;break;
    	       	case 16:return 156;break;
    	       	case 32:return 157;break;
    	       	case 64:return 158;break;
    	       	case 128:return 159;break;
    	       	default:break;}
    	       	break;// fin de 19 		     
       case 20:switch(byte[bit]){
    	        case 1: return 160;break;
    	       	case 2:return 161;break;
    	        case 4:return 162;break;
    	       	case 8:return 163;break;
    	        case 16:return 164;break;
    	       	case 32:return 165;break;
    	        case 64:return 166;break;
    	       	case 128:return 167;break;
    	       	default:break;}
    	       	break;// fin de 20       	        	        	     	       	        	     		            	     		     
       case 21:switch(byte[bit]){
    	        case 1: return 168;break;
    	        case 2:return 169;break;
    	        case 4:return 170;break;
    	        case 8:return 171;break;
    	        case 16:return 172;break;
    	        case 32:return 173;break;
    	        case 64:return 174;break;
    	        case 128:return 175;break;
    	        default:break;}
    	        break;// fin de 21      	     		       		       		     
       case 22:switch(byte[bit]){
    	       	case 1: return 176;break;
    	       	case 2:return 177;break;
    	       	case 4:return 178;break;
    	       	case 8:return 179;break;
    	        case 16:return 180;break;
    	       	case 32:return 181;break;
    	       	case 64:return 182;break;
    	       	case 128:return 183;break;
    	      	default:break;}
       	       	break;// fin de 22       	        	        	     	       	        	     		            	     		     
       case 23:switch(byte[bit]){
    	       	case 1: return 184;break;
    	       	case 2:return 185;break;
    	       	case 4:return 186;break;
    	       	case 8:return 187;break;
    	       	case 16:return 188;break;
    	        case 32:return 189;break;
    	        case 64:return 190;break;
    	       	case 128:return 191;break;
    	       	default:break;}
    	       	break;// fin de 23          		          		     	      
    	   default:break;}      
    	
       
       		       	      
	 	
}//fin de get_Coord_Num-----------------------------------

/* sacar un numero a la potencia en float*/
float power(float n2,unsigned char potencia){
unsigned char i;
float f=1;
      if(potencia==0)
    	  return 1;
      if(potencia==1)
    	  return n2;
	  for(i=0;i<potencia;i++)
	       f*=n2;
return f;	  
}//fin power----------------------------------------------

float factorial(unsigned char n1){
  switch(n1){
	  case 0:
	  case 1:return 1;
	  case 2:return 2;
	  case 3:return 6;
	  case 4:return 24;
	  case 5:return 120;
	  case 6:return 720;
	  case 7:return 5040;
	  case 8:return 40320;
	  case 9:return 362880;
	  case 10:return 3628800;
	  case 11:return 39916800;
	  case 12:return 479001600;
	  case 13:return 6227020800;
	  case 14:return 87178291200;
	  case 15:return 1307674368000;
	  case 16:return 20922789888000;
	  case 17:return 355687428096000;
	  case 18:return 6402373705728000;
	  case 19:return 121645100408832000;
	  case 20:return 2432902008176640000;
	  default:__asm(Halt);} //error de software debug
}//fin factorial---------------------------------------------------


/* redondeamos a la cifra mas cercana hacia a rriba o  abajo*/
unsigned short int redondear(float n){
unsigned char i;	
	if(n>200)
		__asm(Halt);//error de ingenira softwre
	i=(unsigned char)n;//quitamos decimanes
	n=n-((float)i); //quitamos enteros
	if(n>0.55)
		 return i+1;
	else return i;
}//fin redondear ----------------------------------------------

//formatea el numero a del signo resultado de 0-90° si pasa de eso
/*convierte el grado de 0 a 90 con salida negativa, e.g input=91 salida 89, negativo
* e.g.  in:179 out:1° negativo.    e.g. 90.1 sal:89.9negativo 
*  si es positivo es return 0 sino return 0xFF*/
unsigned char Formatear_Negativos(float *a){
	   if(*a>90){
		 if(*a>180){*a=0;return 0;}
		 else{*a=180-*a; return 0xFF;}}//salida negativa
	   else{return 0;}	 	   
}//Formatear_Negativos-------------------------------------------------

/*  coordenas para dibujar la linea de phase del DDS
 * PARAM: s,signo del angulo y cordenas 0:x   1:y del las
 *  absisa y ordenada del triangulo de la hipotenusa del angulo del
 *  punto P2, param *centro es el centro, origen de las lineas
 *  param *c: es el Punto de la linea dibuar, del centro a c*/
void get_coordenadas(unsigned char s,unsigned short int *c,unsigned short int *centro)
{unsigned char i;
 unsigned short int P[4];//x1,y1,x2,y2;
	if(!s){//signo positivo
	   P[0]=*centro+*c;    //CENTX+ux  //P1
	   P[1]=*(centro+1)-*(c+1);
	   P[2]=*centro-*c;
	   P[3]=*(centro+1)+*(c+1);}
	else{P[0]=*centro-*c;//CENTX-ux
	     P[1]=*(centro+1)-*(c+1);
	     P[2]=*centro+*c;
	     P[3]=*(centro+1)+*(c+1);}
   for(i=0;i<4;i++){
	   if(P[i]>127)
		   P[i]--;
	   *(c+i)=P[i];}
}//get coordenadas to DDS draw line for phase------------------------------------------------

/* obtenemos la coordenada Cx donde cruza la linea con la absisa
 *  en funcion con el zoom y la sensibilidad  */
unsigned char getCentroX(void){
	 
	 
}//fin unsigned char getCentroX(void)----------------


/*obtener las coordenadas de la recta, las coordenadas visibles
 *param1: m: pendiente 
 *param2: Cx: interseccion con la absisa,de la recta.
 *param3: cf0:x1  cf2:y1  cf2:x2  cf3:y2
 * return TRUE: graficable, else FALSE no graficable*/
unsigned char get_coordenadas2(float m,float Cx, float *cf,float deg){
unsigned short int i;
float y;
unsigned char e=0,ret=FALSE;
unsigned char signo=0;//monitorear el cambio de signo en el cuadrante y para las ordenadas(Y)

	for(i=0;i<192;i++){
	    y=ecRecta(m,i,Cx,-64);
	    switch(e){
	    	case 0:if(y<0){//negativo porque es el cuadrante IV
	                  y*=-1;
	                  signo|=0x01;
	                  if(y<127){
	    	            *(cf+0)=i;
	                    *(cf+1)=y;
	                    e++;}
	                  else signo|=0x02;}
 	    	       break;
	    	case 1:if(y>0)
	    		       signo|=0x04;
	    		   if(y<0){
	    		     y*=-1;
	    		     if(y<127){
	    		    	 *(cf+2)=i;
	    		    	 *(cf+3)=y;
	    		         ret=TRUE;}
	    	         else break;}
	    	      break;
	    	default:e=0;break;}}
	if(!ret){//no encontro buenas coordenadas
	   if((signo&0x07)==0x07){//cuMple con que la señal paso por cuadrante IV de -127 a +1, recta casi de 90°	
		   *cf=0;*(cf+2)=0;	
		   for(i=0;i<192;i++){	   //busqueda de las coordenadas con otro metdodo  
			   y=ecRecta(m,i,Cx,-64);
			   if(y<0)
				   *(cf+0)=i;
			   if(y>0){
				   *(cf+2)=i;
				   break;}}
		  if((*cf>0)&&(*(cf+2)>0)){
		          *(cf+1)=127;
		          *(cf+3)=0;
		          ret=TRUE;}}}
	//revision de la ecuacion para ver si esta bien
	if(((deg>60)&&(deg<90))||((deg>90)&&(deg<110))){
	       *(cf+1)=127;
	       *(cf+3)=0;}
return ret;	
}	//fin get_coord2  ----------------------------------------------------------------


/* ecuacion de la recta resuelve la variable Y
 * param1: m: pendiente
 * param2: x: varible independiente 
 * param 3 y 4 : puntos de la recta para sacar la ecuacion*/
float ecRecta(float m,unsigned short int x,float x1,float y1){
     return((m*(x-x1))+y1);
}//finecRecta---------------------------------------------



/* Square Root , aproximacion Tylos
    Make an initial guess. Guess any positive number x0.
    Improve the guess. Apply the formula x1 = (x0 + S / x0) / 2. The number x1 is a better approximation to sqrt(S).
    Iterate until convergence. Apply the formula xn+1 = (xn + S / xn) / 2 until the process converges. Convergence is achieved when the digits of xn+1 and xn agree to as many decimal places as you desire.
 *       */
float Square_Root(float S){
float x0,x1;	
   x0=S/2;x1=0;	
   for(;;){
	   x1=(x0+(S/x0))/2;
	   if((x0-x1)<0.5) //presicion de la raiz
		   break;
	   else x0=x1;}
}//finSquare_Root---------------------------------


/* vamos a obtener todos los puntos de la recta en uchar 
 * visibles en la pantalla */
//void get_Puntos_EcRecta(unsigned short int Cx,float angulo,unsigned short int *x, unsigned short int *y)
//{unsigned short int i;	
//float m,yy;
//    m=sin(angulo)/cos(angulo);//pendiente de la recta
//    for(i=0;i<SIZE_XY;i++){
//          *x++=i;
//    	  yy=ecRecta(m,i,Cx,_Y_);
//          *y++=(unsigned short int)yy;}
//}//fin get_Puntos_EcRecta------------------------------------


/*los puntos que no sean visibles se van a poner como 0,0 */
//void get_Puntos_Visibles(unsigned short int  *x, unsigned short int *y){
//unsigned short int i;
//	for(i=0;i<SIZE_XY;i++)
//	  if((*x>192)||(*y>127)){
//		     *x=0;*y=0;} 
//			 
//}//fin get_Puntos_Visibles--------------------------------------


/* verificamos que las coordenadas no se pasen del cuadro por un
 *  mal calculo o resta inversa*/
unsigned char verificar_coordenadas(unsigned short int *c){
unsigned char i;	
for(i=0;i<4;i++) //codigo de verificacion de anti errores
	   if(*(c+i)>192)
          return FALSE; // asm(Halt);//debug error de softwar
return TRUE;          
}//finverificar_coordenadas------------------------------------

void reset_usint(unsigned short int *c,unsigned char size){
unsigned char i;	
    if(size>254)
         __asm(Halt);//debug error de software
	for(i=0;i<size;i++) //codigo de verificacion de anti errores
		    *(c+i)=0;
}//fin resetear_coordenadas------------------------------------


/* dibujar linea paralela inferior de la central de phase */
//l(s,&c[0],&centro[0],&p1[0],20);
/* param1 s: es el signo del angulo 0|FFh, 
 * param2 *c: coordenadas del punto P central de la linea
 * param3: *centro coordenada x&y del centro
 * param4: coordenadas que se producen para dibujar P2 inferior,
 * param5: es la distancia el offset de distancia del centro enntre parallelas */
//void get_coordenadas_pll(unsigned char s,unsigned short int *c,unsigned short int *cnt,unsigned short int *P,unsigned char Alt){
//	
//	
//	
//}//fin get_coordenadas_pll-----------------------------------------------------------





/* Serie de Maclauren para el SENO - - - - - - - - - - - - - - - */
float  sin(float a){
unsigned char n;
float sum=0,b;
	 for(n=0;n<10;n++){
	   b=((power(-1,n))/(factorial(2*n+1)))*power(a,(2*n+1));
	   sum+=b;}
return sum;
}//fin sinus---------------------------------------------------------------------------------

float  cos(float a){
unsigned char n;
float sum=0;
	 for(n=0;n<10;n++)
	   sum+=((power(-1,n))/(factorial(2*n)))*power(a,2*n);
return sum;
}//fin sinus------------------------------------------------------------------

/* convertir de grados a Radianes   */
float convert_Deg_a_Rad(float g){
	return  ((g*3.141592653)/180);
}//fin convert_Deg_a_Rad------------------------------------------------------


