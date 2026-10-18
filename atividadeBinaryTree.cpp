
//Exemplo de código para montar a primeira arvore binária
//Neste exemplo também teremos leitura de arquivos CSV e manipulação de strings
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>
using namespace std;

//Exemplo do arquivo CSV que será lido
//Matricula,CPF,Nome,Nota,Idade,Curso,Cidade
//A0000000,915.216.859-08,Wallace Sampaio,20.35,23,Direito,Rio de Janeiro

struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[40];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
};

struct No{
    Aluno *aluno= NULL;
    No *pai = NULL;
    No *dir = NULL;
    No *esq = NULL;
    int altura = 0;
    int grau = 0;
    int nivel = 0;
};

struct Arvore{
    No *raiz = NULL;
    int quantidadeElementosDeAlunos;
    int nivelMaximo;

};

// FUNÇÕES DO PROGRAMA
void lerArquivoCSV(const char* nomeArquivo);
void inicializa(Arvore *a);
void cleanTree(No **raiz);
void adicionarAluno(No *novo , No **raiz);




// BASE PRIMARIA
Arvore a;
// FUNÇÃO DE EXECUÇÃO
int main(){
    inicializa(&a);
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");
    
    time_t inicio, fim;
    inicio = clock();
    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos
    lerArquivoCSV("alunos.csv");
    fim = clock();
    //exibirAlunos();
    printf("\n\n");
    //buscarAlunoPorNome();
    //se eu quiser pegar como inteiro o valor do tempo

    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);
    printf("\n\n Maior altura: %d", a.nivelMaximo) ;
    printf("\n");
    //se eu quiser pegar como double o valor do tempo
    // double tempo2 = difftime(fim, inicio);
    // printf("Tempo de leitura: %.2f segundos\n", tempo2);
    // Exibir todos os alunos carregados
    //exibirAlunos();
    
    system("pause");
    return 0;
}

void lerArquivoCSV(const char* nomeArquivo) { 
    FILE* arquivo = fopen(nomeArquivo, "r");
    int count = 0;
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    char linha[300];
    
    printf("Iniciando leitura do arquivo CSV...\n");
    
    // Pular a primeira linha (cabeçalho)
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        printf("Arquivo vazio ou erro na leitura\n");
        fclose(arquivo);
        return;
    }
    // Ler cada linha usando fscanf diretamente na struct
    Aluno* novo;
    printf("\n Carregando...");
    while ((novo = new Aluno) != NULL) {
        //%N significa que fará a leitura de até N caracteres, evitando overflow
        //O [^caractere] é uma classe de caracteres negativa - significa "qualquer caractere EXCETO o especificado".
        //É muito útil para parar a leitura quando encontrar um delimitador específico (como vírgula ou quebra de linha).
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
            No *novoNo = new No;
            novoNo->aluno = novo;
            adicionarAluno(novoNo , &a.raiz);

        } else {
            // Se não conseguiu ler todos os campos, liberar memória e sair
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
}

void inicializa(Arvore *a){
    cleanTree(&a->raiz);
    a->nivelMaximo = 0;
    a->quantidadeElementosDeAlunos = 0;

}

void cleanTree(No **raiz){
    if (*raiz == NULL)
        return;
    cleanTree(&(*raiz)->dir);
    cleanTree(&(*raiz)->esq);
    delete *raiz;
}

void adicionarAluno(No *novo , No **raiz){
    if(*raiz == NULL){
        if (novo->pai != NULL )
        {
            if (novo->pai->grau < 1) 
                novo->pai->altura++;
            novo->pai->grau++;
        }
        *raiz = novo;
        if(novo->nivel > a.nivelMaximo) a.nivelMaximo  = novo->nivel;
        return;
    }
    novo->nivel++;
    novo->pai = *raiz;
    if(strcmp(novo->aluno->nome, (*raiz)->aluno->nome) == 0)
    {
        delete novo->aluno;
        delete novo;
        printf("\n [ERRO] - Aluno ja cadastrado");
        return;
    }
    else if(strcmp(novo->aluno->nome, (*raiz)->aluno->nome) < 0)
    {
        adicionarAluno(novo , &(*raiz)->esq);
        if((*raiz)->altura == (*raiz)->esq->altura )
            (*raiz)->altura++;
    }
    else
    {
        adicionarAluno(novo , &(*raiz)->dir);
        if((*raiz)->altura == (*raiz)->dir->altura )
            (*raiz)->altura++;
    }
    
}

int alturaDireita(No *raiz){
    return(raiz == NULL || raiz->dir == NULL)? 0:raiz->dir->altura;
}
int alturaEsquerda(No *raiz){
    return(raiz == NULL || raiz->esq == NULL)? 0:raiz->esq->altura;
}

void rotationLelf(No **raiz){
    No *h1 = *raiz, *h2 = (*raiz)->dir;
    h1->dir = h2->esq;
    h2->pai = h1->pai;
    h1->pai = h2;
    h2->esq = h1;
    if (h1->dir != NULL)
    {
        h1->dir->pai = h1;
    }
    h1->altura--;
    *raiz = h2;
}

void rotationRight(No **raiz){
    No *h1 = *raiz, *h2 = (*raiz)->esq;
    h1->esq = h2->dir;
    h2->pai = h1->pai;
    h1->pai = h2;
    h2->dir = h1;
    if (h1->esq != NULL)
    {
        h1->esq->pai = h1;
    }
    h1->altura--;
    *raiz = h2;
}

void incNivel(No *raiz){
    if(raiz == NULL) return;
    incNivel(raiz->esq);
    incNivel(raiz->dir);
    raiz->nivel++;
}

void decNivel(No *raiz){
    if(raiz == NULL) return;
    incNivel(raiz->esq);
    incNivel(raiz->dir);
    raiz->nivel--;
}

void balanceTree(No **raiz){
    if (*raiz == NULL)
        return;
    int dif = alturaEsquerda(*raiz) - alturaDireita(*raiz);
    if(dif > 1)
    {
        if(alturaEsquerda((*raiz)->esq) - alturaDireita((*raiz)->esq) < 0)
        {
            rotationLelf(&(*raiz)->esq);
        }
        else
        {
            rotationRight(raiz);
        }
    }
    else if(dif < -1)
    {
         if(alturaEsquerda((*raiz)->dir) - alturaDireita((*raiz)->dir) > 0)
        {
            rotationLelf(&(*raiz)->dir);
        }
        else
        {
            rotationRight(raiz);
        }
    }
    else
    {
        balanceTree(&(*raiz)->esq);
        balanceTree(&(*raiz)->dir);
    }
}