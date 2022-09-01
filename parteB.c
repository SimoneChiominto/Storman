#include <stdio.h>
#include <stdlib.h>
#include "funzionivarie.h"
#include "storman.h"

//ok
void *** block_info(void ** ptr_addr, void **lowaddr, void **highaddr, size_t * num_ptr){
   struct zone *curr_zone=malloc(sizeof(struct zone));
   struct block *curr_block=malloc(sizeof(struct block));
   struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  
   if(!lookfor( ptr_addr, &curr_zone, &curr_block,NULL))//se non trovo il blocco allora non c'è nulla da fare
     return NULL;
   else{

     //sono arrivato in curr_block che è il blocco a cui sto puntando con lookfor
     //2) setto *lowaadr e *highadddr con gli indirizzi di partenza e fine di curr_block
     
     *lowaddr = curr_block->placement;
     *highaddr = (void *) ((unsigned long) curr_block->placement + (unsigned long) curr_block->tot_mem -1);

     //accedo alla lista dei puntatori che puntano in curr_bock
     
     curr_ptr = curr_block->ptr_listhead;

     // calcolo il numero di puntatori 
     int n=0;      
     while(curr_ptr){
       n++;
       curr_ptr=curr_ptr->next;
     }

     //4) alloco un vettore di n void**
     
     void ***a; 
     a=malloc(n*sizeof(void **));

     //5) setto *num_ptr a n
     
     *num_ptr=n;

     //6) copia gli indirizzi di p1,..,pn in a
     
     curr_ptr= curr_block->ptr_listhead;
     for(int i=0;i<n;i++){
       a[i]=curr_ptr->ptrptr;
       curr_ptr=curr_ptr->next;	 
     }

     return a;     
   }
}





int pointer_info(void ** ptr_addr, unsigned int *type){
  
  if(!lookfor( ptr_addr, NULL, NULL, NULL))
    return 1;

  else{
    
    /*
     * se il puntatore al puntatore è in storman allora il tipo è dinamico
     */
    
    if(lookfor_block( ptr_addr, NULL, NULL)) {
      *type=1; //dinamico = 1
    }
    else *type=0; //automatico o statico = 0
    return 0;
  }

}



/*
 * DA QUA IN POI BLOCK_REALLOC
 */



int ptrMoreThanSize( struct block *block, size_t size){
  
  /*
   * Mi controlla se posso restringere il blocco a da destra
   */
  
  struct list_blockPtr *curr_ptr=block->ptr_listhead;
  while( curr_ptr ){
    if( curr_ptr->ptr - block->placement > size) return 1;
    curr_ptr= curr_ptr->next;
  }
  return 0;
}

int thereIsRightSpace(struct zone *curr_zone, struct block *curr_block, size_t size){

  /*
   * Mi controlla se posso allargare il blocco a destra
   */
  
  void *base_placement= curr_block->placement;

  /*
   * se esco dalla zona
   */
  
  if( (unsigned long) base_placement + size > (unsigned long) curr_zone->placement + curr_zone->tot_mem) return 0;

  //printf("\nse sono qui va bene");
  
  /*
   * se rimango all'interno della zona
   */
  
  struct block *block= curr_block->next;
  while(block && block->placement - base_placement < size){   
    if(block->ptr_listhead){

      /*
       * se non è un blocco libero scorro tutti i puntatori per vedere se puntatno 
       * a posti dopo lo spazio che vorrei allocare      
       */
      
      // struct list_blockPtr *curr_ptr= block->ptr_listhead;
      //while(curr_ptr){
      //if(curr_ptr->ptr - base_placement < size)
      return 0;
      //	curr_ptr=curr_ptr->next;	
      //}
    }
    block= block->next;
  } 
  return 1;
}




void myMemCpy( void *from, void *to, size_t n){
  
  /*
   * Mi copia lo storage byte per byte da una parte all'altra
   */
  
  char *fromc= (char *) from;
  char *toc= (char *) to;

  for(int i=0; i<n; i++)
    toc[i]=fromc[i];  
}


//FORSE FUNZIONA, ma di ricontrollare la fine c'è qualcosa che non quadra

