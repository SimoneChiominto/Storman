#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/lists.h"


int pointer_assign(void ** ptr_addr, void * val){
  
  struct zone *curr_zone=malloc(sizeof(struct zone)) ;
  struct block *curr_block=malloc(sizeof(struct block));

  /*
   * Controllo se val è un indirizzo di un blocco di storman con lookfor_block
   * che inoltre mi porta curr_zone e curr_block nella zona e nel blocco desiderato
   */
  
  if(search_block(val,&curr_zone,&curr_block))  //val è un indirizzo di un blocco gestito da storman
    {
      //da adesso in poi non ci spostiamo più dalla zona  e dal blocco di val

      /*
       * se *ptr_addr == val allora non c'è nulla da fare
       */
      
      if(val==*ptr_addr)
	return 0;

      else{
	
	/*
	 * se *ptr_addr è un indirizzo gestito da storman allora lo rilascio per poi riassegnarlo
	 */
	
	if(search(ptr_addr,NULL,NULL,NULL) && val!=*ptr_addr) {
	  block_release(ptr_addr); //forse pointer_release??
	}
	
	/*
	 * ora so che *ptr non fa più parte di storman quindi lo assegno a val
	 * e mi aggiorno la strutture dati
	 */
	
	*ptr_addr=val;
	struct list_blockPtr *new_ptr=malloc(sizeof(struct list_blockPtr));
	new_ptr->ptr=*ptr_addr;
	new_ptr->ptrptr=ptr_addr;
	insert(curr_block->ptr_listhead, new_ptr);
	return 0;
      }
    }

  /*
   * Se val non é un indirizzo di un blocco di storman
   */
  
  else 
    return 1;
}

