#include <stdlib.h>



#define insert(__list,__node)						\
  do {									\
    __node->next=__list;						\
    __list=__node;							\
  } while(0)



#define delete(__list,__node)						\
  do{									\
    if(__list && __node){						\
      if(__list==__node){						\
	__list=__node->next;						\
	free(__node);							\
      }									\
      else{								\
	typeof(__node) __curr_node;						\
	for(__curr_node=__list; __curr_node->next!=__node; __curr_node=__curr_node->next); \
	__curr_node->next=__node->next;					\
       	free(__node);						\
      }									\
    }									\
  } while(0)

  
