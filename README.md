# otus-cpp / lab7 — `bulk` (packet command processor)

Утилита для пакетной обработки команд с консольным и файловым логированием.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

## Использование

```bash
# Статические блоки по 3 команды
echo -e "cmd1\ncmd2\ncmd3\ncmd4\ncmd5" | ./build/bulk 3

# Динамические блоки
echo -e "cmd1\ncmd2\n{\ncmd3\ncmd4\n}\ncmd5" | ./build/bulk 3
```

## Архитектура

Паттерн **Observer** обеспечивает низкую связанность модулей:

| Модуль             | Ответственность                        |
|--------------------|----------------------------------------|
| `CommandProcessor` | Парсинг ввода, накопление блоков       |
| `ConsoleLogger`    | Вывод блоков в `stdout`               |
| `FileLogger`       | Сохранение блоков в `bulk*.log` файлы |
| `IObserver`        | Интерфейс-контракт между модулями      |

```
stdin → CommandProcessor → notify → ConsoleLogger → stdout
                                  → FileLogger    → bulk*.log
```
