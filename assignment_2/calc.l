%{
#define YYSTYPE float
#include <math.h>
#include <stdio.h>
#include "y.tab.h"
extern float yylval;
%}
 
%%
[0-9.]+  yylval=atof(yytext); return NUMBER;
"+"      return PLUSSIGN;
"-"      return MINUSSIGN;
"*"      return MULSIGN;
"/"      return DIVSIGN;
"("      return OPENBRACKETSIGN;
")"      return CLOSEBRACKETSIGN;
"\n"     return EOL;
%%