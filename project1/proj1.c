/*
# File:  proj1.c
# Author:  Francisco Sousa ist195579
# Description: Sistema de logistica na linguagem C
*/

#include <stdio.h>
#include <string.h>

#define STRMAX 63       /*tamanho maximo que a string da descricao de um produto pode ter*/
#define IDMAX 10000     /*o numero maximo de produtos*/
#define ORDERMAX 500    /*o numero maximo de encomendas*/
#define WEIGHTMAX 200   /*peso maximo de um produto*/
#define PRODMAX 200     /*numero maximo de produtos numa encomenda (peso minimo eh um)*/

#define NO 0
#define YES 1

/*estrutura correspondente a um produto*/
typedef struct
{
    int id;
    char descricao[STRMAX];
    int preco;
    int peso;
    int stock;
}Produto;

/*utilizada pela estrutura Encomenda para armazenar a quandidade e o id de cada produto nessa encomenda*/
typedef struct
{
    int quantidade;
    int id_produto;
}Data;

/*estrutura correspondente a uma encomenda*/
typedef struct
{
    int peso;
    int num_produtos;
    Data produtos[PRODMAX];
}Encomenda;

/*usado pela funcao quicksort*/
typedef struct
{
    int id_produto;
    int to_sort;
    char *desc;
}ToSort;

/*procura o indice do produto na encomenda*/
int produto_in_encomenda(int idp, Encomenda encomenda)
{
    int i;
    for (i = 0; i < encomenda.num_produtos; i++)
    {
        if (idp == encomenda.produtos[i].id_produto) /*se o idp corresponder com o procurado*/
            return i;                                /*devolve o indice do produto na encomenda*/
    }
    return -1; /*se produto nao estiver na encomenda, devolve -1*/
}

/*recebe um vetor[] (estrutura de 2 variaveis), remove o vetor[i_repor] e "empurra" para a esquerda todos os indices a direita de i_repor*/
/*usado pela funcao R para organizar os produtos duma encomenda apos um produto ser retirado desta*/
void repor_vetor(Data vetor[], int i_repor, int i_limite)
{
    int i;
    for (i = i_repor; i < i_limite; i++)
    {
        /*todos os termos para a direita do i_repor repoeem os anterioes*/
        vetor[i] = vetor[i + 1];
    }
    /*reinicia as variaveis da estrutura de Data*/
    vetor[i_limite].id_produto = 0;
    vetor[i_limite].quantidade = 0;
}

/*adiciona um novo produto ao sistema*/
void a(Produto prod[IDMAX], int prod_proximo)
{
    /*prod_proximo eh o produto que vamos preencher*/
    scanf(" %[^:]:%d:%d:%d", prod[prod_proximo].descricao, &prod[prod_proximo].preco, &prod[prod_proximo].peso, &prod[prod_proximo].stock);
    prod[prod_proximo].id = prod_proximo;
    printf("Novo produto %d.\n", prod[prod_proximo].id);
}

/*adiciona stock a um produto existente no sistema*/
void q(Produto prod[IDMAX], int prod_proximo)
{
    int idp;
    int qnt;
    scanf(" %d:%d", &idp, &qnt);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    if (idp >= prod_proximo || idp < 0)
        printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", idp);
    else 
    /*stock de um produto eh a soma da nova quantidade com a anterior*/
        prod[idp].stock += qnt;
}

/*cria uma nova encomenda*/
void N(Encomenda enco[ORDERMAX], int enco_proximo)
{
    if (enco_proximo <= ORDERMAX - 1)
    /*o indice da encomenda nao pode ser ORDERMAX ou acima*/
    {
        enco[enco_proximo].peso = 0;
        enco[enco_proximo].num_produtos = 0;
        printf("Nova encomenda %d.\n", enco_proximo);
    }
}

