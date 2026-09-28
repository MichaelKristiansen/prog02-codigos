#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

No *criarNo(int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    novo->valor = valor;
    novo->anterior = NULL;
    novo->proximo = NULL;

    return novo;
}

void inserirInicio(Lista *lista, int valor) {
    No *novo = criarNo(valor);

    novo->proximo = lista->inicio;

    if (lista->inicio != NULL) {
        lista->inicio->anterior = novo;
    } else {
        lista->fim = novo;
    }

    lista->inicio = novo;
    lista->tamanho++;
}

void inserirFim(Lista *lista, int valor) {
    No *novo = criarNo(valor);

    novo->anterior = lista->fim;

    if (lista->fim != NULL) {
        lista->fim->proximo = novo;
    } else {
        lista->inicio = novo;
    }

    lista->fim = novo;
    lista->tamanho++;
}

int inserirPosicao(Lista *lista, int valor, int posicao) {
    if (posicao < 1 || posicao > lista->tamanho + 1) {
        return 0;
    }

    if (posicao == 1) {
        inserirInicio(lista, valor);
        return 1;
    }

    if (posicao == lista->tamanho + 1) {
        inserirFim(lista, valor);
        return 1;
    }

    No *atual = lista->inicio;

    for (int i = 1; i < posicao; i++) {
        atual = atual->proximo;
    }

    No *novo = criarNo(valor);

    novo->anterior = atual->anterior;
    novo->proximo = atual;

    atual->anterior->proximo = novo;
    atual->anterior = novo;

    lista->tamanho++;

    return 1;
}

int remover(Lista *lista, int posicao) {
    if (lista->inicio == NULL || posicao < 1 || posicao > lista->tamanho) {
        return 0;
    }

    No *atual = lista->inicio;

    for (int i = 1; i < posicao; i++) {
        atual = atual->proximo;
    }

    if (atual->anterior != NULL) {
        atual->anterior->proximo = atual->proximo;
    } else {
        lista->inicio = atual->proximo;
    }

    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    } else {
        lista->fim = atual->anterior;
    }

    free(atual);
    lista->tamanho--;

    return 1;
}

int buscar(Lista *lista, int valor) {
    int posicao = 1;
    No *atual = lista->inicio;

    while (atual != NULL) {
        if (atual->valor == valor) {
            return posicao;
        }

        atual = atual->proximo;
        posicao++;
    }

    return -1;
}

void listar(Lista *lista) {
    if (lista->inicio == NULL) {
        printf("Lista vazia\n");
        return;
    }

    int posicao = 1;
    No *atual = lista->inicio;

    while (atual != NULL) {
        printf("Posicao %d | valor = %d | anterior = ",
               posicao, atual->valor);

        if (atual->anterior != NULL) {
            printf("%p", (void *)atual->anterior);
        } else {
            printf("NULL");
        }

        printf(" | proximo = ");

        if (atual->proximo != NULL) {
            printf("%p", (void *)atual->proximo);
        } else {
            printf("NULL");
        }

        printf("\n");

        atual = atual->proximo;
        posicao++;
    }
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int main(void) {
    Lista lista = {0};

    int opcao, valor, posicao, resultado;

    do {
        printf("\n===== MENU =====\n"
               "1 - Inserir no inicio\n"
               "2 - Inserir em uma posicao\n"
               "3 - Inserir no final\n"
               "4 - Remover\n"
               "5 - Buscar\n"
               "6 - Listar\n"
               "0 - Sair\n"
               "> ");

        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);

                inserirInicio(&lista, valor);
                break;

            case 2:
                printf("Valor: ");
                scanf("%d", &valor);

                printf("Posicao: ");
                scanf("%d", &posicao);

                resultado = inserirPosicao(&lista, valor, posicao);

                if (resultado) {
                    printf("Valor inserido na posicao %d.\n", posicao);
                } else {
                    printf("Posicao invalida.\n");
                }
                break;

            case 3:
                printf("Valor: ");
                scanf("%d", &valor);

                inserirFim(&lista, valor);
                break;

            case 4:
                printf("Posicao: ");
                scanf("%d", &posicao);

                resultado = remover(&lista, posicao);

                if (resultado) {
                    printf("No removido com sucesso.\n");
                } else {
                    printf("Posicao invalida.\n");
                }
                break;

            case 5:
                printf("Valor: ");
                scanf("%d", &valor);

                posicao = buscar(&lista, valor);

                if (posicao != -1) {
                    printf("Valor encontrado na posicao %d.\n", posicao);
                } else {
                    printf("Valor nao encontrado.\n");
                }
                break;

            case 6:
                listar(&lista);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    liberarLista(&lista);

    return 0;
}