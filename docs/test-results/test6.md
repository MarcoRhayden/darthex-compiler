# Programas extras

## Contador

```text
program contador
valor, limite : int;
begin
read(limite);
valor = 0;
while valor < limite do
write(valor);
valor = valor + 1
end;
write("Fim")
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

## Classificação

```text
program classificacao
nota : float;
aprovado : int;
begin
read(nota);
if nota >= 60.0 then
aprovado = 1;
write("Aprovado")
else
aprovado = 0;
write("Reprovado")
end;
repeat
nota = nota + 1.0
until nota >= 100.0
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
