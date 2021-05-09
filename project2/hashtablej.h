/*
 * File:  hashtablej.h
 * Author:  Francisco Sousa
 * Description: Ficheiro de cabecalho de hashtablej.c
*/
#ifndef HASHTABLEP_H
#define HASHTABLEP_H

#include "structs.h"

typedef struct tablej
{
    pJogo* table;
    int tamanho;
    int n_elementos;
} *HashtableJ;

HashtableJ Jogoinit(int size);
void Jogoinsert(pJogo item, HashtableJ st);
void Jogodelete(char* nome, HashtableJ st); 
pJogo Jogosearch(char* nome, HashtableJ st);    

void freeTableJ(HashtableJ st);

#endif