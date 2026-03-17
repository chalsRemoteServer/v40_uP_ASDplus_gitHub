/*
 * heap_1.h
 *
 *  Created on: Apr 11, 2024
 *      Author: desarrollo-1
 */

#ifndef HEAP_1_H_
#define HEAP_1_H_


/* IMPLEMENTA EL ALGORITHMO FIRST FIT
 *  y permite liberar memoria, y permite fusion
 *   de memoria libre.
 * 
 * 
 * */

#include "PE_Types.h"
#include "system.h"

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
    #error "This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0"
#endif

#ifndef configHEAP_CLEAR_MEMORY_ON_FREE
    #define configHEAP_CLEAR_MEMORY_ON_FREE    0
#endif
#define X_PROC_MAX_MEMO 80//maxima memoria por proceso
#define heapSIZE_MAX 200 //byte de memoria para las Proc
#define heapFREE_BLOCK 10 //BLOQUES DE MEMORIA LIBRE fragmentada
#define _255 7//nunmero de tamaño de apuntador de nodos
#define _128 6//libres
#define _64  5
#define _32  4
#define _16  3
#define _8   2
#define _4   1
#define _2   0
#define BLOCK_NEW 0xFF//BLOCK NO asignado numero de block


 struct _NodoH_{ //Nodo Heap
     struct _NodoH_ *next;
     byte size;//the tamaño del blockque
     byte *addr;//adress pointing where the block of memo begins
     byte id;//id del nodo
     byte pid;//pid que solicita la memoria
     byte status;//estatus del nodo PRFREE|PRREADY|0=no asignado a nada
     byte blockNum;
};


 struct _HEAP_{
  byte Heap[heapSIZE_MAX];
  unsigned short int memo_used;
  unsigned short int memo_free;
  byte memo_solicitada;
  struct _NodoH_ *FMBC[8];//free Memory Block Control
  struct _NodoH_ nodos[PROC_MAX];
  byte  NMBO[PROC_MAX];//Numero de id de  Block de Memoria Ordenados como Numero de Bloques, del block 0 al n sea budy or free
};


void init_Heap(void);	
byte *malloc_memory_Heap(byte *size, byte pid);
struct _NodoH_ *get_Nodo_Free(void);
byte evaluar_tamano_de_bloque_necesario(void);
byte construir_Nodos(byte index);
byte hay_nodos_libres_ofsize(byte index);
byte sacar_nodo_de_su_tren(byte size);
//void agregar_a_lista_de_ocupados(int index);
byte BTNF(byte hijo,byte padre);
byte agregar_a_lista_de_ocupados(byte index,byte pid);
byte  Buscar_siguiente_nodo_libre(void);
void dividir_Bloque_enDos(byte index);
byte get_Numero_Pointer(byte index);
byte  dividir_Bloque_bloque_en_Dos(byte  *index);
byte get_size_to_Low(byte size);
byte convertSize2Index(byte size);
void construir_nuevo_tren_de_size_n(byte  n1,byte n2);
void Re_Impresion_de_Num_Blocks(void);
void Actualizar_Num_Blocks(void);
void Merge_NMBO(byte p,byte q,byte r);
void MergeSort_NMBO_add(byte  p,byte r);
byte free_memory_heap(byte *addresBlock);
byte find_Nodo_From_Address(byte *addr);
void limpiar_Nodo_(int index);
byte convertIndex2Size(int index);

#endif /* HEAP_1_H_ */
