
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "storman.h"
#include "utils2.h"








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
