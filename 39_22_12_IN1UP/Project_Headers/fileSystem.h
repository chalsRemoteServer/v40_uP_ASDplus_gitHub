/*
 * fileSystem.h
 *
 *  Created on: Jan 28, 2024
 *      Author: desarrollo-1
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "PE_Types.h"



//#define FILE_NAME_SIZE 10//tamaño del nombre de archivo maximo
#define SEEK_SET  0xA3
#define SEEK_CUR  0xA4
#define SEEK_END  0xA5
#define SIZE_MEMORIA_EEPROM_EN_BITS 1024000
#define TAMAÑO_MEMORIA_EEPROM_EN_BYTES 1024000/8  //128000bytes
#define SIZE_DE_BLOCK 50 //50 bytes
#define SIZE_TABLES_ORGA 10000//bytes for tables asignation size file system
#define SIZE_MEMO_PARA_DATOS TAMAÑO_MEMORIA_EEPROM_EN_BYTES-SIZE_TABLES_ORGA//118000
#define NUMERO_DE_BLOCKS SIZE_MEMO_PARA_DATOS/SIZE_DE_BLOCK //2350
#define NUM_BLOCKS_FILES NUMERO_DE_BLOCKS //numero de bloques para datos 
//#define NUM_FILES  NUMERO_DE_BLOCKS //2350, NUMERO MAXIMO DE ARCHIVOS
#define BLOQUES_BITS NUMERO_DE_BLOCKS/8  //293  // tabla de bits, para ver los bloques disponibles,1Mbit=125kbytes blocks de 50bytes, 2500 blocks, numero de bytes par mapear los block usados,y disponibles de la memo entera
#define SIZE_TABLA_FILES NUM_FILES//4700 numero de archivos que podemos manejar maximo
#define SIZE_INODE_DATA_HEAD SIZE_DE_BLOCK-FILE_NAME_SIZE-sizeof(word)//
#define SIZE_INODE_DATA  SIZE_DE_BLOCK-sizeof(word)//



#if SIZE_TABLES_ORGA<(SIZE_TABLA_FILES+BLOQUES_BITS)
  //#error "TABLA DE ASIGNACION de sistema de Archivos Mal Configurada"
  #pragma message("Memoria EEPROM="SIZE_MEMORIA_EEPROM_EN_BITS"bytes")
#endif

/*Archivos MANEJO DE NOMBRES DE ARCHIVOS*/
#define FILE_VARS_SYSTEM "/varSystem.dat"




//struct FILE{
//  word *fileBlock;//pointer to block-file Logic, header block of the file
//  word *filechar;//points to the byte of the file actual to read
//  byte tipo;//tipo de dato que se esta leyendo, para el paso del puntador
//  uint32 addressFile;//direccion fisica del archivo abierto head
//  char name[FILE_NAME_SIZE];
//  char mode[3];
//};





//struct _Inode_Head{//inodo LOGICO 
//  char name[FILE_NAME_SIZE];//0-9 byte
//  byte data[SIZE_INODE_DATA_HEAD];//10-47byte
//  word *p;//48,49byte pointer to next block          
//};

struct _Inode{//inodo LOGICO 
  byte data[SIZE_INODE_DATA];//0-47byte
  word *p;//48,49byte pointer to next block          
};



struct _Basic_File_Sytem{//BASIC FILE SYSTE
  unsigned char x0;
  unsigned char x1;
};




struct FILE  *get_FileDescriptor(void);
byte cargar_variables_de_sistema(byte *memo);



#endif /* FILESYSTEM_H_ */
