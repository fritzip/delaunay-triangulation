#ifndef __PRINT_H__
#define __PRINT_H__

#include "linked_list.h"

/*----------------------------------------------------------------------------------*/
//                                  Print functions prototypes
/*----------------------------------------------------------------------------------*/

void print_lnk(int const LNK);

void print_info(Dllist const *dll, int const LNK);

void print_vertex(Vertex const *vert);

void print_dll(Dllist const *dll, int const LNK);

void print_dll_full(Dllist const *dll, int const LNK);

#endif