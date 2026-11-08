#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdlib>
#define TamInit 20

struct Vertice;
struct Vizinho;


struct Vertice{
    char dado;
    Vizinho *vizinho = NULL;
};
struct Vizinho
{
    int peso;
    Vertice *vizinho = NULL;
    Vizinho *prox = NULL;
};

struct DadosGrafo
{
    int qtVertices;
    bool direcionado;
    int preenchimentoArestas;
};

struct Grafo{
    Vertice **ListaVertices = NULL;
    bool direcionado;
    int qtVertices;
    int maxVertices;
};

//DECLARAÇÕES DE FUNÇÕES
void menu();
void liberaVertices(Grafo *g , int tamanho);
void inicializaGrafo(Grafo **gr , int tamanho);
void LiberaGrafo(Grafo **g);
int intRand(int num);
void printtable(Grafo *g);
void preencherGrafo(Grafo *g , DadosGrafo *dg);
DadosGrafo* getDadosGrafo();
void conectandoVertices(Grafo *g);
void conectar(Grafo *g, int v1 , int v2 , int peso);
void conectar(Grafo *g, char v1 , char v2 , int peso);
bool inserirVertice(Grafo *g , char c);
int buscarVertice(Grafo *g , char c);
void lerArquivoDOT(Grafo *g);
void geraArquivoDOT(Grafo *g);

//FUNÇÕES AUXILIARES
void cleanBuffer(){
    char c;
    while ((c = getchar()) != '\n' && c !=  EOF);
}

bool letInt(int &n){
    if (scanf("%d" , &n) != 1)
    {
        cleanBuffer();
        return false;
    }
    cleanBuffer();
    return true;
}

//DECLARAÇÃO DO MAIN
int main(int argc, char const *argv[])
{
    srand(time(NULL));
    menu();
    return 0;
}

//FUNÇÕES DO CÓDIGO
void menu(){
    int op;
    Grafo *grafo = NULL;
    inicializaGrafo(&grafo , TamInit);
    do
    {
        for ( int i = 0; i < 30; i++)printf("\n");

        printf("\n=== MENU ===\n");

        printf("\n1-> Renicializar grafo");
        printf("\n2-> Gerar grafo");
        printf("\n3-> Conectar vertices");
        printf("\n4-> Gera Arquivo .DOT");
        printf("\n5-> Ler o Arquivo .DOT");
        printf("\n6-> Mostra tabela do grafo");
        printf("\n0-> sair");
        printf("\nEscolhe: ");
        if (!letInt(op)) op = -1;
        switch (op)
        {
        case 1:
            inicializaGrafo(&grafo, TamInit);
            break;
        case 2:
            preencherGrafo(grafo , getDadosGrafo());
            break;
        case 3:
            conectandoVertices(grafo);
            break;
        case 4:
            geraArquivoDOT(grafo);
            break;
        case 5:
            lerArquivoDOT(grafo);
            break;
        case 6:
            printtable(grafo);
            break;
        case 0:
            break;
        
        default:
            break;
        }
        printf("\n\n\n Precione qualquer teclar para seguir");
        system("pause");
    } while (op != 0);
    LiberaGrafo(&grafo);
}

void liberaVertices(Grafo *g , int tamanho){
    Vizinho *del, *atual;
    for(int i  = 0; i < g->qtVertices;i++)
    {
        if(g->ListaVertices[i] != NULL)
        {
            atual = g->ListaVertices[i]->vizinho;
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
    g->qtVertices = 0;
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
        temp->qtVertices = 0;
    }
    else
    {
        liberaVertices(*gr , tamanho);
    }
}

void LiberaGrafo(Grafo **g){
    if(*g ==  NULL) return; 
    Vizinho *del, *atual;
    liberaVertices(*g , (*g)->maxVertices);
    delete [](*g)->ListaVertices;
    delete *g;
    *g =  NULL; 
}
 
int intRand(int num){
    return (rand() % num);
}

void printtable(Grafo *g){
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
            atual = g->ListaVertices[i]->vizinho;
            printf("\n %d) Vertice: %c -- Seus Vizinhos: " ,i+1, g->ListaVertices[i]->dado);
            while (atual != NULL)
            {
                printf("-> %c distancia: %d " , atual->vizinho->dado, atual->peso);
                atual = atual->prox;
            }
        }
}

