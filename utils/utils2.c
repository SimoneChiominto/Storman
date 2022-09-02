#include "../headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "../headers/storman.h"
#include "../headers/utils2.h"

struct list_ptr_queue ptr_enqueue( struct list_ptr_queue C, struct list_blockPtr *curr_ptr){

  struct list_ptr *e=malloc(sizeof(struct list_ptr));
  e->ptr=curr_ptr;
  e->next=NULL;
  
  if(!C.tail){
    C.head=e;
    C.tail=e;
  }
  else{
    C.tail->next=e;
    C.tail=e;
  }
  return C; 
}

struct list_blockPtr *ptr_dequeue(struct list_ptr_queue *C){
  if(C->head)
    {
      struct list_ptr *e=C->head;
      C->head=e->next;
      if(!C->head)
	C->tail=NULL;
      return e->ptr;
    }
  return NULL;
}

struct list_ptr_queue block_enqueue( struct list_ptr_queue C, struct block *B){

  /*
   * è una funzione che mi trova tutti i puntatori di storman tale che ptrptr è in B
   */

  /*
   * passo in rassegna tutti i puntatori di storman
   * ad uno a uno lookfor_block di ptr ptr
   * se lookfor_block mi restituisce proprio B inserisco nella coda
   */

  struct zone *curr_zone;
  struct block *curr_block;
  struct list_blockPtr *curr_ptr;

  for(curr_zone=zone_listhead; curr_zone; curr_zone=curr_zone->next){//cerco zona per zona
    for(curr_block=curr_zone->block_listhead; curr_block; curr_block= curr_block->next){//blocco per blocco
      for( curr_ptr=curr_block->ptr_listhead; curr_ptr; curr_ptr=curr_ptr->next){//puntatore per puntatore

	struct block *block_found=NULL;
	search_block(curr_ptr->ptrptr,NULL,&block_found);
	if(B==block_found){
	  C=ptr_enqueue(C,curr_ptr);
	}
      }
    }
  }
  return C;
}




int count_storman_block();
void write_graph( struct block_graph *g, int n);

struct block_graph *create_block_graph(int *n){
  
  //mi restituisce un array di nodi di grafo

  /*
   * 1) calcolo il numero di blocchi occupati
   */
  
  *n=count_storman_block(); //mi conta il numero di blocchi non liberi
  struct block_graph *g=malloc( (*n) *sizeof(struct block_graph));

  /*
   * 2) scrivo le liste di adiacenza
   */
  
  write_graph( g,*n );
  return g;
}



int count_storman_block()
{
  
  /*
   * conta i blocchi occupati in storman
   */

  struct zone *curr_zone=zone_listhead;
  struct block *curr_block=malloc(sizeof(struct block));
  int i=0; 
  while( curr_zone)
    {
      curr_block=curr_zone->block_listhead;
      while(curr_block)
	{
	  if(curr_block->ptr_listhead)
	    {
	      i++;
	    }
	  curr_block=curr_block->next;
	}
      curr_zone=curr_zone->next;
    }
  return i;
}



void write_graph( struct block_graph *g, int n)//da testare
{

  /*
   * prima metto tutti i blocchi occupati nel vettore/grafo
   */
  
  int i=0;
  struct zone *curr_zone=zone_listhead;
  struct block *curr_block=malloc(sizeof(struct block));
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct list_blockPtr));
  while( curr_zone)
    {

      curr_block=curr_zone->block_listhead;
      while(curr_block)
	{
	  if(curr_block->ptr_listhead)
	    {
	      g[i].block=curr_block;
	      g[i].marked=0;
	      g[i].ad_block_listhead=NULL;
	      i++;
	    }
	  curr_block=curr_block->next;

	}
      curr_zone=curr_zone->next;
    }

  /*
   * dopo creo le liste di adiacenza
   */
  
  for(int i=0; i<n; i++){
    
    curr_ptr=g[i].block->ptr_listhead; 
    while(curr_ptr){
      struct block *block_found=malloc(sizeof(struct block));
      if(search_block(curr_ptr->ptrptr,NULL,&block_found)){
	//trovo l'indice corrispondente a block found
	int k=0;
	if(block_found!=g[k].block){	  
	  k++;
	}
	g[i].ad_block_listhead = push(g[i].ad_block_listhead,k);
      }
      curr_ptr=curr_ptr->next;
    }
  }
  

}



void visita(struct block_graph *g, int i){

  g[i].marked=1;

  struct list *pila=NULL;
  pila=push(pila,i);

  while(pila){
    
    //levo dalla pila
    int curr_node =pop(&pila);
    
    //inserisco nella pila i nodi adiacenti   
    struct list *nodo_adiacente;
    nodo_adiacente=g[curr_node].ad_block_listhead;
    
    while(nodo_adiacente){
      if(!g[nodo_adiacente->key].marked){

	g[nodo_adiacente->key].marked=1;
	//inserisco nella pila
	pila=push(pila,nodo_adiacente->key);	  
      }
    
    nodo_adiacente=nodo_adiacente->next;
    }
  }
}


struct list *push(struct list *pila, int x)
{
  struct list *p;

  p= malloc(sizeof(struct list));
  p->key=x;
  
  p->next=pila;
  return p;
}

int pop(struct list **pila)
{
  struct list *p;
  
  if(*pila)
    {
      p=*pila;
      *pila=p->next;
      return p->key;
    }
  return -1;
}
