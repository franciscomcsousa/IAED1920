/*
 * File:  structs.h
 * Author:  Francisco Sousa
 * Description: Ficheiro que define as estruturas e de cabecalho para structs.c
*/
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*a key correponde ao nome*/
#define keyj(a) (a->nome)
#define keye(a) (a->nome)

/*keys sao iguais se strcmp == 0*/
#define eqj(a, b) (strcmp(a, b) == 0)
#define eqe(a, b) (strcmp(a, b) == 0)

struct jogo{
    char* nome;
    char* equipaA;
    char* equipaB;
    unsigned int scoreA;
    unsigned int scoreB;
};

typedef struct jogo* pJogo;

struct equipa{
    char* nome;
    int vitorias;
};

typedef struct equipa* pEquipa;

typedef struct str_node  {
    struct str_node * next, * previous;
    pJogo pointer;
} node;

typedef struct  {
    struct str_node * head, * last;
} list;


void freeItemJ(pJogo a);
void freeItemE(pEquipa a);  

list* mk_list();
void add_last(list* lista, pJogo jogo);   
void free_list(list* l);                      
void remove_from_list(list* ls, pJogo jogo);

#endif