
# Atividade 1 — Análise de Processos da Justiça Eleitoral (DF)

📚 **Disciplina:** Estrutura de Dados — UCB  
📅 **Entrega:** 11/04/2025  

## 🎯 Objetivo
Implementar, em C, um sistema para **ler, ordenar e analisar** dados de processos judiciais da Justiça Eleitoral do DF a partir de um arquivo CSV.

## 📝 Funcionalidades
- Ordenar por `id` (crescente)
- Ordenar por `data_ajuizamento` (decrescente)
- Contar processos por `id_classe`
- Identificar quantidade de `id_assuntos` distintos
- Listar processos com mais de um assunto
- Calcular dias de tramitação de um processo

## 📂 Estrutura de Arquivos
- `processo.c` → Implementação das funções
- `processo.h` → Definição do TAD `Processo`
- `main.c` → Função principal com menu de opções

## ⚙️ Compilação e Execução
```bash
gcc processo.c main.c -o processos.exe
./processos.exe
