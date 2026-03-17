/*
 * Reloj.h
 *
 *  Created on: Jul 20, 2015
 *      Author: chals
 */

#ifndef RELOJ_H_
#define RELOJ_H_




#define _HORA_  0xB0U
#define _MES_   0xB1U
#define _MIN_   0xB2U
#define _YEAR_  0xB3U
#define _DIA_   0xB4U //dia del mes
#define _DIAS_  0xB5U //dia de la semana de domingo a lunes 
#define _SEG_   0xB6U


struct _TIME_DATE{
	  unsigned char seg;
	  unsigned char min;
	  unsigned char hora;
	  unsigned char dia;// de lunes a viernes de 1 a 7
	  unsigned char fecha;//numero del dia de mes
	  unsigned char mes;
	  unsigned char year;
};




unsigned char  gethora(void);
unsigned char getYear(void);
unsigned char getMonth(void);
unsigned char getDate(void);
unsigned char getminutos(void);
unsigned char getsegundos(void);

void setYear(unsigned char n);
void setMonth(unsigned char n);
void setDate(unsigned char n);
void sethora(unsigned char n);
void setminutos(unsigned char n);
void setsegundos(unsigned char n);
void saveClockMoficado(void);
unsigned char convertDate(unsigned char n,unsigned char tipo);
void init_Reloj(void);
void displaySegundero_IRQ(void);
void segundero(unsigned char status);
unsigned char  convertBCD2Binary(unsigned char n);
unsigned char convertBinary2BCD(unsigned char n);
void saveTimeNVRAM(void);
unsigned char getDay(void);
void setDay(unsigned char d);
unsigned char getControl(void);
void setControl(unsigned char c);
void getTimeNVRAM(void);//gets time var and pours into embedded time vars
void readclock(struct _TIME_DATE *td);
unsigned long int get_TimeStampYear(unsigned char year);
unsigned long int get_TimeStampMonth(unsigned char month);
unsigned long int get_TimeStampDay(unsigned char day);
unsigned long int get_TimeStampHour(unsigned char hora);
unsigned long int get_TimeStampMin(unsigned char min,unsigned char seg);
unsigned long int get_TimeStamp(void);



#endif /* RELOJ_H_ */
