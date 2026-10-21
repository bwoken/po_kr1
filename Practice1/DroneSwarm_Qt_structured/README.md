# DroneSwarmQt — задание 3, вариант 1

## Структура проекта

```text
DroneSwarm_Qt/
├── CMakeLists.txt
├── README.md
├── Headers/
│   ├── drone.h
│   ├── droneswarm.h
│   ├── chargehistogram.h
│   └── mainwindow.h
└── Sources/
    ├── main.cpp
    ├── drone.cpp
    ├── droneswarm.cpp
    ├── chargehistogram.cpp
    └── mainwindow.cpp
```

`Headers` содержит объявления классов (`.h`), `Sources` — реализации (`.cpp`) и точку входа `main.cpp`.

## Запуск
1. Установите Qt с компонентом Qt Widgets и CMake.
2. Откройте `CMakeLists.txt` через **File → Open File or Project** в Qt Creator.
3. Выберите Kit, нажмите **Configure Project**, затем **Build** и **Run**.

Qt Charts не требуется: гистограмма рисуется через `QPainter`.
