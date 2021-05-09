/*
 * File:  main.c
 * Author:  Francisco Sousa
 * Description: Ficheiro main do projeto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

/*tamanho incial da hashtable, quer de equipas, quer de jogos*/
#define INITSIZE 127

int main()
{
    HashtableJ table_jogos = Jogoinit(INITSIZE);
    HashtableE table_equipas = Equipainit(INITSIZE);
    list* lista = mk_list();

    int c;
    int nl = 1;

    while ((c = getchar()) != 'x')
    {
        switch (c)
        {
        case 'a':
            add_jogo(table_jogos, table_equipas, lista, nl);
            break;
        case 'A':
            add_equipa(table_equipas, nl);
            break;
        case 'l':
            lista_jogos(lista, nl);
            break;
        case 'p':
            procura_jogo(table_jogos, nl);
            break;
        case 'P':
            procura_equipa(table_equipas, nl);
            break;
        case 'r':
            remove_jogo(table_jogos, table_equipas, lista, nl);
            break;
        case 's':
            altera_score(table_jogos, table_equipas, nl);
            break;
        case 'g':
            mais_ganhos(table_equipas, nl);
            break;
        default:
            nl++;
            break;
        }
    }
    freeTableE(table_equipas);
    freeTableJ(table_jogos);
    free_list(lista);
    return 0;
}