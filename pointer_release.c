#include <stdio.h>
#include <stdlib.h>
#include "funzionivarie.h"
#include "storman.h"
#include "lists.h"





int pointer_release(void ** ptr_addr){

  struct zone *curr_zone=malloc(sizeof(struct zone));
  struct block *curr_block=malloc(sizeof(struct block));
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  /*
   * se ptr_addr non punta a un puntatore di storman non faccio niente e ritorno 1
   */
  
  if(!search(ptr_addr,&curr_zone,&curr_block,& curr_ptr))
    return 1;

  else{
    
    /*
     * Controllo se il blocco di *ptr_addr ha un solo puntatore
     * in quel caso non posso rilasciare *ptr_addr e ritorno 2
     */
    
    if( curr_ptr==curr_block->ptr_listhead //se è il primo elemento della lista
	&& !curr_ptr->next ) //ed è anche l'ultimo
      //ovvero è l'unico elemento della lista
      return 2;

    else{
      
      /*
       * se ci sono altri puntatori al blocco rilascio il puntatore
       */

      delete(curr_block->ptr_listhead, curr_ptr );
      *ptr_addr=NULL;
      return 0;
    }    
  }
}
