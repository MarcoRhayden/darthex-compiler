# DarthEx Compiler

> **Official repository:** [github.com/MarcoRhayden/darthex-compiler](https://github.com/MarcoRhayden/darthex-compiler)

> A handwritten compiler built in C++17.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.20-blue)
![License](https://img.shields.io/badge/license-MIT-blue)

---

## About

DarthEx is a handwritten compiler built in C++17 following Clean Architecture principles.

## Architecture

| Layer         | Responsibility                                                         |
| ------------- | ---------------------------------------------------------------------- |
| `domain`      | Pure value types and language rules. No I/O.                           |
| `application` | Use cases that drive the compilation pipeline.                         |
| `adapters`    | CLI and console output. The only layer that touches the outside world. |

## Syntax Grammar

The grammar used by the parser and its adjustments are documented in
[docs/syntax-grammar.md](docs/syntax-grammar.md).

## Requirements

- CMake ≥ 3.20
- A C++17-compliant compiler (GCC 9+, Clang 10+, AppleClang 12+)
- Internet access on first build (Catch2 is fetched via `FetchContent`)

## Getting Started

```bash
git clone https://github.com/MarcoRhayden/darthex-compiler.git

cd darthex-compiler
cmake -S . -B build
cmake --build build
./build/darthexc <source-file.dx>
```

## Tests

```bash
cmake --build build && ctest --test-dir build --output-on-failure
```

### Unit tests (`tests/unit/`)

| Suite | Layer |
|-------|-------|
| `source_location`, `token`, `token_kind`, `lexical_error`, `syntax_error`, `identifier`, `keyword`, `char_class` | `domain` |
| `lexer`, `symbol_table` | `adapters` |
| `parser_port` | `application/ports` |
| `lex_service` | `application/services` |

### Acceptance tests (`tests/acceptance/`)

Fixtures in `tests/fixtures/lexer/` reproduce the five programs from the assignment spec.

| Fixture | Lexical error(s) |
|---------|------------------|
| `test1.dex` | Unterminated comment — `{*…}` missing `*}`, swallows file from line 2 |
| `test2.dex` | Identifier starts with digit — `1c` on line 2 |
| `test3.dex` | Unterminated string — `"Par…` not closed before end of line 15 |
| `test4.dex` | Unterminated comment — never closed, swallows file from line 2 |
| `test5.dex` | Unterminated string `");` on line 22; unterminated char const `'n)` on line 26 |
