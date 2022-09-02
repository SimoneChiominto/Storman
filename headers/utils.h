#include <stdlib.h>

struct zone{
  /*
   * Le zone sono gestite come liste
   */
  void *value; //l'indirizzo dell'inizio della zona
  size_t tot_mem;  //la memoria totale
  struct block *block_listhead; //puntatore all'inizio della lista dei blocchi della zona
  struct zone *next;
};

struct zone *zone_listhead;

void zone_alloc(size_t size, struct zone **);
void clean_zone( struct block *head);

struct block{
  
  /*
   * I blocchi sono gestiti come liste
   */
  
  void *value; //l'indirizzo dell'inizio del blocco
  size_t tot_mem;  //la memoria totale
  size_t alignment;//allineamento
  struct list_blockPtr *ptr_listhead; //puntatore all'inizio della lista dei blocchi della zona, se NULL allora è un blocco libero
  struct block *next;
};

void block_print_list(struct block *L);

struct list_blockPtr{

  /*
   * I puntatori dei blocchi sono gestiti come liste
   */
  
  void *ptr;      //è il punto di memoria del blocco a cui punta
  void *ptrptr;   //è il punto di memoria in cui è salvato (il void **ptr_addr di block_alloc per intendersi) 
  struct list_blockPtr *next;
};


//funzioni di ricerca
int search( void **ptr, struct zone **curr_zone, struct block **curr_block, struct list_blockPtr **curr_ptr );
int search_block( void *ptr, struct zone **curr_zone, struct block **curr_block);
void print_storman();









