#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdlib>
#define TamInit 20

struct DadosGrafo
{
    int qtVertices;
    bool direcionado;
    int preenchimentoArestas;
};

struct Grafo{
    int **TabelaArestas;
    char *Vertices;
    int maxVertices;
    bool direcionado;
    int qtVertices;
};

void menu();
void inicializaGrafo(Grafo **gr , int tamanho);
bool limparGrafo(Grafo *g , int tamanho);
void LiberaGrafo(Grafo **g);
int intRand(int num);
void printtable(Grafo *g);
void preencherGrafo(Grafo *g , DadosGrafo *dg);
DadosGrafo* getDadosGrafo();
bool conectandoVertices(Grafo *g);
void conectar(Grafo *g, int v1 , int v2 , int peso);
bool inserirVertice(Grafo *g , char c);
int buscarVertice(Grafo *g , char c);
void lerArquivoDOT(Grafo *g);
void geraArquivoDOT(Grafo *g);
void conectar(Grafo *g, char v1 , char v2 , int peso);

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

void inicializaGrafo(Grafo **gr , int tamanho){
    Grafo *temp = *gr;
    if (*gr == NULL)
    {
        *gr = new Grafo;
        temp = *gr;
        temp->TabelaArestas = new int*[tamanho];
        temp->Vertices = new char[tamanho];
        for(int i = 0; i < tamanho; i++)
            temp->TabelaArestas[i] = new int[tamanho];
        
    }  
    limparGrafo(temp , tamanho);
}

bool limparGrafo(Grafo *g , int tamanho){
    if(g == NULL) return false;
    g->maxVertices = tamanho;
    g->qtVertices = 0;
    for (int i = 0; i < tamanho; i++)
        for(int j = 0; j < tamanho;j++)
            g->TabelaArestas[i][j] = 0;  
    return true;
}

void LiberaGrafo(Grafo **g){
    if(*g ==  NULL) return; 
    for (int i = 0; i < (*g)->maxVertices; i++)
        delete [](*g)->TabelaArestas[i];
    delete [](*g)->TabelaArestas;
    delete [](*g)->Vertices;
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
    printf("|   |");
    for(int i = 0; i < g->qtVertices; i++) printf(" %d |" , i+1);

    for (int i = 0; i < g->qtVertices; i++)
    {
        printf("\n| %d |" , i+1);
        for (int j = 0; j < g->qtVertices; j++) printf(" %d |", g->TabelaArestas[i][j]);
    }

    printf("\n\n\n");
    for (int i = 0; i < g->qtVertices; i++)
        printf("\n%d -> %c" , i+1, g->Vertices[i]);
}

void preencherGrafo(Grafo *g , DadosGrafo *dg){
    if(g == NULL || dg == NULL) return;
    limparGrafo(g, g->maxVertices);
    char nomeVert = 'A';
    int faltaNV = (dg->qtVertices*(dg->qtVertices-1) * dg->preenchimentoArestas)/(((dg->direcionado)?1:2)*100);
    int ind , rest, peso;
    g->direcionado = dg->direcionado;
    g->qtVertices = dg->qtVertices;
    for(int i = 0; i < dg->qtVertices;i++){
        g->Vertices[i] = nomeVert;
        nomeVert = (char)(g->Vertices[i] + 1);
    }
    while(faltaNV > 0){
        ind = intRand(g->qtVertices);
        rest = intRand(g->qtVertices);
        if (ind == rest || g->TabelaArestas[ind][rest] != 0) continue;
        peso = intRand(10) + 1;
        conectar(g,ind , rest, peso);
        faltaNV--;
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

bool conectandoVertices(Grafo *g){
    if (g == NULL || g->qtVertices == 0)
        return false;
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
    return true;
}

void conectar(Grafo *g, int v1 , int v2 , int peso){
    g->TabelaArestas[v1][v2] = peso;
    if(!g->direcionado) g->TabelaArestas[v2][v1] = peso;
}

void conectar(Grafo *g, char v1 , char v2 , int peso){
    int nv1 = buscarVertice(g , v1), nv2 = buscarVertice(g,v2);
    if(nv1 != -1 && nv2 != -1) conectar(g, nv1, nv2 , peso);
}

bool inserirVertice(Grafo *g , char c){
    if (g == NULL)
    return false;
    for (int i = 0; i < g->qtVertices; i++)
        if (g->Vertices[i] == c)
            return false;
    g->Vertices[g->qtVertices] = c;
    g->qtVertices++;
    return true;
}

int buscarVertice(Grafo *g , char c){
    if (g == NULL || g->qtVertices == 0) return -1;
    for (int i = 0; i < g->qtVertices; i++)
        if (g->Vertices[i] == c)
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
    if(!limparGrafo(g , g->maxVertices)) return;
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
        printf("\n\n [ERRO] -  Grafo não incializado]");
        return;
    }
    FILE *arq = fopen("MeuGrafo.dot" , "w");
    if (arq == NULL)return;
    if(g->direcionado) fprintf( arq,"digraph MeuGrafo {\n");
    else fprintf( arq,"graph MeuGrafo {\n");
    for(int i = 0; i < g->qtVertices;i++)
        fprintf(arq , "\t%c;\n" , g->Vertices[i]);
    for(int i = 0; i < g->qtVertices; i++)
            for(int j = 0; j < g->qtVertices;j++)
                if (g->TabelaArestas[i][j] > 0)
                    if(g->direcionado)
                    {
                        fprintf(arq, "\t%c -> %c [label = \"%d\" , weight = %d];\n", g->Vertices[i], g->Vertices[j] , g->TabelaArestas[i][j] ,g->TabelaArestas[i][j]); 
                    }
                    else 
                    {
                        if(j > i)
                            fprintf(arq, "\t%c -- %c [label = \"%d\" , weight = %d];\n", g->Vertices[i], g->Vertices[j] , g->TabelaArestas[i][j] ,g->TabelaArestas[i][j]); 
                    }
    fprintf(arq , "\n}");
    fclose(arq);
}