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

Cada programa tem uma versão com erro sintático e outra corrigida. Assim, é
possível conferir a mensagem de erro e depois confirmar que o programa correto
é aceito pelo compilador.

| Programa | Erro usado no teste | Mensagem | Correção |
| --- | --- | --- | --- |
| [Teste 1](../tests/fixtures/parser/invalid/test1.dex) | `write "Area: "` sem parênteses | `5:7 syntax error: expected '(', found "\"Area: \""` | Usar `write("Area: ")`. [Versão corrigida](../tests/fixtures/parser/valid/test1.dex). |
| [Teste 2](../tests/fixtures/parser/invalid/test2.dex) | Atribuição escrita como `val := 34` | `4:5 syntax error: expected '=', found ":"` | Usar `val = 34`. [Versão corrigida](../tests/fixtures/parser/valid/test2.dex). |
| [Teste 3](../tests/fixtures/parser/invalid/test3.dex) | Falta o `end` do `if` | `9:4 syntax error: expected 'end', found "."` | Fechar o `if` antes do `end.` do programa. [Versão corrigida](../tests/fixtures/parser/valid/test3.dex). |
| [Teste 4](../tests/fixtures/parser/invalid/test4.dex) | Parêntese aberto sem fechamento no `while` | `4:58 syntax error: expected ')', found "do"` | Fechar a condição antes de `do`. [Versão corrigida](../tests/fixtures/parser/valid/test4.dex). |
| [Teste 5](../tests/fixtures/parser/invalid/test5.dex) | Falta `then` depois da condição | `5:1 syntax error: expected 'then', found "maior"` | Adicionar `then` antes do corpo. [Versão corrigida](../tests/fixtures/parser/valid/test5.dex). |

Depois da correção, os cinco programas produzem:

```text
Syntax analysis completed successfully.
```

## Programas extras

Também foram adicionados dois programas válidos:

- [contador](../tests/fixtures/parser/valid/extra1.dex), com atribuição,
  `while`, leitura e escrita;
- [classificação](../tests/fixtures/parser/valid/extra2.dex), com `if`, `else`,
  `repeat`, expressões e constantes de ponto flutuante.

Os dois produzem a mensagem de sucesso da análise sintática.
