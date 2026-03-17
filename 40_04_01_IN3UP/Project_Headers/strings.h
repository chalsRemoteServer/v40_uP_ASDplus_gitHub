/*
 * strings.h
 *
 *  Created on: Mar 5, 2020
 *      Author: desarrollo-1
 */

#ifndef STRINGS_H_
#define STRINGS_H_



#endif /* STRINGS_H_ */
#include "PE_Types.h"

unsigned char compareString(unsigned char *a,unsigned char *b,unsigned char size);
void getASCIIFromUChar1(unsigned char n,unsigned char *cc);
//unsigned char TextInsertSpace2(unsigned char indice);
void getCharsFromUINT_var(unsigned char *p,unsigned short int var);
//void convertir_Voltaje_A_Chars(double voltaje,unsigned char *N,unsigned char tarjeta);
unsigned char   voltagePositivo_a_Chars(double voltaje,unsigned char *N);
unsigned char convertir_Voltaje_A_Chars(double voltaje,unsigned char *N,unsigned char tarjeta);
unsigned char convertir_SignedShortInt_A_Char(signed short int ss,unsigned char *p);
unsigned short int convertCharstoUINT(unsigned char *p);
void getUChar2Chars(unsigned char *p,unsigned char n ,char modoAscii);
unsigned char findLastChar(unsigned char *s,unsigned char size);
void TextInsertSpace(unsigned char indice);
unsigned char TextBackSpace(unsigned char indice);
word length(unsigned char *c,unsigned char size);
word length2(unsigned char *c);
unsigned char isNumLetter(char a);
unsigned char isNum(char a);
void convertNum2Chars(unsigned char *p,unsigned char size);
unsigned char Procesamiento_con_Signo(unsigned char *p);
unsigned char Procesamiento_con_Signo2(unsigned char *p,unsigned char signo);
unsigned char isNumAscii(char a);
void convertir_usint_to_BCD(unsigned char *p,unsigned short int var);
void suma_BCD(unsigned char dm1,unsigned char m1,unsigned char c1,unsigned char d1,unsigned char u1,unsigned char *p);
void suma_BCD_digito(unsigned char op,unsigned char n,unsigned char *p);
void getASCII_from_UChar(unsigned char n,unsigned char *p);
void procesar_ASCII(unsigned char *p,unsigned char op,unsigned char digito);
unsigned char convert_ASCII_to_char(unsigned char *p);
void operacion_ASCII_Phase2(unsigned char digito,unsigned char op,unsigned char *p);	
void getASCII_from_Fase(unsigned char *p,unsigned char fase,unsigned char frac);
void setASCII_to_Fase(unsigned char *p,unsigned char *fase,unsigned char *frac);
unsigned short int sizeof1(unsigned char *p,unsigned short int size);
void vaciar_A2B(unsigned char *a,unsigned char *b,unsigned char i,unsigned char f);
void cleanArray(unsigned char *arr,unsigned short int size,unsigned char k);
void swapArrays(unsigned char *a,unsigned char *b,unsigned char size);
void cleanArrayName(unsigned char *a,unsigned char size,unsigned char k,unsigned char m);
void insertChar(unsigned char *s,unsigned char size,unsigned char index,unsigned char c);
void getCharFromFloat(unsigned char *p,float f);
unsigned char getCharsFromFloat(unsigned char *frac,float f);
float get_Float_from_Phase(unsigned char f,unsigned char fr);
void getASCIIFromFloat(unsigned char *p,float f);
float get_Float_From_ASCII(unsigned char *p);	
unsigned char strcpy1(unsigned char *dest,unsigned char *orig,unsigned char size );
unsigned char strcpy2(unsigned char *dest,const unsigned char *orig,unsigned char size );
void getASCIIfromUCharArray(unsigned char *orig,unsigned char *dest,unsigned char size);
void Formato_USInt(unsigned char *orig,unsigned char *dest,unsigned char size,unsigned char *n);
void get_Char2ASCII(unsigned char *orig,unsigned char *dest,unsigned char size);
unsigned char Formato_Phase(unsigned char *orig);
void suma_BCD2(unsigned char a5,unsigned char a4,unsigned char a3,unsigned char a2,unsigned char a1,unsigned char dm1,unsigned char m1,unsigned char c1,unsigned char d1,unsigned char u1,unsigned char *p);
void convertir_ULInt_to_ASCII(unsigned char *p,unsigned long int var);
void suma_BCD_digito2(unsigned char op,unsigned char n,unsigned char *p,unsigned char flo);
unsigned char Formato_ULInt(unsigned char i,unsigned char *p);
unsigned char* getCharsFromFloat1(unsigned char *frac,float f,unsigned char *inst);
unsigned char getCharsFromUINT_var1(unsigned char *p,unsigned short int var,unsigned char *v);
void clearFormato(unsigned char *a);
