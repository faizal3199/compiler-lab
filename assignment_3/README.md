# Build
`$ flex parser.l`

This builds a file named
* lex.yy.c

# Create a executable
`$ gcc -o parser.out lex.yy.c -lfl`

This creates an `parser.out` file.

# Usage
> $ ./parser.out \<file-name\> 