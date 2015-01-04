#ifndef __DLL_H__
#define __DLL_H__
/*! \file dll.h
 * \author M. Sainlot
 * \date 2014
 */

#include "vertex.h"

typedef struct Dllist
{
	int length;
	Vertex *root;
} Dllist ;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll( Dllist *dll, Vertex* root ) ;

Dllist* create_dll( ) ;

/**
 * @brief Add a vertex (vert) at the end of the dll given a link (LNK)
 * @details  root <=> a <=> b <=> c <=> d      <=>     root
 * @details  root <=> a <=> b <=> c <=> d <=> vert <=> root
 * 
 * @param dll
 * @param vert
 * @param LNK
 */
void add_end_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

/**
 * @brief Add a vertex (vert) at the begining of the dll given a link (LNK)
 * @details  root     <=>      a <=> b <=> c <=> d <=> root
 * @details  root <=> vert <=> a <=> b <=> c <=> d <=> root
 * 
 * @param dll 
 * @param vert 
 * @param LNK 
 */
void add_begin_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

/**
 * @brief Insert a vertex (ins) after another (prev) in a dll given a link (LNK)
 * @details  root <=> a <=> prev     <=>     b <=> c <=> d <=> e <=> f <=> root 
 * @details  root <=> a <=> prev <=> ins <=> b <=> c <=> d <=> e <=> f <=> root 
 * @param dll
 * @param prev
 * @param ins
 * @param LNK
 */
void insert_after( Dllist *dll, Vertex *prev, Vertex *ins, const int LNK ) ;

/**
 * @brief Remove the vertex after prev in a dll given a link (LNK)
 * @details root <=> a <=> prev <=> b <=> c <=> d <=> e <=> f <=> root 
 * @details root <=> a <=> prev    <=>    c <=> d <=> e <=> f <=> root 
 * @param dll 
 * @param prev 
 * @param LNK 
 */
void rm_after( Dllist *dll, Vertex *prev, const int LNK ) ;

/**
 * @brief Remove the last vertex of a dll given a link (LNK)
 * @details root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
 * @details root <=> a <=> b <=> c <=> d <=> e    <=>    root 
 * 
 * @param dll 
 * @param LNK 
 */
void rm_end_dll( Dllist *dll, const int LNK ) ;

/**
 * @brief Remove the first vertex of a dll given a link (LNK)
 * @details root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
 * @details root    <=>    b <=> c <=> d <=> e <=> f <=> root 
 * 
 * @param dll
 * @param LNK
 */
void rm_begin_dll( Dllist *dll, const int LNK ) ;

/**
 * @brief Insert a vertex (ins) between two vertices (inf and sup) of a dll given a link (LNK)
 * @details root <=> a <=> inf <=> b <=> c <=> d <=> sup <=> e <=> f <=> root 
 * @details root <=> a <=> inf <=>      ins      <=> sup <=> e <=> f <=> root
 * 
 * @param dll
 * @param inf
 * @param sup
 * @param ins
 * @param LNK
 */
void insert_btw( Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, const int LNK ) ;

/**
 * @brief Move a vertex (move) after another (prev) given a link (LNK)
 * @details root <=> a <=> prev <=> b <=> c <=> d <=> move <=> e <=> f <=> root 
 * @details root <=> a <=> prev <=> move <=> b <=> c <=> d <=> e <=> f <=> root 
 * 
 * @param p
 * @param q
 * @param LNK
 */
void move_after( Vertex *prev, Vertex *move, const int LNK ) ;

/**
 * @brief Switch two vertices given a link (LNK)
 * @details root <=> a <=> P <=> b <=> c <=> d <=> Q <=> e <=> f <=> root 
 * @details root <=> a <=> Q <=> b <=> c <=> d <=> P <=> e <=> f <=> root 
 * 
 * @param p
 * @param q
 * @param LNK
 */
void swich_cells( Vertex *p, Vertex *q, const int LNK ) ;

/**
 * @brief Copy the order of the link SRC of a dll to the link DEST of tho dll
 * @details Usefull for intializing a link order (which is NULL by default)
 * 
 * @param dll
 * @param SRC
 * @param DEST
 */
void copy_order( Dllist *dll, const int SRC, const int DEST ) ;

/**
 * @brief Sew two vertices together given a link (LNK) 
 * @details left <=> right
 * 
 * @param left
 * @param right
 * @param LNK
 */
void sew( Vertex *left, Vertex *right, const int LNK );

/**
 * @brief Insert a portion of a dll (defined by two vertices inf and sup) into another dll given a link (LNK)
 * 
 * @param dll 
 * @param inf lower bound vertex of the portion to insert
 * @param sup upper bound vertex of the portion to insert 
 * @param len length of the portion to insert
 * @param LNK 
 */
void add_dll_end_dll( Dllist *dll, Vertex *inf, Vertex *sup, const int len, const int LNK );

#endif