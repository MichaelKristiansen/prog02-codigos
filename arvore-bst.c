#include <stdio.h>
#include <stdlib.h>

struct no {
    int valor;
    struct no* esquerda;
    struct no* direita;
};

typedef struct no No;

No* criarNo(int valor) {
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\n[ERRO]: erro ao alocar memória\n\n");
        exit(1);
    }
    novoNo->valor = valor;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

No* inserirValor(No* raiz, int valor, int* inserido) {
    if (raiz == NULL) {
        *inserido = 1;
        return criarNo(valor);
    }
    if (valor < raiz->valor) {
        raiz->esquerda = inserirValor(raiz->esquerda, valor, inserido);
    } else if (valor > raiz->valor) {
        raiz->direita = inserirValor(raiz->direita, valor, inserido);
    } else {
        *inserido = 0; /* valor repetido: ignorado */
    }
    return raiz;
}

int buscarValor(No* raiz, int valor) {
    if (raiz == NULL) return 0;
    if (valor == raiz->valor) return 1;
    if (valor < raiz->valor) return buscarValor(raiz->esquerda, valor);
    return buscarValor(raiz->direita, valor);
}

No* encontrarMinimo(No* raiz) {
    while (raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

No* removerValor(No* raiz, int valor, int* removido) {
    if (raiz == NULL) {
        *removido = 0;
        return NULL;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = removerValor(raiz->esquerda, valor, removido);
    } else if (valor > raiz->valor) {
        raiz->direita = removerValor(raiz->direita, valor, removido);
    } else {
        *removido = 1;

        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        } else if (raiz->esquerda == NULL) {
            No* filhoDireito = raiz->direita;
            free(raiz);
            return filhoDireito;
        } else if (raiz->direita == NULL) {
            No* filhoEsquerdo = raiz->esquerda;
            free(raiz);
            return filhoEsquerdo;
        } else {
            No* sucessor = encontrarMinimo(raiz->direita);
            raiz->valor = sucessor->valor;
            int auxiliarRemovido;
            raiz->direita = removerValor(raiz->direita, sucessor->valor, &auxiliarRemovido);
        }
    }
    return raiz;
}

void percorrerPreOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        percorrerPreOrdem(raiz->esquerda);
        percorrerPreOrdem(raiz->direita);
    }
}

void percorrerEmOrdem(No* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerEmOrdem(raiz->direita);
    }
}

void percorrerPosOrdem(No* raiz) {
    if (raiz != NULL) {
        percorrerPosOrdem(raiz->esquerda);
        percorrerPosOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

No* liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
    return NULL;
}

void menuPercursos(No* raiz) {
    int opcao;

    if (raiz == NULL) {
        printf("Árvore vazia!\n");
        return;
    }

    printf("\n\t1 - Pré-ordem\n\t2 - Em ordem\n\t3 - Pós-ordem\nOpção: ");
    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n');
        printf("\n[ERRO]: entrada inválida!\n");
        return;
    }

    switch (opcao) {
        case 1:
            printf("Pré-ordem: ");
            percorrerPreOrdem(raiz);
            printf("\n");
            break;
        case 2:
            printf("Em ordem: ");
            percorrerEmOrdem(raiz);
            printf("\n");
            break;
        case 3:
            printf("Pós-ordem: ");
            percorrerPosOrdem(raiz);
            printf("\n");
            break;
        default:
            printf("\n[ERRO]: opção inválida!\n");
            break;
    }
}

int main() {
    No* raiz = NULL;
    int opcao, valor, flagOperacao;

    do {
        printf("\n\t1 - Inserir valor\n\t2 - Buscar valor\n\t3 - Remover valor\n\t4 - Percorrer árvore\n\t0 - Sair\nOpção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                printf("Digite o valor a inserir: ");
                if (scanf("%d", &valor) != 1) {
                    while (getchar() != '\n');
                    printf("\n[ERRO]: entrada inválida!\n");
                    break;
                }
                raiz = inserirValor(raiz, valor, &flagOperacao);
                if (flagOperacao) {
                    printf("Valor %d inserido com sucesso!\n", valor);
                } else {
                    printf("Valor %d já existe na árvore (repetidos são ignorados).\n", valor);
                }
                break;
            case 2:
                printf("Digite o valor a buscar: ");
                if (scanf("%d", &valor) != 1) {
                    while (getchar() != '\n');
                    printf("\n[ERRO]: entrada inválida!\n");
                    break;
                }
                if (buscarValor(raiz, valor)) {
                    printf("O valor %d ESTÁ na árvore.\n", valor);
                } else {
                    printf("O valor %d NÃO está na árvore.\n", valor);
                }
                break;
            case 3:
                printf("Digite o valor a remover: ");
                if (scanf("%d", &valor) != 1) {
                    while (getchar() != '\n');
                    printf("\n[ERRO]: entrada inválida!\n");
                    break;
                }
                raiz = removerValor(raiz, valor, &flagOperacao);
                if (flagOperacao) {
                    printf("Valor %d removido com sucesso!\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
            case 4:
                menuPercursos(raiz);
                break;
            case 0:
                raiz = liberarArvore(raiz);
                printf("\n[ Memória liberada. Saindo... ]\n");
                break;
            default:
                printf("\n[ERRO]: operação inválida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
