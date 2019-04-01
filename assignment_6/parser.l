%{
#define YYSTYPE char*
#include <math.h>
#include <stdio.h>
extern char* yylval;
extern void handleInput(char*);
%}

while      "while"
begin      "begin"
end        "end"
eol        "\n"
space      " "
tab        "\t"
semi       ";"
lparn      "("
rparn      ")"
num        [0-9]+
id         [a-z]+
rop        "<" | ">"  | "<=" | ">=" | "!=" | "=="
aop        "=" | "*=" | "/=" | "%=" | "-=" | "+="
mop        "*" | "/"  | "%"  | "-"  | "+"

%%
[ \t\n]                         {}
"("                             {handleInput("lparn");}
")"                             {handleInput("rparn");}
[0-9]+                          {handleInput("num");}
"+"                             {handleInput("add");}
"-"                             {handleInput("sub");}
"*"                             {handleInput("mul");}
"/"                             {handleInput("div");}
"$"                             {handleInput("$");}
%%

/* The trick */ 
int yywrap(){
    return 1;
}