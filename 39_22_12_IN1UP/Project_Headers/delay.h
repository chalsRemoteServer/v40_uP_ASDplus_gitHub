/*
 * delay.h
 *
 *  Created on: Sep 12, 2015
 *      Author: chals
 */

#ifndef DELAY_H_
#define DELAY_H_





void delay_ms(unsigned short int t);
void delay1ms(void);
void delay1us(void);
void delay_us(unsigned short int t);
//void delay_us_VFD(unsigned short int t);
//unsigned char delay_us_VFD(unsigned short int t);
unsigned char delay_us_VFD(unsigned short int t);
unsigned char delay_ms_VFD(unsigned short int t);
//unsigned char delay_us_v2(unsigned short int t,unsigned char *p);

unsigned char delay_us_VFD_exclusivo(unsigned short int t);
//unsigned char delay_ms_VFD_exclusivo(unsigned short int t);
unsigned char delay_us_v3(unsigned short int t,unsigned char *p);
unsigned char delay_general_1ms(unsigned long long int *delay1,unsigned long long int t_deseado,unsigned char *inst);
unsigned char delay_ms_OS(unsigned short int t);
unsigned char buscar_PID(unsigned char *regpid);
unsigned char buscar_Thread_Libre(unsigned char *regPid);
unsigned char thread_sleep(unsigned short int time);



#endif /* DELAY_H_ */
