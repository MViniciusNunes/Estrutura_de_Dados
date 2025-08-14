#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>
#include "cidades.h"

// Função auxiliar para comparar cidades por posição
int compararCidades(const void *a, const void *b) {
    const Cidade *c1 = (const Cidade *)a;
    const Cidade *c2 = (const Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

// Função para limpar espaços em branco no início e fim de uma string
void limparEspacos(char *str) {
    // Remove espaços do início
    char *inicio = str;
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }

    if (inicio != str) {
        memmove(str, inicio, strlen(inicio) + 1);
    }

    // Remove espaços do fim
    char *fim = str + strlen(str) - 1;
    while (fim > str && isspace((unsigned char)*fim)) {
        *fim-- = '\0';
    }
}

// Função para inicializar a estrutura Estrada a partir de um arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        fclose(arquivo);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->T) != 1 || estrada->T < 3 || estrada->T > 1000000) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->N) != 1 || estrada->N < 2 || estrada->N > 10000) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(sizeof(Cidade) * estrada->N);
    if (!estrada->C) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    int *posicoes = (int *)malloc(sizeof(int) * estrada->N);
    if (!posicoes) {
        fclose(arquivo);
        free(estrada->C);
        free(estrada);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(arquivo, "%d ", &estrada->C[i].Posicao) != 1 ||
            fgets(estrada->C[i].Nome, sizeof(estrada->C[i].Nome), arquivo) == NULL) {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        // Remove '\n' e '\r'
        estrada->C[i].Nome[strcspn(estrada->C[i].Nome, "\r\n")] = '\0';

        // Substitui '_' por espaço
        for (char *p = estrada->C[i].Nome; *p; p++) {
            if (*p == '_') *p = ' ';
        }

        // Limpa espaços extras
        limparEspacos(estrada->C[i].Nome);

        // Valida posição
        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        // Verifica posições duplicadas
        for (int j = 0; j < i; j++) {
            if (estrada->C[i].Posicao == posicoes[j]) {
                free(posicoes);
                free(estrada->C);
                free(estrada);
                fclose(arquivo);
                return NULL;
            }
        }
        posicoes[i] = estrada->C[i].Posicao;
    }

    free(posicoes);
    fclose(arquivo);

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);
    return estrada;
}

// Função para calcular a menor vizinhança
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1;

    double menorVizinhanca = DBL_MAX;

    for (int i = 0; i < estrada->N; i++) {
        double inicio, fim;

        if (i == 0) {
            inicio = 0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = estrada->T;
        } else {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double vizinhanca = fim - inicio;
        if (vizinhanca < menorVizinhanca)
            menorVizinhanca = vizinhanca;
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

// Função para retornar o nome da cidade com menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    double menorVizinhanca = DBL_MAX;
    int indice = -1;

    for (int i = 0; i < estrada->N; i++) {
        double inicio, fim;

        if (i == 0) {
            inicio = 0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = estrada->T;
        } else {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double vizinhanca = fim - inicio;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            indice = i;
        }
    }

    char *nomeCidade = NULL;
    if (indice != -1) {
        nomeCidade = strdup(estrada->C[indice].Nome);
        if (nomeCidade) limparEspacos(nomeCidade); // segurança extra
    }

    free(estrada->C);
    free(estrada);
    return nomeCidade;
}
