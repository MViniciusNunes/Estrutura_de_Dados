#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    int qtd = 0;
    processo *dados = ler_dados("processo_043_202409032338.csv", &qtd);
    menu(dados, qtd);
    free(dados);
    system("pause");
    return 0;
}
