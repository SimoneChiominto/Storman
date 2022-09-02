#include <stdlib.h>
#include <stdio.h>
#include "../headers/storman.h"
#include "../headers/utils.h"

/*
 * In questo file mettere le prove per testare storman
 */

int main()
{

  static void *a,*b,*c;
  block_alloc(&a,2*sizeof(void *),32*sizeof(void));
  print_storman();
  block_alloc(&b,2*sizeof(void *),32*sizeof(void));
  //pointer_assign(&b,a);
  print_storman();
	pointer_assign(&c,a);
//block_alloc(&c,2*sizeof(void *),32*sizeof(void));
print_storman();
pointer_release(&c);
print_storman();
}
