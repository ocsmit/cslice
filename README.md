see [test file](./src/test.c) for example usage which outputs below

```sh
$ make && ./bin

gcc -g -Wall -O0 -std=c11 -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -Wconversion  -Wstrict-prototypes -Wnewline-eof   -c -o src/arena.o src/arena.c
gcc -g -Wall -O0 -std=c11 -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -Wconversion  -Wstrict-prototypes -Wnewline-eof -o bin src/arena.o src/slice.o src/test.o
testing stack i8
0 : 8
1 : 237
2 : 1
3 : 2
4 : 44
5 : 99
6 : 1
7 : 2
8 : 44
9 : -99
40, 40
40, 40
failed with error : LIMIT ERROR
pop
36, 40
push
40, 40
DONE

testing arena
arena cap: 80
arena offset: 40
stack cap: 40
0 : 8
1 : 237
2 : 1
3 : 2
4 : 44
5 : 99
6 : 1
7 : 2
8 : 44
9 : -99
40, 40
0 : 8
1 : 237
2 : 1
3 : 9999
4 : 44
5 : 99
6 : 1
7 : 2
8 : 44
9 : -99
```
