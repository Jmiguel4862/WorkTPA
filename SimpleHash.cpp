#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>
#define max 1600
 

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
};



struct  Hash{
    Aluno **aluno;
    int quantidade;
    int tamanhoAtual;
};

//CHAMADAS DE FUNÇÕES;

void InicializaList(Hash *ls);
void lerArquivoCSV(const char* nomeArquivo);
void adicionarAluno(Aluno *novo , Hash *ls);
int calculoHash(char* nome , Hash *ls);
int calculoH2(char* nome , Hash *ls);
int calculoReHash(int resultadoCalculoAnterior, int resultadoH2 , Hash *ls);


Hash simplehash;
int colizao = 0;

int main(){
    InicializaList(&simplehash);
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");
    
    time_t inicio, fim;
    inicio = clock();
    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos
    lerArquivoCSV("alunos.csv");
    fim = clock();
    //exibirAlunos();
    printf("\n\n");
    //se eu quiser pegar como inteiro o valor do tempo

    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);

    system("pause");
    return 0;
}

void InicializaList(Hash *ls){
    if(ls->aluno != NULL)
        delete [] ls->aluno;
    ls->aluno = new Aluno*[max];
    for(int i = 0; i < ls->tamanhoAtual;i++)
        ls->aluno[i] = NULL;
    ls->quantidade = 0;
    ls->tamanhoAtual = max;
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
    while ((novo = new Aluno) != NULL) {
        //%N significa que fará a leitura de até N caracteres, evitando overflow
        //O [^caractere] é uma classe de caracteres negativa - significa "qualquer caractere EXCETO o especificado".
        //É muito útil para parar a leitura quando encontrar um delimitador específico (como vírgula ou quebra de linha).
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
            adicionarAluno(novo , &simplehash);
            count++;
            std::  cout<<"\rPorcentagem : "<<count<<std::flush;

        } else {
            // Se não conseguiu ler todos os campos, liberar memória e sair
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
}

void adicionarAluno(Aluno *novo , Hash *ls){
    if (ls->quantidade == ls->tamanhoAtual)
    {
        return;
    }
    int hashAtual = calculoHash(novo->nome, ls);
    while (ls->aluno[hashAtual] != NULL)
    {

        hashAtual = calculoReHash(hashAtual , calculoH2(novo->nome , ls) , ls);
        colizao++;
    }
    ls->aluno[hashAtual] = novo;
    ls->quantidade++;
}

int calculoHash(char* nome , Hash *ls){
    unsigned int k = 0;
    for (int i = 0; nome[i] != '\0';i++)
        k = k * 31 + (unsigned char)nome[i];
    return (int)(k % ls->tamanhoAtual);

}

int calculoH2(char* nome , Hash *ls){
    unsigned int k = 0;
    unsigned int h2;
    for(int i = 0; nome[i] != '\0';i++)
        k = k * 33 + (unsigned char)nome[i];
    h2 = k % (ls->tamanhoAtual - 1);
    return (int)(1 + h2);
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2 , Hash *ls){
    return (resultadoCalculoAnterior + resultadoH2) % ls->tamanhoAtual;
}