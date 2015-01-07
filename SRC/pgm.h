#ifndef __PGM_H__
#define __PGM_H__
/*! \file pgm.h
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief The PGMData structure
 * @details Allows us to read a *.pgm picture
 * 
 * @param row number of rows
 * @param col number of columns
 * @param max_gray the maximum gray scale value
 * @param matrix the value (in grayscale) of the picture (once it is read)
 */
typedef struct PGMData 
{
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int **allocate_dynamic_matrix(int row, int col);
 
void deallocate_dynamic_matrix(int **matrix, int row);

void skip_comments(FILE *fp);

PGMData* read_PGM(const char *file_name, PGMData *data);

#endif