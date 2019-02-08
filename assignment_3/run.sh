#!/bin/bash

flex parser.l
gcc -o parser.out lex.yy.c -lfl

echo -n 'test1.input => ';
./parser.out test1.input

echo -n 'test2.input => ';
./parser.out test2.input

echo -n 'test3.input => ';
./parser.out test3.input

rm lex.yy.c