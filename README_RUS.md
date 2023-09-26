# Утилиты

___

  Мой первый проект сделанный на языке си. Его цель была повторить реализацию утилит оболочки bash (cat, grep).

___

* Утилиты:

  * [cat](#cat)
  * [grep](#grep)

* [сборка](#сборка) 
* [пример использования](#пример-использования)

___

## cat

Подробнее об утилите - [cat](https://ru.wikipedia.org/wiki/Cat#:~:text=cat%20(%D0%BE%D1%82%20%D0%B0%D0%BD%D0%B3%D0%BB.,%C2%BB%2C%20%D1%82%D0%BE%20%D1%87%D0%B8%D1%82%D0%B0%D0%B5%D1%82%D1%81%D1%8F%20%D1%81%D1%82%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D1%82%D0%BD%D1%8B%D0%B9%20%D0%B2%D0%B2%D0%BE%D0%B4.)

- реализованы флаги: 
    
    - b (number-nonblank)
    - e 
    - E
    - n (number)
    - s (squeeze-blank)
    - t
    - T


- запуск ``` ./cat <flag> <file> ```
___

## grep

Подробнее про утилиту - [grep](https://ru.wikipedia.org/wiki/Grep)

- реализованы флаги: 

    - e
    - i
    - v
    - c
    - l
    - n
    - h
    - s
    - f
    - o

- запуск ``` ./grep <flags> <files> ```
---

## сборка

Для различных операционных систем, собранные файлы находятся в папке build (пока что только для macOS).

Сборка происходит с помощью makefile в соответсвубщих папках проекта (используйте команду make в директории с makefile).
___

## пример использования:

```bash
jokerooo7@pc macOS % pwd
/.../utilities_bash_on_c/build/macOS
jokerooo7@pc macOS %./cat -n ../../src/cat/cat.h
#ifndef UTILITES_C_LANGUAGE_CAT_CAT_H_
#define UTILITES_C_LANGUAGE_CAT_CAT_H_

struct CatFlags {
  int flag_b;
  int flag_e;
...
#endif // UTILITES_C_LANGUAGE_CAT_CAT_H_
jokerooo7@pc macOS %
```