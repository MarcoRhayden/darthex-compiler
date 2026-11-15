# DarthEx Compiler

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
| `domain`      | Pure value types: tokens, locations, errors, AST nodes. No I/O.        |
| `application` | Use cases that drive the compilation pipeline.                         |
| `adapters`    | CLI and console output. The only layer that touches the outside world. |

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

## Running Tests

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```
