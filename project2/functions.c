/*
 * File:  functions.c
 * Author:  Francisco Sousa
 * Description: Ficheiro com as funcoes executaveis pelo utilizador.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

/*tamanho do buffer das strings*/
#define BUFFERSIZE 1024

/*strdup retorna uma nova string, copia da string c (com memoria ja alocada)*/
char *strdup(const char *c)
{
    char *dup = malloc(sizeof(char) * (strlen(c) + 1));
    if (dup != NULL)
       strcpy(dup, c);
    return dup;
}

/*adiciona um jogo*/
void add_jogo(HashtableJ table_jogos, HashtableE table_equipas, list* lista, int nl)
{
    pEquipa *pa = (pEquipa*) malloc(sizeof(pEquipa));
    pEquipa *pb = (pEquipa*) malloc(sizeof(pEquipa));
    pJogo j = (pJogo) malloc(sizeof(struct jogo));
    char buffer[3][BUFFERSIZE];

    scanf(" %[^:\n]:%[^:\n]:%[^:\n]:%u:%u", buffer[0], buffer[1], buffer[2], &(j->scoreA), &(j->scoreB));

    j->nome = strdup(buffer[0]);
    j->equipaA = strdup(buffer[1]);
    j->equipaB = strdup(buffer[2]);

    if(Jogosearch(j->nome, table_jogos) != NULL)
    {
        printf("%d Jogo existente.\n", nl);
        freeItemJ(j);
        free(pa);
        free(pb);
        return;
    }

    if ((*pa = Equipasearch(j->equipaA, table_equipas)) == NULL || (*pb = Equipasearch(j->equipaB, table_equipas)) == NULL)
    {
        printf("%d Equipa inexistente.\n", nl);
        freeItemJ(j);
        free(pa);
        free(pb);
        return;
    }

    if(j->scoreA > j->scoreB)
        (*pa)->vitorias++;
    else if (j->scoreB > j->scoreA)
        (*pb)->vitorias++;

    Jogoinsert(j, table_jogos);
    add_last(lista, j);

    free(pa);
    free(pb);
    return;
}

/*adiona uma equipa*/
void add_equipa(HashtableE table_equipas, int nl)
{
    pEquipa equipa = (pEquipa)malloc(sizeof(struct equipa));
    char buffer[BUFFERSIZE];

    scanf(" %[^:\n]", buffer);

    if((Equipasearch(buffer, table_equipas)) != NULL)
    {
        printf("%d Equipa existente.\n", nl);
        free(equipa);
        return;
    }

    equipa->nome = strdup(buffer);
    equipa->vitorias = 0;

    Equipainsert(equipa, table_equipas);

    return;
}

/*lista os jogos por ordem de insercao*/
void lista_jogos(const list* list, int nl)
{
    node *i;
    for (i = list->head; i != NULL; i = i->next)
    {
        printf("%d %s %s %s %d %d\n", nl, i->pointer->nome, i->pointer->equipaA, i->pointer->equipaB, i->pointer->scoreA, i->pointer->scoreB);
    }
}

/*remove um jogo*/
void remove_jogo(HashtableJ table_jogo, HashtableE table_equipa, list* lista, int nl)
{
    char buffer[BUFFERSIZE];
    pJogo jogo;
    scanf(" %[^:\n]", buffer);

    jogo = Jogosearch(buffer, table_jogo);

    if (jogo == NULL)
    {
        printf("%d Jogo inexistente.\n", nl);
        return;
    }
    if (jogo->scoreA > jogo->scoreB)
        Equipasearch(jogo->equipaA, table_equipa)->vitorias--;
    else if (jogo->scoreB > jogo->scoreA)
        Equipasearch(jogo->equipaB, table_equipa)->vitorias--;
    
    remove_from_list(lista, jogo);

    Jogodelete(jogo->nome, table_jogo);
}

/*procura um jogo*/
void procura_jogo(HashtableJ table_jogo, int nl)
{
    char buffer[BUFFERSIZE];
    pJogo jogo;
    scanf(" %[^:\n]", buffer);
    jogo = Jogosearch(buffer, table_jogo);
    if(jogo == NULL)
        printf("%d Jogo inexistente.\n", nl);
    else
         printf("%d %s %s %s %d %d\n", nl, jogo->nome, jogo->equipaA, jogo->equipaB, jogo->scoreA, jogo->scoreB);
}

/*procura uma equipa*/
void procura_equipa(HashtableE table_equipa, int nl)
{
    char buffer[BUFFERSIZE];
    pEquipa equipa;
    scanf(" %[^:\n]", buffer);
    equipa = Equipasearch(buffer, table_equipa);
    if(equipa == NULL)
        printf("%d Equipa inexistente.\n", nl);
    else
        printf("%d %s %d\n", nl, equipa->nome, equipa->vitorias);
}

/*altera o resultado de um jogo*/
void altera_score(HashtableJ table_jogo, HashtableE table_equipa, int nl)
{
    char buffer[BUFFERSIZE];
    pJogo jogo;
    int a, b;
    scanf(" %[^:\n]:%d:%d", buffer, &a, &b);
    if((jogo = Jogosearch(buffer, table_jogo)) == NULL)
    {
        printf("%d Jogo inexistente.\n", nl);
        return;
    }
    if (jogo->scoreA > jogo->scoreB)
        Equipasearch(jogo->equipaA, table_equipa)->vitorias--;
    if (jogo->scoreB > jogo->scoreA)
        Equipasearch(jogo->equipaB, table_equipa)->vitorias--;
    
    jogo->scoreA = a;
    jogo->scoreB = b;

    if(a > b)
        Equipasearch(jogo->equipaA, table_equipa)->vitorias++;
    if(b > a)
        Equipasearch(jogo->equipaB, table_equipa)->vitorias++;
    
    return;
}

/*funcao utilizada por qsort (que recebe como argumentos const void * a, const void * b)*/
int compare (const void * a, const void * b){
    return strcmp (*(const char **) a, *(const char **) b);
}

/*lista por ordem lexicografica as equipas com mais jogos ganhos*/
void mais_ganhos(HashtableE equipas, int nl)
{
    int i;
    int counter = 0;
    int maior = 0;
    char** t = (char**) malloc(sizeof(char*) * equipas->n_elementos);
    if(equipas->n_elementos == 0)
    {
        free(t);
        return;
    }
    for (i = 0; i < equipas->tamanho; i++)
    {
        if(equipas->table[i] != NULL && equipas->table[i]->vitorias > maior)
            maior = equipas->table[i]->vitorias;
    }
    for (i = 0; i < equipas->tamanho; i++)
    {
        if(equipas->table[i] != NULL && equipas->table[i]->vitorias == maior)
            t[counter++] = strdup(equipas->table[i]->nome);
    }
    qsort(t, counter, sizeof(const char *), compare);
    printf("%d Melhores %d\n", nl, maior);
    for (i = 0; i < counter; i++)
    {
        printf("%d * %s\n", nl, t[i]);
        free(t[i]);
    }
    free(t);
    return;
}