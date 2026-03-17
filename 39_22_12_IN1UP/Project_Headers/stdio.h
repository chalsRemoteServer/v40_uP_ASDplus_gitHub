/*
 * stdio.h
 *
 *  Created on: Feb 22, 2024
 *      Author: desarrollo-1
 */

#ifndef STDIO_H_
#define STDIO_H_


#include "queue.h"


/* SISTEMA DE ARCHIVOS*/
#define O_RDONLY 1//Open for reading only
#define O_WRONLY 2//open for writing only
#define O_RDWR   4//open for reading and writing
#define O_APPEND 8//open for reading and writong
#define O_CREAT 16// if O_RDWR is set,this causes the file  to be created if it does not exist and truncated if it does, Without this flag,. O_RDWR would expect the file to exist.
#define O_EXCL  32 //if O_EXCL and O_CREAT are set, "open" will fail if the exists and "errno" will be set ro EEXIST, if the file does not exist, "open" will fail and "errno" will be set to EACCESS
#define O_TRUNC 64//open the file for writinf, if  the O_RDWR flag is set,the file is truncated to 0 or created if necessary

#define O_r       0xE1
#define O_rb      0xE2
#define O_w       0xE3
#define O_wb      0xE4
#define O_a       0xE5
#define O_ab      0xE6
#define O_rPlus   0xE7
#define O_rbPlus  0xE8
#define O_wPlus   0xE9
#define O_wbPlus  0xEA
#define O_aPlus   0xEB
#define O_abPlus  0xEC


#define ENOENT 0xF1 //"name" was the NULL pointer
#define EACCES 0xF2 //"open" was unable to open file
#define EEXIST 0xF3 //the file was successfully opened, but the O_CREAT and O_EXCL flag were both set.Because of this, the file was immediately closed again.

#define SEEK  0x01 //System call, que se le va hacer al llamado al sysstema


struct FILE *fopen(const char *file,const char *mode,byte *mem);
byte fwrite(const void *p,byte size,byte nelemts,struct FILE *f);
byte fputs(char *s,struct FILE f);
char fputc(char c,struct FILE *f);	
char *fgets(char *s,byte n,struct FILE *f);
void rewind(struct FILE *f);
byte fclose(struct FILE *f);
char *fgetc(struct FILE *f);
byte *fread(void *p,byte size,byte nelemts,struct FILE *f);
struct FILE *fopen(const char *file,const char *mode,byte *mem);
byte *fseek(struct FILE *f,word offset,byte origin);
unsigned char parseMode(const char *mode);

#endif /* STDIO_H_ */
