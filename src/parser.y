%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
%}

%union {
    int integer;
    bool bl;
    char* identifier;
}

%token <integer> INT
%token <identifier> IDENTIFIER
%token <bl> TRUE FALSE
%token SET IF ELSE DOWHILE DOFORI CALL RETURN
%token TO
%token ASSIGN
%token LPAREN RPAREN
%token LBRACE RBRACE
%token SEMICOLON 
%token AND OR NOT

%left '+' '-'
%left '*' '/'
%left AND OR
%right NOT
%start program
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
    | IF LPAREN expression RPAREN statement 
    | IF LPAREN expression RPAREN statement ELSE statement
    | DOWHILE LPAREN expression RPAREN statement
    | DOFORI IDENTIFIER ASSIGN INT TO INT statement
    | CALL IDENTIFIER SEMICOLON
    | RETURN expression SEMICOLON
    | LBRACE statement_list RBRACE
    ;

expression:
    INT
    | IDENTIFIER
    | TRUE
    | FALSE
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | LPAREN expression RPAREN
    ;

%%



void yyerror(const char *s) {
    fprintf(stderr, "Erreur: %s\n", s);
}



