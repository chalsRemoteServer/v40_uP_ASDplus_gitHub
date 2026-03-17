
#ifndef MATHS_H_
#define MATHS_H_



#include "PE_Types.h"


union FloatChar {
	float f;
	unsigned char c[4];
};


struct Coorduc {
	unsigned char x;
	unsigned char y;
};

struct Coordssi{
	signed short int x;
	signed short int y;
};

struct Coordusi{
	unsigned short int x;
	unsigned short int y;
};


struct Coordsuc {
	struct Coorduc Pa;
	struct Coorduc Pb;
};


struct Coordf {//coordenadas float
	float x;
	float y;
};

struct Coorducf{
  struct Coorduc uc;
  float   f;
};

struct Coordsf {//coordenadas float  pero dos coordenadas
	struct Coordf Pm;
	struct Coordf Pn;
};

struct Ec_Recta {
	float m;
	float b;
};

struct Coordsf AgetEcuation(struct Ec_Recta* f, struct Coordf P1, float angulo);
struct Ec_Recta getEcuation(struct Coordf P1, struct Coordf P2);
struct Coordsf get_x2r(unsigned short int s, struct Ec_Recta f, struct Coordf P);
struct Coordsf get_Evaluar_(struct Coordsf P, struct Ec_Recta f);
double raizCuadrada(unsigned short int numero);


signed short int absSSInt(signed short int a);
double absd(double m);//get absoluto from double
float  absf(float m);//get absolute from float
unsigned long int absul(unsigned long int m);


float tan1(float deg);
void operacionPhase(unsigned char digito, unsigned char op);
void operacionUShortInt(unsigned short int* n, unsigned char digito, unsigned char op);
signed short int getMaximun(unsigned char zoom);
float redondearFloat(float f);
double convertir_ADC_word_A_miliVolt(unsigned short int ADCval, unsigned char channel);
//unsigned char getCheckSUM(unsigned char cmd, unsigned char* data, unsigned char len);
unsigned char FIFOcount(unsigned char var, unsigned char op);
void getBytes_from_SSInt(unsigned char* byteMSB, unsigned char* byteLSB, signed short int ssi);
unsigned char getSigno_Convert(signed short int* n);
unsigned char is_get_number_from_pixel(unsigned char* x, unsigned char* y);
void re_Calcular_Buffers_DDS_Modificar_Uno(unsigned char zoom1);
void re_Calcular_Buffers_DDS(unsigned char zoom_wished);
unsigned char get_Coord_Num(unsigned char xn, unsigned char bit);
unsigned char get_cuadrante(unsigned char xi, unsigned char yi);
void reconvertir_coord_reducir_uno(unsigned char xn, unsigned char yn, unsigned char cuadrante, unsigned char* xr, unsigned char* yr, unsigned char palanca);
//unsigned char getCheckSUM(unsigned char cmd, unsigned char* data, unsigned char len);
float seno(float valor);
float tan1(float deg);
void generador_de_signal_Debug(signed short int* x, signed short int* y);
unsigned char convert_BCD_to_uchar(unsigned char BCD);
float convert_Phase_to_Float(unsigned char fase, unsigned char frac);
double raizCuadrada(unsigned short int numero);
float  raizCuadf(float r);
float tanr(float x);
struct Ec_Recta getEcuation(struct Coordf P1, struct Coordf P2);
struct Coordsf AgetEcuation(struct Ec_Recta* f, struct Coordf P1, float angulo);
void cifras(float r, int* aprox, int* miniaprox);
struct Coordsf get_Puntos_Perpendiculares(float m, struct Coordf P, unsigned short int s);
//struct Coordsuc traducirCoordenadas_to_VFD(struct Coordsf f);
struct Coordsf get_x2r(unsigned short int s, struct Ec_Recta f, struct Coordf P);
struct Coordsf get_Evaluar_(struct Coordsf P, struct Ec_Recta f);
float get_Float_Fase(unsigned char* p);
struct Coordf get_Evaluar(struct Coordf P, struct Ec_Recta f);
float Raiz2(float num);
float atan1(double a);
float rad_a_deg(double rad);
float deg_a_rad(double deg);
struct Coordsf AgetEcuation2(struct Ec_Recta* f, struct Coordf P1, float angulo);
unsigned char getFloat_uchar(float n);
unsigned char getFloat_Frac(float n);
struct Coordsf AgetEcuation3(struct Ec_Recta *g,struct Ec_Recta f, struct Coordf P, float angulo);
double tan4(double angulo);
double sin(double g);
double cos(double g);
float pow1(double num1, unsigned char pot);
double factorial(unsigned char a);
//float fyrecta(struct Ec_Recta* f, float y);
//float  frecta(struct Ec_Recta* f, float x);
struct Coordsf get_Evaluar_3(struct Coordsf P,struct Ec_Recta f);
struct Coordsf get_Evaluar_2(struct Coordsf P,struct Ec_Recta f,float angulo);
float frecta(struct Ec_Recta* f,char c, float x);
struct Ec_Recta resolvEcuation(struct Coordf P1, struct Coordf P2);
//struct Coordsf get_Puntos(struct Coordsf P,struct Ec_Recta f,float angulo);
struct Ec_Recta getEcuacion_Recta(struct Coordf P1,float angulo);
struct Coordsf get_Puntos_Perpendiculares(float m, struct Coordf P, unsigned short int s) ;
struct Coordsf get_Puntos(struct Ec_Recta f,float angulo,unsigned char linea,unsigned short int s);
float calculo_pendiente_inversa(float ang);
struct Ec_Recta getEcuacion_Recta_Punto_Pendiente(struct Coordf P,float m);
float pow1f(float num1, unsigned char pot);
struct Coordsf busqueda_Puntos_Altura(struct Ec_Recta f,unsigned  short int s);
float get_angulo_Perpendicular(float m,float angulo);
struct Coordf  get_Puntos_de_Interseccion(struct Ec_Recta f,struct Ec_Recta g);
float Obtener_Mitad_Recta(float M,float m);
struct Coordf Buscar_Punto_en_Recta_auna_Distancia(unsigned char dir,struct Ec_Recta f,struct Coordf P,unsigned short int s);
float Buscar_Maximo_para_Busqueda_Binaria(unsigned char Dir,float i,struct Ec_Recta f, struct Coordf P,unsigned short int s);
float dist2Pnts(struct Ec_Recta f,struct Coordf P1,struct Coordf P2);
double Raizd(double num);
double pow1d(double num1, unsigned char pot);
float distancia_Entre_Puntos(struct Ec_Recta f,struct Coordf P1,float x2);
float Busqueda_Binaria_de_un_Punto(float xM,struct Ec_Recta f,struct Coordf P1,unsigned short int s,float e, float xmin);
struct Coorducf get_MaxDDSzoom(unsigned char zoom);
unsigned char inZoom(struct Coordsf P1,unsigned char zoom1);
unsigned char isCoords_inZoom(struct Coordsf P1,unsigned char zoom1);
//struct Coordsf Procesar_puntos_Altura(struct Coordsf P,struct Ec_Recta A);
//struct Coordsuc traducirCoordenadas_to_VFD(struct Coordsf f,struct Ec_Recta fx);
struct Coordusi Maximo_Zoom(unsigned char z);
//unsigned char isDisplayable_in_ACtual_Zoom(float x,float y,unsigned char zoom);		   
struct Coordf Centrar_Puntos_en_DDS(struct Coordf P);
unsigned char isDisplayable_in_ACtual_Zoom(struct Coordf P,unsigned char zoom1);
unsigned char find_Cuadrante(struct Coordf P);
struct Coordf Centrar_Puntos_en_DDS(struct Coordf P);

