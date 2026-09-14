//Exemplo de código para utilizar contagem de tempo em execução de funções ou trechos de código
//Neste exemplo também teremos leitura de arquivos CSV e manipulação de strings
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>

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

#define TAMANHO_HASH_INICIAL 1000000
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
        a.hashOcupada[i] = false;
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
            adicionarAluno(0,novo);
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
    printf("Leitura concluida. Total de alunos: %d\n", a[0].quantidade);
}

// Função para exibir todos os alunos
void exibirAlunos() {
    printf("\n=== LISTA DE ALUNOS ===\n");
    Aluno* atual = a[0].inicio;
    int contador = 1;
    
    while (atual != NULL) {
        printf("Aluno %d:\n", contador);
        printf("  Matricula: %s\n", atual->matricula);
        printf("  CPF: %s\n", atual->cpf);
        printf("  Nome: %s\n", atual->nome);
        printf("  Nota: %.2f\n", atual->nota);
        printf("  Idade: %d\n", atual->idade);
        printf("  Curso: %s\n", atual->curso);
        printf("  Cidade: %s\n", atual->cidade);
        printf("  ---\n");
        
        atual = atual->prox;
        contador++;
    }
    printf("Total: %d alunos\n\n", a[0].quantidade);
}

int calculoHash(char* nome){
    //faça uma função com base nos caracteres do nome do aluno
    // K -> a conta sobre os caracteres

    // retornar o calculo
    // K mod TAMANHO_HASH_INICIAL

}

int calculoH2(char* nome){
    //faça uma função com base nos caracteres do nome do aluno
    // K -> a conta sobre os caracteres

    // retornar o calculo
    // 1 + (K mod (TAMANHO_HASH_INICIAL - 1))
}

int calculoReHash(int resultadoCalculoAnterior, int resultadoH2){
    return (resultadoCalculoAnterior + resultadoH2) % TAMANHO_HASH_INICIAL;
}


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