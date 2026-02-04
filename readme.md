# Инструкция по сборке


## Windows (MinGW)
**Требования**
1. Установлен MinGW
2. Установлен CMake

**Сборка в CMake GUI**
1. Указать в ***source*** путь к папке с исходными файлами
2. Указать в ***build*** папку(например, /build), в которой будет собрано приложение
3. Для избежания проблем рекомендую явно указать компилятор, для избежания проблем. Для этого в ***Add Entry*** добавить перееменные:
CMAKE_C_COMPILER - путь к gcc.exe (например, "C:\MinGW\bin\gcc.exe");
CMAKE_CXX_COMPILER - путь к g++.exe
4. Нажать ***Configure***. Выбрать ***MinGW Makefiles***. Нажать ***Finish***
5. Нажать ***Generate***
6. В папке /build (из пункта 2) выполнить команду в терминале (PowerShell или CMD): *mingw32-make*
7. Для запуска выполнить кманду:
*YADRO_intern.exe ../test_examples/<file_name>.txt* (в CMD); 
*./YADRO_intern.exe ../test_examples/<file_name>.txt* (в PowerShell)

## Linux (gcc)
**Требования**
1.  Установлены build-essential и cmake. Если нет - выполнить команду в терминале:
*sudo apt install --no-install-recommends build-essential cmake*

**Сборка**
1. В директории с исходными файлами создать и перейти в папку /build, где будет потом собрано приложение (для этого можно выполниь команду *mkdir build && cd build*)
2. В папке /build выполнить: 
*cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)*
3. Для запуска выполнить:
*./YADRO_intern ../test_examples/<file_name>.txt*


## Примечание 
В папке test_examples хранятся тестовые примеры.