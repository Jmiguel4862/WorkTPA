//Exemplo de código para utilizar contagem de tempo em execução de funções ou trechos de código

//Neste exemplo também teremos leitura de arquivos CSV e manipulação de strings

#include <cstdio>

#include <cstdlib>

#include <ctime>

#include <string.h>

#include <iostream>
//Exemplo do arquivo CSV que será lido

//Matricula,CPF,Nome,Nota,Idade,Curso,Cidade

//A0000000,915.216.859-08,Wallace Sampaio,20.35,23,Direito,Rio de Janeiro
// TERMINE O CÓDIGO PARA CONSEGUIR INSERIR OS ALUNOS CALCULANDO

// A HASH COM BASE NO NOME DO ALUNO.

// POR ENQUANTO, NÃO SE PREOCUPE COM CPFs REPETIDOS.

// IMPLEMENTE EXCLUSAO E BUSCA POR NOME

// CALCULE QUANTAS COLISOES ACONTECERAM NA INSERÇÃO
struct Aluno{

    char matricula[9];

    char cpf[15];

    char nome[40];

    double nota;

    int idade;

    char curso[40];

    char cidade[40];

};
#define TAMANHO_HASH_INICIAL 2000000

struct Alunos{

    Aluno **hash;

    bool *hashOcupada;

    int tamanhoAtual;

    int quantidade;

};
Alunos a;
void inicializa(){

    a.tamanhoAtual = TAMANHO_HASH_INICIAL;

    a.quantidade = 0;

    a.hash = new Aluno*[a.tamanhoAtual];

    a.hashOcupada = new bool[a.tamanhoAtual];

    for(int i=0; i<a.tamanhoAtual; i++){

        a.hash[i] = NULL;

        a.hashOcupada[i] = false;

    }

}
void adicionarAluno(Aluno * a);

void lerArquivoCSV(const char* nomeArquivo);

void exibirAlunos();

int calculoHash(char* nome);

int calculoH2(char* nome);

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2);



int main(){

    inicializa();

    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");

    //.....

    Aluno* alunoTemp;

    ///

    alunoTemp = new Aluno;

    //...ja li o aluno.. e salvei no alunoTemp (aqui teve um new)

    a.hash[1548] = alunoTemp;

    time_t inicio, fim;

    inicio = clock();

    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos

    lerArquivoCSV("alunos.csv");

    fim = clock();

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



void adicionarAluno(Aluno * al){

    int h1 = calculoHash(al->nome), ind = h1 , count = 1;

    while (a.hashOcupada[ind])

    {

        if (strcmp(a.hash[ind]->cpf , al->cpf) == 0)

        {

            std::cout<<"\n\n[ERRO] Aluno já cadastrado.\n"<<std::endl;

            delete al;

            return;

        }

        /*if (count >= TAMANHO_HASH_INICIAL)

        {

            std::cout<<"\n Não tem mais espaços disponiveis\n"<<std::endl;

            delete al;

            return;

        }*/

        ind = calculoReHash(ind, calculoH2(al->nome));

        count++;

    }

    a.hash[ind] = al;

    a.hashOcupada[ind] = true;

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

           

            //pega o endereço que deve ser inserido no vetor de alunos

            adicionarAluno(novo);

            //printf("Aluno adicionado: %s - %s\n", novo->matricula, novo->nome);

            // Consumir a quebra de linha restante

            //fgetc(arquivo);

        } else {

            // Se não conseguiu ler todos os campos, liberar memória e sair

            delete novo;

            break;

        }

    }

   

    fclose(arquivo);

    printf("Leitura concluida. Total de alunos: %d\n", a.quantidade);

}



// Função para exibir todos os alunos

void exibirAlunos() {

    printf("\n=== LISTA DE ALUNOS ===\n");

    int contador = 1;

   

    for(int i = 0; i < a.quantidade && a.hash[i] != NULL;i++) {

        printf("  Aluno %d:\n", contador);

        printf("  Matricula: %s\n", a.hash[i]->matricula);

        printf("  CPF: %s\n", a.hash[i]->cpf);

        printf("  Nome: %s\n", a.hash[i]->nome);

        printf("  Nota: %.2f\n", a.hash[i]->nota);

        printf("  Idade: %d\n", a.hash[i]->idade);

        printf("  Curso: %s\n", a.hash[i]->curso);

        printf("  Cidade: %s\n", a.hash[i]->cidade);

        printf("  ---\n");

        contador++;

    }

    printf("Total: %d alunos\n\n", a.quantidade);

}



int calculoHash(char* nome){

    int total = 1;

    for(int i = 0; i < strlen(nome); i++)

        total = total + nome[i];

    return ((total<0)?-(total):total) % TAMANHO_HASH_INICIAL;

}



int calculoH2(char* nome){

    int resul = calculoHash(nome) + 1;

    return ((resul < 0)?-(resul):resul);

}



int calculoReHash(int resultadoCalculoAnterior, int resultadoH2){

    return (resultadoCalculoAnterior + resultadoH2) % TAMANHO_HASH_INICIAL;

}
