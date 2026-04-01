# otus-cpp / lab5 — `Editor`

Каркас простейшего векторного графического редактора (MVC).

## Структура

```
src/editor/
├── include/
│   ├── model/          # Модель: примитивы и документ
│   │   ├── point.h
│   │   ├── ishape.h    # Абстрактный примитив
│   │   ├── circle.h
│   │   ├── rectangle.h
│   │   ├── line.h
│   │   └── document.h  # Документ = коллекция примитивов
│   ├── view/           # Представление
│   │   ├── iview.h     # Абстрактное представление
│   │   └── console_view.h
│   ├── controller/     # Контроллер
│   │   └── controller.h
│   └── serialization/  # Импорт / экспорт
│       ├── iserializer.h
│       └── file_serializer.h
└── main.cpp            # Точка входа + обработчики GUI
```

## Сборка

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Запуск

```bash
./editor
```
