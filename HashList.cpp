#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>
#define max 10000
 

struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[40];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
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
};

//CHAMADAS DE FUNÇÕES;

void InicializaList(genericList *gls);
void lerArquivoCSV(const char* nomeArquivo);
int indCpf(Aluno *a);
int indNome(Aluno *a);
void adicionarAluno(Aluno *novo , genericList *gls , int(*cmp)(Aluno* , Aluno*) , int (*calcInd)(Aluno *));
int getNumberMatricula(char*matr);
int cpfcmp(Aluno *a1 , Aluno *a2);
int nomecmp(Aluno *a1 , Aluno *a2);
void adicionarNovoAluno();
Hash* buscarCPF(char *cpf);
Hash* buscarNome(char *cpf);
void deletaAluno(Aluno *a);
void deletaHash(genericList *gls , Hash **del , int (*calcInd)(Aluno *));
void exibirAlunos(genericList *gls);
void limpar_buffer();



genericList listCPF[max] , listNome[max];
int quantidade = 0;
int ultimaMatricula = 0;

int main(){
    int op;
    char text[40];
    Hash *buscado;
    InicializaList(listCPF);
    /*printf("=== SISTEMA DE LEITURA DE ALUNOS CSV ===\n\n");
    
    time_t inicio, fim;
    inicio = clock();
    // Ler arquivo CSV (você pode alterar o nome do arquivo) Essa função já cria a lista dinâmica com os alunos
    lerArquivoCSV("alunos.csv");
    fim = clock();
    //exibirAlunos();
    printf("\n\n");
    //se eu quiser pegar como inteiro o valor do tempo

    printf("Tempo de leitura: %ld milissegundos\n", (int)fim - inicio);

    system("pause");*/
    do
    {
        printf("\n=====  MENU  =====");
    printf("\n1 -> Reinicializar Listas");
    printf("\n2 -> Ler Arquivo");
    printf("\n3 -> Adicionar Aluno");
    printf("\n4 -> Buscar Aluno por CPF");
    printf("\n5 -> Buscar Aluno por Nome");
    printf("\n6 -> Deleta Aluno por CPF");
    printf("\n7 -> Deleta Aluno por Nome");
    printf("\n8 -> Exibir Aluno por CPF");
    printf("\n9 -> Exibir Aluno por Nome");
    printf("\n0 -> sair");
    printf("\n Operador:");
    scanf("%d" , &op);
    limpar_buffer();
    switch (op)
    {
        case 1:
            InicializaList(listCPF);
            InicializaList(listNome);
            break;
        case 2:
            lerArquivoCSV("alunos.csv");
            break;
        case 3:
            adicionarNovoAluno();
            break;
        case 4:
            printf("\n\n -> Busca por: ");
            fgets(text , 15 , stdin);
            limpar_buffer();
            buscado = buscarCPF(text);
            if (buscado != NULL) printf("\n|%-10s | %-40s | %-15s|" , buscado->aluno->matricula, buscado->aluno->nome, buscado->aluno->cpf);
            else  printf("\n [ERRO] - Aluno não encontrado!");
            break;
        case 5:
            printf("\n\n -> Busca por: ");
            fgets(text , 40 , stdin);
            limpar_buffer();
            buscado = buscarNome(text);
            if (buscado != NULL) printf("\n|%-10s | %-40s | %-15s|" , buscado->aluno->matricula, buscado->aluno->nome, buscado->aluno->cpf);
            else  printf("\n [ERRO] - Aluno não encontrado!");
            break;
        case 6:
            printf("\n\n -> Busca por: ");
            fgets(text , 15 , stdin);
            limpar_buffer();
            deletaAluno(buscarCPF(text)->aluno);
            break;
        case 7:
            printf("\n\n -> Busca por: ");
            fgets(text , 40 , stdin);
            limpar_buffer();
            deletaAluno(buscarNome(text)->aluno);
            break;
        case 8:
            exibirAlunos(listCPF);
            break;
        case 9:
            exibirAlunos(listNome);
            break;
        case 0:
            break;
        default:
            printf("\n[ERRO] - Operado Invalido!!\n\n");
            break;
    }
    }while (op != 0); 
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
        quantidade = 0;
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
            adicionarAluno(novo , listCPF , cpfcmp , indCpf);
            adicionarAluno(novo , listNome , nomecmp , indNome);
            int matr = getNumberMatricula(novo->matricula);
            if(matr > ultimaMatricula) ultimaMatricula = matr; 
            quantidade++;

        } else {
            // Se não conseguiu ler todos os campos, liberar memória e sair
            delete novo;
            break;
        }
    }
    
    fclose(arquivo);
}

