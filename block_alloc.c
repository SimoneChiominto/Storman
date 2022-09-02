//block_alloc

#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/lists.h"

int isAccettable(size_t n);
int found_freeBlock(struct zone **curr_zone,struct block **curr_block, size_t alignment,size_t size) ;
void *rightSpace(struct block *curr_block, size_t alignment);


struct zone *zone_listhead=NULL;

int block_alloc(void ** ptr_addr, size_t alignment, size_t size)
{

  /*
   * Errore, alignement non ripetta il vincolo
   */
  
  if(!isAccettable(alignment))
    return 2;
  
  else{

    struct zone *curr_zone=zone_listhead;

    /*
     * Se l'indirizzo in ptr_addr è di un puntatore già gestito allora applico block_release
     * per poi riassegnargli un blocco-
     */

    if(search(ptr_addr, &curr_zone, NULL, NULL)){//se *ptr_addr è gestito da storman lo levo temporaneamente per assegnargli poi un nuovo blocco
       block_release(ptr_addr);
       //return block_alloc(ptr_addr, alignment, size);
    }

    /*
     * Da qua in poi l'effettiva allocazione del blocco, perché
     * sono certo che ptr_addr non è gestito da storman
     */
    
    //else{ //ora so che *ptr_addr non è gestito da storman  
      struct block *freeBlock=malloc(sizeof(struct block));
      while(!found_freeBlock(&curr_zone,&freeBlock,alignment,size)){//qua cerco e se lo trovo individuo quale è il freeblock buono
	
	/*
	 * Se non trovo un blocco con abbastanza spazio libero alloco una nuova zona
	 */
	
	zone_alloc(size,&zone_listhead);
      }

      /*
       * Altrimenti, cerco il giusto posizionamento del blocco in modo da rispettare alignment
       */
      
      void *block_ptr=rightSpace(freeBlock,alignment);//mi trova il posto giusto all'interno del blocco
      *ptr_addr=block_ptr;

      /*
       * Da qua in poi devo sistemare le mie strutture dati interne
       * inizio con i blocchi
       */

      
      /*
       * mi si potrebbero essere creati fino a tre nuovi blocchi,
       * 1) prec_block che si crea se ho dovuto lasciare uno spazio bianco per rispettare alignment
       * 2) new_block che è il nuovo blocco occupato che volevo allocare fin dall'inizio
       * 3) succ_block che è lo spazio che si crea dalla fine di new_block fino al successivo blocco occupato
       */
      
      
      //inizializzo i tre blocchi: prec, new e succ;
      struct block *prec_block=malloc(sizeof(struct block));
      struct block *new_block =malloc(sizeof(struct block));
      struct block *succ_block=malloc(sizeof(struct block));
      
      //prec_block
      //se non lascia spazio bianco allora non c'è altrimenti lo creo
      if(block_ptr==freeBlock->value)//se non c'è spazio prima
	prec_block=NULL;
      else{
	prec_block->value = freeBlock->value;
	prec_block->tot_mem = ( block_ptr - prec_block->value);
	prec_block->alignment=1;
	prec_block->ptr_listhead=NULL;	
      }
      
      //new_block
      new_block->value = block_ptr;
      new_block->tot_mem = size;
      new_block->alignment= alignment;
      new_block->ptr_listhead=NULL;
      
      //succ_block
      if((block_ptr - freeBlock->value)+size == freeBlock->tot_mem )//se non c'è spazio dopo
	succ_block=NULL;
      else{
	succ_block->value = block_ptr+size;
	succ_block->tot_mem = freeBlock->value + freeBlock->tot_mem - succ_block->value;
	succ_block->alignment=1;
	succ_block->ptr_listhead=NULL;
      }



      /*
       * Inserisco il puntatore nella lista dei puntatori del nuovo blocco
       */
      
      //aggiorno lista dei pointer al blocco
      struct list_blockPtr *new_blockPtr=malloc(sizeof(struct list_blockPtr));
      new_blockPtr->ptr = block_ptr;
      new_blockPtr->ptrptr = ptr_addr;
      insert( new_block->ptr_listhead, new_blockPtr);

      
      
      /*
       * Infine inserisco i blocchi nella lista ordinata dei blocchi della zona
       */
      delete( curr_zone->block_listhead, freeBlock );
      //se esistono
      if(prec_block)
	sorted_insert( curr_zone->block_listhead, prec_block );
      sorted_insert( curr_zone->block_listhead, new_block );
      if(succ_block)
	sorted_insert( curr_zone->block_listhead, succ_block );
      //}
  }
 
  // print_storman();
  /*
   * Se tutto è corretto allora return 0
   */
  if(*ptr_addr) return 0;

  /*
   * Se ci sono stati problemi con malloc nell'allocare la zona allora return 1;
   */
  else return 1;
}







int isAccettable(size_t n)
{
  /*
   * è la funzione ricorsiva che vede se alignment rispetta il vincolo
   */
  
  if(n==sizeof(void *))
    return 1;
  else if(n%2)
    return 0;
  else
    return isAccettable( n/2);
}



int found_freeBlock(struct zone **curr_zone,struct block **curr_block, size_t alignment,size_t size )//in teoria corretto
{
  /*
   * é la funzione che mi cerca all'intero dei blocchi di storman se ce ne è uno libero 
   * con abbastanza memoria, facendo attenzione allo spazio da lasciare a causa dell'alignment
   */
  for( *curr_zone=zone_listhead;  *curr_zone; *curr_zone=(*curr_zone)->next){
    for( *curr_block=(*curr_zone)->block_listhead; *curr_block; *curr_block=(*curr_block)->next){
      if(! (*curr_block)->ptr_listhead){ 
	int i=0;//mi indicherà quanto spazio devo lasciare per ripettare l'alignment
	//while( (((unsigned long)(*curr_block)->value) + i) %alignment){
	int resto;
	if(( resto= ((unsigned long) (*curr_block)->value) % alignment) ){
	    i= alignment - resto;	  
	  //	  i++;
	}
	/*
	 * se trovo il blocco, ritorno vero e ritorno per indirizzo anche la zona e il blocco giusti
	 */
	if( (*curr_block)->tot_mem - i >= size) 
	return 1;
      }
    }
  }
  /*
   * Altrimenti ritorno falso
   */
  return 0;
}



void *rightSpace(struct block *curr_block, size_t alignment){
  /*
   * é la funzione che mi calcola quanto spazio libero devo lasciare per rispettare l'alignment
   * e mi ritorna il punto della memoria esatto da assegnare a *ptr_addr
   */
  unsigned long ptr= (unsigned long) curr_block->value;
  //int i=0;
  int resto;
  if (( resto = ptr %alignment)){
    ptr += alignment - resto; 

    //printf("%d",i);
  }
  return (void *) ptr;
}
