%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%union {
    int number;
    
    char* identifier;
}

%token <number> NUMBER
%token <identifier> IDENTIFIER
%token SET IF ELSE DOWHILE DOFORI CALL RETURN
%token ASSIGN LPAREN RPAREN SEMICOLON

%%

// Règles de grammaire
program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    SET IDENTIFIER ASSIGN expression SEMICOLON
    | IF LPAREN expression RPAREN statement ELSE statement
    | DOWHILE LPAREN expression RPAREN statement
    | DOFORI IDENTIFIER ASSIGN NUMBER TO NUMBER statement
    | CALL IDENTIFIER SEMICOLON
    | RETURN expression SEMICOLON
    ;

expression:
    NUMBER
    | IDENTIFIER
    | expression '+' expression
    | expression '-' expression
    ;

%%



void yyerror(const char *s) {
    fprintf(stderr, "Erreur: %s\n", s);
}