int indCpf(Aluno *a)
{
    int n1 , n2 , n3;
    if(sscanf(a->cpf, "%d.%d.%d" , &n1 , &n2 , &n3 ) != 3)
        return 0;
    return ((n1 * 3) + (n2 * 5) + (n3 * 7)) % max;
}

int indNome(Aluno * a)
{
    unsigned int num = 0;
    for(int i = 0;a->nome[i] != '\0';i++ )
        num += 31 * (unsigned char) a->nome[i];
    return (num % max);
}

void adicionarAluno(Aluno *novo , genericList *gls , int(*cmp)(Aluno* , Aluno*) , int (*calcInd)(Aluno *)){
    int ind = calcInd(novo);
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
            if (cmp(novo,current->aluno) == 0)
            {
                printf("\n [ERRO] - Aluno já cadastrado!");
                delete novaHash;
                delete novo;
                return;
            }
            else if (cmp(novo,current->aluno) < 0)
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
}

int getNumberMatricula(char*matr){
    int num;
    if(sscanf(matr, "A%d", &num) != 1)
        return -1;
    return num;
}

int cpfcmp(Aluno *a1 , Aluno *a2){
    return strcmp(a1->cpf,a2->cpf);
}

int nomecmp(Aluno *a1 , Aluno *a2){
    return strcmp(a1->nome,a2->nome);
}

void adicionarNovoAluno(){
    Aluno *novo = new Aluno;
    printf("\n\n PREENCHA AS INFORMAÇÕES ABAIXO\n\n:");
    printf("\nNome:");
    fgets(novo->nome , 40 , stdin);
    limpar_buffer();
    printf("\nCPF:");
    fgets(novo->cpf , 40 , stdin);
    limpar_buffer();
    printf("\nIdade:");
    scanf("%d" , &novo->idade);
    limpar_buffer();
    printf("\nCidade:");
    fgets(novo->cidade , 40 , stdin);
    limpar_buffer();
    printf("\nCurso:");
    fgets(novo->curso , 40 , stdin);
    limpar_buffer();
    ultimaMatricula++;
    sprintf(novo->matricula, "%d", ultimaMatricula);
    novo->nota = 0;
    adicionarAluno(novo , listCPF , cpfcmp , indCpf);
    adicionarAluno(novo , listNome , nomecmp, indNome);
}

Hash* buscarCPF(char *cpf){
    int ind;
    Hash *current;
    Aluno a;
    strcpy(a.cpf , cpf);
    ind = indCpf(&a);
    current = listCPF->start;
    while (current != NULL)
    {
        if(strcmp(current->aluno->cpf, cpf) == 0)
            return current;
    }
    return NULL;
}

Hash* buscarNome(char *nome){
    int ind;
    Hash *current;
    Aluno a;
    strcpy(a.nome, nome);
    ind = indNome(&a);
    current = listNome->start;
    while (current != NULL)
    {
        if(strcmp(current->aluno->nome, nome) == 0)
            return current;
    }
    return NULL;
}

void deletaAluno(Aluno *a){
    Hash *cpf = buscarCPF(a->cpf);
    Hash *nome = buscarNome(a->nome);
    if(cpf != NULL && nome != NULL)
    {
        deletaHash(listCPF, &cpf , indCpf);
        deletaHash( listNome, &nome , indNome);
        quantidade--;
        delete a;
    }
    else
    {
        printf("\n [ERRO] - Aluno não encontrado!");
    }
    
}

void deletaHash(genericList *gls , Hash **del , int (*calcInd)(Aluno *)){
    int ind = calcInd((*del)->aluno);
    Hash *current = gls[ind].start , *delPrev = (*del)->prev, *delNext = (*del)->next;
    if (gls[ind].start == *del)
    {
        gls[ind].start = gls[ind].start->next;
        gls[ind].start->prev = NULL;
    }
    else if (gls[ind].end == *del)
    {
        gls[ind].end = gls[ind].end->next;
        gls[ind].end->next = NULL;
    }
    else
    {
        delPrev->next = delNext;
        delNext->prev = delPrev;
    }
    delete *del;
}


void exibirAlunos(genericList *gls){
    Hash *current;
    printf("\n|%-10s | %-40s | %-15s|" , "Matricula" , "Nome" , "CPF");
    for(int i = 0; i < max; i++)
    {
        current = gls[i].start;
        while (current != NULL)
        {
            printf("\n|%-10s | %-40s | %-15s|" , current->aluno->matricula, current->aluno->nome, current->aluno->cpf);
            current = current->next;
        }
    }
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
