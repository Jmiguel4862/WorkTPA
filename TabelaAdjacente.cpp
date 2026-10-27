#define NULL 0
struct Vizinho {
    Vertice *vizinho;
    Vizinho *proximoVizinho;
};

struct Vertice {
    int id;
    Vizinho *vizinhos;
};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}


Vertice *grafo;
void inicializa(int tamanho){
    grafo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafo[i].id=i;
        grafo[i].vizinhos = NULL;
    }
}

//nao direcionado
//falar que o B é vizinho de A
void adicionaVizinho(Vertice *a, Vertice *b){
    if (a == NULL || b == NULL)
        return;
    Vizinho *aux;
    if(a->vizinhos == NULL){
        Vizinho *temp = new Vizinho;
        temp->vizinho = b;
        temp->proximoVizinho = NULL;
        a->vizinhos = temp;
    }else{
        aux = a->vizinhos;
        while (aux->proximoVizinho != NULL){
            aux = aux->proximoVizinho;
        }
        Vizinho *temp = new Vizinho;
        temp->vizinho = b;
        temp->proximoVizinho = NULL;
        aux->proximoVizinho = temp;
    }
    //agora eu preciso fazer o mesmo processo para
    //fazer o A ser vizinho de B
}