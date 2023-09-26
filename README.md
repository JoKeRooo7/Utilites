# Utilities

___

  My first project made in c. His goal was to replicate the implementation of bash shell utilities (cat, grep).

___

* Utilities:

  * [cat](#cat)
  * [grep](#grep)

* [build](#build) 
* [usage example](#usage example)

___

## cat

Learn more about the utility - [cat](https://en.wikipedia.org/wiki/Cat_(Unix))

- implemented flags: 
    
    - b (number-nonblank)
    - e 
    - E
    - n (number)
    - s (squeeze-blank)
    - t
    - T


- launch `` ./cat <flag> <file>``
___

## grep

Learn more about the utility - [grep](https://en.wikipedia.org/wiki/Grep)

- implemented flags: 

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

- launch ``` ./grep <flags> <files> ```
---

## assembly

For various operating systems, the collected files are located in the build folder (so far only for macOS).

The assembly takes place using a makefile in the corresponding folders of the project (use the make command in the directory with the makefile).
___

## usage example:

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
