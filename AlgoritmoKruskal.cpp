#include <cstdio>
#include <cstring>
#include <ctime>
#include <algorithm>
#define TamInit 2000000

struct Vertice;
struct Vizinho;


struct Vertice{
    int dado;
    int grau = 0;
    int qtVizinhos = 0;
    Vizinho *comecoVizinho = NULL;
    Vizinho *finalVizinho = NULL;

};
struct Vizinho
{
    int peso;
    Vertice *pai;
    Vertice *vizinho = NULL;
    Vizinho *prox = NULL;
};

struct Grafo{
    Vertice **ListaVertices = NULL;
    bool direcionado = false;
    int qtVertices = 0;
    int maxVertices;
};

//DECLARAÇÕES DE FUNÇÕES
void liberaVertices(Grafo *g , int tamanho);
void inicializaGrafo(Grafo **gr , int tamanho);
void LiberaGrafo(Grafo **g);
bool temVizinho(Vertice *v, Vertice *buscado);
bool conectarUM(Grafo *g, int v1 , int v2 , int peso);
void conectar(Grafo *g, int v1 , int v2 , int peso);
bool inserirVertice(Grafo *g , int c);
void lerArquivoDOT(Grafo **g , char*arquivo);
bool temVertice(Grafo *g, int dado);
void printGrafo(Grafo *g);
Vizinho *menorVizinho(Vertice *raiz);
void algoritmoKruskal(Grafo **g);
void geraArquivoDOT(Grafo *g);

int main()
{
    Grafo *grafo = NULL;
    clock_t inicio, fim;
    double time;
    char arquivo[] = "g01_mini.dot";
    inicio = clock();
    inicializaGrafo(&grafo , TamInit);
    printf("\n\n arquivo lido: %s\n\n" , arquivo);
    lerArquivoDOT(&grafo , arquivo);
    algoritmoKruskal(&grafo);
    fim = clock();
    time = (double)(fim - inicio)/CLOCKS_PER_SEC;
    printf("\n\nTempo de execução do codigo: %lf segundos ou  %lf minutos" , time, time/60 );
    printf("\n\n%d" , grafo->qtVertices);
    geraArquivoDOT(grafo);
    LiberaGrafo(&grafo);
    return 0;
}


//FUNÇÕES 
void liberaVertices(Grafo *g , int tamanho){
    Vizinho *del, *atual;
    for(int i  = 0; i < g->maxVertices;i++)
    {
        if(g->ListaVertices[i] != NULL)
        {
            atual = g->ListaVertices[i]->comecoVizinho;
            while (atual != NULL)
            {
                del = atual;
                atual = atual->prox;
                delete del;
            }
            delete g->ListaVertices[i];
            g->ListaVertices[i] = NULL;
        }
    }
    g->maxVertices = tamanho;
}

void inicializaGrafo(Grafo **gr , int tamanho){
    if (*gr == NULL)
    {
        Grafo *temp;
        *gr = new Grafo;
        temp = *gr;
        temp->ListaVertices = new Vertice*[tamanho];
        for(int i = 0; i < tamanho;i++) temp->ListaVertices[i] = NULL;
        temp->maxVertices = tamanho;
    }
    else
    {
        liberaVertices(*gr , tamanho);
    }
}

void LiberaGrafo(Grafo **g){
    if(*g ==  NULL) return; 
    liberaVertices(*g , (*g)->maxVertices);
    delete [](*g)->ListaVertices;
    delete *g;
    *g =  NULL; 
}

bool temVizinho(Vertice *v, Vertice *buscado){
    if(v == NULL || buscado == NULL)
        return false;
    Vizinho *atual = v->comecoVizinho;
    while (atual != NULL)
    {
        if (atual->vizinho == buscado)
            return true;    
        atual = atual->prox;
    }
    return false;
}

void printGrafo(Grafo *g){
    if(g == NULL || g->qtVertices == 0)
    {
        printf("\n\n-> Grafo Vazio ou inesitente.\n\n");
        return;
    }
    Vizinho *atual;
    printf("\n\nAPRESENTAÇÃO DOS DADOS DO GRAFO\n");
    for(int i = 0; i < g->maxVertices; i++)
        if(g->ListaVertices[i] != NULL)
        {
            atual = g->ListaVertices[i]->comecoVizinho;
            printf("\n %d) Vertice: %d -- Seus Vizinhos: " ,i+1, g->ListaVertices[i]->dado);
            while (atual != NULL)
            {
                printf("-> %d distancia: %d " , atual->vizinho->dado, atual->peso);
                atual = atual->prox;
            }
        }
}
 
bool conectarUM(Grafo *g, int v1 , int v2 , int peso){
    if(temVizinho(g->ListaVertices[v1], g->ListaVertices[v2]) )
        return false;
    Vizinho *atual = g->ListaVertices[v1]->comecoVizinho , *novo = new Vizinho;
    Vertice *vizinho = g->ListaVertices[v2];
    novo->peso = peso;
    novo->vizinho = vizinho;
    novo->pai = g->ListaVertices[v1];
    if (atual == NULL)
    {
        g->ListaVertices[v1]->comecoVizinho = novo;
        g->ListaVertices[v1]->finalVizinho= novo;
    }
    else
    {
        g->ListaVertices[v1]->finalVizinho->prox = novo;
        g->ListaVertices[v1]->finalVizinho = novo;
    }
    g->ListaVertices[v1]->grau++;
    g->ListaVertices[v2]->grau++;
    g->ListaVertices[v1]->qtVizinhos++;
    return true;
}