Vizinho *hasVizinho(Vertice *v, Vertice *buscado){
    if(v == NULL || buscado == NULL)
        return NULL;
    Vizinho *atual = v->vizinho;
    while (atual != NULL)
    {
        if (atual->vizinho == buscado)
            return atual;    
        atual = atual->prox;
    }
    return NULL;
}

void preencherGrafo(Grafo *g , DadosGrafo *dg){
    if(g == NULL || dg == NULL) return;
    liberaVertices(g , g->maxVertices);
    char nomeVert = 'A';
    int FaltaNV = (dg->qtVertices*(dg->qtVertices-1) * dg->preenchimentoArestas)/(((dg->direcionado)?1:2)*100);
    int line, alvo, peso;
    Vizinho *v;
    g->direcionado = dg->direcionado;
    for(int i = 0; i < dg->qtVertices;i++)
    {
        inserirVertice(g, nomeVert); 
        nomeVert = (char)(nomeVert+1);
    }
    while(FaltaNV > 0){
        line = intRand(g->qtVertices);
        alvo = intRand(g->qtVertices);
        peso = intRand(10) + 1;
        if (v = hasVizinho(g->ListaVertices[line] , g->ListaVertices[alvo]))continue;
        conectar(g,line , alvo, peso);
        if(!g->direcionado)conectar(g,alvo , peso, peso);
        FaltaNV--;
    }
    delete dg;
}

DadosGrafo* getDadosGrafo(){
    DadosGrafo *dg = new DadosGrafo;
    char c = ' ';
    do
    {
        printf("\n-> Quantas vertices você quer no grafo(1-20): ");
    } while (!letInt(dg->qtVertices)|| dg->qtVertices < 1 || dg->qtVertices > 20);
    do
    {
        printf("\n-> Porcentagen das arestas preenchidas(1-100): ");

    } while ( !letInt(dg->preenchimentoArestas) || dg->preenchimentoArestas > 100 || dg->preenchimentoArestas < 1);
    do
    {
        printf("\n-> Direcionado(S/N): ");
        scanf("%c" , &c);
        cleanBuffer();
        if(c >='a')  c = c - 32;

    } while(c != 'S' && c != 'N');
    if(c == 'S') dg->direcionado = true;
    else  dg->direcionado = false;
    return dg;
}

void conectandoVertices(Grafo *g){
    if (g == NULL || g->qtVertices == 0)
        return;
    int v1 , v2 , peso;
    printtable(g);
    do{
        printf("\n\n -> Digite o inde do primeiro vertice (1-%d): " , g->qtVertices);
    } while (!letInt(v1) || v1 > g->qtVertices || v1 < 1);
    do{
        printf("\n\n -> Digite o inde do segundo vertice(1-%d): " , g->qtVertices);
    } while (!letInt(v2) || v2 > g->qtVertices || v2 < 1);
    do{
        printf("\n\n -> Distancia entre eles(1-10): ");
    } while (!letInt(peso) || peso > 10 || peso < 1);
    v1--;
    v2--;
    conectar(g, v1, v2 , peso);
}

void conectar(Grafo *g, int v1 , int v2 , int peso){
    if(g->ListaVertices[v1] == NULL || g->ListaVertices[v1] == NULL)
        return;

    Vizinho *atual = g->ListaVertices[v1]->vizinho;
    Vertice *vizinho = g->ListaVertices[v2];

    while(atual != NULL) atual = atual->prox;

    atual = new Vizinho;
    atual->vizinho = vizinho;
    atual->peso = peso;
}

void conectar(Grafo *g, char v1 , char v2 , int peso){
    int nv1 = buscarVertice(g , v1), nv2 = buscarVertice(g,v2);
    if(nv1 != -1 && nv2 != -1) conectar(g, nv1, nv2 , peso);
}

