#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 50
#define MAX_NOTAS 5
#define ARQUIVO_DADOS "alunos.dat"

typedef struct {
    char nome[50];
    float notas[MAX_NOTAS];
    float media;
} Aluno;

// Funções principais
void cadastrarAluno(Aluno alunos[], int *numAlunos);
void calcularMedias(Aluno alunos[], int numAlunos);
void exibirAlunos(Aluno alunos[], int numAlunos);
void exibirAprovados(Aluno alunos[], int numAlunos, float mediaMinima);

// Funções de persistência
void salvarDados(Aluno alunos[], int numAlunos);
int carregarDados(Aluno alunos[]);

// Funções auxiliares
void limparBuffer();
void menu();

int main() {
    Aluno alunos[MAX_ALUNOS];
    int numAlunos = carregarDados(alunos);
    int opcao;

    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarAluno(alunos, &numAlunos);
                break;
            case 2:
                calcularMedias(alunos, numAlunos);
                break;
            case 3:
                exibirAlunos(alunos, numAlunos);
                break;
            case 4: {
                float mediaMinima;
                printf("Digite a média mínima para aprovação: ");
                scanf("%f", &mediaMinima);
                limparBuffer();
                exibirAprovados(alunos, numAlunos, mediaMinima);
                break;
            }
            case 5:
                salvarDados(alunos, numAlunos);
                printf("Dados salvos com sucesso!\n");
                break;
            case 6:
                printf("Saindo do sistema...\n");
                salvarDados(alunos, numAlunos);
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}

void menu() {
    printf("\n--- Sistema de Gerenciamento de Notas ---\n");
    printf("1. Cadastrar Aluno\n");
    printf("2. Calcular Médias\n");
    printf("3. Exibir Alunos\n");
    printf("4. Exibir Alunos Aprovados\n");
    printf("5. Salvar Dados\n");
    printf("6. Sair\n");
}

void cadastrarAluno(Aluno alunos[], int *numAlunos) {
    if (*numAlunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    printf("Cadastro do aluno %d:\n", *numAlunos + 1);
    printf("Digite o nome do aluno: ");
    fgets(alunos[*numAlunos].nome, sizeof(alunos[*numAlunos].nome), stdin);
    alunos[*numAlunos].nome[strcspn(alunos[*numAlunos].nome, "\n")] = '\0'; // Remove o '\n'

    for (int i = 0; i < MAX_NOTAS; i++) {
        do {
            printf("Digite a nota %d (0 a 10): ", i + 1);
            scanf("%f", &alunos[*numAlunos].notas[i]);
            limparBuffer();
            if (alunos[*numAlunos].notas[i] < 0 || alunos[*numAlunos].notas[i] > 10) {
                printf("Nota inválida! Digite um valor entre 0 e 10.\n");
            }
        } while (alunos[*numAlunos].notas[i] < 0 || alunos[*numAlunos].notas[i] > 10);
    }

    alunos[*numAlunos].media = 0; // Inicializa a média como 0
    (*numAlunos)++;
}

void calcularMedias(Aluno alunos[], int numAlunos) {
    for (int i = 0; i < numAlunos; i++) {
        float soma = 0;
        for (int j = 0; j < MAX_NOTAS; j++) {
            soma += alunos[i].notas[j];
        }
        alunos[i].media = soma / MAX_NOTAS;
    }
    printf("Médias calculadas com sucesso!\n");
}

void exibirAlunos(Aluno alunos[], int numAlunos) {
    if (numAlunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Alunos ---\n");
    for (int i = 0; i < numAlunos; i++) {
        printf("Aluno %d: %s\n", i + 1, alunos[i].nome);
        printf("Notas: ");
        for (int j = 0; j < MAX_NOTAS; j++) {
            printf("%.2f ", alunos[i].notas[j]);
        }
        printf("\nMédia: %.2f\n", alunos[i].media);
    }
}

void exibirAprovados(Aluno alunos[], int numAlunos, float mediaMinima) {
    int encontrou = 0;

    printf("\n--- Alunos Aprovados ---\n");
    for (int i = 0; i < numAlunos; i++) {
        if (alunos[i].media >= mediaMinima) {
            printf("Aluno: %s\n", alunos[i].nome);
            printf("Média: %.2f\n", alunos[i].media);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum aluno com média acima de %.2f.\n", mediaMinima);
    }
}

void salvarDados(Aluno alunos[], int numAlunos) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para salvar os dados");
        return;
    }

    if (fwrite(&numAlunos, sizeof(int), 1, arquivo) != 1) {
        printf("Erro ao salvar o número de alunos no arquivo!\n");
        fclose(arquivo);
        return;
    }

    if (fwrite(alunos, sizeof(Aluno), numAlunos, arquivo) != (size_t)numAlunos) {
        printf("Erro ao salvar os dados dos alunos no arquivo!\n");
        fclose(arquivo);
        return;
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso! %d aluno(s) armazenado(s).\n", numAlunos);
}

int carregarDados(Aluno alunos[]) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (arquivo == NULL) {
        printf("Nenhum dado encontrado. Iniciando com base vazia.\n");
        return 0;
    }

    int numAlunos;
    if (fread(&numAlunos, sizeof(int), 1, arquivo) != 1) {
        printf("Erro ao ler o número de alunos do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    if (numAlunos < 0 || numAlunos > MAX_ALUNOS) {
        printf("Dados corrompidos! Número de alunos inválido.\n");
        fclose(arquivo);
        return 0;
    }

    if (fread(alunos, sizeof(Aluno), numAlunos, arquivo) != (size_t)numAlunos) {
        printf("Erro ao carregar os dados dos alunos do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso! %d aluno(s) encontrado(s).\n", numAlunos);
    return numAlunos;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Apenas consome os caracteres restantes no buffer
    }
}