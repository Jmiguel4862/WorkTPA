#include <cstdio>
#include <iostream>
#include <cstring>
#define max 10000
 

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
    Aluno *aluno = new Aluno[max];
    int quantidade = 0;
    int tamanhoAtual = max;
};

//CHAMADAS DE FUNÇÕES;

void InicializaList(Hash);
void lerArquivoCSV(const char* nomeArquivo);
int calcularhash(char *cpf);
void adicionarAluno(Aluno *novo , genericList *gls);


Hash simplehash;

int main(int argc, char const *argv[])
{
    InicializaList(listCPF);

    lerArquivoCSV("alunos.csv");
    return 0;
}

void InicializaList(Hash ls){
    for (int i = 0 ; i < max; i++)
    {
        if (gls[i].start != NULL)
        {
            Hash *temp = gls[i].start , *del = NULL;
            while (temp != NULL)
            {
                del = temp;
                temp = temp->next;
                delete del->aluno;
                delete del;
            }
        }
        gls[i].start = NULL;
        gls[i].end = NULL;
        gls[i].quantidade = 0;
    }
}

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
        if (fscanf(arquivo, "%8[^,],%14[^,],%39[^,],%lf,%d,%39[^,],%39[^\n]\n", novo->matricula, novo->cpf, novo->nome, &novo->nota, &novo->idade, novo->curso, novo->cidade) == 7) {
            adicionarAluno(novo , listCPF);

        } else {
            // Se não conseguiu ler todos os campos, liberar memória e sair
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
}