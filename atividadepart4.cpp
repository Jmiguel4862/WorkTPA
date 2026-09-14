#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>

struct Aluno {
    char matricula[9];
    char cpf[15];
    char nome[40];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
};

#define TAMANHO_HASH_INICIAL 2000000

struct Alunos {
    Aluno **hash;
    bool *hashOcupada;
    bool *hashRemovida; // Para controle de lápides (tombstones) na exclusão
    int tamanhoAtual;
    int quantidade;
    long long totalColisoes; // Contador de colisões
};

Alunos a;

void inicializa() {
    a.tamanhoAtual = TAMANHO_HASH_INICIAL;
    a.quantidade = 0;
    a.totalColisoes = 0;
    a.hash = new Aluno*[a.tamanhoAtual];
    a.hashOcupada = new bool[a.tamanhoAtual];
    a.hashRemovida = new bool[a.tamanhoAtual];
    for (int i = 0; i < a.tamanhoAtual; i++) {
        a.hash[i] = NULL;
        a.hashOcupada[i] = false;
        a.hashRemovida[i] = false;
    }
}

// Funções de Hash
int calculoHash(const char* nome) {
    unsigned long hash = 5381; // Algoritmo djb2 (reduz muito colisões de strings)
    int c;
    while ((c = *nome++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % TAMANHO_HASH_INICIAL;
}

int calculoH2(const char* nome) {
    int val = calculoHash(nome);
    // Garante um salto ímpar e não nulo para percorrer toda a tabela
    return 1 + (val % (TAMANHO_HASH_INICIAL - 1));
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2) {
    return (resultadoCalculoAnterior + resultadoH2) % TAMANHO_HASH_INICIAL;
}

// Inserção com contagem de colisões
void adicionarAluno(Aluno *al) {
    int h1 = calculoHash(al->nome);
    int h2 = calculoH2(al->nome);
    int ind = h1;
    int tentativas = 0;
    int primeiroLivre = -1;

    while (a.hashOcupada[ind] || a.hashRemovida[ind]) {
        // Se encontramos um slot marcado como removido, guardamos para reuso caso não achem duplicado
        if (a.hashRemovida[ind] && primeiroLivre == -1) {
            primeiroLivre = ind;
        }

        // Verifica duplicidade por CPF se o slot estiver ocupado ativamente
        if (a.hashOcupada[ind] && strcmp(a.hash[ind]->cpf, al->cpf) == 0) {
            std::cout << "[ERRO] Aluno com CPF " << al->cpf << " já cadastrado.\n";
            delete al;
            return;
        }
        
        a.totalColisoes++;
        tentativas++;
        if (tentativas >= TAMANHO_HASH_INICIAL) {
            std::cout << "[ERRO] Tabela Hash cheia.\n";
            delete al;
            return;
        }

        ind = calculoReHash(ind, h2);
    }

    // Se passou por uma posição removida anteriormente, insere nela
    int posicaoFinal = (primeiroLivre != -1) ? primeiroLivre : ind;

    a.hash[posicaoFinal] = al;
    a.hashOcupada[posicaoFinal] = true;
    a.hashRemovida[posicaoFinal] = false;
    a.quantidade++;
}

// Busca por Nome
Aluno* buscarAlunoPorNome(const char* nome) {
    int ind = calculoHash(nome);
    int h2 = calculoH2(nome);
    int tentativas = 0;

    while (a.hashOcupada[ind] || a.hashRemovida[ind]) {
        if (a.hashOcupada[ind] && strcmp(a.hash[ind]->nome, nome) == 0) {
            return a.hash[ind];
        }
        tentativas++;
        if (tentativas >= TAMANHO_HASH_INICIAL) break;
        ind = calculoReHash(ind, h2);
    }
    return NULL;
}

// Exclusão por Nome
bool excluirAlunoPorNome(const char* nome) {
    int ind = calculoHash(nome);
    int h2 = calculoH2(nome);
    int tentativas = 0;

    while (a.hashOcupada[ind] || a.hashRemovida[ind]) {
        if (a.hashOcupada[ind] && strcmp(a.hash[ind]->nome, nome) == 0) {
            delete a.hash[ind];
            a.hash[ind] = NULL;
            a.hashOcupada[ind] = false;
            a.hashRemovida[ind] = true; // Marca como removido (lápide)
            a.quantidade--;
            return true;
        }
        tentativas++;
        if (tentativas >= TAMANHO_HASH_INICIAL) break;
        ind = calculoReHash(ind, h2);
    }
    return false;
}

void lerArquivoCSV(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    char linha[300];
    
    printf("Iniciando leitura do arquivo CSV...\n");
    
    // Pular cabeçalho
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
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
    printf("Leitura concluida. Total de alunos cadastrados: %d\n", a.quantidade);
    printf("Total de colisões durante a inserção: %lld\n", a.totalColisoes);
}

void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    int exibidos = 0;
    
    for (int i = 0; i < a.tamanhoAtual; i++) {
        if (a.hashOcupada[i]) {
            exibidos++;
            printf("Aluno %d (Índice Hash %d):\n", exibidos, i);
            printf("  Matricula: %s | Nome: %s | CPF: %s\n", a.hash[i]->matricula, a.hash[i]->nome, a.hash[i]->cpf);
            printf("  Nota: %.2f | Idade: %d | Curso: %s | Cidade: %s\n", a.hash[i]->nota, a.hash[i]->idade, a.hash[i]->curso, a.hash[i]->cidade);
            printf("  ---\n");
        }
    }
    printf("Total exibido: %d alunos\n\n", exibidos);
}

int main() {
    inicializa();
    printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");

    clock_t inicio = clock();
    lerArquivoCSV("alunos.csv");
    clock_t fim = clock();

    double tempoMs = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    printf("Tempo de leitura e inserção: %.2f ms\n\n", tempoMs);

    // Teste de Busca
    const char* nomeBusca = "Wallace Sampaio";
    Aluno* achado = buscarAlunoPorNome(nomeBusca);
    if (achado) {
        printf("[BUSCA] Aluno encontrado: %s - %s (Nota: %.2f)\n", achado->matricula, achado->nome, achado->nota);
    } else {
        printf("[BUSCA] Aluno '%s' não encontrado.\n", nomeBusca);
    }

    // Teste de Exclusão
    if (excluirAlunoPorNome(nomeBusca)) {
        printf("[EXCLUSÃO] Aluno '%s' removido com sucesso.\n", nomeBusca);
    }

    // Validação da Exclusão
    achado = buscarAlunoPorNome(nomeBusca);
    if (!achado) {
        printf("[BUSCA] Confirmação: Aluno '%s' não existe mais na Hash.\n", nomeBusca);
    }

    return 0;
}