/*
 * BarraDisplay.c
 *
 *  Created on: Jul 14, 2020
 *      Author: desarrollo-1
 */

#include "BarraDisplay.h"
#include "system.h"
#include "VFD.h"
#include "VFDmenu.h"

//extern volatile unsigned char menu.contexto.Actual;
extern struct _Menu_ menu;

//despliegua el cuadro Madre de deteccion y los dos cuadros hijos vacios en la coordenada x,y
void displayCuadroMadre(void){
    
     if(menu.contexto.Actual==AJUSTE_DE_VIBRACION){
    	 VFDboxLine(ON,BOX_VACIA,POS_X1_BOX_AV,POS_Y1_BOX_AV,POS_X2_BOX_AV,POS_Y2_BOX_AV);//CUADRO MADRE
    	 VFDboxLine(ON,BOX_VACIA,POS_X1_BOX1_AV,POS_Y1_BOX1_AV,POS_X2_BOX1_AV,POS_Y2_BOX1_AV);//CUADRO-1  HIJO VACIO	    
    	 VFDboxLine(ON,BOX_VACIA,POS_X1_BOX2_AV,POS_Y1_BOX2_AV,POS_X2_BOX2_AV,POS_Y2_BOX2_AV);//CUADRO-1  HIJO VACIO	 	 
         }//fin if principal Ajuste de vibracin menu
     else{if(menu.contexto.Actual==PORTAL_INICIO){
    	       VFDboxLine(ON,BOX_VACIA,POS_X1_BOX_PI,POS_Y1_BOX_PI,POS_X2_BOX_PI,POS_Y2_BOX_PI);//CUADRO MADRE
    	       VFDboxLine(ON,BOX_VACIA,POS_X1_BOX1_PI,POS_Y1_BOX1_PI,POS_X2_BOX1_PI,POS_Y2_BOX1_PI);//CUADRO-1  HIJO VACIO	    
    	       VFDboxLine(ON,BOX_VACIA,POS_X1_BOX2_PI,POS_Y1_BOX2_PI,POS_X2_BOX2_PI,POS_Y2_BOX2_PI);//CUADRO-1  HIJO VACIO      
              }//FIN portal inicio
          else{//else seconf
        	  if(menu.contexto.Actual==AJUSTE_PARAMETRICO_DE_PRODUCTO){
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX_AP,POS_Y1_BOX_AP,POS_X2_BOX_AP,POS_Y2_BOX_AP);//CUADRO MADRE
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX1_AP,POS_Y1_BOX1_AP,POS_X2_BOX1_AP,POS_Y2_BOX1_AP);//CUADRO-1  HIJO VACIO	    
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX2_AP,POS_Y1_BOX2_AP,POS_X2_BOX2_AP,POS_Y2_BOX2_AP);//CUADRO-1  HIJO VACIO  	  
        	  }//fin //AJUSTE_PARAMETRICO_DE_PRODUCTO
        	  else{//thrids else 
        		  if(menu.contexto.Actual==PARAMETROS_DE_DETECCION){
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX_PD,POS_Y1_BOX_PD,POS_X2_BOX_PD,POS_Y2_BOX_PD);//CUADRO MADRE
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX1_PD,POS_Y1_BOX1_PD,POS_X2_BOX1_PD,POS_Y2_BOX1_PD);//CUADRO-1  HIJO VACIO	    
        		  VFDboxLine(ON,BOX_VACIA,POS_X1_BOX2_PD,POS_Y1_BOX2_PD,POS_X2_BOX2_PD,POS_Y2_BOX2_PD);}//CUADRO-1  HIJO VACIO  	  
        	  }//else third ends  
          }//else second
     }//fin first else
}//fin del despliegue del cuadro Madre y los dos hijos en funcion del contexto----------------------------

