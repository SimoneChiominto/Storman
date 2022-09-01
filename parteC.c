#include <stdio.h>
#include <stdlib.h>
#include "funzionivarie.h"
#include "storman.h"
#include "funzioniParteC.h"






int ext_block_release(void ** ptr_addr, int * num_released)
{

  
  /*
   * cerco il pointer con indirizzo ptr_addr e lo aggiungo alla coda dei pointer da eliminare
   */
  
  *num_released=0;
  struct block *curr_block=malloc(sizeof(struct block));
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));

  if(!lookfor(ptr_addr,NULL,NULL,&curr_ptr)) return 1;

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
    lookfor(curr_ptr->ptrptr,NULL,&curr_block,NULL); //controllare ptrptr
    if( curr_block->ptr_listhead && !curr_block->ptr_listhead->next){//se è l'unico elemento del blocco     
      *num_released+=1;
      C=block_enqueue(C,curr_block);
      //una funzione che mi deve dare tutti i puntatori tale che ptrptr è in curr_block e inserirmeli nella coda
    }
    block_release(curr_ptr->ptrptr);
  }

  return 0;
  
}





int ext_block_alloc(void ** ptr_addr , size_t alignment , size_t size){
  if(lookfor(ptr_addr, NULL, NULL, NULL)){
    int n;
    ext_block_release(ptr_addr,&n);
     return block_alloc(ptr_addr,alignment,size);
  }
  else{
    return block_alloc(ptr_addr, alignment, size);
  }
}

















int automatic_block(struct block *block){

  /*
   * Vede se block ha almeno un puntatore con storage automatico, in caso si ferma e ritorna 1
   */

  unsigned int type;
  struct list_blockPtr *curr_ptr=block->ptr_listhead;

  while(curr_ptr){
    pointer_info( curr_ptr->ptrptr,&type);
    if(!type) return 1;
    curr_ptr=curr_ptr->next;      
  }
  return 0;
}




int sweeping_release()
{


  
  /*
   * Creo grafo 
   */
  int n;             //numero di nodi nel grafo
  struct block_graph *g =create_block_graph(&n);

  

  
  /*
   * ora ad oggni blocco ho associato un intero, ovvero l'indice del grafo
   * creo la lista dei blocchi che hanno un ptr con storage automatico o statico
   * questa sarà in conseguenza di quello che ho detto prima una lista di indici interi
   */

  struct list *automatic_block_stack = NULL;
  

  struct zone *curr_zone=malloc(sizeof(struct zone));
  curr_zone=zone_listhead;
  
  struct block *curr_block=malloc(sizeof(struct block));
  

  for(int i=0; i<n; i++)
    {
      if(automatic_block(g[i].block)){
	automatic_block_stack = push(automatic_block_stack,i); 
      }
    }

  
  /*
   * Faccio una visita di grafo che mi segna marked ogni elemento raggiunto 
   * a partire da ogni blocco con storage automatico
   */

  while(automatic_block_stack){
    int i=pop(&automatic_block_stack);   //mi tira fuori l'indice di un blocco automatico
    if(!g[i].marked){
      visita(g,i);                       
    }
  }



  /*
   * ora libero completamente tutti i blocchi che non sono segnati come marked
   * e ritorno il numero di blocchi liberati
   */

  int k=0;
  for(int i=0;i<n;i++){
    if (!g[i].marked){
      k++;
      while(g[i].block && g[i].block->ptr_listhead){
	block_release(g[i].block->ptr_listhead->ptrptr);
      }      
    }
  }
  return k;
}
