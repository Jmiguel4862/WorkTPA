
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

struct NoAluno{
    Aluno *aluno;
    NoAluno *pai;
    NoAluno *dir;
    NoAluno *esq;
    int altura;
    int grau;
    int nivel;
};

struct Arvore{
    NoAluno *raiz;
    int quantidadeElementosDeAlunos;
    int nivelMaximo;

};

// CHAMADAS DE FUNÇÕES DO PROGRAMA
void clsbuffer();
void cleanBuffer(char *c);
void inicializa();
void clean_tree(NoAluno *raiz);
void adicionarAluno(Aluno* novo);
void insertIntoTree(NoAluno *novo , NoAluno *raiz);
void buscarAlunoPorNome();
NoAluno *searchAlunoInTree(char *name , NoAluno *raiz);
void lerArquivoCSV(const char* nomeArquivo);
void exibirAlunos();
void listItensOfTree(NoAluno *current);
// BASE PRIMARIA
Arvore a;
// FUNÇÃO DE EXECUÇÃO
int main(){
    inicializa();
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");
    
    time_t inicio, fim;
    inicio = clock();
    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos
    lerArquivoCSV("alunos.csv");
    fim = clock();
    exibirAlunos();
    printf("\n\n");
    buscarAlunoPorNome();
    //se eu quiser pegar como inteiro o valor do tempo

    printf("Tempo de leitura: %d milissegundos\n", (int)fim - inicio);
    //se eu quiser pegar como double o valor do tempo
    // double tempo2 = difftime(fim, inicio);
    // printf("Tempo de leitura: %.2f segundos\n", tempo2);
    // Exibir todos os alunos carregados
    //exibirAlunos();
    
    system("pause");
    return 0;
}

void clsbuffer(){
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cleanBuffer(char *c){
    if(strlen(c) == 400)
        clsbuffer();
}

void inicializa(){
    clean_tree(a.raiz);
    a.raiz = NULL;
    a.nivelMaximo = 0;
    a.quantidadeElementosDeAlunos = 0;
}

void clean_tree(NoAluno *raiz){
    if (raiz == NULL)
        return;
    if (raiz->dir != NULL)
        clean_tree(raiz->dir);
    if (raiz->esq != NULL)
        clean_tree(raiz->esq);
    delete raiz;
}
// Trocar pra função generica recebendo a lista como
// parametro
void adicionarAluno(Aluno* novo) {
    NoAluno *novoNodo = new NoAluno;
    novoNodo->aluno = novo;
    novoNodo->dir = NULL;
    novoNodo->esq = NULL;
    novoNodo->pai = NULL;
    novoNodo->grau = 0;
    novoNodo->nivel = 0;
    novoNodo->altura = 0;
    insertIntoTree(novoNodo, a.raiz);
}

void insertIntoTree(NoAluno *novo , NoAluno *raiz){
    if (raiz == NULL)
    {
        if(novo->pai == NULL && novo->pai->grau == 0)
            novo->pai->altura++;
        printf("\n ----------\n");
        novo->pai->grau++;
        raiz == novo;
        a.quantidadeElementosDeAlunos++;
    }
    
    else 
    {
        novo->nivel++;
        novo->pai = raiz;
        if (strcmp(novo->aluno->nome , raiz->aluno->nome) < 0)
        {
            printf("\n esquerda");
            insertIntoTree(novo , raiz->esq);
            if(raiz->altura == raiz->esq->altura)
                raiz->altura++;
        }
        else
        {
            printf("\n direita");
            insertIntoTree(novo , raiz->dir);
            if(raiz->altura == raiz->dir->altura) 
                raiz->altura++;
        }

    }
}

void buscarAlunoPorNome(){
    char name [400];
    NoAluno *soerch;
    cout<<"\n\nDigite o nome que deseja pesquisar: ";
    fgets(name, 400, stdin);
    soerch = searchAlunoInTree(name, a.raiz);
    if(soerch != NULL)
    {
        printf("\n\n");
        printf("  Matricula: %s", soerch->aluno->matricula);
        printf("  CPF: %s", soerch->aluno->cpf);
        printf("  Nome: %s", soerch->aluno->nome);
        printf("  Nota: %.2f", soerch->aluno->nota);
        printf("  Idade: %d", soerch->aluno->idade);
        printf("  Curso: %s", soerch->aluno->curso);
        printf("  Cidade: %s", soerch->aluno->cidade);
        printf("\n\n");
    }
    else
    {
        printf("\n\n [ERRO] - Aluno não encontrado");
    }
}

NoAluno *searchAlunoInTree(char *name , NoAluno *raiz){
    int val; 
    if(raiz == NULL)
        return NULL;
    val = strcmp(raiz->aluno->nome , name) == 0;
    if(val == 0)
        return raiz;
    if (val < 0)
        return searchAlunoInTree(name, raiz->esq);
    else
        return searchAlunoInTree(name, raiz->dir);
}

// Função para ler arquivo CSV
void lerArquivoCSV(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
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
    while ((novo = new Aluno) != NULL) {
        //%N significa que fará a leitura de até N caracteres, evitando overflow
        //O [^caractere] é uma classe de caracteres negativa - significa "qualquer caractere EXCETO o especificado".
        //É muito útil para parar a leitura quando encontrar um delimitador específico (como vírgula ou quebra de linha).
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", 
                   novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
            
            adicionarAluno(novo);
        } else {
            // Se não conseguiu ler todos os campos, liberar memória e sair
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
    printf("Leitura concluida. Total de alunos: %d\n", a.quantidadeElementosDeAlunos);
}

// Função para exibir todos os alunos
// Função já transformada em generica
//!!! arrumar essa funcao
void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    listItensOfTree(a.raiz);
    printf("Total: %d alunos\n\n", a.quantidadeElementosDeAlunos);
}

void listItensOfTree(NoAluno *current){
    if (current->dir != NULL)
        listItensOfTree(current->dir);
    if (current->esq != NULL)
        listItensOfTree(current->esq);
    printf("  Matricula: %s", current->aluno->matricula);
    printf("  CPF: %s", current->aluno->cpf);
    printf("  Nome: %s", current->aluno->nome);
    printf("  Nota: %.2f", current->aluno->nota);
    printf("  Idade: %d", current->aluno->idade);
    printf("  Curso: %s", current->aluno->curso);
    printf("  Cidade: %s", current->aluno->cidade);
    printf("  ---\n");
}