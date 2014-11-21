#ifndef __RESSOURCES_H__
#define __RESSOURCES_H__
/*! \file ressources.h
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

/*----------------------------------------------------------------------------------*/
//                                  #define
/*----------------------------------------------------------------------------------*/

#define DEBUG 0

#define DIM 2

#define BWD 0 
#define FWD 1

#define STD 0
#define LEX 1
#define POL 2
#define GRA 3
#define JAR 4
#define LXC 5
#define DAC 6
#define NBL 7 // Nb de Liens : A incrémenter manuellement, LAST + 1

#define NB_H 4
#define NB_VERTEX 33
#define NB_SIMPLEX ( 2*(NB_VERTEX - 1) - NB_H )

#define minX	0
#define maxX	500
#define minY	0
#define maxY	500
#define margin	10

#endif