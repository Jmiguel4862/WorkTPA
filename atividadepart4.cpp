#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>

struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[40];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
};

#define TAMANHO_HASH_INICIAL 1000000

struct Alunos{
    Aluno **hash;
    bool *hashOcupada;
    bool *hashRemovido; // Utilizado para controle de exclusão em endereçamento aberto
    int tamanhoAtual;
    int quantidade;
};

Alunos a;
int colisoesTotais = 0;

void inicializa(){
    a.tamanhoAtual = TAMANHO_HASH_INICIAL;
    a.quantidade = 0;
    a.hash = new Aluno*[a.tamanhoAtual];
    a.hashOcupada = new bool[a.tamanhoAtual];
    a.hashRemovido = new bool[a.tamanhoAtual];
    
    for(int i = 0; i < a.tamanhoAtual; i++){
        a.hashOcupada[i] = false;
        a.hashRemovido[i] = false;
        a.hash[i] = NULL;
    }
}

// Funções Hash baseadas nos caracteres do nome
int calculoHash(char* nome){
    unsigned long k = 0;
    while (*nome) {
        k = k * 31 + (unsigned char)(*nome);
        nome++;
    }
    return k % TAMANHO_HASH_INICIAL;
}

int calculoH2(char* nome){
    unsigned long k = 0;
    while (*nome) {
        k = k * 33 + (unsigned char)(*nome);
        nome++;
    }
    return 1 + (k % (TAMANHO_HASH_INICIAL - 1));
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2){
    return (resultadoCalculoAnterior + resultadoH2) % TAMANHO_HASH_INICIAL;
}

// Função para adicionar aluno na tabela hash tratando colisões
void adicionarAluno(Aluno* novo) {
    int h1 = calculoHash(novo->nome);
    int h2 = calculoH2(novo->nome);
    int idx = h1;
    int tentativas = 0;

    while (a.hashOcupada[idx]) {
        colisoesTotais++;
        tentativas++;
        if (tentativas >= a.tamanhoAtual) {
            printf("Erro: Tabela hash cheia!\n");
            delete novo;
            return;
        }
        idx = calculoReHash(idx, h2);
    }

    a.hash[idx] = novo;
    a.hashOcupada[idx] = true;
    a.hashRemovido[idx] = false;
    a.quantidade++;
}

// Função de Busca por Nome
Aluno* buscarAluno(char* nome) {
    int h1 = calculoHash(nome);
    int h2 = calculoH2(nome);
    int idx = h1;
    int tentativas = 0;

    while (a.hashOcupada[idx] || a.hashRemovido[idx]) {
        if (a.hashOcupada[idx] && strcmp(a.hash[idx]->nome, nome) == 0) {
            return a.hash[idx];
        }
        tentativas++;
        if (tentativas >= a.tamanhoAtual) break;
        idx = calculoReHash(idx, h2);
    }
    return NULL; // Não encontrado
}

// Função de Exclusão por Nome
bool excluirAluno(char* nome) {
    int h1 = calculoHash(nome);
    int h2 = calculoH2(nome);
    int idx = h1;
    int tentativas = 0;

    while (a.hashOcupada[idx] || a.hashRemovido[idx]) {
        if (a.hashOcupada[idx] && strcmp(a.hash[idx]->nome, nome) == 0) {
            delete a.hash[idx];
            a.hash[idx] = NULL;
            a.hashOcupada[idx] = false;
            a.hashRemovido[idx] = true; // Marca como removido (tombstone)
            a.quantidade--;
            return true; // Excluído com sucesso
        }
        tentativas++;
        if (tentativas >= a.tamanhoAtual) break;
        idx = calculoReHash(idx, h2);
    }
    return false; // Não encontrado para exclusão
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
    
    Aluno* novo;
    while ((novo = new Aluno) != NULL) {
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", 
                   novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
            
            adicionarAluno(novo);
        } else {
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
    printf("Leitura concluida. Total de alunos inseridos: %d\n", a.quantidade);
    printf("Total de colisoes registradas: %d\n", colisoesTotais);
}

// Função para exibir todos os alunos cadastrados na Hash
void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    int contador = 1;
    
    for(int i = 0; i < a.tamanhoAtual; i++){
        if(a.hashOcupada[i] && a.hash[i] != NULL){
            Aluno* atual = a.hash[i];
            printf("Aluno %d (Posicao %d):\n", contador, i);
            printf("  Matricula: %s\n", atual->matricula);
            printf("  CPF: %s\n", atual->cpf);
            printf("  Nome: %s\n", atual->nome);
            printf("  Nota: %.2f\n", atual->nota);
            printf("  Idade: %d\n", atual->idade);
            printf("  Curso: %s\n", atual->curso);
            printf("  Cidade: %s\n", atual->cidade);
            printf("  ---\n");
            contador++;
        }
    }
    printf("Total exibido: %d alunos\n\n", a.quantidade);
}

int main(){
    inicializa();
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");

    time_t inicio, fim;
    inicio = clock();
    
    // Ler arquivo CSV
    lerArquivoCSV("alunos.csv");
    
    fim = clock();

    printf("Tempo de leitura e insercao: %d milissegundos\n", (int)(fim - inicio));
    
    // Exemplo de teste de busca
    char nomeBusca[40] = "Wallace Sampaio";
    Aluno* resBusca = buscarAluno(nomeBusca);
    if(resBusca != NULL){
        printf("\n[Busca] Aluno encontrado: %s | Curso: %s\n", resBusca->nome, resBusca->curso);
    } else {
        printf("\n[Busca] Aluno nao encontrado.\n");
    }

    system("pause");
    return 0;
}