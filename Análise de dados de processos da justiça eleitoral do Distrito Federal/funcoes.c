#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

void remover_aspas(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '"') *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

void remover_chaves(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '{' && *src != '}') *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

processo *ler_dados(const char *arquivo, int *quantidade) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        exit(ERRO);
    }

    processo *dados = malloc(MAX_LINHAS * sizeof(processo));
    if (!dados) {
        printf("Erro de memória.\n");
        fclose(fp);
        exit(ERRO);
    }

    char *linha = NULL;
    size_t tam = 0;
    int i = 0;

    getline(&linha, &tam, fp);

    while (getline(&linha, &tam, fp) != -1 && i < MAX_LINHAS) {
        linha[strcspn(linha, "\r\n")] = '\0';

        char *campos[6];
        int campo = 0;
        char *ptr = linha;
        int entre_aspas = 0;
        campos[campo++] = ptr;

        while (*ptr && campo < 6) {
            if (*ptr == '"') {
                entre_aspas = !entre_aspas;
            } else if (*ptr == ',' && !entre_aspas) {
                *ptr = '\0';
                campos[campo++] = ptr + 1;
            }
            ptr++;
        }

        if (campo < 6) continue;

        dados[i].id = atof(campos[0]);
        strncpy(dados[i].numero, campos[1], 24);
        dados[i].numero[24] = '\0';
        strncpy(dados[i].data_ajuizamento, campos[2], 23);
        dados[i].data_ajuizamento[23] = '\0';
        strncpy(dados[i].id_classe, campos[3], 99);
        dados[i].id_classe[99] = '\0';
        strncpy(dados[i].id_assunto, campos[4], 99);
        dados[i].id_assunto[99] = '\0';
        dados[i].ano_eleicao = atoi(campos[5]);

        remover_aspas(dados[i].id_classe);
        remover_aspas(dados[i].id_assunto);

        i++;
    }

    free(linha);
    fclose(fp);
    *quantidade = i;
    return dados;
}

void ordenar_por_id(processo *dados, int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (dados[j].id > dados[j + 1].id) {
                processo tmp = dados[j];
                dados[j] = dados[j + 1];
                dados[j + 1] = tmp;
            }
        }
    }

    FILE *fp = fopen("id_ORDERNADO.csv", "w");
    if (!fp) {
        printf("Erro ao salvar arquivo.\n");
        return;
    }

    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%.0lf,\"%s\",%s,%s,%s,%d\n",
                dados[i].id, dados[i].numero,
                dados[i].data_ajuizamento,
                dados[i].id_classe,
                dados[i].id_assunto,
                dados[i].ano_eleicao);
    }

    fclose(fp);
    printf("Arquivo id_ORDERNADO.csv criado. Processos ordenados por ID.\n");
}

void ordenar_por_data(processo *dados, int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (strcmp(dados[j].data_ajuizamento, dados[j + 1].data_ajuizamento) < 0) {
                processo tmp = dados[j];
                dados[j] = dados[j + 1];
                dados[j + 1] = tmp;
            }
        }
    }

    FILE *fp = fopen("data_ajuizamento_ORDERNADO.csv", "w");
    if (!fp) {
        printf("Erro ao salvar arquivo.\n");
        return;
    }

    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%.0lf,\"%s\",%s,%s,%s,%d\n", dados[i].id, dados[i].numero,
                dados[i].data_ajuizamento, dados[i].id_classe,
                dados[i].id_assunto, dados[i].ano_eleicao);
    }

    fclose(fp);
    printf("Arquivo data_ajuizamento_ORDERNADO.csv criado. Processos ordenados por data.\n");
}

void dias_em_tramitacao(processo *dados, int qtd) {
    double id;
    printf("Digite o ID do processo: ");
    scanf("%lf", &id);

    for (int i = 0; i < qtd; i++) {
        if (dados[i].id == id) {
            struct tm dt = {0};
            sscanf(dados[i].data_ajuizamento, "%d-%d-%d", &dt.tm_year, &dt.tm_mon, &dt.tm_mday);
            dt.tm_year -= 1900;
            dt.tm_mon--;

            time_t ajuiz = mktime(&dt);
            time_t agora = time(NULL);
            int dias = (int)(difftime(agora, ajuiz) / (60 * 60 * 24));
            printf("Processo ID %.0lf está em tramitação há %d dias.\n", id, dias);
            return;
        }
    }

    printf("ID %.0lf não encontrado.\n", id);
}

void mais_de_um_assunto(processo *dados, int qtd) {
    for (int i = 0; i < qtd; i++) {
        char temp[100];
        strcpy(temp, dados[i].id_assunto);
        remover_chaves(temp);

        int count = 0;
        char *tok = strtok(temp, ",");
        while (tok) {
            count++;
            tok = strtok(NULL, ",");
        }

        if (count > 1) {
            printf("Processo %.0lf tem MAIS de um assunto: %s\n", dados[i].id, dados[i].id_assunto);
        }
    }
}



void contar_assuntos(processo *dados, int qtd) {
    char **lista = NULL;
    size_t total = 0;

    for (int i = 0; i < qtd; i++) {
        char temp[100];
        strcpy(temp, dados[i].id_assunto);
        remover_chaves(temp);

        char *tok = strtok(temp, ",");
        while (tok) {
            int existe = 0;
            for (size_t k = 0; k < total; k++) {
                if (strcmp(lista[k], tok) == 0) {
                    existe = 1;
                    break;
                }
            }

            if (!existe) {
                lista = realloc(lista, (total + 1) * sizeof(char *));
                lista[total++] = strdup(tok);
            }

            tok = strtok(NULL, ",");
        }
    }

    printf("Total de id_assuntos únicos: %zu\n", total);

    for (size_t i = 0; i < total; i++) free(lista[i]);
    free(lista);
}

void processos_por_classe(processo *dados, int qtd) {
    char classe[100];
    printf("Digite o id_classe: ");
    scanf(" %99[^\n]", classe);
    remover_aspas(classe);

    int total = 0;
    for (int i = 0; i < qtd; i++) {
        char copia[100];
        strcpy(copia, dados[i].id_classe);
        remover_chaves(copia);

        char *tok = strtok(copia, ",");
        while (tok) {
            if (strcmp(tok, classe) == 0) {
                total++;
                break;
            }
            tok = strtok(NULL, ",");
        }
    }

    printf("Total de processos com id_classe \"%s\": %d\n", classe, total);
}

void menu(processo *dados, int qtd) {
    int op;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Ordenar por ID\n");
        printf("2. Ordenar por data de ajuizamento\n");
        printf("3. Contar processos por id_classe\n");
        printf("4. Contar id_assuntos únicos\n");
        printf("5. Verificar processos com mais de um assunto\n");
        printf("6. Dias de tramitação por ID\n");
        printf("7. Sair\n");
        printf("Opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1: ordenar_por_id(dados, qtd); break;
            case 2: ordenar_por_data(dados, qtd); break;
            case 3: processos_por_classe(dados, qtd); break;
            case 4: contar_assuntos(dados, qtd); break;
            case 5: mais_de_um_assunto(dados, qtd); break;
            case 6: dias_em_tramitacao(dados, qtd); break;
            case 7: return;
            default: printf("Opção inválida.\n");
        }
    }
}
