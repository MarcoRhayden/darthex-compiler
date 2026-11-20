# Teste 2

## Primeira análise

```text
program teste2
a, b, c : int;
d, _var : float;
teste2 = 1;
read(a);
b = a * a;
c = b + a / 2 * (35 / b);
write(c);
val := 34.;
c = val + 2.2 + a;
write(val)
end.
```

Saída:

```text
4:8  syntax error: expected ':', found "="
```

O comando `teste2 = 1` apareceu antes de `begin` e foi lido como uma
declaração. Correção: adicionar `begin` antes dos comandos.

## Segunda análise

```text
program teste2
a, b, c : int;
d, _var : float;
begin
teste2 = 1;
read(a);
b = a * a;
c = b + a / 2 * (35 / b);
write(c);
val := 34.;
c = val + 2.2 + a;
write(val)
end.
```

Saída:

```text
10:5  syntax error: expected '=', found ":"
```

Correção: trocar `:=` por `=`.

## Terceira análise

```text
program teste2
a, b, c : int;
d, _var : float;
begin
teste2 = 1;
read(a);
b = a * a;
c = b + a / 2 * (35 / b);
write(c);
val = 34.;
c = val + 2.2 + a;
write(val)
end.
```

Saída:

```text
10:9  syntax error: expected 'end', found "."
```

O número `34.` não é um `float`, pois não há dígitos depois do ponto.
Correção: usar `34`.

## Resultado final

```text
program teste2
a, b, c : int;
d, _var : float;
begin
teste2 = 1;
read(a);
b = a * a;
c = b + a / 2 * (35 / b);
write(c);
val = 34;
c = val + 2.2 + a;
write(val)
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
