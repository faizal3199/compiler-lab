%{
#include <stdio.h>
%}

%option yymore

WHILETOKEN      "while"
BEGINTOKEN      "begin"
ENDTOKEN        "end"
EOL             "\n"
SPACE           " "
TAB             "\t"
SEMICOLONTOKEN  ";"

EQUALTOKEN      "="
PLUSSIGN        "+"
MINUSSIGN       "-"
MULSIGN         "*"
DIVSIGN         "/"
OPENBRACKET     "("
CLOSEBRACKET    ")"
MODULOSIGN      "%"
OPENANGLE       "<"
CLOSEANGLE      ">"
EXCLIMATION     "!"

NUMBER          [-]?[0-9]+
IDENTITY        [a-z]+
WHITESPACE      ({SPACE}|{TAB})*

OPERATION       {PLUSSIGN}|{MINUSSIGN}|{MULSIGN}|{DIVSIGN}|{EXCLIMATION}|{MODULOSIGN}
GREATERTHAN     {CLOSEANGLE}{EQUALTOKEN}
LESSERTHAN      {OPENANGLE}{EQUALTOKEN}
EQUALTO         {EQUALTOKEN}{EQUALTOKEN}
NOTEQUAL        {EXCLIMATION}{EQUALTOKEN}

VARIABLE        {IDENTITY}|{NUMBER}
COMPTOKEN       {GREATERTHAN}|{LESSERTHAN}|{EQUALTO}|{NOTEQUAL}|{OPENANGLE}|{CLOSEANGLE}
CONDITION       {VARIABLE}{COMPTOKEN}{VARIABLE}

NORMALSTATEMENT {IDENTITY}{WHITESPACE}{EQUALTOKEN}{WHITESPACE}{VARIABLE}{WHITESPACE}{OPERATION}{WHITESPACE}{VARIABLE}
ASSIGNSTATEMENT {IDENTITY}{WHITESPACE}{EQUALTOKEN}{WHITESPACE}{VARIABLE}

STATEMENT       {NORMALSTATEMENT}|{ASSIGNSTATEMENT}
LINEONE         {WHILETOKEN}{WHITESPACE}{OPENBRACKET}{WHITESPACE}{CONDITION}{WHITESPACE}{CLOSEBRACKET}{WHITESPACE}{EOL}
LINETWO         {BEGINTOKEN}{WHITESPACE}{EOL}
LINETHREE       {WHITESPACE}{STATEMENT}{WHITESPACE}{SEMICOLONTOKEN}{WHITESPACE}{EOL}
LINEFOUR        {ENDTOKEN}{WHITESPACE}({EOL})*

%%
{LINEONE}{LINETWO}({LINETHREE}*){LINEFOUR}  {printf("Valid Syntax\n");}
.+                                          {printf("Invalid Syntax\n");exit(1);}
%%


int main(int argc, char** argv){
    if(argc != 2){
        printf("Usage: %s <file-name>",argv[0]);
        return 1;
    }
    else{
        yyin = fopen(argv[1],"r");
    }
    yylex();
    // {LINEONE}   printf("1: `%s`",yytext);
    // {LINETWO}   printf("2: `%s`",yytext);
    // {LINETHREE}   printf("3: `%s`",yytext);
    // {LINEFOUR}   printf("4: `%s`",yytext);

    // {LINEONE}{LINETWO}({LINETHREE}*){LINEFOUR} {printf("Done");};
}