# Atividade 2 — Estrada de Wakanda

📚 **Disciplina:** Estrutura de Dados — UCB  
👨‍🏫 **Professor:** Marcelo Eustáquio  
📅 **Semestre:** 1º/2025  

## 🎯 Objetivo
Criar funções em **C** para determinar a **menor vizinhança** de estrada entre cidades em uma rodovia que cruza Wakanda, bem como identificar a cidade que possui essa menor vizinhança.

## 📝 Funcionalidades
- Ler dados de um arquivo de entrada contendo:
  - Comprimento total da estrada
  - Número de cidades
  - Posição e nome de cada cidade
- Calcular a menor vizinhança
- Identificar a cidade com menor vizinhança
- Retornar dados em um TAD `Estrada`

## 📂 Estrutura de Arquivos
- `cidades.c` → Implementação das funções definidas em `cidades.h`
- `cidades.h` → Arquivo de cabeçalho (não pode ser modificado)
- `main.c` → Programa exemplo para testar as funções

## ⚙️ Compilação e Execução
```bash
gcc main.c cidades.c -o cidades.exe
./cidades.exe

