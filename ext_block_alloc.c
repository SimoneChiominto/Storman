#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/storman.h"
#include "headers/utils2.h"




int ext_block_alloc(void ** ptr_addr , size_t alignment , size_t size){
  if(search(ptr_addr, NULL, NULL, NULL)){
    int n;
    ext_block_release(ptr_addr,&n);
     return block_alloc(ptr_addr,alignment,size);
  }
  else{
    return block_alloc(ptr_addr, alignment, size);
  }
}


