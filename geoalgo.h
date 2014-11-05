#ifndef __GEOALGO_H__
#define __GEOALGO_H__

#include "linked_list.h"
#include "math_fn.h"

// TABLEAU GLOBAL //
Vertex *tablex[NB_VERTEX];

typedef struct
{
    Vertex *vmin;
    Vertex *vmax;
} EConv;

/*----------------------------------------------------------------------------------*/
//                                  Ordering functions
/*----------------------------------------------------------------------------------*/

int isSuperior(Vertex *p, Vertex *q, Dllist *dll, int LNK);

/*----------------------------------------------------------------------------------*/
//                                  Algos
/*----------------------------------------------------------------------------------*/

void graham(Dllist *dll);

void jarvis(Dllist *dll);

void lexconvex(Dllist *dll);

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////// FUSION /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


EConv miniConvexe(int idmin, int idmax);
  
EConv fusionec(EConv ECg, EConv ECd);

EConv divideAndConquer(int idmin, int idmax);

void divac(Dllist *dll);

/*----------------------------------------------------------------------------------*/
//                                  Merge sort
/*----------------------------------------------------------------------------------*/

Vertex* merge(Vertex *p, int P, Vertex* q, int Q, Dllist *dll, int LNK);

Vertex* sort(Vertex *p, int n, Dllist *dll, int LNK);

#endif