#include <stdlib.h>
#include <stdio.h>
#include "storman.h"
#include "funzionivarie.h"
/*
 * In questo file mettere le prove per testare storman
 */

int main()
{

  static void *a,*b;
  block_alloc(&a,2*sizeof(void *),404*sizeof(void));
  print_storman();
  pointer_assign(&b,a);
  print_storman();
}