//signed short int absusi(unsigned short int a);
struct Coordsuc traducirCoordenadas_to_VFD_v2(struct Coordsf f,struct Ec_Recta fx);
struct Coordsuc traducirCoordenadas_to_VFD(struct Coordsf f);
unsigned char getCheckSUM(unsigned char cmd, unsigned char *data, unsigned char len);
unsigned char Display_en_DDS(struct Coordf *P,struct Coorduc *Pc,struct Coordusi *P1,unsigned char *inst,double *instd);
unsigned char Convert_2_DDS(unsigned char *ret1,struct Coorduc *Pc,struct Coordusi *P1,unsigned char *inst,double *instd);
unsigned char Procesador_Central_de_Cetrado_de_Datos_v2(void);
unsigned char  Detection_Y_Analisis_v2(signed short int x, signed short int  y);
struct Coordf find_Punto_from_Distancia_y_Recta(struct Ec_Recta f,float Max,struct Coordf P,unsigned short int sens,unsigned char dir);
struct Coordsf get_Puntos_Perpendiculares_v2(struct Ec_Recta f, struct Coordf P, unsigned short int s);
unsigned char is_graficable(struct Ec_Recta gx,struct Ec_Recta hx);
unsigned char Centrado_Y_Offset_de_Datos_v3(unsigned char *inst,signed short int *ssi);
unsigned char Monitor_de_Offset(unsigned char *i,signed short int *ssi);
unsigned char Control_de_Rechazo(void);
unsigned char Remove_Offset_de_Datos_v4(signed short int Raw,signed short int *Cent,signed short int *av1,unsigned char *inst,signed short int *ssi);
unsigned char Monitor_de_Offset_v2(signed short int Cent,unsigned char *i,signed short int *ssi);
unsigned char Monitor_de_Offset_v7(signed short int Raw,signed short int *Cent,signed short int *av1,unsigned char *i);
unsigned char DC_Blocker_Filter_v1(signed short int Raw,signed short int *Cent,unsigned char *inst);
byte CheckSumByte(byte dato,byte suma);

#endif

