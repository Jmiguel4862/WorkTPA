//Exemplo de código para utilizar contagem de tempo em execução de funções ou trechos de código
//Neste exemplo também teremos leitura de arquivos CSV e manipulação de strings
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>


struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[40];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
};

#define TAMANHO_HASH_INICIAL 1500
struct Alunos{
    Aluno **hash;
    bool *hashOcupada;
    int tamanhoAtual;
    int quantidade;
};

Alunos a;
long int colidion = 0;

//FUNÇÕES DO PROGRAMA
void inicializa();
void lerArquivoCSV(const char* nomeArquivo);
void exibirAlunos();
void adicionarAluno(Aluno *novo);
void exibirAlunos();
int calculoHash(char* nome);
int calculoH2(char* nome);
int calculoReHash(int resultadoCalculoAnterior, int resultadoH2);

int main(){
    inicializa();
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");
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

    //se eu quiser pegar como double o valor do tempo
    // double tempo2 = difftime(fim, inicio);
    // printf("Tempo de leitura: %.2f segundos\n", tempo2);
    // Exibir todos os alunos carregados
    //exibirAlunos();
    exibirAlunos();
    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);
    printf("\nTotal de Colições: %ld  \n" ,colidion);
    
    system("pause");
    return 0;
}

void inicializa(){
    a.tamanhoAtual = TAMANHO_HASH_INICIAL;
    a.quantidade = 0;
    a.hash = new Aluno*[a.tamanhoAtual];
    a.hashOcupada = new bool[a.tamanhoAtual];
    for(int i=0; i<a.tamanhoAtual; i++){
        a.hashOcupada[i] = false;
        a.hash[i] = NULL;
    }
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

void adicionarAluno(Aluno * novo){
    long int hash = calculoHash(novo->nome);
    if (a.quantidade == a.tamanhoAtual)
    {
        std::cout<< "\n [ERRO] Memoria cheia!!\n";
        return;
    }
    
    while (a.hashOcupada[hash]){
        if (strcmp(novo->cpf, a.hash[hash]->cpf) == 0)
        {
            std::cout<< "\n [ERRO] Aluno ja cadastrado!!\n";
            delete novo;
        }
        
        hash = calculoReHash(hash, calculoH2(novo->nome));
        colidion++;
    }

    a.hashOcupada[hash] = true;
    a.hash[hash] = novo;
    a.quantidade++;
}

// Função para exibir todos os alunos
void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    Aluno* atual;
    int contador = 0;
    
    for (int i = 0; i < a.tamanhoAtual; i++)
     {
        if(a.hashOcupada[i])
        {
            atual = a.hash[i];
            printf("\nAluno : %d  ", contador);
            printf("  Matricula: %s  ", atual->matricula);
            printf("  CPF: %s  ", atual->cpf);
            printf("  Nome: %s  ", atual->nome);
            printf("  Nota: %.2f  ", atual->nota);
            printf("  Idade: %d  ", atual->idade);
            printf("  Curso: %s  ", atual->curso);
            printf("  Cidade: %s  ", atual->cidade);
            printf("  \n---\n");
            contador++;
        }
    }
    printf("Total: %d alunos\n\n", a.quantidade);
}

int calculoHash(char* nome){
    long int k = 0;
    for (int i = 0; nome[i] != '\0';i++)
        k = k * 31 + nome[i];
    if(k < 0) k = -k;
    return k % a.tamanhoAtual;

}

int calculoH2(char* nome){
    long int k = 0;
    for(int i = 0; nome[i] != '\0';i++)
        k = k * 33 + nome[i];
    if(k < 0) k = -k;
    return 1 + (k % a.tamanhoAtual - 1);
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2){
    int finalresult = resultadoCalculoAnterior + resultadoH2;
    if(finalresult < 0) finalresult = -finalresult; 
    return finalresult % a.tamanhoAtual;
}