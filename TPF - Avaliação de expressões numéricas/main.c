#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

int main() {
    int opcao = -1; // Inicializa com um valor inválido
    char entrada[512]; // Buffer temporário para a entrada do usuário
    Expressao minhaExpressao; // Declara uma variável do tipo Expressao

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1 - Traduzir expressao infixada -> pos-fixada\n");
        printf("2 - Traduzir expressao pos-fixada -> infixada\n");
        printf("3 - Avaliar expressao infixada\n");
        printf("4 - Avaliar expressao pos-fixada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        // --- LÓGICA DE VALIDAÇÃO DE ENTRADA DO MENU ---
        char buffer[512];
        char char_extra;
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d %c", &opcao, &char_extra) != 1) {
            opcao = -1; // Define como inválida se houver lixo ou se não for um número.
        }
        //------------------------------------------------

        switch (opcao) {
            case 1:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                // Substitui vírgula por ponto na entrada
                for (int i = 0; entrada[i]; i++) {
                    if (entrada[i] == ',') entrada[i] = '.';
                }
                
                // Copia a entrada infixa para a struct
                strcpy(minhaExpressao.inFixa, entrada);

                // Chama a função para obter a forma pós-fixa
                char *posFixaResult = getFormaPosFixa(minhaExpressao.inFixa);
                
                if (posFixaResult == NULL) {
                    printf(">> ERRO: Nao foi possivel traduzir a expressao.\n");
                } else {
                    strcpy(minhaExpressao.posFixa, posFixaResult); // Guarda na struct
                    printf("Expressao pos-fixada: %s\n", minhaExpressao.posFixa);
                }
                break;

            case 2:
                printf("Digite a expressao pos-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';

                // Copia a entrada pós-fixa para a struct
                strcpy(minhaExpressao.posFixa, entrada);

                // Chama a função para obter a forma infixa
                char *inFixaResult = getFormaInFixa(minhaExpressao.posFixa);
                if (inFixaResult == NULL) {
                    printf(">> ERRO: Nao foi possivel traduzir a expressao.\n");
                } else {
                    strcpy(minhaExpressao.inFixa, inFixaResult); // Guarda na struct
                    printf("Expressao infixada: %s\n", minhaExpressao.inFixa);
                }
                break;

            case 3:
                printf("Digite a expressao infixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';
                
                // Substitui vírgula por ponto na entrada
                for (int i = 0; entrada[i]; i++) {
                    if (entrada[i] == ',') entrada[i] = '.';
                }

                // Copia a entrada infixa para a struct
                strcpy(minhaExpressao.inFixa, entrada);

                // Chama a função para calcular o valor da expressão infixa
                minhaExpressao.Valor = getValorInFixa(minhaExpressao.inFixa); // Guarda na struct
                
                if (isnan(minhaExpressao.Valor) || isinf(minhaExpressao.Valor)) {
                    printf(">> ERRO: Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    if (fabs(minhaExpressao.Valor - (int)minhaExpressao.Valor) < 0.00001) {
                        printf("Resultado: %d\n", (int)minhaExpressao.Valor);
                    } else {
                        printf("Resultado: %.2f\n", minhaExpressao.Valor);
                    }
                }
                break;

            case 4:
                printf("Digite a expressao pos-fixada:\n> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = '\0';

                // Copia a entrada pós-fixa para a struct
                strcpy(minhaExpressao.posFixa, entrada);

                // Chama a função para calcular o valor da expressão pós-fixa
                minhaExpressao.Valor = getValorPosFixa(minhaExpressao.posFixa); // Guarda na struct
                
                if (isnan(minhaExpressao.Valor) || isinf(minhaExpressao.Valor)) {
                    printf(">> ERRO: Nao foi possivel calcular o valor da expressao.\n");
                } else {
                    if (fabs(minhaExpressao.Valor - (int)minhaExpressao.Valor) < 0.00001) {
                        printf("Resultado: %d\n", (int)minhaExpressao.Valor);
                    } else {
                        printf("Resultado: %.2f\n", minhaExpressao.Valor);
                    }
                }
                break;

            case 0:
                printf("Encerrando o programa.\n");
                return 0;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}