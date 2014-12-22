#ifndef __PRINT_H__
#define __PRINT_H__
/*! \file print_fn.h
 * \author M. Sainlot
 * \date 2014
 */

#include "dll.h"
#include "vertex.h"
#include "simplex.h"
#include "fdp.h"

/*----------------------------------------------------------------------------------*/
//                                  Print functions prototypes
/*----------------------------------------------------------------------------------*/

void print_lnk(int const LNK);

void print_info(Dllist const *dll, int const LNK);

void print_vertex(Vertex const *vert);

void print_simplex(Simplex const *simp);

void print_fdp(FDP const *fdp );

void print_dll(Dllist const *dll, int const LNK);

void print_dll_full(Dllist const *dll, int const LNK);

#endif