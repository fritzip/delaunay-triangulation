#ifndef __PGM_H__
#define __PGM_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _PGMData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int **allocate_dynamic_matrix(int row, int col);
 
void deallocate_dynamic_matrix(int **matrix, int row);

void SkipComments(FILE *fp);

PGMData* readPGM(const char *file_name, PGMData *data);

void writePGM(const char *filename, const PGMData *data);

#endif