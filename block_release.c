//block_release
#include <stdio.h>
#include <stdlib.h>
#include "funzionivarie.h"


int block_release(void ** ptr_addr){

  
  struct zone *curr_zone=malloc(sizeof(struct zone));
  struct block *curr_block=malloc(sizeof(struct block));;
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  /*
   * Cerco il blocco e il puntatore da rilasciare,
   * se non lo trovo allora return 2
   */
  if(!lookfor(ptr_addr,&curr_zone,&curr_block, &curr_ptr))
    return 2;

  
  else{
    
    /*
     * Se trovo il puntatore lo rilascio
     */
    ptr_list_delete( &(curr_block->ptr_listhead), curr_ptr );
    *ptr_addr=NULL;
    
    /*
     * Se sono rimasti altri puntatori al blocco 
     * ritorno 1
     */
    if( curr_block->ptr_listhead )
      return 1;

   
    else{
      
      /*
       * Se non sono rimasti altri puntatori al blocco allora il mio blocco è diventato libero
       * l'unica cosa che devo controllare è che se due blocchi consecutivi sono liberi
       * li voglio unire in un solo blocco libero ( è ciò che fa clean_zone) 
       */
      
      clean_zone( curr_zone->block_listhead);
    }
    
    if(curr_zone->tot_mem == curr_zone->block_listhead->tot_mem && !curr_zone->block_listhead->ptr_listhead){
      
      /*
       * Se la zona è composta da un solo blocco libero, dealloco l'intera zona
       */
      zone_list_delete(&zone_listhead,curr_zone);
    }
  }
  return 0;
}


