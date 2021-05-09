/*
 * File:  hashtablej.c
 * Author:  Francisco Sousa
 * Description: Ficheiro com todas as funcoes relativas a hashtable de jogos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtablej.h"

/*funcao de hash para strings*/
int hashj(char *v, int M)
{
    int h = 0, a = 127;
    for (; *v != '\0'; v++)
        h = (a*h + *v) % M;
    return h;
}       

/*aumenta o tamanho da hashtable (rehash)*/
void Jogoexpand(HashtableJ st) 
{
    int i;
    pJogo *t = st->table;
    HashtableJ new = Jogoinit(st->tamanho * 2);
    for (i = 0; i < new->tamanho / 2; i++)
    {
        if (t[i] != NULL)
            Jogoinsert(t[i], new);
    }
    st->table = new->table;
    st->tamanho = new->tamanho;
    free(t);
    free(new);
}

/*inicia hashtable com tamanho size*/
HashtableJ Jogoinit(int size) 
{
    int i;
    HashtableJ st = (HashtableJ)malloc(sizeof(struct tablej));
    st->n_elementos = 0;
    st->tamanho = size;
    st->table = (pJogo*)malloc(st->tamanho*sizeof(pJogo));
    for (i = 0; i < st->tamanho; i++)
        st->table[i] = NULL;
    return st;
}

/*insere pJogo item na hashtable st*/
void Jogoinsert(pJogo item, HashtableJ st) 
{
    char* v = keyj(item);
    int i = hashj(v, st->tamanho);
    while (st->table[i] != NULL) i = (i+1) % st->tamanho;
        st->table[i] = item;
    st->n_elementos++;
    if(st->n_elementos > st->tamanho / 2)
        Jogoexpand(st);
}

/*remove pJogo de nome nome na hashtable st*/
void Jogodelete(char* nome, HashtableJ st)
{
    int j, i = hashj(nome, st->tamanho);
    pJogo v;
    while (st->table[i] != NULL)
    {
        if (keyj(st->table[i]) == nome) 
            break;
        else 
            i = (i+1) % st->tamanho;
    }
    if (st->table[i] == NULL) return;

    freeItemJ(st->table[i]);
    st->table[i] = NULL;
    st->n_elementos--;

    for (j = (i+1) % st->tamanho; st->table[j] != NULL; j = (j+1) % st->tamanho) 
    {
        v = st->table[j];
        st->table[j] = NULL;
        Jogoinsert(v, st);
    }
}

/*procura pJogo de nome v na hashtable st*/
pJogo Jogosearch(char* nome, HashtableJ st)
{
    int i = hashj(nome, st->tamanho);
    while (st->table[i] != NULL)
    {
        if (eqj(nome, keyj(st->table[i])))
            return st->table[i];
        else
            i = (i+1) % st->tamanho;
    }
    return NULL;
}

/*liberta toda a memoria associada a hashtable st*/
void freeTableJ(HashtableJ st)
{
    int i;
    for (i = 0; i < st->tamanho; i++)
    {
        if (st->table[i] != NULL)
        {
            freeItemJ(st->table[i]);
        }
    }
    free(st->table);
    free(st);
}

