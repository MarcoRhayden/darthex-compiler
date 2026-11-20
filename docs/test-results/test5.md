# Teste 5

## Primeira análise

```text
{* Teste 5 do compilador *}
program teste5
a, b, c, maior : int;
opc : int;
begin
repeat
write("A: ");
read(a);
write("B: ");
read(b);
write("C: ");
read(c);
if (a > b) && (a > c) end
maior = a
else
if b > c then
maior = b
else
maior = c
end
end;
write("Maior valor: ");
write(maior);
write("Continuar? (S/N) ");
read(outro)
until (opc == 'N' || opc == 'n')
end.
```

Saída:

```text
13:23  syntax error: expected 'then', found "end"
```

Correção: trocar o `end` depois da condição por `then`.

## Segunda análise

```text
{* Teste 5 do compilador *}
program teste5
a, b, c, maior : int;
opc : int;
begin
repeat
write("A: ");
read(a);
write("B: ");
read(b);
write("C: ");
read(c);
if (a > b) && (a > c) then
maior = a
else
if b > c then
maior = b
else
maior = c
end
end;
write("Maior valor: ");
write(maior);
write("Continuar? (S/N) ");
read(outro)
until (opc == 'N' || opc == 'n')
end.
```

Saída:

```text
26:26  syntax error: expected ')', found "=="
```

Correção: agrupar as duas comparações da condição do `until`.

## Resultado final

```text
{* Teste 5 do compilador *}
program teste5
a, b, c, maior : int;
opc : int;
begin
repeat
write("A: ");
read(a);
write("B: ");
read(b);
write("C: ");
read(c);
if (a > b) && (a > c) then
maior = a
else
if b > c then
maior = b
else
maior = c
end
end;
write("Maior valor: ");
write(maior);
write("Continuar? (S/N) ");
read(outro)
until (opc == 'N') || (opc == 'n')
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
