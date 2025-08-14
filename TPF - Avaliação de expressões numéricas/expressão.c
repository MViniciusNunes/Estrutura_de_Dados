#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 512

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Converte graus para radianos
static double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

void toLowerStr(char *str); // <-- Adicione esta linha aqui

//==================================================================
// ESTRUTURAS DE PILHA
//==================================================================

// --- Pilha de Strings (Para conversões de notação) ---
typedef struct {
    char itens[MAX][MAX];
    int topo;
} PilhaStr;

void initPilhaStr(PilhaStr *p) { p->topo = -1; }
int isEmptyStr(PilhaStr *p) { return p->topo == -1; }
void pushStr(PilhaStr *p, char *val) { if (p->topo < MAX - 1) strcpy(p->itens[++(p->topo)], val); }
char* popStr(PilhaStr *p) { return !isEmptyStr(p) ? p->itens[(p->topo)--] : NULL; }
char* peekStr(PilhaStr *p) { return !isEmptyStr(p) ? p->itens[p->topo] : NULL; }

// --- Pilha de Floats (Para cálculo de valores) ---
typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

void initPilhaFloat(PilhaFloat *p) { p->topo = -1; }
int isEmptyFloat(PilhaFloat *p) { return p->topo == -1; }
void pushFloat(PilhaFloat *p, float val) { if (p->topo < MAX - 1) p->itens[++(p->topo)] = val; }
float popFloat(PilhaFloat *p) { return !isEmptyFloat(p) ? p->itens[(p->topo)--] : 0.0; }

// Estrutura auxiliar para armazenar expressão e prioridade
typedef struct {
    char expr[MAX];
    int prioridade;
} ExprComPrioridade;

// Nova pilha para ExprComPrioridade
typedef struct {
    ExprComPrioridade itens[MAX];
    int topo;
} PilhaExpr;

void initPilhaExpr(PilhaExpr *p) { p->topo = -1; }
int isEmptyExpr(PilhaExpr *p) { return p->topo == -1; }
void pushExpr(PilhaExpr *p, ExprComPrioridade val) { if (p->topo < MAX - 1) p->itens[++(p->topo)] = val; }
ExprComPrioridade popExpr(PilhaExpr *p) { return p->itens[(p->topo)--]; }

//==================================================================
// FUNÇÕES AUXILIARES
//==================================================================

int prioridade(char *op) {
    char opCopia[MAX];
    strcpy(opCopia, op);
    toLowerStr(opCopia);

    if (strcmp(opCopia, "sen") == 0 || strcmp(opCopia, "cos") == 0 || strcmp(opCopia, "tg") == 0 ||
        strcmp(opCopia, "log") == 0 || strcmp(opCopia, "raiz") == 0) return 4;
    if (strcmp(opCopia, "^") == 0) return 3;
    if (strcmp(opCopia, "*") == 0 || strcmp(opCopia, "/") == 0 || strcmp(opCopia, "%") == 0) return 2;
    if (strcmp(opCopia, "+") == 0 || strcmp(opCopia, "-") == 0) return 1;
    return 0; // Para parênteses
}

int ehOperador(char *token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
           strcmp(token, "%") == 0 || strcmp(token, "^") == 0 ||
           strcmp(token, "log") == 0 || strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
           strcmp(token, "raiz") == 0;
}

void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

//==================================================================
// FUNÇÕES PRINCIPAIS COM VALIDAÇÃO COMPLETA
//==================================================================

/**
 * Converte Infixa para Pós-fixa, com validação de erros.
 */
