#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_CAP 5
#define PILHA_CAP 3

typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

/* --- Variáveis das estruturas --- */
Peca fila[FILA_CAP];
int fila_front = 0;
int fila_size = 0;

Peca pilha[PILHA_CAP];
int pilha_top = -1; // -1 = vazia

int nextId = 0;

/* Gera peça aleatória com id único */
Peca gerarPeca() {
    Peca p;
    int r = rand() % 4;
    p.nome = (r == 0) ? 'I' : (r == 1) ? 'O' : (r == 2) ? 'T' : 'L';
    p.id = nextId++;
    return p;
}

/* Enfileira no final (rear) da fila circular — retorna 1 sucesso, 0 falha */
int enqueue(Peca p) {
    if (fila_size == FILA_CAP) return 0;
    int rear = (fila_front + fila_size) % FILA_CAP;
    fila[rear] = p;
    fila_size++;
    return 1;
}

/* Desenfileira da frente — retorna 1 sucesso e põe peça em *out, 0 se vazia */
int dequeue(Peca *out) {
    if (fila_size == 0) return 0;
    *out = fila[fila_front];
    fila_front = (fila_front + 1) % FILA_CAP;
    fila_size--;
    return 1;
}

/* Empilha — retorna 1 sucesso, 0 se cheia */
int push(Peca p) {
    if (pilha_top + 1 >= PILHA_CAP) return 0;
    pilha[++pilha_top] = p;
    return 1;
}

/* Desempilha — retorna 1 sucesso e coloca peça em *out, 0 se vazia */
int pop(Peca *out) {
    if (pilha_top < 0) return 0;
    *out = pilha[pilha_top--];
    return 1;
}

/* Mostra o estado atual: fila (da frente ao fim) e pilha (topo → base) */
void exibirEstado() {
    printf("\nEstado atual:\n");
    printf("Fila de peças\n");
    for (int i = 0; i < FILA_CAP; i++) {
        if (i < fila_size) {
            int idx = (fila_front + i) % FILA_CAP;
            printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        } else {
            printf("[|   ] ");
        }
    }
    printf("\n");
    printf("Pilha de reserva\n");
    printf("(Topo → base): ");
    if (pilha_top < 0) {
        // mostrar placeholders
        for (int i = 0; i < PILHA_CAP; i++) printf("[|   ] ");
    } else {
        // imprime do topo até a base
        for (int i = pilha_top; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
        // se houver espaços vazios, mostra placeholders
        for (int i = pilha_top + 1; i < PILHA_CAP; i++) printf("[|   ] ");
    }
    printf("\n\n");
}

/* Troca a peça da frente da fila com o topo da pilha — retorna 1 sucesso, 0 falha */
int trocarFrenteComTopo() {
    if (fila_size == 0 || pilha_top < 0) return 0;
    int idxFront = fila_front;
    Peca tmp = fila[idxFront];
    fila[idxFront] = pilha[pilha_top];
    pilha[pilha_top] = tmp;
    return 1;
}

/* Troca os 3 primeiros da fila com as 3 da pilha — retorna 1 sucesso, 0 falha */
int trocarTres() {
    if (fila_size < 3 || pilha_top + 1 < 3) return 0;
    int indicesFila[3];
    for (int i = 0; i < 3; i++) indicesFila[i] = (fila_front + i) % FILA_CAP;
    // trocas individuais
    for (int i = 0; i < 3; i++) {
        Peca tmp = fila[indicesFila[i]];
        fila[indicesFila[i]] = pilha[pilha_top - (2 - i)]; // manter ordem relativa: primeiro da fila troca com posição que será topo->base consistente com exemplo
        pilha[pilha_top - (2 - i)] = tmp;
    }
    return 1;
}

/* Inicializa fila com FILA_CAP peças geradas */
void inicializar() {
    fila_front = 0;
    fila_size = 0;
    pilha_top = -1;
    nextId = 0;
    for (int i = 0; i < FILA_CAP; i++) enqueue(gerarPeca());
}

/* Lê opção do usuário com tratamento simples */
int lerOpcao() {
    int op;
    printf("Opções disponíveis:\n");
    printf("1 - Jogar peça da frente da fila\n");
    printf("2 - Enviar peça da fila para a pilha de reserva\n");
    printf("3 - Usar peça da pilha de reserva\n");
    printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("6 - Visualizar estado atual\n");
    printf("0 - Encerrar\n");
    printf("Opção escolhida: ");
    if (scanf("%d", &op) != 1) {
        while (getchar() != '\n'); // limpa buffer
        return -1;
    }
    while (getchar() != '\n'); // limpa o resto da linha
    return op;
}

int main() {
    srand((unsigned)time(NULL));
    inicializar();
    printf("=== Tetris Stack — Nível Mestre ===\n");
    exibirEstado();

    int running = 1;
    while (running) {
        int opt = lerOpcao();
        Peca temp;
        switch (opt) {
            case 1: // Jogar peça da frente da fila (dequeue)
                if (dequeue(&temp)) {
                    printf("\nAção: Jogar peça [ %c %d ] da frente da fila.\n", temp.nome, temp.id);
                    // gera nova peça para manter fila cheia, se possível
                    if (!enqueue(gerarPeca())) {
                        // não deveria acontecer pois acabamos de remover uma
                        printf("Aviso: não foi possível preencher a fila.\n");
                    }
                } else {
                    printf("\nAção inválida: fila vazia.\n");
                }
                exibirEstado();
                break;

            case 2: // Enviar peça da fila para a pilha de reserva
                if (fila_size == 0) {
                    printf("\nAção inválida: fila vazia.\n");
                } else if (pilha_top + 1 >= PILHA_CAP) {
                    printf("\nAção inválida: pilha cheia.\n");
                } else {
                    dequeue(&temp);
                    if (push(temp)) {
                        printf("\nAção: peça [ %c %d ] enviada para a pilha de reserva.\n", temp.nome, temp.id);
                        // gera nova peça para manter fila cheia
                        if (!enqueue(gerarPeca())) {
                            printf("Aviso: não foi possível preencher a fila.\n");
                        }
                    } else {
                        // re-enfileirar caso falhe (proteção)
                        enqueue(temp);
                        printf("\nErro inesperado ao enviar para pilha.\n");
                    }
                }
                exibirEstado();
                break;

            case 3: // Usar peça da pilha de reserva (pop)
                if (pop(&temp)) {
                    printf("\nAção: usar peça reservada [ %c %d ].\n", temp.nome, temp.id);
                    // peça é consumida (não volta)
                } else {
                    printf("\nAção inválida: pilha vazia.\n");
                }
                exibirEstado();
                break;

            case 4: // Trocar peça da frente da fila com o topo da pilha
                if (trocarFrenteComTopo()) {
                    printf("\nAção: troca realizada entre a peça da frente da fila e o topo da pilha.\n");
                } else {
                    printf("\nAção inválida: verifique se há peça na fila e na pilha.\n");
                }
                exibirEstado();
                break;

            case 5: // Trocar os 3 primeiros da fila com as 3 peças da pilha
                if (trocarTres()) {
                    printf("\nAção: troca realizada entre os 3 primeiros da fila e as 3 peças da pilha.\n");
                } else {
                    printf("\nAção inválida: é necessário ter ao menos 3 peças na fila e 3 na pilha.\n");
                }
                exibirEstado();
                break;

            case 6:
                exibirEstado();
                break;

            case 0:
                running = 0;
                printf("\nEncerrando programa. Até logo!\n");
                break;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}