#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_LINHAS 19000
#define ERRO -1

typedef struct {
    double id;
    char numero[25];
    char data_ajuizamento[24];
    char id_classe[100];
    char id_assunto[100];
    int ano_eleicao;
} processo;

void remover_aspas(char *str);
void remover_chaves(char *str);
processo *ler_dados(const char *arquivo, int *quantidade);
void ordenar_por_id(processo *dados, int qtd);
void ordenar_por_data(processo *dados, int qtd);
void dias_em_tramitacao(processo *dados, int qtd);
void mais_de_um_assunto(processo *dados, int qtd);
void contar_assuntos(processo *dados, int qtd);
void processos_por_classe(processo *dados, int qtd);
void menu(processo *dados, int qtd);

#endif