char *getFormaPosFixa(char *Str) {
    static char saida[MAX] = "";
    PilhaStr pilha;
    initPilhaStr(&pilha);
    saida[0] = '\0';

    char token[MAX];
    int i = 0;
    while (Str[i] != '\0') {
        if (isspace(Str[i])) {
            i++;
            continue;
        }

        if (isdigit(Str[i]) || Str[i] == '.') {
            int j = 0;
            while (isdigit(Str[i]) || Str[i] == '.') {
                token[j++] = Str[i++];
            }
            token[j] = '\0';
            strcat(saida, token);
            strcat(saida, " ");
        } else if (isalpha(Str[i])) {
            int j = 0;
            while (isalpha(Str[i])) {
                token[j++] = Str[i++];
            }
            token[j] = '\0';
            char tokenCopia[MAX];
            strcpy(tokenCopia, token);
            toLowerStr(tokenCopia);
            if(strcmp(tokenCopia, "sen") != 0 && strcmp(tokenCopia, "cos") != 0 && strcmp(tokenCopia, "tg") != 0 && strcmp(tokenCopia, "log") != 0 && strcmp(tokenCopia, "raiz") != 0) {
                printf("ERRO: Funcao desconhecida: '%s'\n", token);
                return NULL;
            }
            pushStr(&pilha, tokenCopia);
        } else if (Str[i] == '(') {
            token[0] = '('; token[1] = '\0';
            pushStr(&pilha, token);
            i++;
        } else if (Str[i] == ')') {
            while (!isEmptyStr(&pilha) && strcmp(peekStr(&pilha), "(") != 0) {
                strcat(saida, popStr(&pilha));
                strcat(saida, " ");
            }
            if (isEmptyStr(&pilha)) {
                printf("ERRO: Parentese de fechamento ')' sem um par de abertura correspondente.\n");
                return NULL;
            }
            popStr(&pilha); // remove '('
            i++;
        } else { // Operador
            token[0] = Str[i]; token[1] = '\0';
            if (!ehOperador(token)) {
                printf("ERRO: Operador ou caractere invalido: '%s'\n", token);
                return NULL;
            }
            while (!isEmptyStr(&pilha) && strcmp(peekStr(&pilha), "(") != 0 && prioridade(peekStr(&pilha)) >= prioridade(token)) {
                strcat(saida, popStr(&pilha));
                strcat(saida, " ");
            }
            pushStr(&pilha, token);
            i++;
        }
    }

    while (!isEmptyStr(&pilha)) {
        char *op = popStr(&pilha);
        if (strcmp(op, "(") == 0) {
            printf("ERRO: Parentese de abertura '(' sem um par de fechamento correspondente.\n");
            return NULL;
        }
        strcat(saida, op);
        strcat(saida, " ");
    }

    int len = strlen(saida);
    if (len > 0 && saida[len - 1] == ' ')
        saida[len - 1] = '\0';

    return saida;
}

/**
 * Converte Pós-fixa para Infixa, com validação completa de erros.
 */
char *getFormaInFixa(char *StrPosFixa) {
    static char inFixaResult[MAX];
    PilhaExpr pilha;
    initPilhaExpr(&pilha);

    char strCpy[MAX];
    strcpy(strCpy, StrPosFixa);
    char *token = strtok(strCpy, " ");

    while (token != NULL) {
        ExprComPrioridade novo;
        char tokenCopia[MAX];
        strcpy(tokenCopia, token);
        toLowerStr(tokenCopia);

        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            snprintf(novo.expr, MAX, "%s", token);
            novo.prioridade = 10; // prioridade máxima para operandos
            pushExpr(&pilha, novo);
        } else if (ehOperador(tokenCopia)) {
            int prio = prioridade(tokenCopia);
            if (strcmp(tokenCopia, "sen") == 0 || strcmp(tokenCopia, "cos") == 0 || strcmp(tokenCopia, "tg") == 0 ||
                strcmp(tokenCopia, "log") == 0 || strcmp(tokenCopia, "raiz") == 0) {
                if (isEmptyExpr(&pilha)) {
                    printf("ERRO: Expressao pos-fixa malformada. Operador '%s' sem operandos.\n", token);
                    return NULL;
                }
                ExprComPrioridade op = popExpr(&pilha);
                snprintf(novo.expr, MAX, "%s(%s)", tokenCopia, op.expr);
                novo.prioridade = 4; // prioridade das funções
                pushExpr(&pilha, novo);
            } else {
                if (pilha.topo < 1) {
                    printf("ERRO: Expressao pos-fixa malformada. Operador '%s' sem operandos suficientes.\n", token);
                    return NULL;
                }
                ExprComPrioridade op2 = popExpr(&pilha);
                ExprComPrioridade op1 = popExpr(&pilha);

                // Adiciona parênteses se a prioridade do operando for menor que a do operador atual
                char esq[MAX], dir[MAX];
                if (op1.prioridade < prio)
                    snprintf(esq, MAX, "(%s)", op1.expr);
                else
                    snprintf(esq, MAX, "%s", op1.expr);

                if (op2.prioridade < prio || (prio == 3 && op2.prioridade == prio)) // para associatividade à direita do ^
                    snprintf(dir, MAX, "(%s)", op2.expr);
                else
                    snprintf(dir, MAX, "%s", op2.expr);

                snprintf(novo.expr, MAX, "%s %s %s", esq, tokenCopia, dir);
                novo.prioridade = prio;
                pushExpr(&pilha, novo);
            }
        } else {
            printf("ERRO: Token invalido na expressao pos-fixa: '%s'\n", token);
            return NULL;
        }
        token = strtok(NULL, " ");
    }

    if (pilha.topo != 0) {
        printf("ERRO: Expressao pos-fixa malformada. Sobraram operandos.\n");
        return NULL;
    }
    strcpy(inFixaResult, pilha.itens[0].expr);
    return inFixaResult;
}