/*adiciona um produto a uma encomenda*/
void A(Produto prod[IDMAX], Encomenda enco[ORDERMAX], int prod_proximo, int enco_proximo)
{
    int ide;
    int idp;
    int qnt;
    int in_order; /* in_order eh o indice do produto na encomenda*/
    scanf(" %d:%d:%d", &ide, &idp, &qnt);
    /*se indice da encomenda eh maior do que o da proxima ou menor que 0, nao existe*/
    if (ide >= enco_proximo || ide < 0)
        printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    else if (idp >= prod_proximo || idp < 0)
        printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", idp, ide);
    /*se a quantidade eh maior que o stock, stock eh insuficiente*/
    else if (prod[idp].stock - qnt < 0)
        printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", idp, ide);
    /*se ao adicionarmos o peso total do novo produto a encomenda, esta nao poder exceder WEIGHTMAX*/
    else if (enco[ide].peso + prod[idp].peso * qnt > WEIGHTMAX)
        printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de %d.\n", idp, ide, WEIGHTMAX);
    else
    {
        /*caso produto esteja na encomenda*/
        if ((in_order = produto_in_encomenda(idp, enco[ide])) != -1)
            /*adiciona quantidade a produto existente*/
            enco[ide].produtos[in_order].quantidade += qnt; 
        /*caso nao esteja na encomenda*/
        else
        {
            /*introduz produto a encomenda*/
            enco[ide].produtos[enco[ide].num_produtos].id_produto = idp;
            enco[ide].produtos[enco[ide].num_produtos].quantidade = qnt;
            enco[ide].num_produtos += 1;
            /*num_produtos eh o indice do novo produto na encomenda*/
        }
        /*quer esteja quer nao, stock do produto diminui e peso da encomenda aumenta*/
        prod[idp].stock -= qnt;
        enco[ide].peso += prod[idp].peso * qnt;
    }
}

/*remove stock a um produto existente*/   
void r(Produto prod[IDMAX], int proximo)
{
    int idp;
    int qnt;
    scanf(" %d:%d", &idp, &qnt);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    if (idp >= proximo || idp < 0)
        printf("Impossivel remover stock do produto %d. Produto inexistente.\n", idp);
    /*se a quantidade eh maior que o stock, stock eh insuficiente*/
    else if (prod[idp].stock - qnt < 0)
        printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", qnt, idp);
    else 
        /*remover stock é subtrair a quantidade desejada ao stock*/
        prod[idp].stock -= qnt;
}

/*remove um produto de uma encomenda*/
void R(Produto prod[IDMAX], Encomenda enco[ORDERMAX], int prod_proximo, int enco_proximo)
{
    int idp;
    int ide;
    int in_order; /* in_order eh o indice do produto na encomenda*/
    scanf(" %d:%d", &ide, &idp);
    /*se indice da encomenda eh maior do que o da proxima ou menor que 0, nao existe*/
    if (ide >= enco_proximo || ide < 0)
        printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    else if (idp >= prod_proximo || idp < 0)
        printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", idp, ide);
    /*se o produto estiver na encomenda*/  
    else if ((in_order = produto_in_encomenda(idp, enco[ide])) != -1)
    {
        prod[idp].stock += enco[ide].produtos[in_order].quantidade;                 /*ao remover um produto de uma encomenda, stock eh reposto*/
        enco[ide].peso -= enco[ide].produtos[in_order].quantidade * prod[idp].peso; /*subtrai-se ao peso da encomenda qnt do produto na encomenda * peso do produto*/
        enco[ide].num_produtos -= 1;                                                /*encomenda fica com menos um produto*/
        repor_vetor(enco[ide].produtos, in_order, enco[ide].num_produtos);          /*vetor eh reposto, eliminando o produto de indice de encomenda "in_order"*/
    }
}

/*calcula o custo de uma encomenda*/
void C(Produto prod[IDMAX], Encomenda enco[ORDERMAX], int enco_proximo)
{
    int i, ide, idp, in_order, custo = 0;
    scanf(" %d", &ide);
    /*se indice da encomenda eh maior do que o da proxima ou menor que 0, nao existe*/
    if (ide >= enco_proximo || ide < 0)
        printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", ide);
    else
    {
        for (i = 0; i < enco[ide].num_produtos; i++)
        {
            idp = enco[ide].produtos[i].id_produto;                                 /*obtem o idp*/
            in_order = produto_in_encomenda(idp, enco[ide]);                        /*obtem o indice do produto na encomenda*/
            custo += prod[idp].preco * enco[ide].produtos[in_order].quantidade;     /*soma o preco do produto ao custo*/
        }
        printf("Custo da encomenda %d %d.\n", ide, custo);
    }
}

/*altera o preço de um produto existente no sistema*/
void p(Produto prod[IDMAX], int prod_proximo)
{
    int idp;
    int novo_preco;
    scanf(" %d:%d", &idp, &novo_preco);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    if (idp >= prod_proximo || idp < 0)
        printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", idp);
    else
        prod[idp].preco = novo_preco;   /*preco passa a ser o novo_preco*/
}