int block_realloc(void ** ptr_addr, size_t newsize){
  
  struct zone *curr_zone=malloc(sizeof(struct zone));
  struct block *curr_block=malloc(sizeof(struct block));
  //se non punta a un blocco allora errore
  if(!lookfor( ptr_addr, &curr_zone, &curr_block, NULL))
    return 1;

  //se punta a un blocco gestito da storman
  else{
    
    //divido in tre casi: se new size uguale, maggiore o minore

    
    /*
     * se |B|=new_size non faccio nulla
     */
    
    if( curr_block->tot_mem == newsize )
      return 0;

    
    /*
     * se new_size è più piccolo
     */
    
    else if( curr_block->tot_mem > newsize){

      /*
       * se non posso contrarre il blocco a destra, errore: ritorno 2
       */

      if(ptrMoreThanSize(curr_block,newsize)) return 2;

      /*
       * se posso contrarre il blocco a destra, restringo il blocco e allargo quello successivo
       */
      
      else {
	size_t diff= curr_block->tot_mem - newsize;
	curr_block->tot_mem = newsize;
	if(curr_block->next && !curr_block->next->ptr_listhead){
	  curr_block->next->placement = (void *)((unsigned long) curr_block->placement + newsize);
	  curr_block->next->tot_mem +=  diff;
	}
	else{
	  struct block *new_block=malloc(sizeof(struct block));
	  new_block->placement = (void *)((unsigned long) curr_block->placement + newsize);
	  new_block->tot_mem= diff;
	  new_block->alignment=1;
	  block_list_insert( &(curr_zone->block_listhead), new_block);
	}
      }
    }


    /*
     * se new_size è più grande
     */
    
    else{
      
      /*
       * se c'è spazio a destra, allargo il mio blocco
       */
      
      if(thereIsRightSpace(curr_zone, curr_block, newsize)){
      

	curr_block->tot_mem = newsize; //aggiorno la nuova memoria

	/*
	 * Nell'allargare potrei dover cancellare il blocco successivo 
	 */
	
	struct block *succ_block=curr_block->next;
	while(succ_block && succ_block->placement - curr_block->placement < newsize){//finchè sono in un blocco che devo eliminare o restringere
	  //non è detto che esista succ_block->next nel caso sia l'ultimo blocco della zona, ma allora io gia so che non sto fuoriusceno dalla zona quindi è da restringere a meno che non sia esattamente tutto ricopetro	  
	  if( (unsigned long) succ_block->placement + succ_block->tot_mem - (unsigned long) curr_block->placement <=  newsize ){//se il blocco successivo è anche da restringere o questo blocco è completamente da eliminare
	    struct block *succsucc_block=succ_block->next;
	    block_list_delete( &(curr_zone->block_listhead), succ_block);	    	      
	      succ_block=succsucc_block;
	  }
	  
	  else{ //il blocco è solo da restringere
	    succ_block->tot_mem = (unsigned long) succ_block->placement + succ_block->tot_mem - (unsigned long) curr_block->placement - newsize;
	    succ_block->placement= (void *) ( (unsigned long) curr_block->placement + newsize );
	  }
	}		
      }

      
      else{
	
	/*
	 *non c'è spazio a destra, alloco un nuovo blocco e copio tutto
	 */
	

	struct block *new_block=malloc(sizeof(struct block));
	//size_t alignment = findAlignment((size_t) curr_block->placement);//prendo il più grande allinamento possibile di questo blocco
	block_alloc( &(new_block->placement) , curr_block->alignment, newsize);

	/*
	 * faccio una copia byte per byte dello storage
	 */
	
	myMemCpy( curr_block->placement, new_block->placement, curr_block->tot_mem);

	
	//aggiorno puntatori
	//adesso new_block è il puntatore alla struttura blocco

	struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));
	curr_ptr=curr_block->ptr_listhead;

		
	while(curr_block->ptr_listhead){
	  pointer_assign(curr_ptr->ptrptr, new_block->placement + ( curr_ptr->ptr - curr_block->placement));
	  
	  if(curr_block)
	    curr_ptr=curr_block->ptr_listhead;
	}
	
	*ptr_addr += ( new_block->placement - curr_block->placement );

	//devo levare il primo pointer allocato
	pointer_release( &(new_block->placement));


	/*
	 * Dealloco B
	 */	
	
	while(curr_block && curr_block->ptr_listhead){
	  block_release( curr_block->ptr_listhead->ptrptr);

	}
      }
    }
    return 0;
  }
    
}
