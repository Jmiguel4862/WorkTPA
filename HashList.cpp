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
    Aluno *aluno = NULL;
    Hash *next = NULL;
    Hash *prev = NULL;
};

struct genericList
{
    Hash *start = NULL;
    Hash *end = NULL;
    int quantidade = 0;
};

//CHAMADAS DE FUNÇÕES;

void InicializaList(genericList *gls);
void lerArquivoCSV(const char* nomeArquivo);
int calcularhash(char *cpf);
void adicionarAluno(Aluno *novo , genericList *gls);


genericList listCPF[max];

int main(int argc, char const *argv[])
{
    InicializaList(listCPF);

    lerArquivoCSV("alunos.csv");
    return 0;
}

void InicializaList(genericList *gls){
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

int calcularhash(char *cpf)
{
    int n1 , n2 , n3;
    if(sscanf(cpf, "%d.%d.%d" , &n1 , &n2 , &n3 ) != 3)
        return 0;
    return ((n1 * 3) + (n2 * 5) + (n3 * 7)) % max;
}

void adicionarAluno(Aluno *novo , genericList *gls){
    int ind = calcularhash(novo->cpf);
    Hash *current = gls[ind].start , *novaHash = new Hash;
    novaHash->aluno = novo;
    if (current == NULL)
    {
        gls[ind].start = novaHash;
        gls[ind].end = novaHash;
    }
    else
    {
        while (current != NULL)
        {
            if (strcmp(novo->cpf,current->aluno->cpf) == 0)
            {
                printf("\n [ERRO] - Aluno já cadastrado!");
                delete novaHash;
                delete novo;
                return;
            }
            else if (strcmp(novo->cpf,current->aluno->cpf) < 0)
            {
                if(current == gls[ind].start)
                {
                    novaHash->next = gls[ind].start;
                    gls[ind].start->prev = novaHash;
                    gls[ind].start = novaHash;
                    break;
                }
                else
                {
                    novaHash->next = current;
                    novaHash->prev = current->prev;
                    current->prev->next = novaHash;
                    current->prev = novaHash;
                    break;
                }
            }
        current = current->next;
        }
        if(current == NULL)
        {
            novaHash->prev = gls[ind].end;
            gls[ind].end->next = novaHash;
            gls[ind].end = novaHash;
        }
    }
    gls[ind].quantidade++;
}
