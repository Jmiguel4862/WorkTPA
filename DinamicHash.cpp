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

#define TAMANHO_HASH_INICIAL 10
struct Alunos{
    Aluno **hash;
    bool *hashOcupada;
    int tamanhoAtual;
    int quantidade;
};

Alunos *a = new Alunos;
long int colidion = 0;

//FUNÇÕES DO PROGRAMA
void inicializa();
void lerArquivoCSV(const char* nomeArquivo);
void exibirAlunos();
void reallocHash();
void adicionarAluno(Aluno *novo);
void exibirAlunos();
int calculoHash(char* nome);
int calculoH2(char* nome);
int calculoReHash(int resultadoCalculoAnterior, int resultadoH2);

int main(){
    inicializa();
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");

    time_t inicio, fim;
    inicio = clock();
    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos
    lerArquivoCSV("alunos.csv");
    fim = clock();
    exibirAlunos();
    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);
    printf("\nTotal de Colições: %ld  \n" ,colidion);
    
    system("pause");
    return 0;
}

void inicializa(){
    a->tamanhoAtual = TAMANHO_HASH_INICIAL;
    a->quantidade = 0;
    a->hash = new Aluno*[a->tamanhoAtual];
    a->hashOcupada = new bool[a->tamanhoAtual];
    for(int i=0; i<a->tamanhoAtual; i++){
        a->hashOcupada[i] = false;
        a->hash[i] = NULL;
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
    printf("Leitura concluida-> Total de alunos: %d\n", a->quantidade);
}

void adicionarAluno(Aluno * novo){
    if(((double)a->quantidade / (double)a->tamanhoAtual) > 0.5)
        reallocHash();
    long int hash = calculoHash(novo->nome);
    while (a->hashOcupada[hash]){
        //std::cout<<"\n ind: "<< hash;
        if (strcmp(novo->cpf, a->hash[hash]->cpf) == 0)
        {
            std::cout<< "\n [ERRO] Aluno ja cadastrado!!\n";
            delete novo;
            return;
        }
        
        hash = calculoReHash(hash, calculoH2(novo->nome));
        colidion++;
    }
    a->hashOcupada[hash] = true;
    a->hash[hash] = novo;
    a->quantidade++;
}


void reallocHash(){
    Alunos *oldAlunos = a;
    printf("\n tamanho pre-for: %d" , oldAlunos->tamanhoAtual);
    a = new Alunos;
    a->tamanhoAtual = oldAlunos->tamanhoAtual * 2;
    a->quantidade = 0;
    a->hash = new Aluno*[a->tamanhoAtual];
    a->hashOcupada = new bool[a->tamanhoAtual];

    for(int i=0; i < a->tamanhoAtual; i++){
        a->hashOcupada[i] = false;
        a->hash[i] = NULL;
    }
    
    for (int i = 0; i < oldAlunos->tamanhoAtual; i++)
        if(oldAlunos->hashOcupada[i])
            adicionarAluno(oldAlunos->hash[i]);

    
    delete []oldAlunos->hash;
    delete []oldAlunos->hashOcupada;
    delete oldAlunos;
    printf("\n finalizo");
}

// Função para exibir todos os alunos
void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    Aluno* atual;
    int contador = 0;
    
    for (int i = 0; i < a->tamanhoAtual; i++)
     {
        if(a->hashOcupada[i])
        {
            atual = a->hash[i];
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
    printf("Total: %d alunos\n\n", a->quantidade);
}

int calculoHash(char* nome){
    long int k = 0;
    for (int i = 0; nome[i] != '\0';i++)
        k = k * 31 + nome[i];
    if(k < 0) k = -k;
    return k % a->tamanhoAtual;

}

int calculoH2(char* nome){
    long int k = 0;
    int h2;
    for(int i = 0; nome[i] != '\0';i++)
        k = k * 33 + nome[i];
    h2 = k % (a->tamanhoAtual - 1);
    return 1 + (h2 < 0 ? -h2 : h2);
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2){
    int finalresult = resultadoCalculoAnterior + resultadoH2;
    if(finalresult < 0) finalresult = -finalresult; 
    return finalresult % a->tamanhoAtual;
}