# Analisador sintático

O analisador sintático verifica se os tokens formam um programa válido de
acordo com a [gramática do projeto](syntax-grammar.md). Ele foi escrito à mão
usando descida recursiva e não cria uma árvore sintática, pois a validação da
estrutura é suficiente para este compilador.

## Funcionamento

O arquivo informado no terminal passa primeiro pelo analisador léxico. Se
houver erro léxico, a análise termina e esses erros são mostrados. Quando não
há erro léxico, os tokens são enviados ao parser.

O parser para no primeiro erro sintático. A mensagem contém:

- linha e coluna do erro;
- item esperado;
- token encontrado.

Programas válidos exibem apenas uma mensagem de sucesso. Tokens e tabela de
símbolos não são mostrados na saída.

## Principais classes

| Classe | Função |
| --- | --- |
| `Lexer` | Lê o texto e produz os tokens. |
| `LexService` | Reúne tokens e erros léxicos e preenche a tabela de símbolos. |
| `Parser` | Confere declarações, comandos e expressões. |
| `SyntaxError` | Guarda o token inesperado e o item esperado. |
| `ConsoleReporter` | Mostra erros e a mensagem de sucesso no terminal. |

## Ajustes na gramática

Algumas regras foram ajustadas para combinar com os programas fornecidos e
permitir a implementação por descida recursiva:

- as declarações ficam antes de `begin`;
- `:` separa os identificadores do tipo;
- a recursão à esquerda foi removida de `simple-expr` e `term`;
- `factor-a` é o nome usado em todas as regras;
- `while` e `repeat` foram escritos sem regras auxiliares;
- um `;` é aceito antes de `end`, `else` e `until`.

A forma completa dessas regras está em
[docs/syntax-grammar.md](syntax-grammar.md).

## Uso

```bash
./build/darthexc tests/fixtures/parser/valid/test1.dex
```

Saída:

```text
Syntax analysis completed successfully.
```

## Programas de teste

Foram usados cinco programas para testar o analisador sintático. Os erros
léxicos desses arquivos já foram corrigidos.

Cada programa tem versões sucessivas, com uma correção por vez. Assim, é
possível conferir cada mensagem até o programa ser aceito pelo compilador.

A sequência completa mostra o código analisado, a mensagem recebida e o código
usado depois de cada correção:

- [resultados dos cinco programas](syntax-test-results.md).

## Programas extras

Também foram adicionados dois programas válidos:

- [contador](../tests/fixtures/parser/valid/extra1.dex), com atribuição,
  `while`, leitura e escrita;
- [classificação](../tests/fixtures/parser/valid/extra2.dex), com `if`, `else`,
  `repeat`, expressões e constantes de ponto flutuante.

Os dois produzem a mensagem de sucesso da análise sintática.
