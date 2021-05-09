/*
 * File:  hashtablej.h
 * Author:  Francisco Sousa
 * Description: Ficheiro de cabecalho de hashtablee.c
*/
#ifndef HASHTABLEE_H
#define HASHTABLEE_H

#include "structs.h"

typedef struct tablee
{
    pEquipa* table;
    int tamanho;
    int n_elementos;
} *HashtableE;

HashtableE Equipainit(int size);
void Equipainsert(pEquipa item, HashtableE st);
pEquipa Equipasearch(char* v, HashtableE st);   

void freeTableE(HashtableE st);

#endif