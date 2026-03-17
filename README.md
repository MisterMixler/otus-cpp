# otus-cpp / lab4 — `ip_print`

## Что реализовано в проекте

В проекте собирается исполняемый файл **`ip_print`** (см. `src/ip_print`).

- **`ip_print::print_ip(...)`** (`src/ip_print/include/print_ip.h`)
  - Перегруженные шаблонные функции через SFINAE:
    - целочисленные типы: печать побайтово от старшего к младшему через `.`
    - строки: печать как есть
    - `std::vector` / `std::list`: печать элементов через `.`
    - `std::tuple` (опционально): печать элементов через `.`, компиляция только если все типы одинаковы

- **Демонстрация в `main`** (`src/ip_print/main.cpp`)
  - Содержит вызовы из условия лабораторной.

## Сборка и запуск

### Сборка (out-of-source)

```bash
cmake -S . -B build
cmake --build build
```

После сборки бинарник будет в `build/ip_print` (в корневом `CMakeLists.txt` задан `RUNTIME_OUTPUT_DIRECTORY`).

### Запуск

```bash
./build/ip_print
```

## Doxygen

```bash
cmake -S . -B build
cmake --build build --target doxygen
```

## CI / packaging

В `.github/workflows/release.yml` используются команды `cmake`/`ctest` и сборка пакета (`cpack`).

