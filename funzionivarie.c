#include "funzionivarie.h"
#include <stdio.h>
#include "lists.h"

/*
void zone_list_insert(struct zone **L, struct zone *x)//ok
{
  if (x)
    x->next=*L;
  *L=x;
}


void zone_list_delete(struct zone **L,struct zone *x)//ok
{
  if (x==*L){
    *L=x->next;
    free(x);
  }
  else    
    zone_list_delete( &((*L)->next),x);
}

*/
void zone_alloc(size_t size, struct zone **zone_listhead){
  struct zone *new_zone=malloc(sizeof(struct zone));
  struct block *new_block=malloc(sizeof(struct block));//blocco che contiene tutta la zona

  /*
   * Alloca il doppio di size e inserisce nella struttura dati la nuova zona e il nuovo blocco
   */
  
  new_zone->placement = malloc( 2*size );
  new_zone->tot_mem =2*size;
  new_zone->block_listhead= NULL;
  insert(*zone_listhead,new_zone);
  new_block->placement= new_zone->placement;
  new_block->tot_mem= new_zone->tot_mem;
  new_block->alignment=1;
  new_block->ptr_listhead = NULL;
  insert( new_zone->block_listhead, new_block );
}

void clean_zone( struct block *head)
{
  
  /*
   * Se due blocchi consecutivi sono liberi, li unisce in un solo blocco
   */
  
  struct block *curr_block;
  for(curr_block= head ; curr_block ; curr_block=curr_block->next){
    while( curr_block->next && !curr_block->ptr_listhead && !curr_block->next->ptr_listhead ){//se due blocchi consecutivi sono liberi
      curr_block->tot_mem += curr_block->next->tot_mem;
      delete(curr_block,curr_block->next);
    }
  }
}


void block_list_insert(struct block **L, struct block *x)
{
  
  /*
   * Inserisce mantenendo l'ordine crescente per il valore di placement
   */
  
  if(!*L){
    *L=x;
  }
  else if((*L)->placement > x->placement){
    x->next=*L;
    *L=x;
  }
  else{
    block_list_insert(&((*L)->next),x);
  }  
}

/*
void block_list_delete(struct block **L,struct block *x)//ok
{
  if (x==*L){
    *L=x->next;
    free (x);
  }
  else    
    block_list_delete( &((*L)->next),x);   
}
*/

void block_print_list(struct block *L)
{
  while(L)
    {
      printf("%p\n",L->placement);
      L=L->next;
    }
  printf("\n");
}
/*
void ptr_list_insert(struct list_blockPtr **L, struct list_blockPtr *x)
{
  if (x)
    x->next=*L;
  *L=x;
}
*/

/*
void ptr_list_delete(struct list_blockPtr **L,struct list_blockPtr *x)
{
  if (x==*L){
    *L=x->next;
    free(x);
  }
  else    
    ptr_list_delete( &((*L)->next),x);   
}
*/

int lookfor( void **ptr, struct zone **curr_zone, struct block **curr_block, struct list_blockPtr **curr_ptr )
{
  
  /*
   * Cerca se un puntatore è gestito da storman
   * se lo trova restituisce 1 oltre alla zona, il blocco e il puntatore della mia struttura interna 
   * corrispondenti a ptr
   */

  /*
   * Se non voglio che mi ritorni i valori della zona, blocco e puntatore allora chiamo la funzione con NULL
   * le righe successive servono per inizializzare curr_zone, curr_block e curr_ptr in questo caso
   */
  if(!curr_zone){
    struct zone *new_zone=malloc(sizeof(struct zone));
    curr_zone=&new_zone;
  }  
  if(!curr_block){
    struct block *new_block=malloc(sizeof(struct block));
    curr_block=&new_block;
  }
  if(!curr_ptr){
    struct list_blockPtr *new_ptr=malloc(sizeof(struct list_blockPtr));
    curr_ptr=&new_ptr;
  }
  
  for(*curr_zone=zone_listhead; *curr_zone; *curr_zone=(*curr_zone)->next){//cerco zona per zona
    for(*curr_block=(*curr_zone)->block_listhead; *curr_block; *curr_block= (*curr_block)->next){//blocco per blocco
      for( *curr_ptr=(*curr_block)->ptr_listhead; *curr_ptr; *curr_ptr=(*curr_ptr)->next){//puntatore per puntatore
	if( (*curr_ptr)->ptrptr == ptr)//trovo un elemento della lista dei puntatori del blocco uguale al mio puntatore
	  return 1;
      }
    }
  }
  return 0;
}

int lookfor_block( void *val, struct zone **curr_zone, struct block **curr_block){

  /*
   * Simile a lookfor, ma trova solamente se val ricade nell'intervallo di un blocco
   */

  if(!curr_zone){
    struct zone *new_zone=malloc(sizeof(struct zone));
    curr_zone=&new_zone;
  }  
  if(!curr_block){
    struct block *new_block=malloc(sizeof(struct block));
    curr_block=&new_block;
  }

  for(*curr_zone=zone_listhead; *curr_zone; *curr_zone=(*curr_zone)->next){//cerco zona per zona
    
    for(*curr_block=(*curr_zone)->block_listhead; *curr_block; *curr_block= (*curr_block)->next){//blocco per blocco
      
      if( (*curr_block)->ptr_listhead && val >= (*curr_block)->placement  &&  val < ((*curr_block)->placement +(*curr_block)->tot_mem)){
	return 1;
      }
    }
  }   
  return 0;
}

void print_storman()
{
  /*
   * Funzione utilissima per il debug
   */
  
  printf("\n\n\n\nstato di storman\n");
  struct zone *curr_zone=zone_listhead;
  struct block *curr_block=malloc(sizeof(struct block));
  struct list_blockPtr *curr_ptr=malloc(sizeof(struct block));
  //curr_zone=head;
  while( curr_zone)
    {
      printf("\nsiamo nella zona %p\n",curr_zone->placement);
      curr_block=curr_zone->block_listhead;
      while(curr_block)
	{
	  printf("\n%p",curr_block->placement);
	  if(curr_block->ptr_listhead)
	    {
	      printf(" con i puntatori ");
	      curr_ptr=curr_block->ptr_listhead;
	      while(curr_ptr)
		{
		  printf("%p ",curr_ptr->ptrptr);
		  curr_ptr=curr_ptr->next;
		}
	    }
	  curr_block=curr_block->next;
	}
      curr_zone=curr_zone->next;
    }
}



































