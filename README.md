# Expression
практикум №2, реализация символьного вычисления производных

## Содержание
- [Установка](#начало-работы)
- [Подключение](#Подключение)
- [Использование](#Использование)
- [Тесты](#Тесты)

## Установка
Соберите проект:
```sh
$ mkdir build && cd build
$ cmake ..
$ make
```
## Подключение
Для подключения основной библиотеки
```cpp
#include "inc/Expression.h"
```
В CMakeList
```cmake
include_directories(inc)

file(GLOB EXPRESSION_SOURCES
        src/BinaryExpression.cpp
        src/ConstantExpression.cpp
        src/MonoExpression.cpp
        src/VarExpression.cpp
        src/Optimization.cpp
)

add_library(EXPRESSION ${EXPRESSION_SOURCES})
target_link_libraries(YOUR_TARGET PRIVATE EXPRESSION)
```

Для подключения парсера из строчки в Expression (вместе с Expression)
```cpp
#include "inc/Parser.h"
```
В CMakeList
```cmake
include_directories(inc)

file(GLOB EXPRESSION_SOURCES
        src/BinaryExpression.cpp
        src/ConstantExpression.cpp
        src/MonoExpression.cpp
        src/VarExpression.cpp
        src/Optimization.cpp
)

file(GLOB TOKEN_SOURCES
        src/Token.cpp
)

file(GLOB PARSER_SOURCES
        src/Parser.cpp
)

add_library(EXPRESSION ${EXPRESSION_SOURCES})
add_library(TOKEN ${TOKEN_SOURCES})
add_library(PARSER ${PARSER_SOURCES})

target_link_libraries(PARSER PRIVATE TOKEN EXPRESSION)
target_link_libraries(YOUR_TARGET PRIVATE EXPRESSION TOKEN PARSER)


```

## Использование

Создание выржаения
```cpp
auto expr = std::make_shared<BinaryExpression>(
    std::make_shared<MonoExpression>(
        std::make_shared<VarExpression>("x"),
        sin_func
    ),
    std::make_shared<ConstantExpression>(3),
    mul_op
);
```
Класс поддерживает операции div(std::string &s) и eval(std::map<std::string, double> &params)
```cpp
auto expr_diff = expr->diff(s);
double value = expr>eval(params);
```
Для печати есть функция toString()
```cpp
std::cout >> expr->toString();
```
Также можно подключить отптимизацию
```cpp
auto expr_optimize = optimize(expr);
```
Чтобы преобразовать строку в Expression
```cpp
auto tokens = tokenize(str);
auto expr = parseExpression(tokens.get())
```

## Тесты
Тесты будут находиться в папке test, запустить их можно следубщими командами
```sh
$ cd tests
$ ./testsType
```