/*retorna a descrição e a quantidade de um produto numa dada encomenda*/
void E(Produto prod[IDMAX], Encomenda enco[ORDERMAX], int prod_proximo, int enco_proximo)
{
    int ide;
    int idp;
    int in_order;
    scanf(" %d:%d", &ide, &idp);
    /*se indice da encomenda eh maior do que o da proxima ou menor que 0, nao existe*/
    if (ide >= enco_proximo || ide < 0)
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    else if (idp >= prod_proximo || idp < 0)
        printf("Impossivel listar produto %d. Produto inexistente.\n", idp);
    else
    {
        if ((in_order = produto_in_encomenda(idp, enco[ide])) != -1)
            /*se o produto existir, a quantidade eh a quantidade do proprio produto*/
            printf("%s %d.\n", prod[idp].descricao, enco[ide].produtos[in_order].quantidade);
        else
            /*se o produto nao existir, a quantidade eh 0*/
            printf("%s %d.\n", prod[idp].descricao, 0);
    }
}

/*retorna o identificador da encomenda em que um dado produto ocorre mais vezes*/
void m(Encomenda enco[ORDERMAX], int prod_proximo, int enco_proximo)
{
    int i;
    int idp;
    int in_order;
    int maior = - 1; /*variavel inicia a -1 caso nao exista produto idp em nenhuma encomenda*/
    scanf(" %d", &idp);
    /*se indice do produto eh maior do que o do proximo ou menor que 0, nao existe*/
    if (idp >= prod_proximo || idp < 0)
        printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", idp);
    else
    {
        for (i = 0; i < enco_proximo; i++)
        {
            /*in_order eh diferente para cada encomenda, logo eh esta dentro do ciclo*/
            in_order = produto_in_encomenda(idp, enco[i]);
            if (in_order != -1)
            {
                /*como o proximo "i" sera sempre um ide maior que "maior", apenas troca-los quando "i" eh maior organiza ide em caso de igualdade de quantidade*/
                if (enco[i].produtos[in_order].quantidade > enco[maior].produtos[produto_in_encomenda(idp, enco[maior])].quantidade)
                {
                    /*sempre que a quantidade de idp for maior para essa ecomenda de indice i, maior passa a ser i*/
                    maior = i;
                }
            }
        }
        if (maior != -1)
            /*apenas apresenta o maior produto caso ele exista*/
            printf("Maximo produto %d %d %d.\n", idp, maior, enco[maior].produtos[produto_in_encomenda(idp, enco[maior])].quantidade);
    }
}

