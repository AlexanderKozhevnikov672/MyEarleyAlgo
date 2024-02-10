# Алгоритм Эрли

## Сборка и запуск решения

1) Создайте папку build и перейдите в неё

```bash
mkdir build && cd build
```

2) Выполните сборку проекта

```bash
cmake .. && make
```

3) Появится папка bin с двумя исполняемыми файлами:
- Testing - для запуска unit тестов
- Manual - для запуска на пользовательском входе

4) Для запуска программы на произвольных тестах, необходимо записать входные данные в manual/test.txt и запустить исполняемый файл Manual. Результат появится в manual/answer.txt