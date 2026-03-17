/*
 * strings.c
 *
 *  Created on: Mar 5, 2020
 *      Author: desarrollo-1
 */


#include "strings.h"
#include "system.h"
#include "VFDmenu.h"
#include "maths.h"
#include "VFDisplay.h"
 


extern unsigned char Text[NOMBRE_PRODUCTO_SIZE];//texto que regresa  del procesador de textos
extern struct _GLOBAL_CONTEXTO gxc,gxc2,gxc3,gxc4,gxc5,gxc6;
//extern unsigned char igxc5;
extern struct _Menu_  menu;
//extern struct Displ1 


/* *a pointer to char
 * *b pointer to char
 * return  */
unsigned char compareString(unsigned char *a,unsigned char *b,unsigned char size){
unsigned char i,r=TRUE;
    for(i=0;i<size;i++){
    	if((*(a+i)==0)||(*(b+i)==0)){
    		if((*(a+i)==0)&&(*(b+i)==0))
    			     break;
    		else {r=FALSE;break;}}
	    if(*(a+i)!=*(b+i)){
	    	  r=FALSE;break;}}
return r;
}//fin compare strings --------------------------------------------------

//copea el origen al destino y necesita saber cuantos van a copiar
unsigned char strcpy1(unsigned char *dest,unsigned char *orig,unsigned char size ){
unsigned char i;
    if(size==0)
    	 return 0;
    for(i=0;i<size;i++)
    	  *(dest+i)=*(orig+i);
return 1;    
}//--------------------------------------------------------


//copea el origen al destino y necesita saber cuantos van a copiar
unsigned char strcpy2(unsigned char *dest,const unsigned char *orig,unsigned char size ){
unsigned char i;
    if(size==0)
    	 return 0;
    for(i=0;i<size;i++)
    	  *(dest+i)=*(orig+i);
return 1;    
}//--------------------------------------------------------



//n: es el numero que queremos obtener sus asciis
//*cc: es el apuntador a un array u string de tamaño tres, se apunta al cero
// indice 0:centena 1:decena 2:unidad
void getASCIIFromUChar1(unsigned char n,unsigned char *cc){
	unsigned char n1, n2=0, i, j, * p,e=0;
	p = cc;
	if (n == 0) {
		*cc++ = '0'; *cc++ = '0'; *cc = '0';
		return;}
	if (n < 200) {
		if (n > 99) {
			*cc = '1'; n1 = n - 100;
			if (n1 < 10) {  }
			if (n1 > 99) { e+=1; }}
		else { *cc = '0'; n1 = n; }}
	else {
        *cc = '2'; n1 = n - 200;
		if (n1 > 99) {  e+=2; }}
	if (n1 > 99) {  e+=3; }
	if (n1 > 9) {
		for (i = 10, j = 1; i < 100; i += 10, j++) {
			if (((n1 - i) < 10) || ((n1 - i) == 0))
				break;}
		*(cc + 1) = j + 0x30;
		if (n1 > i)
			n2 = n1 - i;}
	else {*(cc + 1) = '0';
		  n2 = n1;}
	if (n2 > 9) {  e+=4; }
	*(cc + 2) = n2 + '0';
	if(e>0){__asm(nop);
            __asm(Halt);}
return;
}//fin get char from unsigned char---------------------------------------

// indice 0:centena 1:decena 2:unidad, Regresa los tres ascii
void getASCII_from_UChar(unsigned char n,unsigned char *p){
unsigned char i,j,m=0;
     if(n>254){
    	     *p='0';
    	 *(p+1)='0';
    	 *(p+2)='0';
    	 *(p+3)='.';
    	 *(p+4)='0';
    	 return;}
	 if(n>199){
	      *p='2';
	      m=n-200;}
	 else{if(n>99){
			  *p='1';
			  m=n-100;}
	      else{*p='0';
	           m=n;}} 
	 if(m>9){
		 for(i=10,j=1;i<=90;i+=10,j++){
			if((m-i)<10)
			   break;}
		 *(p+1)=j+'0';
	     m-=i;}//restamos la decena
	 else{*(p+1)='0';}
	 if(m>10){
		 __asm(Halt);
	     return;}
	 else{ *(p+2)=m+'0';}
	 
}//fin getASCIIFromUChar1--------------------------------------------------






// 54321 en chars seria c[]={5,4,3,2,1}
/* obtiene  5 chars de una variable  unsigned int,(bug-> deberia ser short int esta variable */
void getCharsFromUINT_var(unsigned char *p,unsigned short int var){
//unsigned char unidad=0,dec=0,cent=0,mil=0,decmil=0;
//unsigned int n;// este metodo puede tener bugs porque la variable debe ser unsigned shrt int	          	          
/*         if(var>32000)
               var=32000;
         n=var;  
         if(var!=0) 
         while(n--!=0){
             if(++unidad>9){unidad=0;
	               if(++dec>9){dec=0;
    	                  if(++cent>9){cent=0;
	                        if(++mil>9){mil=0;
    	                         if(++decmil>9){decmil=0;unidad=0;dec=0;cent=0;mil=0;decmil=0;}}}}}}      
	     
  	    *(p+0)=decmil;          
	    *(p+1)=mil;
  	    *(p+2)=cent;
	    *(p+3)=dec;
  	    *(p+4)=unidad;*/
	convertir_usint_to_BCD(p,var);
         
}//fin get a five chars from a uint number that should be unsigned short int
   

