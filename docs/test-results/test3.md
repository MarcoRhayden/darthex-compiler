# Teste 3

## Primeira análise

```text
program
a, aux : int;
b : float
begin
b = 0;
read(a);
read(b);
if a > b then
aux = b;
b = a;
a = aux
end;
write(a);
write(b);
if a % 2 == 0 write("Par")
else write("Impar");
```

Saída:

```text
2:2  syntax error: expected 'begin', found ","
```

O identificador `a` foi lido como nome do programa. Correção: adicionar um
nome depois de `program`.

## Segunda análise

```text
program teste3
a, aux : int;
b : float
begin
b = 0;
read(a);
read(b);
if a > b then
aux = b;
b = a;
a = aux
end;
write(a);
write(b);
if a % 2 == 0 write("Par")
else write("Impar");
```

Saída:

```text
4:1  syntax error: expected ';', found "begin"
```

Correção: adicionar `;` depois da declaração de `b`.

## Terceira análise

```text
program teste3
a, aux : int;
b : float;
begin
b = 0;
read(a);
read(b);
if a > b then
aux = b;
b = a;
a = aux
end;
write(a);
write(b);
if a % 2 == 0 write("Par")
else write("Impar");
```

Saída:

```text
15:15  syntax error: expected 'then', found "write"
```

Correção: adicionar `then` depois da condição do segundo `if`.

## Quarta análise

```text
program teste3
a, aux : int;
b : float;
begin
b = 0;
read(a);
read(b);
if a > b then
aux = b;
b = a;
a = aux
end;
write(a);
write(b);
if a % 2 == 0 then
write("Par")
else
write("Impar");
```

Saída:

```text
19:1  syntax error: expected 'end', found end of file
```

Correção: fechar o segundo `if` e o programa com `end` e `end.`.

## Resultado final

```text
program teste3
a, aux : int;
b : float;
begin
b = 0;
read(a);
read(b);
if a > b then
aux = b;
b = a;
a = aux
end;
write(a);
write(b);
if a % 2 == 0 then
write("Par")
else
write("Impar")
end
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
