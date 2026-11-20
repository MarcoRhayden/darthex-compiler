# Teste 4

## Primeira análise

```text
program teste4
{* Teste 4 do compilador *}
pontuacao, pontuacaoMaxima, disponibilidade : int;
pontuacaoMinima : char;
begin
pontuacaoMinima = 50;
pontuacaoMaxima = 100;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade);
while (pontuacao > 0 && (pontuacao <= pontuacaoMaxima) do
if (pontuacao > pontuacaoMinima) && (disponibilidade == 1) then
write("Candidato aprovado. ")
else
write("Candidato reprovado. ")
end
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade)
end
end
```

Saída:

```text
12:56  syntax error: expected ')', found "do"
```

Correção: fechar a primeira comparação antes de `&&`.

## Segunda análise

```text
program teste4
{* Teste 4 do compilador *}
pontuacao, pontuacaoMaxima, disponibilidade : int;
pontuacaoMinima : char;
begin
pontuacaoMinima = 50;
pontuacaoMaxima = 100;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade);
while (pontuacao > 0) && (pontuacao <= pontuacaoMaxima) do
if (pontuacao > pontuacaoMinima) && (disponibilidade == 1) then
write("Candidato aprovado. ")
else
write("Candidato reprovado. ")
end
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade)
end
end
```

Saída:

```text
18:1  syntax error: expected ';', found "write"
```

Correção: adicionar `;` depois do `end` que fecha o `if`.

## Terceira análise

```text
program teste4
{* Teste 4 do compilador *}
pontuacao, pontuacaoMaxima, disponibilidade : int;
pontuacaoMinima : char;
begin
pontuacaoMinima = 50;
pontuacaoMaxima = 100;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade);
while (pontuacao > 0) && (pontuacao <= pontuacaoMaxima) do
if (pontuacao > pontuacaoMinima) && (disponibilidade == 1) then
write("Candidato aprovado. ")
else
write("Candidato reprovado. ")
end;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade)
end
end
```

Saída:

```text
24:1  syntax error: expected '.', found end of file
```

Correção: finalizar o programa com `end.`.

## Resultado final

```text
program teste4
{* Teste 4 do compilador *}
pontuacao, pontuacaoMaxima, disponibilidade : int;
pontuacaoMinima : char;
begin
pontuacaoMinima = 50;
pontuacaoMaxima = 100;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade);
while (pontuacao > 0) && (pontuacao <= pontuacaoMaxima) do
if (pontuacao > pontuacaoMinima) && (disponibilidade == 1) then
write("Candidato aprovado. ")
else
write("Candidato reprovado. ")
end;
write("Pontuacao do candidato: ");
read(pontuacao);
write("Disponibilidade do candidato: ");
read(disponibilidade)
end
end.
```

Saída:

```text
Syntax analysis completed successfully.
```

[Voltar aos resultados](../syntax-test-results.md)
