%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "y.tab.h"
%}

%%
// Règles de lexique 
[ \t\n]+  ; //Ignorer les espaces et les retours à la ligne
"\\SET"        { return SET; } 
"\\IF"         { return IF; }
"\\ELSE"       { return ELSE; }
"\\DOWHILE"    { return DOWHILE; }
"\\DOFORI"     { return DOFORI; }
"\\CALL"       { return CALL; }
"\\RETURN"     { return RETURN; }
[0-9]+         { return yyval.number=atoi(yytext);return NUMBER;}
[a-zA-Z_] [a-zA-Z0-9_]* { yylval.identifier = strdup(yytext); return IDENTIFIER; }
"="              { return ASSIGN; }
"\\("            { return LPAREN; }
"\\)"            { return RPAREN; }
"{"              { return LBRACE; }
"}"              { return RBRACE; }
","              { return COMMA; }
";"              { return SEMICOLON; }
"true"           { yylval.boolean = 1; return BOOL; }
"false"          { yylval.boolean = 0; return BOOL; }
.                { return yytext[0]; } // Retourner tout autre caractère


%%





