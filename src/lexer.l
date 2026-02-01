%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.tab.h"
%}

%option noyywrap

%%
// Règles de lexique 
[ \t\n]+  ; //Ignorer les espaces et les retours à la ligne
"\\SET"        { return SET; } 
"\\IF"         { return IF; }
"\\FI"         { return FI; }
"\\ELSE"       { return ELSE; }
"\\DOWHILE"    { return DOWHILE; }
"\\DOFORI"     { return DOFORI; }
"\\OD"         { return OD; }
"\\CALL"       { return CALL; }
"\\RETURN"     { return RETURN; }
"\\TO"         { return TO;} 

"\\begin{algo}" { return BEGIN_ALGO; } 
"\\end{algo}"   { return END_ALGO; }

[0-9]+         { return yyval.integer=atoi(yytext);return INT;}
[a-zA-Z_] [a-zA-Z0-9_]* { yylval.identifier = strdup(yytext); return IDENTIFIER; }


"=="            { return EQ; }
"!="            { return NEQ; } 
"<="            { return LE; }
">="            { return GE; } 
"<"             { return LT; } 
">"             { return GT; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIV; }
"%"             { return MOD; }


"("            { return LPAREN; }
")"            { return RPAREN; }
"{"              { return LBRACE; }
"}"              { return RBRACE; }
","              { return COMMA; }
";"              { return SEMICOLON; }

"true"           { yylval.bl = true; return TRUE; }
"false"          { yylval.bl = false; return FALSE; }

"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }

.               {fprintf(stderr, "Bad character: %c\n", yytext[0]);
                 yyterminate();} // Retourner tout autre caractère


%%









