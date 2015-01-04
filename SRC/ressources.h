#ifndef __RESSOURCES_H__
#define __RESSOURCES_H__
/*! \file ressources.h
 * \author M. Sainlot
 * \date 2014
 */

/*----------------------------------------------------------------------------------*/
//                                  #define
/*----------------------------------------------------------------------------------*/

#define DIM 3

#define BWD 0 
#define FWD 1

#define DEL 0 // Delaunay stack link 
#define NEW 1 // Newly created or recently modify simplex stack link
#define NB_STK 2 // Number of stack : to increment manually, LAST + 1

#define STD 0
#define NB_LNK 1 // Number of link : to increment manually, LAST + 1

// #define OFFSET 100
 
#define minX	0
#define maxX	1
#define minY	0
#define maxY	1
#define margin	0.01

#endif