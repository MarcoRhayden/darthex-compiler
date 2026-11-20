# Gramática do parser

```text
program     ::= "program" identifier [decl-list] "begin" stmt-list "end" "."
decl-list   ::= decl ";" { decl ";" }
decl        ::= ident-list ":" type
ident-list  ::= identifier { "," identifier }
type        ::= "int" | "float" | "char"

stmt-list   ::= stmt { ";" stmt } [ ";" ]
stmt        ::= assign | if | while | repeat | read | write
assign      ::= identifier "=" simple-expr
if          ::= "if" condition "then" stmt-list [ "else" stmt-list ] "end"
while       ::= "while" condition "do" stmt-list "end"
repeat      ::= "repeat" stmt-list "until" condition
read        ::= "read" "(" identifier ")"
write       ::= "write" "(" writable ")"
writable    ::= simple-expr | string_literal

condition   ::= expression
expression  ::= simple-expr [ relop simple-expr ]
simple-expr ::= term { addop term }
term        ::= factor-a { mulop factor-a }
factor-a    ::= [ "!" | "-" ] factor
factor      ::= identifier | constant | "(" expression ")"

relop       ::= "==" | ">" | ">=" | "<" | "<=" | "!="
addop       ::= "+" | "-" | "||"
mulop       ::= "*" | "/" | "%" | "&&"
constant    ::= integer_const | float_const | char_const
```

## Ajustes feitos

- As declarações ficam antes de `begin`, como nos programas de teste.
- Declarações usam `:` para separar os nomes do tipo.
- A lista de comandos aceita `;` antes de `end`, `else` ou `until`.
- `simple-expr` e `term` usam repetição para evitar recursão à esquerda.
- O nome `factor-a` é usado em todas as regras.
- As regras de `while` e `repeat` foram escritas sem regras auxiliares.

## Precedência dos operadores

Da maior para a menor:

1. `!` e `-` unários;
2. `*`, `/`, `%` e `&&`;
3. `+`, `-` e `||`;
4. `==`, `>`, `>=`, `<`, `<=` e `!=`.
