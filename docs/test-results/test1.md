# Teste 1

## Primeira análise

```text
program teste1
{* Calculo de perimetro e area *}
base, altura : float;
perimetro : float;
area, la is float;
begin
write("Entre com a altura do retangulo:");
read(altura);
write("Entre com a base do retangulo:");
read(base);
area = base * altura;
perimetro = 2 * base + 2 * altura;
write "Area: ";
write(area);
write("Perimetro: ");
write(area)
end.
```

Saída:

```text
5:10  syntax error: expected ':', found "is"
```

Correção: trocar `is` por `:` na declaração de `area` e `la`.

## Segunda análise

```text
program teste1
{* Calculo de perimetro e area *}
base, altura : float;
perimetro : float;
area, la : float;
begin
write("Entre com a altura do retangulo:");
read(altura);
write("Entre com a base do retangulo:");
read(base);
area = base * altura;
perimetro = 2 * base + 2 * altura;
write "Area: ";
write(area);
write("Perimetro: ");
write(area)
end.
```

Saída:

```text
13:7  syntax error: expected '(', found "Area: "
```

Correção: escrever a chamada como `write("Area: ")`.

## Resultado final

```text
program teste1
{* Calculo de perimetro e area *}
base, altura : float;
perimetro : float;
area, la : float;
begin
write("Entre com a altura do retangulo:");
read(altura);
write("Entre com a base do retangulo:");
read(base);
area = base * altura;
perimetro = 2 * base + 2 * altura;
write("Area: ");
write(area);
write("Perimetro: ");
write(area)
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
