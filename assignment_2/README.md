# Build [yacc]
`$ yacc -d calc.y`

This builds two files named
* y.tab.c
* y.tab.h (This file is used by flex)

# Build [lex]
`$ flex calc.l`

This builds a file named
* lex.yy.c

# Link two C files
`$ cc y.tab.c lex.yy.c`

This creates an `a.out` file.

# Usage
> $ ./a.out 
>
>   (12+34)/(43)\*(12\*33)
>
>   Answer: 423.627930