// 5421 en chars seria c[]={5,4,3,2,1}
/* obtiene  5 chars de una variable  unsigned int,(bug-> deberia ser short int esta variable
 * MEMORIA 2 BYTES:uchar */
unsigned char getCharsFromUINT_var1(unsigned char *p,unsigned short int var,unsigned char *v){
auto unsigned char i;
auto unsigned char estado;	
const unsigned short int B[]={0x8000,0x4000,0x2000,0x1000,0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};	
const unsigned char DM[]={3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char  M[]={2,6,8,4,2,1,0,0,0,0,0,0,0,0,0,0};
const unsigned char  C[]={7,3,1,0,0,0,5,2,1,0,0,0,0,0,0,0};
const unsigned char  D[]={6,8,9,9,4,2,1,5,2,6,3,1,0,0,0,0};
const unsigned char  U[]={8,4,2,6,8,4,2,6,8,4,2,6,8,4,2,1};  
auto unsigned char ret=0;
    estado=*v;
    i=*(v+1);
    switch(estado){
    	case 1:i=0;estado++;break;
    	case 2:*(p+i)=0;estado++;break;
    	case 3:if(i<5){ 
    		       i++;estado--;}
    		   else{estado++;}
    	       break;
    	case 4:i=0;estado++;break;
    	case 5:if((var&B[i])==B[i])
    			   suma_BCD(DM[i],M[i],C[i],D[i],U[i],p);
    	       estado++;
    	       break;
    	case 6:if(++i<16){estado--;}else estado++;break;
    	case 7:ret=TRUE;estado=0;break;
    	default:estado=1;break;}//fin switch-----------
*v=estado;
*(v+1)=i;
return ret;
}//fin get a five chars from a uint number that should be unsigned short int



//para el angulo solo hasta 180.x 
//se tienen que pasar a double porque en float hay un error de 0.1
unsigned char getCharsFromFloat(unsigned char *frac,float f){
int i;
double ff,frac2;
unsigned char signo;
	    if(f==0){
		    *frac=0;
		    return 0;}
		if(f<0) signo=1; else signo=0;
		i=(int)absf(f);
		ff=(double)f;
		if(i>180) i=0;
		frac2=(absd(ff)-(double)i)*10+0.001;//sacar los decimales
		*frac=(unsigned char)frac2;
return (unsigned char)i;
}//fin de goet chars froam float------------------------------------


//para el angulo solo hasta 180.x 
//se tienen que pasar a double porque en float hay un error de 0.1
unsigned char* getCharsFromFloat1(unsigned char *frac,float f,unsigned char *inst){
auto int i;
auto double ff,frac2;
auto unsigned char signo,estado,*ret;
      
     i=menu.subMenu.aux4_int[*inst];
     ff=menu.subMenu.aux6_d[*inst];
     frac2=menu.subMenu.aux7_d[*inst];
     signo=menu.subMenu.aux2[*inst];
     estado=menu.subMenu.estado[*inst];
     ret=&menu.subMenu.aux1[*inst][0];
     *ret=FALSE;
     switch(estado){
    	 case 1:if(f==0){
 		            *frac=0;
 		            *ret=TRUE;
 		            *(ret+1)=0;
 		            return ret;}
    	 	 	 else estado++;
    	         break;
    	 case 2:if(f<0) signo=1; else signo=0;
    	 	 	i=(int)absf(f);
    	 	 	estado++;break;
    	 case 3:ff=(double)f;estado++;break;
    	 case 4:if(i>180) i=0;
    	        frac2=(absd(ff)-(double)i)*10+0.001;//sacar los decimales  
    	        estado++;
    	        break;
    	 case 5:*frac=(unsigned char)frac2;
    	        estado++;
    	        break;
    	 case 6:*ret=TRUE;
    	 	 	*(ret+1)=(unsigned char)i;
    	 	 	estado=0;break;
    	 default:estado=1;break;}
i=menu.subMenu.aux4_int[*inst]=i;
ff=menu.subMenu.aux6_d[*inst]=ff;
frac2=menu.subMenu.aux7_d[*inst]=frac2;
signo=menu.subMenu.aux2[*inst]=signo;
estado=menu.subMenu.estado[*inst]=estado; 
return ret;
}//fin de goet chars froam float------------------------------------




float get_Float_From_ASCII(unsigned char *p){
float t;	
  t=(*(p)-0x30)*100+(*(p+1)-0x30)*10+(*(p+2)-0x30)+(*(p+4)-0x30)*0.1f;
return t;  
}//fin de get number float form the flase ASCII-------------

float get_Float_from_Phase(unsigned char f,unsigned char fr){
  return((float)(f+((float)fr)/10));	
}//--------------------------------------------------------------


void getASCIIFromFloat(unsigned char *p,float f){
int i;
unsigned char signo,c,dec;
double frac,ff,fff;
	if(f==0){
	  for(i=0;i<4;i++)
		  *(p+i)=0;
	  *(p+0)='0';
	  *(p+1)='.';
	  *(p+2)='0';
	return;}
	if(f<0) signo=1; else signo=0;
	i=(int)absf(f);
	if(i>180) i=0;
    c=(unsigned char)f;
    ff=(double)f;
    frac=ff-(double)c;
	fff=(frac*10)+0.01;
	dec=(unsigned char)fff;
	getASCIIFromUChar1((unsigned char)i,p);
	if(dec<10)
	     *(p+4)=dec+0x30;  
	*(p+3)='.';
return;	
}//fin de obtener los numeros char de un nuemro float.----------------



/*convierte los numeros en sus numeros BCD de 5 cifras para ser desplegas en display
 * */
void convertir_usint_to_BCD(unsigned char *p,unsigned short int var){
unsigned char i;	
const unsigned short int B[]={0x8000,0x4000,0x2000,0x1000,0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};	
const unsigned char DM[]={3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char  M[]={2,6,8,4,2,1,0,0,0,0,0,0,0,0,0,0};
const unsigned char  C[]={7,3,1,0,0,0,5,2,1,0,0,0,0,0,0,0};
const unsigned char  D[]={6,8,9,9,4,2,1,5,2,6,3,1,0,0,0,0};
const unsigned char  U[]={8,4,2,6,8,4,2,6,8,4,2,6,8,4,2,1};
         for(i=0;i<5;i++)
        	  *(p+i)=0;
         for(i=0;i<16;i++)
        	 if((var&B[i])==B[i])
        		  suma_BCD(DM[i],M[i],C[i],D[i],U[i],p);
}//--------------------------------------------------------------------------------

void suma_BCD(unsigned char dm1,unsigned char m1,unsigned char c1,unsigned char d1,unsigned char u1,unsigned char *p){
	 suma_BCD_digito(_DM_,dm1,p);
	 suma_BCD_digito(_M_ ,m1,p);
	 suma_BCD_digito(_C_ ,c1,p);
	 suma_BCD_digito(_D_ ,d1,p);
	 suma_BCD_digito(_U_ ,u1,p);
}//---------------------------------------------------------------------------------

/*  *(p+0)=DM    *(p+1)=M    *(p+2)=C   *(p+3)=D   *(p+4)=U
 * */
void suma_BCD_digito(unsigned char op,unsigned char n,unsigned char *p){
unsigned char u,d,c,m,dm;
        dm=*(p+0);          
	    m=*(p+1);
  	    c=*(p+2);
	    d=*(p+3);
  	    u=*(p+4);
	 switch(op){
		 case _U_:u+=n;
		          if(u>9){         
		             u-=10; 
		             d++;
		             goto BCD1;} 
		          else goto BCDx;;
		          break;
		 case _D_:d+=n;
BCD1:             if(d>9){
		        	 d-=10;
		        	 c++;
		        	 goto BCD2;}
				  else goto BCDx;
 	 	 	 	  break;
		 case _C_:c+=n;
BCD2:             if(c>9){
	                  c-=10;
	                  m++;
	                  goto BCD3;}
				  else goto BCDx;
				  break;
		 case _M_:m+=n;
BCD3:	  	  	  if(m>9){
		  	  		  m-=10;
		  	  		  dm++;
		  	  		  goto BCD4;}
		  	  	  else goto BCDx;
		  	  	  break;
		 case _DM_:dm+=n;
BCD4:      	     if(dm>9)
		     		   break;
		     	   break;
		 default:break;}//fin switch
BCDx:  
  	  	*(p+0)=dm;          
	    *(p+1)=m;
  	    *(p+2)=c;
	    *(p+3)=d;
  	    *(p+4)=u;
	      
}//---------------------------------------------------------------------------------

/*  *(p+0)=DM    *(p+1)=M    *(p+2)=C   *(p+3)=D   *(p+4)=U
 *  ALGORITMO RECURSIVO              */
void suma_BCD_digito2(unsigned char op,unsigned char n,unsigned char *p,unsigned char flo){
//unsigned char x1,x2;
     if(n==0) return;
     if(!flo)
    	 (*(p+op))+=n;
     if(*(p+op)>9){(*(p+op))-=10;
                   (*(p+op-1))++;
     	 	       suma_BCD_digito2(op-1,n,p,1);}     
}//-FIN suma_BCD_digito--------------------------------------------------------------------------------


void convertir_ULInt_to_ASCII(unsigned char *p,unsigned long int var){
unsigned char i;	
const unsigned long int B[]={
	0x80000000,0x40000000,0x20000000,0x10000000,
	0x08000000,0x04000000,0x02000000,0x01000000,	
	0x00800000,0x00400000,0x00200000,0x00100000,		
	0x00080000,0x00040000,0x00020000,0x00010000, 
	0x00008000,0x00004000,0x00002000,0x00001000,
	0x00000800,0x00000400,0x00000200,0x00000100,
	0x00000080,0x00000040,0x00000020,0x00000010,
	0x00000008,0x00000004,0x00000002,0x00000001};	


const unsigned char A5[]={2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char A4[]={1,0,5,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char A3[]={4,7,3,6,3,6,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char A2[]={7,3,6,8,4,7,3,6,8,4,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char A1[]={4,7,8,4,2,1,5,7,3,1,0,0,5,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char DM[]={8,4,7,3,1,0,5,7,8,9,9,4,2,6,3,6,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char  M[]={3,1,0,5,7,8,4,7,8,4,7,8,4,2,1,5,2,6,8,4,2,1,0,0,0,0,0,0,0,0,0,0};
const unsigned char  C[]={6,8,9,4,7,8,4,2,6,3,1,5,2,1,0,5,7,3,1,0,0,0,5,2,1,0,0,0,0,0,0,0};
const unsigned char  D[]={4,2,1,5,2,6,3,1,0,0,5,7,8,4,7,3,6,8,9,9,4,2,1,5,2,6,3,1,0,0,0,0};
const unsigned char  U[]={8,4,2,6,8,4,2,6,8,4,2,6,8,4,2,5,8,4,2,6,8,4,2,6,8,4,2,6,8,4,2,1};
	 for(i=0;i<10;i++)
		  *(p+i)=0;
	 for(i=0;i<32;i++)
		 if((var&B[i])==B[i])
			  suma_BCD2(A5[i],A4[i],A3[i],A2[i],A1[i],DM[i],M[i],C[i],D[i],U[i],p);	
     for(i=0;i<10;i++)
    	  *(p+i)+='0';
}//fin de obtener el ASCII de unsigned Long integer--------------------


void suma_BCD2(unsigned char a5,unsigned char a4,unsigned char a3,unsigned char a2,unsigned char a1,unsigned char dm1,unsigned char m1,unsigned char c1,unsigned char d1,unsigned char u1,unsigned char *p){
	 suma_BCD_digito2(_A5_,a5,p,0);
	 suma_BCD_digito2(_A4_,a4,p,0);
	 suma_BCD_digito2(_A3_,a3,p,0);
	 suma_BCD_digito2(_A2_,a2,p,0);
	 suma_BCD_digito2(_A1_,a1,p,0);
	 suma_BCD_digito2(_DM_,dm1,p,0);
	 suma_BCD_digito2(_M_ ,m1,p,0);
	 suma_BCD_digito2(_C_ ,c1,p,0);
	 suma_BCD_digito2(_D_ ,d1,p,0);
	 suma_BCD_digito2(_U_ ,u1,p,0);
}//---------------------------------------------------------------------------------




//convierte los uchar de 0 a 9 en ASCIIs de '0' a '9' 
void getASCIIfromUCharArray(unsigned char *orig,unsigned char *dest,unsigned char size){
unsigned char i;
	for(i=0;i<size;i++)
       *(dest+i)=*(orig+i)+0x30;
	for(i=0;i<size;i++){
		if(*(orig+i)>0x39){
			 __asm(nop);
			 __asm(Halt);}}
}//fin getASCIIfromUCharArray &menu.v.uchar1[0],&menu.v.uchar2[0],5);





/* dos ASCII's que representan un numero del 00 al 99 , *p apuntador al array donde se guarda los ascci el indice 0 es la decena.
 * se le suma o resta un uno ala decena o unidad, op: SUMAR|RESTAR  digito:LEFT|RIGHT LEFT:decena y el otro unidad*/
void procesar_ASCII(unsigned char *p,unsigned char op,unsigned char digito){//sumar uno al digito decena
unsigned char n;	
	  if(digito==CENTRAL)
		   n=*(p+1);//save the decena one
	  else n=*(p+2);
	  if(!isNumAscii(n)){
		    n=0;
	        __asm(Halt); //debug error de ingenieria de software
	        return;} 
	  if(op==SUMAR){
		  if(n=='9')
			  n='0';
		  else n++;}
	  else{if(n=='0')
	           n='9';
	  	   else n--;}
	  if(digito==CENTRAL)
		  *(p+1)=n;
	  else *(p+2)=n;

}//fin procesar_ASCII-----------------------------------------------------------------------------------------


unsigned char convert_ASCII_to_char(unsigned char *p){
unsigned char n,m,b;	
	 if(isNumAscii(*p))
	  if(isNumAscii(*(p+1)))
	   if(isNumAscii(*(p+2))){
		     //return((*p)*100+(*(p+1))*10+(*(p+2)));}
	        n=((*p)-'0')*100;
	        m=((*(p+1)-'0')*10)+n;
	        b=(*(p+2)-'0')+m;
	        return b;}
return 0;
}//fin convert_ASCII_to_char-------------------------------------------------------------


/* calcula el cambio de digito por digito dela vriable xxx.y de la fase 
 * digito es el digito a cambiar  el orden es  ------> 54321*/
void operacion_ASCII_Phase2(unsigned char digito,unsigned char op,unsigned char *p){ 
float f,s;
	
    f=get_Float_From_ASCII(p);
    switch(digito){
    	case 5:if(op==SUMAR) s=100; else s=-100; break;
    	case 4:if(op==SUMAR) s=10;  else s=-10;  break;
    	case 3:if(op==SUMAR) s=1;   else s=-1;   break;
    	case 2:break;
    	case 1:if(op==SUMAR) s=0.1; else s=-0.1; break;
    	default:break;}
    if(op==SUMAR){
    	if(f>180.0)
    		f=180.0;
    	if(f<180){
    	  if((f+s)<180)	
    		     f+=s;}}
    else{if(f<0) f=0;
         if(f>0){
        	if((f+s)>0){
        		f+=s;}}}
   getASCIIFromFloat(p,f);
}//fin operacionPhase2---------------------------------------------------------------------

void getASCII_from_Fase(unsigned char *p,unsigned char fase,unsigned char frac){
unsigned char b[5],i;
	getASCII_from_UChar(fase,&b[0]);
	*p=b[0];
	*(p+1)=b[1];
	*(p+2)=b[2];
	getASCII_from_UChar(frac,&b[0]);
	*(p+3)='.';
	*(p+4)=b[2];
	for(i=0;i<5;i++)
	   if(i!=3)
	      if(!isNumAscii(*(p+i))){
		   __asm(Halt);//debug error de ingenieria de softwre
		   return;}
    return;
}//fin getASCII_from_Fase-----------------------------------------------------------------




void setASCII_to_Fase(unsigned char *p,unsigned char *fase,unsigned char *frac){
	
	*fase=((*p)-'0')*100+(((*(p+1))-'0')*10)+((*(p+2))-'0');
	*frac=(*(p+4))-'0';
}//fin setASCII_to_Fase--------------------------------


/*convertir voltaje qu e es numero de 0 a 1.2 float en chars 
 * dentro del array al que apunta, N apuntador a un array de SIZE=4
 * RETURN  SIGNO negativo '-' else ' '*/
unsigned char convertir_Voltaje_A_Chars(double voltaje,unsigned char *N,unsigned char tarjeta){

  
    switch(tarjeta){
    	case CANAL_Y:
    	case CANAL_X:  break;
    	case _DRIVER_: break;
    	case _PICKUP_:if(voltaje>1.2){
		                 voltaje=1.2;
    	                 *(N)++='1';
    	                 *(N)++='.';
    	                 *(N)++='2';
    	                 *(N)++='0';
    	                 *N='0';
    	                 return;}
    	              break;
    	default:break;}
cv2:switch(tarjeta){
	     case _PICKUP_: voltagePositivo_a_Chars(voltaje,N); 
			            break;	
	     case  _DRIVER_:voltagePositivo_a_Chars(voltaje,N);
	     			    break;
	     case  CANAL_Y:			    
		 case  CANAL_X:return(voltagePositivo_a_Chars(voltaje,N));
			            break;
			              
		
		default:break;}
return 0;
}//fin de convertir_Voltaje_A_Chars---------------------------------------------------


//regresa el signo en char '-' || ' ' 
unsigned char   voltagePositivo_a_Chars(double voltaje,unsigned char *N){
unsigned short int voltINT;
unsigned char n,signo;
const unsigned char SIZE2=5;
unsigned char c[SIZE2],*p;

            p=&c[0];
            if(voltaje<0){
                 signo='-';
                 voltaje=voltaje*-1;}
            else signo=' ';
            voltINT=(unsigned short int)voltaje;//convertirlo a integer quitar decimales}
			if(voltaje<10)
				 n=0xAA;
		    else{n=0;
			     getCharsFromUINT_var(p,voltINT);}
			if(n==0xAA){//menor  a diez
			   if(voltaje<1){
			     *N='0';
			     *(N+1)='.';}
			   else  {*N=((unsigned char)voltINT)+'0';//de 1 a 9
			          *(N+1)='.';}
			   voltaje=voltaje-(double)voltINT;//nos quedamos con decimales
			   voltaje=voltaje*1000;//se eleva los decimales
			   voltINT=(unsigned short int)voltaje;//se pasan a integer
			   if(voltINT==0){//no hay decimales
				    *(N+2)=*N;
				    *N=' '; *(N+1)=' ';*(N+3)=' ';*(N+4)=' ';
			        return signo;}
			   getCharsFromUINT_var(p,voltINT);
			   *(N+2)=(*(p+2))+'0';
			   *(N+3)=(*(p+3))+'0';
			   *(N+4)=(*(p+4))+'0';}
		   else{
			    *N=(*(p+3))+'0';
				*(N+1)=(*(p+4))+'0';
			   voltaje=voltaje-(double)voltINT;//nos quedamos con decimales
			   voltaje=voltaje*100;//se eleva los decimales
			   voltINT=(unsigned short int)voltaje;//se pasan a integer
			   if(voltINT==0){//no hay decimales
				      *(N+2)=*(N+1);
				      *(N+1)=*N;
				      *N=' ';*(N+3)=' ';*(N+4)=' ';}
			   else{*(N+2)='.';
				   getCharsFromUINT_var(p,voltINT); 
				   *(N+3)=(*(p+3))+'0';
				   *(N+4)=*(p+4)+'0';}}
return signo;
}//fin voltagePositivo_a_Chars---------------------------------------------------


/*   Procesa un array de tamaño 5 y le mete el signo menos al array
 *  entrada "0.002" salida="-0.002"*/
unsigned char Procesamiento_con_Signo(unsigned char *p){
unsigned char v,c,i;	
	  c=*p;
	  switch(c){
		  case '0':for(i=4;i>=1;i--){
		                 *(p+i)=*(p+i-1);}
		           *p='-';
		           for(i=1;i<5;i++){
		        	   if((*(p+i)=='0')||(*(p+i)=='.'))
		        		   v=0xAA;
		        	   else{ v=0; break;}}
		           if(v==0xAA){
		        	  for(i=0;i<5;i++)
		        		   *(p+i)=' ';
		        	  *(p+2)='0'; }//
		           break;
		  case ' ':if(*(p+1)==' '){
			          if((*(p+3)==0x20)&&(*(p+2)=='0'))
		                	   break;
			          else{
		                *(p+1)='-';
		                break;}}
		  	  	   else{*(p+3)=*(p+2);
		  	  	        *(p+2)=*(p+1);
		  	  	        *(p+1)='-';
		  	  	        return 0;
		  	  	        break;}
		  default:*(p+4)=*(p+3);
				  *(p+3)=*(p+2);
				  *(p+2)=*(p+1);
				  *(p+1)=*(p+0);
				  *p='-';
				  return 0;
				  break;
	  }//fin siwtch-----------------------------------
return 0;
}//fin Procesamiento_con_Signo---------------------------------------------------

unsigned char Procesamiento_con_Signo2(unsigned char *p,unsigned char signo){
const unsigned char SIZEA=5;
//unsigned char a[SIZEA];
unsigned char i;

         for(i=0;i<4;i++){
        	 if(isNumAscii(*(p+i))==TRUE){
        		 if(i==0){
        			 if(signo=='-')
        				 return '-';
        			 else return ' ';}
        		 else{if(signo=='-') 
        		           *(p+i-1)='-';
        		      return ' ';}}}
}// Procesamiento_con_Signo2---------------------------------



//entrada  primer parametro el dato signed short int a convertir a char
unsigned char convertir_SignedShortInt_A_Char(signed short int ss,unsigned char *p){
unsigned char signo;
//signed short int ss;
     //ss=g->ssigxc;
     if(ss==0){
    	  *p=' ';
    	  *(p+1)=' ';
    	  *(p+2)=' ';
    	  *(p+3)=' ';
    	  *(p+4)='0';
          signo=' ';
          return signo;}
     if(ss<0){
    	  signo='-';
    	  ss*=-1;}
     getCharsFromUINT_var(p,(unsigned short int)ss);
     convertNum2Chars(p,5);//array de numeros en array de ascii's
return signo;	
}//fin de convertir_SignedShortInt_A_Char--------------------------------------------------

//convierte los numeros que hay en el array uchar 
//en numeros ascii si no son numero los pone en cero
//p: apuntador al array, size tamaño del array
void convertNum2Chars(unsigned char *p,unsigned char size){
unsigned char i;	
	for(i=0;i<size;i++){
	     if(isNum(*(p+i)))
	    	  *(p+i)=*(p+i)+0x30;
	     else *(p+i)=0;}
	for(i=0;i<(size-1);i++)//quitar los ceros de izquierda
		if(*(p+i)=='0')
			*(p+i)=' ';
		else break;
}//fin  convertNum2Chars

//convierte 5 characteres ascii en un numero UINT no mayor de 99999
unsigned short int convertCharstoUINT(unsigned char *p){
unsigned short int v=0;    
	//signed short int i;
  	     v= (unsigned short int)(*p++)*10000;//0 
	     v+=(unsigned short int)(*p++)*1000; //1
   	     v+=(unsigned short int)(*p++)*100;   //2
	     v+=(unsigned short int)(*p++)*10;  //3
   	     v+=(unsigned short int)(*p);   //4
	    //for(i=10000;i>0;i/=10) 
    	  //        v+=p++*(unsigned int)i; 
	    return v;
}//fin convertit 5 chars to variable UINT de maximo 99999
	

/*obtiene de un unsigned char los tres charactes para ser despleados en pantalla
* podemos mejorar este algoritmo para que calcule slo 23 veces enlugar de 255 veces pero necesitamos el
debueo en vivo    *p:regreso del array ascci,  n:numero uchar a  obtener ascii, modoAscii:regreso en modo ascii o numero  
  POINTER de regreso EL indice 0 es centenas, el 1 DECENAS y el 2 unidades */
void getUChar2Chars(unsigned char *p,unsigned char n ,char modoAscii){
unsigned char i=0,u=0,d=0,c=0,j=0;
unsigned char nc1[]={0,1,2,3,4,5,6,7,8,9,10};//"0123456789a";
unsigned char nc2[]="0123456789a";
    	    
	     while(1){
    	           if(n==i++){
	               if(modoAscii==YES){//YES REGRESA numero en ascii
    	                     *p=nc2[c];*(p+1)=nc2[d];*(p+2)=nc2[u];}
	               else{*p=nc1[c];*(p+1)=nc1[d];*(p+2)=nc1[u];}
    	                       return;}
     	           if(++u>9){ u=0;
    	              if(++d>9){ d=0;c++;}}
	               if(j++>=254) break;//safe contra un bug
    	     }//fin while------------------------------
}// convertir un numero char unsigned a tres characteres ascii--------------------------




//encontrar el ultimo espacio del ultimo caracter
unsigned char findLastChar(unsigned char *s,unsigned char size){
unsigned char i,a;//1234123123123
      i=size;       //             ^
	  do{a=*(s+i);
		  if(isNumLetter(a)){
			 if(*(s+i)==' '){
				 a=*(s+i-1);
				 if(isNumLetter(a)){
					 if(*(s+i-1)==' ')
						 i--;
					 else return i;}
				 else i--;}
			 else{return i+1; }}
		 else i--;
	  }while(i!=0);		 
}//fin de encontrar el ultimo espacio del ultimo caracter

//insertamos un espacio y recorremos los char uno a la derecha para Text de text Processor
void TextInsertSpace(unsigned char indice){//indice es en donde esta actualmente el cursor
unsigned char i=0,j=0,k=0;
	  if(indice>=(NOMBRE_PRODUCTO_SIZE-1)){
		   Text[NOMBRE_PRODUCTO_SIZE-1]=' ';
	       return;}
	  i=NOMBRE_PRODUCTO_SIZE-1;
	  do{ if(i==indice){
		      Text[i]=' ';
		      break;}
		  Text[i]=Text[i-1];
	     }while(i--!=0);
}//fin insertamos un espacio y recorremos los char uno a la derecha para Text de text Processor


//insertamos un espacio y recorremos los char uno a la derecha para Text de text Processor
unsigned char TextBackSpace(unsigned char indice){//indice es en donde esta actualmente el cursor
unsigned char i=0,j=0,k=0;             //indice va de 0 a 19=PRODUCT_SIZE..blabla
	  if(indice==0)
		  return 0;
	  i=0;
	  while((i+1)!=indice){
	        i++;}
	  if(i==NOMBRE_PRODUCTO_SIZE-2)
		     Text[NOMBRE_PRODUCTO_SIZE-1]=0;
      for(j=i;j<(NOMBRE_PRODUCTO_SIZE-1);j++)
         	  Text[j]=Text[j+1];
      return i;     
}//fin insertamos un espacio y recorremos los char uno a la derecha para Text de text Processor

//* Logitud de la cadena si es numero o letra*/
word length(unsigned char *c,unsigned char size){
word count=0;                            
unsigned char i=1,estado=0;

  for(i=0;i<size;i++)
    switch(estado){
	   case 0:if(*c!=NAME_INIT) //primer char es ame init?
		          insertChar(c,size,0,NAME_INIT); 
	          estado++;
	          break;
	   case 1:if(*(c+1)==0)
		         return 0;
	          if(isNumLetter(*(c+1)))
	        	   estado++;
	          else{ *(c+1)=0;return 0;}
	          break;
	   case 2:if(isNumLetter(*(c+i)))
		            break;
	          else return ((unsigned short)(i-1));
              break;
	   default:break;}       
return size;  
}//ends the char chain lenght assess .---------------------------------------------------------------

   


word length2(unsigned char *c){
word count=0;
	unsigned char i=1;
		 if(*c==NAME_INIT){
			 for(i=1;i<NOMBRE_PRODUCTO_SIZE;i++){
			   if(isNumLetter(*(c+i)))
			       count++;
			   else return count;} }
		 else //este es un error de software o memoria
			 return 0;//debug aqui se necesita el manejo de errores  
		 if(count==(NOMBRE_PRODUCTO_SIZE-1))
			 return count;
return 0;		
}//ends the lenght assess for a pointer to string

/*return true if a it is a number from 0 to 9*/
unsigned char isNum(char a){
	if((a<=9)&&(a>=0))
			return TRUE;
	else return FALSE;
}//isNum----------------------------------------------------------

//revisa si el numero es ascii numero '0' hasta '9'
unsigned char isNumAscii(char a){
	if((a<='9')&&(a>='0'))
			return TRUE;
	else return FALSE;
}//is isNumAscii--------------------------------------------------


unsigned char isNumLetter(char a){
   
	 if(a==0x20)
		  return TRUE;
	 if(a==0)
		 return FALSE;
	 if((a<0x5B)&&(a>0x40))  //A=65h, Z=5Ah
		 return TRUE;
	 if((a<0x7B)&&(a>0x60)) //a=61h, z=7Ah
		 return TRUE;
	 if((a<0x3C)&&(a>0x2f))//0=30h  9=39h  :3A ;3B
		 return TRUE;
	 if(a==0xA5)//Ñ
	    return TRUE;
	 if(a==0xA4)//ñ
	    return TRUE;
	 if((a<0xA4)&&(a>0x7B))//   7C<=symbols<=A3
		return TRUE;
	 if(a==0x2C)
		 return TRUE;//','=2Ch
	 if(a==0x2E)//'.'=2Eh
		 return TRUE;
	 else 
		return FALSE;
}//FIN DE LA FUNCion que determina si el ascci es numero o letra

/* Regresa el tamaño real util del array, regresa el numero de datos
 *  que hay en el array sin tomar en cuenta los ceros de dato cero
 *  *p:pointer to array,,,,, size: tamaño total del array
 *     version-1 */
unsigned short int sizeof1(unsigned char *p,unsigned short int size){
unsigned short int i;
    i=size;
	for(i=size-1;i>0;i--)
		 if(*(p+i)!=0)
			  break;
	return(i);
}//fin de obtener el tamaño delos datos utiles del array............


/*da formato a los numero unsigned short int e.g.  n="00032" resultado="32"000 
 *   e.g.    n="00000"  res="0"0000    e.g. n="01234" res="1234"0
 *    para poder mandarlo al display aparti del indice cero que es el MSB  
 *    regresa el numero de digitos a desplegar en el display de 1 a 5  
 *     size: es el tamaño del origen
 *     n: REGRESA  el numero de chars a desplegar*/
void Formato_USInt(unsigned char *orig,unsigned char *dest,unsigned char size,unsigned char *n){
auto unsigned char estado;
auto unsigned char ret=FALSE;
auto unsigned char i=0,j;
unsigned char ret2;
  while(ret!=TRUE){
	if(i>5){__asm(nop);__asm(Halt);}  
    switch(estado){// 00000
    	case 1:if((*(orig+i)>='0')||(*(orig+i)<='9')){
    		      if(*(orig+i)=='0'){//regresa el indice donde  esta el primero a desplegar
    		    	   if(i<4){i++;}
    		    	   else{estado++;}}
    		      else{estado++;}}
    		   else{__asm(nop);__asm(Halt);}
    		   break;
    	case 2:if(i>0)
    		      estado++;
    		   else estado=9;
    	       break;//01234, "xxxx0" "xxx00" "xx000"
    	case 3:if(i>4){__asm(nop);__asm(Halt);}//no debe se mayor a 4
    		   estado++;j=0;ret2=5-i;
    		   break;
    	case 4:if(j>4){ estado++;j=0;}//limpiamos destino
    	       else {*(dest+j)=0;j++;}
    	       break;
    	case 5:*(dest+j)=*(orig+i);
    	       if(++i>4)
    	    	     estado++;
    	       else{j++;}
    	       break;
    	case 6:ret=TRUE;break;
    	case 9:ret2=5;//no ha habido nada que recorrer porque e.g. var="54321"
    		   vaciar_A2B(orig,dest,0,size-1);
    	       ret=TRUE;
    	       break;
    	default:estado=1;ret=0;break;}	
    }//fin while ------------------------------
*n=ret2;
}//fin Formato_USInt-------------------------------------------------------------

/* da formato de un numero long integer *o origen *d destino array 
 * a: es el digito apartir de cual se va a deplegar en pantalla
 * "0000000xxxxxx" se ignoran los ceros izquiedos*/
unsigned char Formato_ULInt(unsigned char i,unsigned char *p){
    if(*(p+i)=='0')
	   return (Formato_ULInt(i+1,p));
   else{if(i>8)
             return 9;
        else {return i;}}
}//fin -------formato de lomg int-----------------------------

/*  Le quitamos alos numeros de 5 chars los 
 * caracteres que no son '9'..'0'*/
void clearFormato(unsigned char *a){
unsigned char j;
      for(j=0;j>4;j++){
         if((*(a+j)>'9')||(*(a+j)<'0'))
    		    *(a+j)=' ';}
}//fin clear dformato----------------------------------------


/*formate los ascii dela phase para quitar los ceros a la izquierda que no
 * se necesita desplegar en la pantalla */
unsigned char Formato_Phase(unsigned char *orig){
unsigned char *a;
unsigned char k,i,m;
    a=orig;
	for(k=0;k<5;k++){
	   if(i==0xAA){break;}
		  switch(k){
				case 0:if(*(a+k)=='0')
					           m=1;
				       else {i=0xAA;m=0;}
				       break;
				case 1:if((m==1)&&(*(a+k)=='0'))
							m=2;
				       else {i=0xAA;m=1;}
				       break;
				case 2:if((m==2)&&(*(a+k)=='0'))
							m=3;
					   else {i=0xAA;m=2;}
				       break;
				case 3:if((m==3)&&(*(a+k)=='0'))
							m=4;
					   else {i=0xAA;m=3;}
				       break;
				case 4:break;
				default:__asm(nop);__asm(Halt);}}
		 if(m>5){__asm(nop);__asm(Halt);}
		 if((*(a+3)=='.')&&(*(a+2)=='0')&&(m==3))
			   m=2;
		 for(i=0;i<m;i++)
			  *(orig+i)=' ';
return m;		 	
}//fin Formato_Phase--------------------------------------------------


/*  vaciar cadena A -> B con los indices indicados
 * inidice inicial i, indice final f
 *  final cuenta desde indice 0 al ultimo indice deseado y es todo serial sizeof(a)-1*/
void vaciar_A2B(unsigned char *a,unsigned char *b,unsigned char i,unsigned char f){
unsigned char ii;	
	if(i>f)
		return;
	if(f==0)
		return;
	if(i==f)
		return;
	for(ii=i;ii<(f+1);ii++)
		*(b+ii)=*(a+ii);
}//fin vaciar_A2B--------------------------------------------------------------


/* arr: es el array a limpiar, size es el tamaño del array, k es la kosntante
 * que se va allenar el array  */
void cleanArray(unsigned char *arr,unsigned short int size,unsigned char k){
unsigned short int i;        
	for(i=0;i<size;i++)
	     *(arr+i)=k;
}//fin de clean array----------------------------------------------------------------

/* pasamos lo de una array al otro array y lode otro array al primer array
 *  a: array 1, b: array2, size: tamaño de los arrays*/
void swapArrays(unsigned char *a,unsigned char *b,unsigned char size){
unsigned char i,k;	
	for(i=0;i<size;i++){
	   k=*(a+i);
	   *(a+i)=*(b+i);
	   *(b+i)=k;}
}//fin swapArrays-----------------------------------------------------------------


//convierte los char del array en ASCII--------------------------
void get_Char2ASCII(unsigned char *orig,unsigned char *dest,unsigned char size){
unsigned char i;
    for(i=0;i<size;i++){
    	if((*(orig+i)<=9)||(*(orig+i)>=0))
     	       *(dest+i)=*(orig+i)+'0';
    	else{__asm(nop);__asm(Halt);}}
}//fin get_Char2ASCII--------------------------------------------------------------



/* Limpia un array que tiene chars despues de donde termina la cadena
 * si detecta un k apartir de alli todos seran M*/
void cleanArrayName(unsigned char *a,unsigned char size,unsigned char k,unsigned char m){
unsigned char i,estado=0;;
    for(i=0;i<size;i++){
      switch(estado){
    	  case 0:if(*(a+i)==k)
    		        estado++;
    		     break;   
    	  case 1:*(a+i)=m;break;
    	  default:i=0;estado=0;break;}}
}//fin de cleanArrayName------------------------------------------------------------------

/* insertar el char en el indice indicado en index, 
 * pero se recorre todo los char a la izquierda */
void insertChar(unsigned char *s,unsigned char size,unsigned char index,unsigned char c){
unsigned char i,b,a;
    b=c;
	for(i=index;i<size;i++){
	     a=*(s+i);
	     *(s+i)=b; 
		 b=a;}
}//fin insertchar----------------------------------------
