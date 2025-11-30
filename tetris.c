#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

typedef struct {
    char nome;
    int id;
} Peca;

char tipos[] = {'I', 'O', 'T', 'L'};

Peca gerarPeca(int id) {
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void exibirFila(Peca fila[], int inicio, int fim, int quantidade) {
    printf("\nFila de peças:\n");

    if (quantidade == 0) {
        printf("[ Fila vazia ]\n");
        return;
    }

    int i = inicio;
    for (int c = 0; c < quantidade; c++) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        i = (i + 1) % TAMANHO_FILA;
    }

    printf("\n");
}

int main() {
    Peca fila[TAMANHO_FILA];
    int inicio = 0, fim = 0, quantidade = 0;
    int ultimoID = 0;
    int opcao;

    srand(time(NULL));

    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila[i] = gerarPeca(ultimoID++);
        quantidade++;
        fim = (fim + 1) % TAMANHO_FILA;
    }

    do {
        system("clear||cls");

        printf("===== TETRIS STACK — FILA DE PEÇAS =====\n");
        exibirFila(fila, inicio, fim, quantidade);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (quantidade == 0) {
                printf("\nA fila está vazia! Não há peça para jogar.\n");
            } else {
                printf("\nPeça jogada: [%c %d]\n", fila[inicio].nome, fila[inicio].id);
                inicio = (inicio + 1) % TAMANHO_FILA;
                quantidade--;
            }
        }

        else if (opcao == 2) {
            if (quantidade == TAMANHO_FILA) {
                printf("\nA fila está cheia! Não é possível inserir nova peça.\n");
            } else {
                fila[fim] = gerarPeca(ultimoID++);
                fim = (fim + 1) % TAMANHO_FILA;
                quantidade++;
                printf("\nNova peça inserida!\n");
            }
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            getchar();
        }

    } while (opcao != 0);

    printf("\nEncerrando o simulador...\n");
    return 0;
}