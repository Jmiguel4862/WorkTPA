#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdlib>
#define TamInit 3

struct DadosGrafo
{
    int qtVertices;
    bool direcionado;
    int preenchimentoArestas;
};

struct Grafo{
    int **TabelaArestas;
    char **Vertices;
    int maxVertices;
    bool direcionado;
    int qtVertices;
};

DadosGrafo* getDadosGrafo();
int menu();
void inicializaGrafo(Grafo **gr , int tamanho);


int main(int argc, char const *argv[])
{
    Grafo *g = NULL;
    inicializaGrafo(&g , TamInit);
    for (int i = 0; i < TamInit; i++)
    {
        for (int j = 0; j < TamInit; j++)
            printf("%d , " , g->TabelaArestas[i][j]);
        printf("\n");
    }

    return 0;
}


int menu(){
    int op;
    DadosGrafo *grafo = NULL;
    do
    {
        for ( int i = 0; i < 30; i++)printf("\n");

        printf("\n=== MENU ===\n");

        printf("1-> Renicializar grafo");
        printf("2-> Gerar grafo");
        printf("3-> Gera Arquivo .DOT");
        printf("4-> Ler o Arquivo .DOT");
        printf("5-> Mostra tabela do grafo");
        printf("0-> sair");
        scanf("%d" , op);

        switch (op)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 0:
            break;
        
        default:
            break;
        }
    } while (op > 3 && op < 0);
    
}


void inicializaGrafo(Grafo **gr , int tamanho){
    Grafo *temp;
    if (*gr == NULL)
    {
        *gr = new Grafo;
        temp = *gr;
        temp->maxVertices = tamanho;
        temp->Vertices = new char*[tamanho];
        temp->TabelaArestas = new int*[tamanho];
        for(int i = 0; i < tamanho; i++)
            temp->TabelaArestas[i] = new int[tamanho];
        
    }
    else
    {
        if (temp->Vertices != NULL)
            for(int i = 0; i < temp->qtVertices; i++)
                delete temp->Vertices[i];
    }
    temp->qtVertices = 0;
}
 
int intRand(int num){
    srand(time(NULL));
    return (rand() % num) + 1;
}

void preencherGrafo(Grafo *g , DadosGrafo *dg){
    char *nomeVert = new char;
    int FaltaNV = (dg->qtVertices*(dg->qtVertices-1) / 100) * dg->preenchimentoArestas;
    *nomeVert = 'A';
    g->direcionado = dg->direcionado;
    g->qtVertices = dg->qtVertices;
    for(int i = 0; i < dg->qtVertices;i++){
        g->Vertices[i] = nomeVert;
        nomeVert = new char;
        *nomeVert = (char)(g->Vertices[i] + 1);
        for(int j = 0; j < g->qtVertices;j++)
                g->TabelaArestas[i][j] = 0;
    }
    while(FaltaNV < 0){

    }
}

DadosGrafo* getDadosGrafo(){
    DadosGrafo *dg = new DadosGrafo;
    char c;
    do
    {
        printf("\n-> Quantas verteces você quer no grafo(1-20): ");
        scanf("%d" , &dg->qtVertices);
    } while (dg->qtVertices < 1 || dg->qtVertices > 20);
    do
    {
        printf("\n-> Porcentagen das arestas preenchidas(1-100): ");
        scanf("%f", &dg->preenchimentoArestas);
    } while (dg->preenchimentoArestas > 100 || dg->preenchimentoArestas < 1);
    do
    {
        printf("\n-> Direcionado(S/N): ");
        scanf("%c" , &c);
        if(c > 65)
            c = c - 32;
    } while(c != 'S' && c != 'N');
    if(c == 'S') dg->direcionado = true;
    else  dg->direcionado = false;
    return dg;
}
