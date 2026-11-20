# DarthEx Compiler

> **Official repository:** [github.com/MarcoRhayden/darthex-compiler](https://github.com/MarcoRhayden/darthex-compiler)

A handwritten compiler built in C++17. The project includes lexical analysis,
a symbol table, and a recursive descent parser.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.20-blue)
![License](https://img.shields.io/badge/license-MIT-blue)

## Architecture

The project follows Clean Architecture principles and separates the code into
three layers:

| Layer | Responsibility |
| --- | --- |
| `domain` | Language rules and value types without input or output. |
| `application` | Contracts and services used by the compiler. |
| `adapters` | Lexer, parser, symbol table, and terminal output. |

## Build

Requirements:

- CMake 3.20 or newer;
- a C++17-compatible compiler;
- internet access during the first build to download Catch2.

```bash
git clone https://github.com/MarcoRhayden/darthex-compiler.git
cd darthex-compiler
cmake -S . -B build
cmake --build build
```

## Usage

```bash
./build/darthexc path/to/program.dex
```

A valid program produces:

```text
Syntax analysis completed successfully.
```

When an error is found, the compiler reports its position, the expected item,
and the token that was found:

```text
5:1  syntax error: expected 'then', found "maior"
```

## Grammar and tests

- [Parser grammar](docs/syntax-grammar.md)
- [Syntax analysis and test results](docs/syntax-analysis.md)

Test programs are stored in:

- `tests/fixtures/lexer`: programs with lexical errors;
- `tests/fixtures/parser/invalid`: programs with one syntax error;
- `tests/fixtures/parser/valid`: corrected programs and additional examples.

Run the complete test suite with:

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```
