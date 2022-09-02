
struct list_ptr{

  /*
   * questa è una lista di puntatori che mi serve in ext_block_release
   * nel momento in cui devo fare la coda dei puntatori da eliminare
   */
  
  struct list_blockPtr *ptr; //qui abbiamo un puntatore al ptr gestito da storman
  struct list_ptr *next;
};


struct list_ptr_queue{

  /*
   * è la coda fatta di elementi della struttura precedente 
   */
  
  struct list_ptr *head;
  struct list_ptr *tail;
};

struct list_ptr_queue ptr_enqueue( struct list_ptr_queue C, struct list_blockPtr *e);
struct list_blockPtr *ptr_dequeue(struct list_ptr_queue *C);
struct list_ptr_queue block_enqueue( struct list_ptr_queue C, struct block *B);



struct block_graph{
  /*
   * nodo del grafo che mi serve in sweeping release
   */
  struct block *block;            //puntatore al blocco 
  int marked;                     //mi serve per vedere se il nodo è stato visitato
  struct list *ad_block_listhead; //lista di adiacenza
};

struct block_graph *create_block_graph(int *n);
void visita(struct block_graph *g, int i);



struct list{
  
  /*
   * è una lista di interi
   * mi serve per fare le lista di elementi del grafo 
   * che si possono identificare con l'indice dell'array in cui ho salvato il grafo
   */
  
  int key;
  struct list *next;
};

struct list *push(struct list *pila, int x);
int pop(struct list **pila);
