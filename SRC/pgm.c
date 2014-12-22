/*! \file pgm.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include "pgm.h"

#define HI(num) (((num) & 0x0000FF00) >> 8)
#define LO(num) ((num) & 0x000000FF)

int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;
 
    ret_val = (int **)malloc(sizeof(int *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
 
    for (i = 0; i < row; ++i)
    {
        ret_val[i] = (int *)malloc(sizeof(int) * col);
        if (ret_val[i] == NULL)
        {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
 
    return ret_val;
}
 
void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;
 
    for (i = 0; i < row; ++i)
        free(matrix[i]);
    free(matrix);
}


void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
 
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#')
    {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } 
    else
        fseek(fp, -1, SEEK_CUR);
}


/*for reading:*/
PGMData* readPGM(const char *file_name, PGMData *data)
{
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
 
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL)
    {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
 
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5"))
    {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
 
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);
 
    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    if (data->max_gray > 255)
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j)
            {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i][j] = (hi << 8) + lo;
            }
    else
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j)
            {
                lo = fgetc(pgmFile);
                data->matrix[i][j] = lo;
            }
 
    fclose(pgmFile);
    return data;
 
}
 