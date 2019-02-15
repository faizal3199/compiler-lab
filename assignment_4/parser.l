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
"while"                         {handleInput("while");}
"begin"                         {handleInput("begin");}
"end"                           {handleInput("end");}
[ \t\n]                         {}
";"                             {handleInput("semi");}
"("                             {handleInput("lparn");}
")"                             {handleInput("rparn");}
[0-9]+                          {handleInput("num");}
[a-z]+                          {handleInput("id");}
"<"|">"|"<="|">="|"!="|"=="     {handleInput("rop");}
"="|"*="|"/="|"%="|"-="|"+="    {handleInput("aop");}
"*"|"/"|"%"|"-"|"+"             {handleInput("mop");}
"$"                             {handleInput("$");}
%%

/* The trick */ 
int yywrap(){
    return 1;
}