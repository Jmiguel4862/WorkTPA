
//Exemplo de código para montar a primeira arvore binária
//Neste exemplo também teremos leitura de arquivos CSV e manipulação de strings
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>
#define Max  2000000
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
    int altura;
    int grau;
    int nivel;
    Aluno *pai;
};


struct Arvore{
    Aluno **raiz;
    int tamanhoAtual;
    int quantidadeElementosDeAlunos;
    int nivelMaximo;
};

// CHAMADAS DE FUNÇÕES DO PROGRAMA
void clsbuffer();
void cleanBuffer(char *c);
void inicializa();
void insertIntoTree(Aluno *novo , int ind);
void buscarAlunoPorNome();
Aluno *searchAlunoInTree(char *name , int ind);
void lerArquivoCSV(const char* nomeArquivo);
void exibirAlunos();
void listItensOfTree(int ind);
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
    cout<<"ok"<<endl;
    fim = clock();
    //exibirAlunos();
    printf("\n\n");
    buscarAlunoPorNome();
    //se eu quiser pegar como inteiro o valor do tempo

    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);

    system("pause");
    return 0;
}


//CONTEUDOS DAS FUNÇÕES
void clsbuffer(){
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cleanBuffer(char *c){
    if(strlen(c) == 400)
        clsbuffer();
}

void inicializa(){
    if (a.raiz == NULL)
    {
        a.raiz = new Aluno*[Max];
    }
    a.nivelMaximo = 0;
    a.tamanhoAtual = Max;
    a.quantidadeElementosDeAlunos = 0;
    for(int i = 0; i < a.tamanhoAtual && a.raiz[i] != NULL;i++)
    {
        delete a.raiz[i];
        a.raiz = NULL;
    }
}

// Trocar pra função generica recebendo a lista como
// parametro

void insertIntoTree(Aluno *novo , int ind){
    //cout<<ind<<endl;
    if (a.raiz[ind] == NULL)
    {
        if(novo->pai != NULL)
        {
            if (novo->pai->grau == 0) novo->pai->altura++;
            novo->pai->grau++;
        }
        a.raiz[ind] = novo;
        if(novo->nivel > a.nivelMaximo)
            a.nivelMaximo = novo->nivel;
        a.quantidadeElementosDeAlunos++;
    }
    
    else 
    {
        //cout<<ind<<endl;
        novo->nivel++;
        novo->pai = a.raiz[ind];
        if (strcmp(novo->nome , a.raiz[ind]->nome) == 0)
        {
            cout<<"[ERRO] - Aluno ja cadastrado"<<endl;
            delete novo;
        }
        else if (strcmp( a.raiz[ind]->nome , novo->nome) < 0)
        {

            insertIntoTree(novo , (ind*2) + 2);
            if(a.raiz[ind]->altura <= a.raiz[(ind*2)+2]->altura)
                a.raiz[ind]->altura++;
        }
        else
        {
            insertIntoTree(novo , (ind * 2)+ 1);
            if(a.raiz[ind]->altura <= a.raiz[(ind*2) + 1]->altura) 
                (a.raiz[ind])->altura++;
        }

    }
}

void buscarAlunoPorNome(){
    char name [400];
    Aluno *soerch;
    cout<<"\n\nDigite o nome que deseja pesquisar: ";
    fgets(name, 400, stdin);
    soerch = searchAlunoInTree(name, 0);
    if(soerch != NULL)
    {
        printf("\n\n");
        printf("  Matricula: %s", soerch->matricula);
        printf("  CPF: %s", soerch->cpf);
        printf("  Nome: %s", soerch->nome);
        printf("  Nota: %.2f", soerch->nota);
        printf("  Idade: %d", soerch->idade);
        printf("  Curso: %s", soerch->curso);
        printf("  Cidade: %s", soerch->cidade);
        printf("\n\n");
    }
    else
    {
        printf("\n\n [ERRO] - Aluno não encontrado\n\n");
    }
}

Aluno *searchAlunoInTree(char *name , int ind){
    int val; 
    if(a.raiz[ind] == NULL)
        return NULL;
    val = strcmp(a.raiz[ind]->nome , name);
    if(val == 0)
        return a.raiz[ind];
    if (val < 0)
        return searchAlunoInTree(name, (ind * 2) + 2);
    else
        return searchAlunoInTree(name, (ind * 2)+1);
}

// Função para ler arquivo CSV
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
    while ((novo = new Aluno) != NULL) {
        //%N significa que fará a leitura de até N caracteres, evitando overflow
        //O [^caractere] é uma classe de caracteres negativa - significa "qualquer caractere EXCETO o especificado".
        //É muito útil para parar a leitura quando encontrar um delimitador específico (como vírgula ou quebra de linha).
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", 
                   novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
        novo->altura = 0;
        novo->grau = 0;
        novo->nivel = 0;
        novo->pai = NULL;
        insertIntoTree(novo , 0);
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
    listItensOfTree(0);
    printf("Total: %d alunos\n\n", a.quantidadeElementosDeAlunos);
    printf("Nivel maximo: %d \n\n", a.nivelMaximo);
}

void listItensOfTree(int ind){
    if(a.raiz[ind] == NULL)
        return;
    if (a.raiz[(ind *2)+2 ] != NULL)
        listItensOfTree((ind *2)+1);
    printf("  Matricula: %s", a.raiz[ind]->matricula);
    printf("  CPF: %s", a.raiz[ind]->cpf);
    printf("  Nome: %s", a.raiz[ind]->nome);
    printf("  Nota: %.2f", a.raiz[ind]->nota);
    printf("  Idade: %d", a.raiz[ind]->idade);
    printf("  Curso: %s", a.raiz[ind]->curso);
    printf("  Cidade: %s", a.raiz[ind]->cidade);
    printf("  ---\n");
    if (a.raiz[(ind *2)+1] != NULL)
    listItensOfTree(ind*2);
}