#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/lists.h"



void *** block_info(void ** ptr_addr, void **lowaddr, void **highaddr, size_t * num_ptr){
   struct zone *curr_zone=malloc(sizeof(struct zone));
   struct block *curr_block=malloc(sizeof(struct block));
   struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  
   if(!search( ptr_addr, &curr_zone, &curr_block,NULL))//se non trovo il blocco allora non c'è nulla da fare
     return NULL;
   else{

     //sono arrivato in curr_block che è il blocco a cui sto puntando con lookfor
     //2) setto *lowaadr e *highadddr con gli indirizzi di partenza e fine di curr_block
     
     *lowaddr = curr_block->value;
     *highaddr = (void *) ((unsigned long) curr_block->value + (unsigned long) curr_block->tot_mem -1);

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

