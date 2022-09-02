#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/utils2.h"






int ext_block_release(void ** ptr_addr, int * num_released)
{

  
  /*
   * cerco il pointer con indirizzo ptr_addr e lo aggiungo alla coda dei pointer da eliminare
   */
  
  *num_released=0;
  struct block *curr_block=malloc(sizeof(struct block));
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  if(!search(ptr_addr,NULL,NULL,&curr_ptr)) return 1;

  struct list_ptr_queue C;
  C.head=NULL;
  C.tail=NULL;

  C=ptr_enqueue( C, curr_ptr);

  
  /*
   * finchè la coda non è vuota
   * * individuo il blocco a cui appartiene ptrptr
   * * se ptr è l'unico ptr del blocco allora posso rilasciarlo
   * * * 
   */

  while(C.head){  
    curr_ptr=ptr_dequeue(&C);
    search(curr_ptr->ptrptr,NULL,&curr_block,NULL); //controllare ptrptr
    if( curr_block->ptr_listhead && !curr_block->ptr_listhead->next){//se è l'unico elemento del blocco     
      *num_released+=1;
      C=block_enqueue(C,curr_block);
      //una funzione che mi deve dare tutti i puntatori tale che ptrptr è in curr_block e inserirmeli nella coda
    }
    block_release(curr_ptr->ptrptr);
  }

  return 0;
  
}


