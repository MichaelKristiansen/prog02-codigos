#include <stdio.h>
#include <stdlib.h>

typedef struct No
{
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int altura(No *no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

int fatorBalanceamento(No *no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

void atualizarAltura(No *no) {
    no->altura = 1 + maior(altura(no->esquerda), altura(no->direita));
}

No *criarNo(int valor) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        exit(EXIT_FAILURE);
    }

    novo->valor = valor;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

No *rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

No *rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

No *rotacaoEsquerdaDireita(No *raiz) {
    raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
    return rotacaoDireita(raiz);
}

No *rotacaoDireitaEsquerda(No *raiz) {
    raiz->direita = rotacaoDireita(raiz->direita);
    return rotacaoEsquerda(raiz);
}

No *balancear(No *raiz) {
    atualizarAltura(raiz);
    int fb = fatorBalanceamento(raiz);

    if (fb > 1) {
        if (fatorBalanceamento(raiz->esquerda) >= 0)
            return rotacaoDireita(raiz);
        return rotacaoEsquerdaDireita(raiz);
    }

    if (fb < -1) {
        if (fatorBalanceamento(raiz->direita) <= 0)
            return rotacaoEsquerda(raiz);
        return rotacaoDireitaEsquerda(raiz);
    }

    return raiz;
}

No *inserir(No *raiz, int valor) {
    if (raiz == NULL)
        return criarNo(valor);

    if (valor < raiz->valor)
        raiz->esquerda = inserir(raiz->esquerda, valor);
    else if (valor > raiz->valor)
        raiz->direita = inserir(raiz->direita, valor);
    else
        return raiz;

    return balancear(raiz);
}

No *menorNo(No *raiz) {
    while (raiz->esquerda != NULL)
        raiz = raiz->esquerda;
    return raiz;
}

No *remover(No *raiz, int valor) {
    if (raiz == NULL)
        return NULL;

    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    } else {
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            No *filho = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(raiz);
            return filho;
        }
        No *sucessor = menorNo(raiz->direita);
        raiz->valor = sucessor->valor;
        raiz->direita = remover(raiz->direita, sucessor->valor);
    }

    return balancear(raiz);
}

No *buscar(No *raiz, int valor) {
    while (raiz != NULL) {
        if (valor == raiz->valor)
            return raiz;
        raiz = (valor < raiz->valor) ? raiz->esquerda : raiz->direita;
    }
    return NULL;
}

void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        emOrdem(raiz->direita);
    }
}

void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

void imprimirArvore(No *raiz, int nivel) {
    if (raiz == NULL)
        return;

    imprimirArvore(raiz->direita, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("    ");

    printf("%d (altura=%d, fb=%d)\n", raiz->valor, raiz->altura,
           fatorBalanceamento(raiz));

    imprimirArvore(raiz->esquerda, nivel + 1);
}

void exibirAlturaEFatores(No *raiz) {
    if (raiz == NULL) {
        printf("Arvore vazia\n");
        return;
    }
    printf("Altura da arvore: %d\n", altura(raiz));
    imprimirArvore(raiz, 0);
}

void liberarArvore(No *raiz) {
    if (raiz == NULL)
        return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int lerInteiro(const char *texto, int *destino) {
    printf("%s", texto);
    return scanf("%d", destino) == 1;
}

void percorrerArvore(No *raiz) {
    int opcao;

    printf("\n--- Percorrer arvore ---\n");
    printf("1 - Pre-ordem\n2 - Em ordem\n3 - Pos-ordem\n");
    if (!lerInteiro("Opcao: ", &opcao))
        return;

    switch (opcao) {
        case 1:
            preOrdem(raiz);
            printf("\n");
            break;
        case 2:
            emOrdem(raiz);
            printf("\n");
            break;
        case 3:
            posOrdem(raiz);
            printf("\n");
            break;
        default:
            printf("Opcao invalida\n");
    }
}

int main() {
    No *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n--- Arvore AVL ---\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("5 - Exibir altura e fator de balanceamento\n");
        printf("0 - Sair\n");
        if (!lerInteiro("Opcao: ", &opcao))
            break;

        switch (opcao) {
            case 1:
                if (!lerInteiro("Valor: ", &valor)) {
                    opcao = 0;
                    break;
                }
                if (buscar(raiz, valor))
                    printf("Valor ja existe na arvore; insercao ignorada\n");
                else
                    raiz = inserir(raiz, valor);
                break;
            case 2:
                if (!lerInteiro("Valor: ", &valor)) {
                    opcao = 0;
                    break;
                }
                printf("%s\n", buscar(raiz, valor) ? "Encontrado" : "Nao encontrado");
                break;
            case 3:
                if (!lerInteiro("Valor: ", &valor)) {
                    opcao = 0;
                    break;
                }
                if (buscar(raiz, valor))
                    raiz = remover(raiz, valor);
                else
                    printf("Valor nao encontrado\n");
                break;
            case 4:
                percorrerArvore(raiz);
                break;
            case 5:
                exibirAlturaEFatores(raiz);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    liberarArvore(raiz);
    return 0;
}