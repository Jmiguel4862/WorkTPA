#include <cstdio>
#include <cstring>
#define max 10

struct Vertice;
struct Vizinho;

struct Vertice {
    int id;
    Vizinho *vizinhos;
};

struct Vizinho {
    Vertice *vizinho;
    Vizinho *proximoVizinho;
};


struct Grafo {
    Vertice *vertices;
    int tamanhoMax;
    int quantidade;
};


//DECLARAÇÕES DE FUNCÕES
void cleanBuffer();
void cleanBuffer(char *c , int tamanho_max);
void printGrafo(Grafo *grafo, int tamanho);
void inicializa(Grafo **grafo, int tamanho);
void adicionaVizinho(Vertice *a , Vertice *b);
bool vizinhoDe(Vertice *a , Vertice *b);

Grafo *grafo = NULL;

int main(int argc, char const *argv[])
{
    int op;
    printf("ok");
    inicializa(&grafo, max);
    
    do{
        printf("======= MENU ======\n");
        printf("1 -> Reinicializar Grafo\n");
        printf("2 -> Adicionar vizinhos\n");
        printf("3 -> Printar Tabela do Grafo\n");
        printf("0 -> Sair\n");
        printf("Opção: ");
        scanf("%d" , &op);
        cleanBuffer();
        switch (op)
        {
        case 1:
            inicializa(&grafo, grafo->tamanhoMax);
            break;
        case 2:
            printf("\n\n=== ADICIONAR VIZINHOS ===\n");
            int v1, v2;
            printf("\n\nDigite o id do vertice 1: ");
            scanf("%d" , &v1);
            printf("\n\nDigite o id do vertice 2: ");
            scanf("%d" , &v2);
            adicionaVizinho(&grafo->vertices[v1], &grafo->vertices[v2]);
            break;
        case 3:
            printGrafo(grafo, grafo->tamanhoMax);
            break;    
        default:
            break;
        }
    for(int i = 0; i < 50;i++)printf("\n");
    }while(op != 0);
    return 0;
}

void cleanBuffer(){
    char c;
    while ((c = getchar()) != '\n' && c == EOF);
}
void cleanBuffer(char *c , int tamanho_max){
    if(c[tamanho_max-1] == '\n') c[tamanho_max-1] = '\0';
    else if (strlen(c) >= tamanho_max) cleanBuffer();
}

void printGrafo(Grafo *grafo, int tamanho){
    for(int i = 0; i < tamanho; i++){
        printf("Vertice %d | vizinhos-> " , grafo->vertices[i].id);
        Vizinho *temp = grafo->vertices[i].vizinhos;
        while (temp != NULL){
            printf("%d ," , temp->vizinho->id);
            temp = temp->proximoVizinho;
        }
        printf("\n");
    }
}

void inicializa(Grafo **grafo, int tamanho){
    if(*grafo != NULL) delete [] *grafo;
    *grafo = new Grafo;
    (*grafo)->vertices = new Vertice[tamanho];
    (*grafo)->tamanhoMax = tamanho;
    (*grafo)->quantidade = 0;
    for(int i = 0; i < tamanho; i++){
        (*grafo)->vertices[i].id=i;
        (*grafo)->vertices[i].vizinhos = NULL;
    }
}

//nao direcionado
//falar que o B é vizinho de A
void adicionaVizinho(Vertice *a , Vertice *b){
    if(a == NULL || b == NULL) return;
    vizinhoDe(a , b);
    vizinhoDe(b , a);
}

bool vizinhoDe(Vertice *a , Vertice *b){
    if(a == NULL || b == NULL) return false;
    Vizinho *temp = a->vizinhos;
    while (temp != NULL){
        if(temp->vizinho == b) return true;
        temp = temp->proximoVizinho;
    }
    temp = new Vizinho;
    temp->vizinho = b;
    temp->proximoVizinho = NULL;
    return true;
}