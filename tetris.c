#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

Peca fila[TAM_FILA];
Peca pilha[TAM_PILHA];

int frente = 0;
int tras = 0;
int topo = -1;
int ultimoID = 0;

char tipos[] = {'I', 'O', 'T', 'L'};

Peca gerarPeca() {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = ultimoID++;
    return p;
}

void enqueue(Peca p) {
    fila[tras] = p;
    tras = (tras + 1) % TAM_FILA;
}

Peca dequeue() {
    Peca p = fila[frente];
    frente = (frente + 1) % TAM_FILA;
    return p;
}

int push(Peca p) {
    if (topo == TAM_PILHA - 1) return 0;
    pilha[++topo] = p;
    return 1;
}

int pop(Peca *p) {
    if (topo == -1) return 0;
    *p = pilha[topo--];
    return 1;
}

void exibirEstado() {
    printf("\n================= ESTADO ATUAL =================\n");

    printf("Fila de pecas:\n");
    for (int i = 0; i < TAM_FILA; i++) {
        int idx = (frente + i) % TAM_FILA;
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
    }

    printf("\n\nPilha de reserva (Topo -> Base):\n");
    if (topo == -1)
        printf("(vazia)");
    else
        for (int i = topo; i >= 0; i--)
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);

    printf("\n=================================================\n");
}

int main() {
    srand(time(NULL));

    // Inicializa fila cheia
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(gerarPeca());

    int opcao;

    do {
        exibirEstado();

        printf("\n1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada = dequeue();
            printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);

            // SEMPRE gera nova peça e mantém fila cheia
            enqueue(gerarPeca());
        }

        else if (opcao == 2) {
            if (topo == TAM_PILHA - 1) {
                printf("\nPilha cheia! Nao foi possivel reservar.\n");
                enqueue(gerarPeca());
            } else {
                Peca p = dequeue();
                push(p);
                printf("\nPeca reservada: [%c %d]\n", p.nome, p.id);
                enqueue(gerarPeca());
            }
        }

        else if (opcao == 3) {
            Peca usada;
            if (pop(&usada))
                printf("\nPeca usada: [%c %d]\n", usada.nome, usada.id);
            else
                printf("\nNao ha pecas reservadas.\n");
        }

    } while (opcao != 0);

    return 0;
}