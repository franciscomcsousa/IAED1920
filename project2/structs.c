/*
 * File:  hashtablej.h
 * Author:  Francisco Sousa
 * Description: Ficheiro com todas as funcoes relativas a estruturas de dados.
*/
#include "structs.h"

/*liberta toda a memoria associada ao pJogo a*/
void freeItemJ(pJogo a)
{
    if (a != NULL)
    {
        free(a->nome);
        free(a->equipaA);
        free(a->equipaB);
        free(a);
    }
}

/*liberta toda a memoria associada ao pEquipa a*/
void freeItemE(pEquipa a)
{
    if(a != NULL)
    {
        free(a->nome);
        free(a);
    }
}

/* cria nova lista vazia */
list* mk_list()
{
    list *lista;
    lista = (list*) malloc(sizeof(list));
    lista->head = NULL;
    lista->last = NULL;
    return lista;
}

/* adiciona o pJogo jogo como o ultimo elemento da lista */
void add_last(list* ls, pJogo jogo)
{
    node* new_head = (node*) malloc(sizeof(node));

    new_head->pointer = jogo;
    new_head->next = NULL;
    new_head->previous = ls->last;

    if (ls->head == NULL)
    {
        ls->head = new_head;
        ls->last = new_head;
    }
    else
    {
        ls->last->next = new_head;
        ls->last = new_head;
    }
}

/* liberta toda a memoria associada a lista */
void free_list(list* ls){

    node* aux;

    while(ls->head != NULL)
    {
        aux = ls->head;
        ls->head = ls->head->next;
        free(aux);
    }
    free(ls);
}

/*remove o pJogo jogo da lista*/
void remove_from_list(list* ls, pJogo jogo)
{
    node* i;

    for (i = ls->head; i->pointer != jogo; i = i->next)
    ;

    if(i == ls->head)
        ls->head = i->next;
    if(i == ls->last)
        ls->last = i->previous;

    if(i->next != NULL)
        i->next->previous = i->previous;
    if(i->previous != NULL)
        i->previous->next = i->next;
    
    free(i);
}