//despliegua el cuadro Madre de deteccion y los dos cuadros hijos vacios en la coordenada x,y
unsigned char displayCuadroMadre_VFD(void){
unsigned char x1a,x1b,x1c;
unsigned char y1a,y1b,y1c;
unsigned char x2a,x2b,x2c;
unsigned char y2a,y2b,y2c;
static unsigned char estado;
unsigned char ret=0;

	switch(menu.contexto.Actual){
		case AJUSTE_DE_VIBRACION:x1a=POS_X1_BOX_AV; y1a=POS_Y1_BOX_AV; x2a=POS_X2_BOX_AV; y2a=POS_Y2_BOX_AV;
		                         x1b=POS_X1_BOX1_AV;y1b=POS_Y1_BOX1_AV;x2b=POS_X2_BOX1_AV;y2b=POS_Y2_BOX1_AV;
		                         x1c=POS_X1_BOX2_AV;y1c=POS_Y1_BOX2_AV;x2c=POS_X2_BOX2_AV;y2c=POS_Y2_BOX2_AV;
		                         break;
		case PORTAL_INICIO:x1a=POS_X1_BOX_PI; y1a=POS_Y1_BOX_PI; x2a=POS_X2_BOX_PI; y2a=POS_Y2_BOX_PI;//CUADRO MADRE
			    	       x1b=POS_X1_BOX1_PI;y1b=POS_Y1_BOX1_PI;x2b=POS_X2_BOX1_PI;y2b=POS_Y2_BOX1_PI;//CUADRO-1  HIJO VACIO	    
			    	       x1c=POS_X1_BOX2_PI;y1c=POS_Y1_BOX2_PI;x2c=POS_X2_BOX2_PI;y2c=POS_Y2_BOX2_PI;
			    	       break;
		case AJUSTE_PARAMETRICO_DE_PRODUCTO:
              x1a=POS_X1_BOX_AP; y1a=POS_Y1_BOX_AP; x2a=POS_X2_BOX_AP; y2a=POS_Y2_BOX_AP;//CUADRO MADRE
      		  x1b=POS_X1_BOX1_AP;y1b=POS_Y1_BOX1_AP;x2b=POS_X2_BOX1_AP;y2b=POS_Y2_BOX1_AP;//CUADRO-1  HIJO VACIO	    
      		  x1c=POS_X1_BOX2_AP;y1c=POS_Y1_BOX2_AP;x2c=POS_X2_BOX2_AP;y2c=POS_Y2_BOX2_AP;//CUADRO-1  HIJO VACIO  	  
              break;
		case PARAMETROS_DE_DETECCION:
			  x1a=POS_X1_BOX_PD; y1a=POS_Y1_BOX_PD; x2a=POS_X2_BOX_PD; y2a=POS_Y2_BOX_PD;//CUADRO MADRE
		      x1b=POS_X1_BOX1_PD;y1b=POS_Y1_BOX1_PD;x2b=POS_X2_BOX1_PD;y2b=POS_Y2_BOX1_PD;//CUADRO-1  HIJO VACIO	    
		      x1c=POS_X1_BOX2_PD;y1c=POS_Y1_BOX2_PD;y2c=POS_X2_BOX2_PD;y2c=POS_Y2_BOX2_PD;//CUADRO-1  HIJO VACIO  	  
		      break;  	
		default:break;}
//	aQUI DEBE PREGUNTAR SI ESTA PUESTA EL SEMAFORO DEL vfd 
//	 SI ESTA PUESTO ESPERAMOS QUE TERMINE SI LO PONEMOS NOSOSTROS EL 
//	 SEMAFORO CADA VEZ QUE PREGUNTAMOS NOS TIENE QUE DEJAR PASAR PORQUE 
//	 NOSOTROS LO PUSIMOS, POR TANTO DEBE PREGUNTAR SI NOSOTROS LO PUSIMOS
//	if(!vfd.bits.b.VDF_busy){ 
		 switch(estado){
			 case 1:if(VFDboxLine1(ON,BOX_VACIA,x1a,y1a,x2a,y2a))
							   estado++;
					break;
			 case 2:if(VFDboxLine1(ON,BOX_VACIA,x1b,y1b,x2b,y2b))
							  estado++;
					break;
			 case 3:if(VFDboxLine1(ON,BOX_VACIA,x1c,y1c,x2c,y2c)){
//				       vfd.bits.b.VFD_busy=0;
				       ret=TRUE;estado++;}
				   break;	 
			 default:estado=1;break;}
return ret;
}//fin del despliegue del cuadro Madre y los dos hijos en funcion del contexto----------------------------
