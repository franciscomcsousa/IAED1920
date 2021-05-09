/*
 * File:  hashtablee.c
 * Author:  Francisco Sousa
 * Description: Ficheiro com todas as funcoes relativas a hashtable de equipas.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtablee.h"

/*funcao de hash para strings*/
int hashe(char *v, int M)
{
    int h = 0, a = 127;
    for (; *v != '\0'; v++)
        h = (a*h + *v) % M;
    return h;
}       

/*aumenta o tamanho da hashtable (rehash)*/
void Equipaexpand(HashtableE st) {
    int i;
    pEquipa *t = st->table;
    HashtableE new = Equipainit(st->tamanho * 2);
    for (i = 0; i < new->tamanho / 2; i++)
    {
        if (t[i] != NULL)
            Equipainsert(t[i], new);
    }
    st->table = new->table;
    st->tamanho = new->tamanho;
    free(t);
    free(new);
}

/*inicia hashtable com tamanho size*/
HashtableE Equipainit(int size) 
{
    int i;
    HashtableE st = (HashtableE)malloc(sizeof(struct tablee));
    st->n_elementos = 0;
    st->tamanho = size;
    st->table = (pEquipa*)malloc(st->tamanho*sizeof(pEquipa));
    for (i = 0; i < st->tamanho; i++)
        st->table[i] = NULL;
    return st;
}

/*insere pEquipa item na hashtable st*/
void Equipainsert(pEquipa item, HashtableE st) 
{
    char* v = keye(item);
    int i = hashe(v, st->tamanho);
    while (st->table[i] != NULL) i = (i+1) % st->tamanho;
        st->table[i] = item;
    st->n_elementos++;
    if (st->n_elementos > st->tamanho / 2)
        Equipaexpand(st);
}

/*procura pEquipa de nome v (key) na hashtable st*/
pEquipa Equipasearch(char* v, HashtableE st)
{
    int i = hashe(v, st->tamanho);
    while (st->table[i] != NULL)
    {
        if (eqe(v, keye(st->table[i])))
            return st->table[i];
        else
            i = (i+1) % st->tamanho;
    }
    return NULL;
}

/*liberta toda a memoria associada a hashtable st*/
void freeTableE(HashtableE st)
{
    int i;
    for (i = 0; i < st->tamanho; i++)
    {
        if (st->table[i] != NULL)
        {
            freeItemE(st->table[i]);
        }
    }
    free(st->table);
    free(st);
}