#include <cstdio>
#include <cstring>
#define tam_init  10
struct Vertice{
    char*text;
    Vertice *next = NULL;
    Vertice *prev = NULL;
};

struct DataTable{
    int **inds = NULL;
    Vertice *start = NULL;
    Vertice *end = NULL;
    int quantidade;
    int tamanhoMax;
};

//DECLARAÇÕES DE FUNCÕES
void cleanBuffer();
void cleanBuffer(char *c , int max);
void printGrafo(DataTable *grafo);
void inicializa(DataTable *grafo, int tamanho);
void insertVertise(DataTable *grafo , char *newv);
void adicionaVizinho(DataTable *grafo);
void removeVertice(DataTable *grafo);

DataTable *grafo = new DataTable;

int main(int argc, char const *argv[])
{
    int op;
    char *text;
    inicializa(grafo, tam_init);
    do{
        printf("======= MENU ======\n");
        printf("1 -> Reinicializar Grafo\n");
        printf("2 -> Inserir Vertice\n");
        printf("3 -> Remover vertices\n");
        printf("4 -> Adicionar vizinhos\n");
        printf("5 -> Printar Tabela do Grafo\n");
        printf("0 -> Sair\n");
        printf("Opção: ");
        scanf("%d" , &op);
        cleanBuffer();
        switch (op)
        {
        case 1:
            inicializa(grafo , grafo->tamanhoMax);
            break;
        case 2:
            text = new char[50];
            printf("\n\nDigite o nome do vertice: ");
            fgets(text, 50, stdin);
            cleanBuffer(text, 50);
            insertVertise(grafo, text);
            break;
        case 3:
            removeVertice(grafo);
            break;
        case 4:
            adicionaVizinho(grafo);
            break;
        case 5:
            printGrafo(grafo);
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
void cleanBuffer(char *c , int max){
    if(c[max-1] == '\n') c[max-1] = '\0';
    else if (strlen(c) >= max) cleanBuffer();
}

void printGrafo(DataTable *grafo){
    if(grafo ->quantidade == 0) return;
    int qtd = grafo->quantidade , ind = 1;
    Vertice *temp =  grafo->start;
    printf("    |");
    for(int i = 0; i < qtd; i++) printf(" %d |" , i+1);
    for (int i = 0; i < qtd; i++)
    {
        printf("\n| %d |", i+1);
        for (int j = 0; j < qtd; j++) printf(" %d |" , grafo->inds[i][j]);
    }
    printf("\n\n === LENGENDA DOS VERTICE ===\n\n");
    while (temp != NULL)
    {
        printf("%d -> %s" , ind , temp->text);
        ind++;
        temp = temp->next;
    }
    
}

void inicializa(DataTable *grafo, int tamanho){
    if(grafo->inds != NULL)
    {
        Vertice *temp = grafo->start;
        while (temp != NULL)
        { 
            delete temp;
            temp = temp->next;
        }
        for(int i = 0; i < tamanho; i++)
        if(grafo->inds[i] != NULL) delete []grafo->inds[i]; 
        delete [] grafo->inds;   
        grafo->inds = NULL;
    }
    grafo->inds = new int*[tamanho];
    grafo->start = NULL;
    grafo->end = NULL;
    grafo->tamanhoMax = tamanho;
    grafo->quantidade = 0;
    for(int i = 0; i < tamanho; i++){
        grafo->inds[i] = new int[tamanho];
        for(int j = 0; j < tamanho;j++)
            grafo->inds[i][j] = 0;
    }
}

void insertVertise(DataTable *grafo , char *newv){
    if(grafo->quantidade >= grafo->tamanhoMax)
        return;
    Vertice *newvert = new Vertice;
    newvert->text = newv;
    if(grafo->start == NULL)
    {
        grafo->start = newvert;
        grafo->end = newvert;
    }
    else
    {
            grafo->end->next = newvert;
            newvert->prev = grafo->end;
            grafo->end = newvert;
    }
    grafo->quantidade++;
}

void removeVertice(DataTable *grafo){
    int i = 0 , ind;
    Vertice* temp =grafo->start;
    printGrafo(grafo);
    printf("\n\n Digite o numero do vertice: ");
    scanf("%d" , &ind);
    ind--;
    if(grafo->quantidade == 0 || ind > grafo->quantidade || ind < 1 )
    {
        printf("\n\n[ERRO] - Numero invalido!!");
    }
    else while (temp != NULL)
        {
            if(i == ind)
            {
                if(temp == grafo->start && temp == grafo->end)
                {
                    grafo->start = NULL;
                    grafo->end = NULL;
                }else if(temp == grafo->start)
                {
                    grafo->start = temp->next;
                    grafo->start->prev = NULL; 
                }
                else if(temp == grafo->end)
                {
                    grafo->end = temp->prev;
                    grafo->end->next = NULL; 
                }
                else
                {
                    temp->next->prev = temp->prev;
                    temp->prev->next = temp->next;
                }
                delete temp;
                grafo->quantidade--;
                return;
            }
            temp = temp->next;
            i++;
        }
}

//nao direcionado
//falar que o B é vizinho de A
void adicionaVizinho(DataTable *grafo){
    int line, cols, dist;
    printGrafo(grafo);
    printf("\n\n ESCOLHA UMA QUE VOCÊ QUER QUE SEJA VIZINHOS DA CONFORMA LER A TABELA PRIMEIRO A LINHA DEPOIS A COLUNA, SE ORIENTANDO PELA LEGENDA DOS VERTICES ABAIXO DA TABELA.");
    printf("\n Escolher vizinho (linha): ");
    scanf("%d" , &line);
    cleanBuffer();
    printf("\n Escolher vizinho (coluna): ");
    scanf("%d" , &cols);
    cleanBuffer();
    printf("\n Distancia entre eles: ");
    scanf("%d" , &dist);
    cleanBuffer();
    grafo->inds[--line][--cols] = dist;
}