# Atividade 3 — Avaliador de Expressões Numéricas

📚 **Disciplina:** Estrutura de Dados — UCB  
👨‍🏫 **Professor:** Marcelo Eustáquio  
📅 **Semestre:** 1º/2025  

## 🎯 Objetivo
Implementar, em C, um programa capaz de **avaliar expressões matemáticas** nas notações **infixa** e **pós-fixa**, com suporte a operações básicas e funções matemáticas especiais, utilizando **pilhas** como estrutura fundamental.

## 📝 Funcionalidades
- Converter **infixa → pós-fixa**
- Converter **pós-fixa → infixa**
- Avaliar expressões com operações:
  - Binárias: `+`, `-`, `*`, `/`, `%`, `^`
  - Unárias: `raiz`, `sen`, `cos`, `tg`, `log` (graus para trigonométricas)
- Calcular valores conforme tabela de testes fornecida
- Verificar inconsistências nas entradas

## 📂 Estrutura de Arquivos
- `expressao.c` → Implementação das funções
- `expressao.h` → Cabeçalho com definição da struct `Expressao`
- `main.c` → Função principal para interação e testes

## ⚙️ Compilação e Execução
```bash
gcc expressao.c main.c -o expressao.exe
./expressao.exe