bool inserirVertice(Grafo *g , char c){
    if (g == NULL || g->qtVertices >= g->maxVertices)
        return false;
    if(buscarVertice(g , c) != -1)
        return false;
    g->ListaVertices[g->qtVertices] =  new Vertice;
    g->ListaVertices[g->qtVertices]->dado = c;
    g->qtVertices++;
    return true;
}

int buscarVertice(Grafo *g , char c){
    if (g == NULL || g->qtVertices == 0) return -1;
    for (int i = 0; i < g->qtVertices; i++)
        if ( g->ListaVertices[i] != NULL && g->ListaVertices[i]->dado == c)
            return i;
    return -1;
}

void lerArquivoDOT(Grafo *g){
    FILE *arq = fopen("MeuGrafo.dot", "r");
    if (arq ==  NULL)
    {
        printf("\n\n\n ERRO AO ABRIR O ARQUIVO !!!");
        return;
    }
    liberaVertices(g , g->maxVertices);
    char line[400];
    char primeiraLinha[200];
    char v1 , v2;
    int peso , num;

    while(fgets(line , 400 , arq) != NULL && strcmp(line, "}") != 0)
    {
        if(sscanf(line , "graph %[^ ] {" , primeiraLinha ) == 1)
        {
            g->direcionado = false;
        }
        else if (sscanf(line , "digraph %[^ ] {" , primeiraLinha ) == 1)
        {
            g->direcionado = true;
        }
        else if(sscanf(line , "\t%c -> %c [label = \"%d\" , weight = %d];" , &v1 , &v2, &peso, &num) == 4)
        {
            if (buscarVertice(g , v1) == -1)  inserirVertice(g, v1);
            if (buscarVertice(g , v2) == -1)  inserirVertice(g, v2);
            conectar(g, v1, v2, peso);
        }
        else if(sscanf(line , "\t%c -- %c [label = \"%d\" , weight = %d];" , &v1 , &v2, &peso, &num) == 4)
        {
            if (buscarVertice(g , v1) == -1)  inserirVertice(g, v1);
            if (buscarVertice(g , v2) == -1)  inserirVertice(g, v2);
            conectar(g, v1, v2, peso);
        }
        else if(sscanf(line , "\t%c;" , &v1) == 1 && buscarVertice(g , v1) == -1)
            inserirVertice(g, v1);
    }
    printf("\n\n !!! Aquivo Lido com sucesso");
    fclose(arq);
}

void geraArquivoDOT(Grafo *g){
    if(g == NULL)
    {
        printf("\n\n [ERRO] -  Grafo não incializado");
        return;
    }
    FILE *arq = fopen("MeuGrafo.dot" , "w");
    Vizinho *atual;
    int j;

    if (arq == NULL)return;
    if(g->direcionado) fprintf( arq,"digraph MeuGrafo {\n");
    else fprintf( arq,"graph MeuGrafo {\n");
    for(int i = 0; i < g->maxVertices;i++)
        if(g->ListaVertices[i] != NULL)
            fprintf(arq , "\t%c;\n" , g->ListaVertices[i]->dado);
    for(int i = 0; i < g->qtVertices; i++)
    {
        atual = g->ListaVertices[i]->vizinho;
        while (atual != NULL)
        {
            if(g->direcionado)
            {
                fprintf(arq, "\t%c -> %c [label = \"%d\" , weight = %d];\n", g->ListaVertices[i]->dado, atual->vizinho->dado, atual->peso, atual->peso); 
            }
            else 
            {
                j = buscarVertice(g , atual->vizinho->dado);
                if(j > i)
                    fprintf(arq, "\t%c -- %c [label = \"%d\" , weight = %d];\n", g->ListaVertices[i]->dado, atual->vizinho->dado, atual->peso, atual->peso); 
            } 
        }      
    }
                
    fprintf(arq , "\n}");
    fclose(arq);
}