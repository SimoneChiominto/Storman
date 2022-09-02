#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/lists.h"






int pointer_info(void ** ptr_addr, unsigned int *type){
  
  if(!search( ptr_addr, NULL, NULL, NULL))
    return 1;

  else{
    
    /*
     * se il puntatore al puntatore è in storman allora il tipo è dinamico
     */
    
    if(search_block( ptr_addr, NULL, NULL)) {
      *type=1; //dinamico = 1
    }
    else *type=0; //automatico o statico = 0
    return 0;
  }

}