/**
 * Calcula o valor de uma expressão Pós-fixa, com tratamento de erros.
 */
float getValorPosFixa(char *StrPosFixa) {
    PilhaFloat pilha;
    initPilhaFloat(&pilha);

    char strCpy[MAX];
    strcpy(strCpy, StrPosFixa);
    char *token = strtok(strCpy, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushFloat(&pilha, atof(token));
        } else { // Se não for número, é um operador ou função
            char tokenLower[MAX];
            strcpy(tokenLower, token);
            toLowerStr(tokenLower);

            // Funções unárias
            if (strcmp(tokenLower, "sen") == 0 || strcmp(tokenLower, "cos") == 0 || strcmp(tokenLower, "tg") == 0 ||
                strcmp(tokenLower, "log") == 0 || strcmp(tokenLower, "raiz") == 0) {

                if (isEmptyFloat(&pilha)) {
                    printf("ERRO: Expressao malformada. Operador '%s' sem operandos suficientes.\n", token);
                    return NAN;
                }
                float operando = popFloat(&pilha);

                if (strcmp(tokenLower, "sen") == 0) {
                    pushFloat(&pilha, sin(operando * M_PI / 180.0)); // Converte para radianos
                } else if (strcmp(tokenLower, "cos") == 0) {
                    pushFloat(&pilha, cos(operando * M_PI / 180.0));
                } else if (strcmp(tokenLower, "tg") == 0) {
                    pushFloat(&pilha, tan(operando * M_PI / 180.0));
                } else if (strcmp(tokenLower, "log") == 0) {
                    pushFloat(&pilha, log10(operando));
                } else if (strcmp(tokenLower, "raiz") == 0) {
                    pushFloat(&pilha, sqrt(operando));
                }
            } else { // Operadores binários
                if (pilha.topo < 1) {
                    printf("ERRO: Expressao malformada. Operador '%s' sem operandos suficientes.\n", token);
                    return NAN;
                }
                float op2 = popFloat(&pilha);
                float op1 = popFloat(&pilha);

                if (strcmp(tokenLower, "+") == 0) pushFloat(&pilha, op1 + op2);
                else if (strcmp(tokenLower, "-") == 0) pushFloat(&pilha, op1 - op2);
                else if (strcmp(tokenLower, "*") == 0) pushFloat(&pilha, op1 * op2);
                else if (strcmp(tokenLower, "/") == 0) {
                    if (op2 == 0) {
                        printf("ERRO: Divisao por zero.\n");
                        return NAN;
                    }
                    pushFloat(&pilha, op1 / op2);
                }
                else if (strcmp(tokenLower, "%") == 0) {
                    if ((int)op2 == 0) {
                        printf("ERRO: Modulo por zero.\n");
                        return NAN;
                    }
                    pushFloat(&pilha, fmod(op1, op2));
                }
                else if (strcmp(tokenLower, "^") == 0) pushFloat(&pilha, pow(op1, op2));
                else {
                    printf("ERRO: Funcao ou operador desconhecido: '%s'\n", token);
                    return NAN;
                }
            }
        }
        token = strtok(NULL, " ");
    }
    return popFloat(&pilha);
}

/**
 * Calcula o valor de uma expressão Infixa, com tratamento de erros.
 */
float getValorInFixa(char *StrInFixa) {
    char *posFixa = getFormaPosFixa(StrInFixa);
    // Se a conversão falhar (retornar NULL), propaga o erro como NAN
    if (posFixa == NULL) {
        return NAN;
    }
    return getValorPosFixa(posFixa);
}