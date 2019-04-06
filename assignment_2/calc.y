%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE float
%}

%start srt_expr

%token NUMBER OPENBRACKETSIGN CLOSEBRACKETSIGN EOL
%token PLUSSIGN MINUSSIGN
%token MULSIGN DIVSIGN
%left PLUSSIGN MINUSSIGN
%left MULSIGN DIVSIGN MODULOSIGN

%%
srt_expr: level_one EOL  { printf("Answer: %.6f\n", (float)$1);exit(0);}
         ;
level_one: level_one PLUSSIGN level_two  { $$ = $1 + $3;}
         | level_one MINUSSIGN level_two  { $$ = $1 - $3;}
         | PLUSSIGN level_two  { $$ = 0 + $2;}
         | MINUSSIGN level_two  { $$ = 0 - $2;}
         | level_two  { $$ = $1;}
         ;
level_two: level_two MULSIGN units  { $$ = $1 * $3 ;}
         | level_two DIVSIGN units  { $$ = $1 / $3 ;}
         | units  { $$ = $1;}
         ;
units: NUMBER  { $$ = $1;}
         | MINUSSIGN NUMBER { $$ = -$2; }
         | OPENBRACKETSIGN level_one CLOSEBRACKETSIGN  { $$ = $2;}
         ;
%%

#include <stdio.h>
#include <ctype.h>
char *progname;
float yylval;

int main(int argc, char* argv[]){
    progname = argv[0];
    yyparse();
    return 0;
}

void warning(char* s ,char* t ){
    fprintf(stderr, "%s: %s\n", progname, s);

    if(t){
        fprintf(stderr, " %s\n", t);
    }
}
 
int yyerror(char* s ){
    warning(s, (char *) 0);
    return 0;
}

int yywrap(){
    return 1;
}