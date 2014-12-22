#ifndef __DLL_H__
#define __DLL_H__
/*! \file dll.h
 * \author M. Sainlot
 * \date 2014
 */

#include "vertex.h"

typedef struct dllist
{
	int length;
	Vertex *root;
} Dllist ;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll( Dllist *dll, Vertex* root ) ;

Dllist* create_dll( ) ;

void add_end_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

void add_begin_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

void insert_after( Dllist *dll, Vertex *prev, Vertex *ins, const int LNK ) ;

void rm_after( Dllist *dll, Vertex *prev, const int LNK ) ;

void rm_end_dll( Dllist *dll, const int LNK ) ;

void rm_begin_dll( Dllist *dll, const int LNK ) ;

void insert_btw( Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, const int LNK ) ;

void move_after( Vertex *p, Vertex *q, const int LNK ) ;

void swich_cells( Vertex *p, Vertex *q, const int LNK ) ;

void copy_order( Dllist *dll, const int SRC, const int DEST ) ;

void sew( Vertex *left, Vertex *right, const int LNK );

void add_dll_end_dll( Dllist *dll, Vertex *inf, Vertex *sup, const int len, const int LNK );

#endif