/*funcao auxiliar a quicksort*/
/*reorganiza a lista com todos os elementos menores que o pivot para a esquerda e os maiores para a direita*/
int partition(ToSort a[], int left, int right, int alphaSwitch)
{
    ToSort temp;
    int pivot = right;  /*pivot eh o ultimo elemento da lista*/
    int i = left;       /*iterador da esquerda*/
    int j = right;      /*iterador da direita*/
    while(i < j)
    {
        /*alphaSwitch alterna entre organizacao alfabetica e nao alfabetica*/
        if (alphaSwitch == YES)
        {
            /*eh utilizado o strcmp para organizar strings por ordem alfabetica*/
            /*desc aponta para uma string*/
            while(strcmp(a[i].desc, a[pivot].desc) < 0)
                i++;
            while((strcmp(a[j].desc, a[pivot].desc) >= 0) && j > left)
                j--;
        }
        else
        {
            /*neste caso, compara-se o valor das variaveis to_sort*/
            while(a[i].to_sort < a[pivot].to_sort)
                i++;
            while(a[j].to_sort >= a[pivot].to_sort && j > left)
                j--;
        }
        if(i < j)
        {
            /*swap(a[i], a[j])*/
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    /*swap(a[pivot], a[i])*/
    temp = a[pivot];
    a[pivot] = a[i];
    a[i] = temp;
    return i;
}

/*funcao que utiliza o algoritmo quick sort para ordenar um array de ordem crescente
com a opcao de organizar de forma alfabetica*/
void quicksort(ToSort a[] , int left, int right, int alphaSwitch)
{
    int i;
    /*funcao apenas para quando right <= left (iteradores encontram-se)*/
    if(left < right)
    {
        /**/
        i = partition(a, left, right, alphaSwitch);
        /*repete quicksort para a esquerda e para a direita*/
        quicksort(a,left,i - 1, alphaSwitch);
        quicksort(a,i + 1,right, alphaSwitch);
    }
}

/*lista todos os produtos existentes no sistema por ordem crescente de preco*/
void l(Produto prod[IDMAX], int prod_proximo)
{
    int i;
    int high;
    int low = -1;
    ToSort lista[IDMAX]; /*ToSort eh a estrutura de entrada na funcao quicksort*/
    for (i = 0; i < prod_proximo; i++)
    {
        /*copia os produtos para uma lista a ordenar*/
        lista[i].id_produto = i;
        lista[i].to_sort = prod[i].preco;
    }
    /*executa a ordenacao em toda a lista, alphaSwitch = NO, nao eh organizacao alfabetica*/
    quicksort(lista, 0, prod_proximo - 1, NO);
    for (i = 1; i < prod_proximo + 1; i++)
    {
        /*se existem elementos com o mesmo preco, serao organizados por ordem crescente de idp*/
        if (lista[i].to_sort == lista[i - 1].to_sort)
        {
            /*se low ainda nao foi iniciado*/
            if (low == -1)
                /*low eh o indice anterior*/
                low = i - 1;
            /*transfere ids para a variavel to_sort da estrutura ToSort*/
            lista[i - 1].to_sort = lista[i - 1].id_produto;
        }
        else
        {
            /*se low foi iniciado*/
            if (low != -1)
            {
                high = i - 1;                                   /*high eh definido*/
                lista[high].to_sort = lista[high].id_produto;   /*transfere id para a variavel to_sort da estrutura ToSort*/
                quicksort(lista, low, high, NO);                /*faz o quicksort da lista no intervalo [low, high], no qual estao os produtos com o mesmo custo*/
                low = -1;                                       /*low volta a ser desativado, e continua a procura por precos iguais*/
            }
        }
    }
    printf("Produtos\n");
    for (i = 0; i < prod_proximo; i++)
    {
        printf("* %s %d %d\n", prod[lista[i].id_produto].descricao, prod[lista[i].id_produto].preco, prod[lista[i].id_produto].stock);
    }
}

/*lista todos os produtos de uma encomenda por ordem alfabetica da descricao*/
void L(Produto prod[IDMAX], Encomenda enco[ORDERMAX], int enco_proximo)
{
    int ide;
    int idp;
    int i;
    ToSort lista[PRODMAX];
    /*se indice da encomenda eh maior do que o da proxima ou menor que 0, nao existe*/
    scanf("%d", &ide);
    if (ide >= enco_proximo || ide < 0)
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
    else
    {
        for (i = 0; i < enco[ide].num_produtos; i++)
        {
            /*transfere pointers das descricoes para a variavel to_sort da estrutura ToSort*/
            lista[i].id_produto = enco[ide].produtos[i].id_produto;
            lista[i].desc = prod[lista[i].id_produto].descricao;
        }
        /*organiza alfabeticamente toda a lista, alphaSwitch = YES, eh uma ordenacao alfabetica*/
        quicksort(lista, 0, enco[ide].num_produtos - 1, YES);
        printf("Encomenda %d\n", ide);
        for (i = 0; i < enco[ide].num_produtos; i++)
        {
            idp = lista[i].id_produto;
            printf("* %s %d %d\n", prod[idp].descricao, prod[idp].preco, enco[ide].produtos[produto_in_encomenda(idp, enco[ide])].quantidade);
        }
    }
}

int main()
{
    char c;
    int prod_proximo = 0;           /*indice do proximo produto*/
    int enco_proximo = 0;           /*indice da proxima encomenda*/
    Encomenda encomendas[ORDERMAX]; /*lista de encomendas*/
    Produto produto[IDMAX];         /*lista de produtos*/
    while ((c = getchar()))
    {
        switch (c)
        {
        case 'a':
            a(produto, prod_proximo);
            prod_proximo += 1;
            break;
        case 'q':
            q(produto, prod_proximo);
            break;
        case 'N':
            N(encomendas, enco_proximo);
            enco_proximo += 1;
            break;
        case 'A':
            A(produto, encomendas, prod_proximo, enco_proximo);
            break;
        case 'r':
            r(produto, prod_proximo);
            break;
        case 'R':
            R(produto, encomendas, prod_proximo, enco_proximo);
            break;
        case 'C':
            C(produto, encomendas, enco_proximo);
            break;
        case 'p':
            p(produto, prod_proximo);
            break;
        case 'E':
            E(produto, encomendas, prod_proximo, enco_proximo);
            break;
        case 'm':
            m(encomendas, prod_proximo, enco_proximo);
            break;
        case 'l':
            l(produto, prod_proximo);
            break;
        case 'L':
            L(produto, encomendas, enco_proximo);
            break;
        case 'x':
            return 0;
        }
    }
    return 1;
}