void conectar(Grafo *g, int v1 , int v2 , int peso){
    conectarUM(g, v1, v2, peso);
    if(!g->direcionado) 
        if(conectarUM(g, v2, v1, peso))
        {
            g->ListaVertices[v1]->grau--;
            g->ListaVertices[v2]->grau--;
        }
}

void inserirConectar(Grafo *g , int v1 , int v2 , int peso){
    if(!temVertice(g , v1) && !inserirVertice(g, v1)) return;
    if(!temVertice(g , v2) && !inserirVertice(g, v2)) return;
    conectar(g, v1, v2, peso);
}

bool inserirVertice(Grafo *g , int i){
    if (g == NULL || i >= g->maxVertices || g->qtVertices >= g->maxVertices || temVertice(g, i))
        return false;
    g->ListaVertices[i] =  new Vertice;
    g->ListaVertices[i]->dado = i;
    g->qtVertices++;
    return true;
}

void geraArquivoDOT(Grafo *g){
    if(g == NULL)
    {
        printf("\n\n [ERRO] -  Grafo não incializado");
        return;
    }
    FILE *arq = fopen("MeuGrafo.dot" , "w");
    Vizinho *atual;

    if (arq == NULL)return;
    if(g->direcionado) fprintf( arq,"digraph MeuGrafo {\n");
    else fprintf( arq,"graph MeuGrafo {\n");
    for(int i = 0; i < g->maxVertices; i++)
    {
        if (g->ListaVertices[i] != NULL)
        {
            atual = g->ListaVertices[i]->comecoVizinho;
            while (atual != NULL)
            {
                if(g->direcionado)
                {
                    fprintf(arq, "  %d -> %d [weight=%d, label =\"%d\"];\n", g->ListaVertices[i]->dado, atual->vizinho->dado, atual->peso, atual->peso); 
                }
                else 
                {
                    if(atual->vizinho->dado > i)
                        fprintf(arq, "  %d -- %d [weight=%d, label =\"%d\"];\n", g->ListaVertices[i]->dado, atual->vizinho->dado, atual->peso, atual->peso); 
                }
                atual = atual->prox;
            }      
        }
    }
                
    fprintf(arq , "\n}");
    fclose(arq);
}

void lerArquivoDOT(Grafo **g , char*arquivo){
    if(*g == NULL) return;
    FILE *arq = fopen(arquivo, "r");
    if (arq ==  NULL)
    {
        printf("\n\n\n ERRO AO ABRIR O ARQUIVO !!!");
        return;
    }
    inicializaGrafo(g , (*g)->maxVertices);
    char line[400];
    char primeiraLinha[200];
    int peso , num , v1 , v2;
    while(fgets(line , 400 , arq) != NULL && strcmp(line, "}\n") != 0)
    {
        if(sscanf(line , "graph %[^ ] {" , primeiraLinha ) == 1)
        {
            (*g)->direcionado = false;
        }
        else if (sscanf(line , "digraph %[^ ] {" , primeiraLinha ) == 1)
        {
            (*g)->direcionado = true;
        }     
        else if(sscanf(line , "  %d -> %d [weight=%d, label =\"%d\"];" , &v1 , &v2, &peso, &num) == 4)
        {
            inserirConectar(*g, v1, v2, peso);
        }
        else if(sscanf(line , "  %d -- %d [weight=%d, label =\"%d\"];" , &v1 , &v2, &peso, &num) == 4)
        {
            inserirConectar(*g , v1 , v2 , peso);
        }
    }
    printf("\n\n !!! Aquivo Lido com sucesso\n\n");
    fclose(arq);
}

bool temVertice(Grafo *g, int dado) {
    if (g == NULL || g->qtVertices == 0 || dado >= g->maxVertices) return false;
    return (g->ListaVertices[dado] != NULL)?true:false; 
}

bool deleteVizinho(Vertice *raiz, int del){
    if (raiz == NULL) return false;
    
    Vizinho *ant = NULL , *atual = raiz->comecoVizinho;
    while (atual != NULL)
    {
        if (atual->vizinho->dado == del)
        {
            if (ant != NULL)    ant->prox = atual->prox;
            else raiz->comecoVizinho = NULL;
            delete atual;
            return true;
        }
        ant = atual;
        atual = atual->prox;
    }
    return false;
}

bool menosPesado(Vizinho *a , Vizinho *b){
    return a < b;
}

void algoritmoKruskal(Grafo **gr){
    if(*gr == NULL || (*gr)->qtVertices == 0) return; 

    Grafo *g = *gr, *novoGrafo = NULL;
    Vizinho **vizinhos , *atual;

    int ind = 0;
    unsigned int maxArestas = 0;
    inicializaGrafo(&novoGrafo, g->maxVertices);
    for (int i = 0; i < g->maxVertices; i++)
        if (g->ListaVertices[i] != NULL)
            maxArestas += g->ListaVertices[i]->qtVizinhos;
    
    vizinhos = new Vizinho*[maxArestas];

    for (int i = 0; i < g->maxVertices; i++)
    { 
        if (g->ListaVertices[i] != NULL)
        {
            atual = g->ListaVertices[ind]->comecoVizinho;
            while (atual != NULL)
            {
                vizinhos[ind] = atual;
                ind++;
                atual = atual->prox;
            }
        }
    }
    ind = 0;
    std:: sort(vizinhos , vizinhos + maxArestas, menosPesado);
    while (novoGrafo->qtVertices < g->qtVertices)
    {
        if(!temVertice(novoGrafo , vizinhos[ind]->vizinho->dado))
        {
            inserirConectar(novoGrafo, vizinhos[ind]->pai->dado, vizinhos[ind]->vizinho->dado, vizinhos[ind]->peso);
        }
        ind++;
    }
    delete []vizinhos;
    LiberaGrafo(&g);
    *gr = novoGrafo;
}

