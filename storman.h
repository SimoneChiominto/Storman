/*******PARTE A********************************/

int block_alloc(void ** ptr_addr, size_t alignment, size_t size);
int block_release(void ** ptr_addr);
int pointer_release(void ** ptr_addr);
int pointer_assign(void ** ptr_addr, void * val);

/*******PARTE B********************************/

void *** block_info(void ** ptr_addr, void **lowaddr, void **highaddr, size_t * num_ptr);
int pointer_info(void ** ptr_addr, unsigned int *type);

#define assign(__lv,__rv,__ret)					\
  do {								\
    if (lookfor(&(__lv), NULL, NULL, NULL)) (__ret)=2;		\
    else if(!lookfor_block(&(__lv),NULL,NULL)) (__ret)=1;	\
    else {							\
      (__ret)=0;						\
      (__lv)=(__rv);						\
    }								\
  } while(0)


int block_realloc(void ** ptr_addr, size_t newsize);

/*******PARTE C********************************/

int ext_block_release(void ** ptr_addr, int * num_released);
int ext_block_alloc(void ** ptr_addr, size_t alignment, size_t size);
int sweeping_release();

/*******PARTE D********************************/

//int compact_blocks(void *** pointers, int num_ptrs);
//int compact_memory(float load, int trigger_anyway);

/*******PARTE E********************************/

//int dedup_blocks(void *** pointers, int num_ptrs);
//int toggle_snapshot(void ** ptr_addr);
