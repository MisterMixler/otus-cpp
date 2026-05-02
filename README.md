# otus-cpp / lab 10 — пакет `bulk_server`

Асинхронный TCP-сервер пакетной обработки команд на базе Boost.ASIO.
Переиспользует библиотеку `async` (lab 9) и `bulk_processor` для парсинга команд.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

## Запуск

```bash
cd build
./bulk_server <port> <bulk_size>
```

Пример (один клиент):

```bash
./bulk_server 9000 3 &
seq 0 9 | nc localhost 9000
```

Вывод:

```
bulk: 0, 1, 2
bulk: 3, 4, 5
bulk: 6, 7, 8
bulk: 9
```

Два одновременных клиента:

```bash
# bulk_server <port> <bulk_size>
./bulk_server 9000 3 &
seq 0 9 | nc localhost 9000 &
seq 10 19 | nc localhost 9000 &
wait
```

Команды из статических блоков смешиваются между соединениями.
Команды из динамических блоков (`{` … `}`) остаются изолированными для каждого соединения.

## Архитектура

| Компонент          | Ответственность                                            |
| ------------------ | ---------------------------------------------------------- |
| `bulk_processor`   | `CommandProcessor` — парсинг строк, блоки команд           |
| `libasync`         | Очереди задач, потоки log / file1 / file2 (lab 9)          |
| `bulk_server`      | Boost.ASIO TCP-сервер, маршрутизация команд                |
| `OutputObserver`   | Уведомление о готовом блоке → очереди log / file            |

```
TCP client ──► Session ──┬──► SharedProcessor (static) ──► OutputObserver ──► очереди ──► stdout / bulk*.log
                         └──► per-session dynamic block ──► OutputObserver ──┘
```

## Паттерны проектирования

| Паттерн               | Где проявляется                                                                                     |
| --------------------- | --------------------------------------------------------------------------------------------------- |
| **Observer**          | `CommandProcessor` → `IObserver::onBlock`; `OutputObserver` ставит задачи в очереди вывода.         |
| **Producer-Consumer** | Сетевой поток формирует блоки → `BlockingQueue` → рабочие потоки log / file1 / file2.              |
| **RAII**              | `shared_ptr<Session>` — время жизни сессии привязано к цепочке async-вызовов.                       |
| **Proactor**          | Boost.ASIO `async_accept` / `async_read_some` — асинхронный ввод-вывод без блокировки.             |

## Пакет

```bash
cmake --build build --target package
```

Устанавливаются `bulk_server`, `libasync.so`, `async_stress`, заголовок `async.h`.
