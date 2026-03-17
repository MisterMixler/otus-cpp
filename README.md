# otus-cpp / lab3 — `stl_allocator`

## Что реализовано в проекте

В проекте собирается исполняемый файл **`stl_allocator`** (см. `src/stl_allocator`).

- **`PoolAllocator<T, Capacity>`** (`src/stl_allocator/include/stl_allocator.h`)
  - Простой пуловый аллокатор фиксированной ёмкости `Capacity` (в элементах).
  - `allocate(n)` выдаёт непрерывный кусок из заранее выделенного буфера (bump allocator), увеличивая внутренний `offset`.
  - `deallocate(...)` ничего не делает — память освобождается целиком при разрушении пула.
  - Пул хранится в `std::shared_ptr`, поэтому копии аллокатора могут разделять один и тот же пул.

- **`SimpleContainer<T, Alloc>`** (`src/stl_allocator/include/simple_container.h`)
  - Учебный контейнер на односвязном списке (узлы `Node`), поддерживает:
    - вставку `push_back` / `emplace_back`
    - итераторы `iterator` / `const_iterator` (forward-итераторы)
    - `size()`, `empty()`, `clear()`
  - Аллокация узлов выполнена через `std::allocator_traits` и `rebind_alloc`, т.е. контейнер умеет работать с пользовательским аллокатором.

- **Демонстрация в `main`** (`src/stl_allocator/main.cpp`)
  - Заполняет `std::map<int,int>` значениями факториала для ключей 0..9:
    - один раз с дефолтным аллокатором
    - один раз с `PoolAllocator<std::pair<const int,int>, 10>`
  - Создаёт `SimpleContainer<int>` с дефолтным аллокатором и `SimpleContainer<int, PoolAllocator<int, 10>>` с пуловым.

## Что реализовано в тестах

Папка `tests/` содержит GoogleTest-тесты:

- **`PoolAllocatorTest.Requirements`** (`tests/pool_allocator_test.cpp`)
  - Проверяет требования лабораторной:
    - создание и заполнение `std::map<int,int>` (ключи `0..9`, значения — `factorial(key)`)
    - создание и заполнение `std::map<int,int>` с `PoolAllocator<std::pair<const int,int>, 10>`
    - вывод содержимого `std::map` в `stdout` в формате `key value` (через пробел), по одной строке на элемент
    - создание и заполнение `SimpleContainer<int>` значениями `0..9`
    - создание и заполнение `SimpleContainer<int, PoolAllocator<int, 10>>` значениями `0..9`
    - вывод содержимого контейнера в `stdout`, по одному значению на строке
  - Тест перехватывает `stdout` и сравнивает его с ожидаемым выводом построчно.

## Сборка и запуск

### Сборка (out-of-source)

```bash
cmake -S . -B build
cmake --build build
```

После сборки бинарник будет в `build/stl_allocator` (в корневом `CMakeLists.txt` задан `RUNTIME_OUTPUT_DIRECTORY`).

### Запуск

```bash
./build/stl_allocator
```

## Тесты

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## CI / packaging

В `.github/workflows/release.yml` используются команды `cmake`/`ctest` и сборка пакета (`cpack`).

