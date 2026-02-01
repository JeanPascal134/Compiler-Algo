<<<<<<< HEAD
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

/* --- Types de tokens venant du lexer --- */
%token <integer> INT
%token <identifier> IDENTIFIER
%token <bl> TRUE FALSE

%token SET IF ELSE FI DOWHILE DOFORI OD CALL RETURN
%token BEGIN_ALGO END_ALGO

%token EQ NEQ LE GE LT GT
%token AND OR NOT
%token PLUS MINUS TIMES DIV MOD

%token LPAREN RPAREN
%token LBRACE RBRACE
%token COMMA SEMICOLON

%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left TIMES DIV MOD
%right NOT
%right UMINUS

%start program

%%

/* ---------------- PROGRAMME COMPLET ---------------- */

program:
      algo_list call_final
    ;

/* liste d’algorithmes */
algo_list:
      /* vide */
    | algo_list algo
    ;

/* un algo : \begin{algo}{Nom}{params} { instructions } \end{algo} */
algo:
      BEGIN_ALGO LBRACE IDENTIFIER RBRACE LBRACE param_list_opt RBRACE block END_ALGO
    ;

/* paramètres formels */
param_list_opt:
      /* vide */
    | param_list
    ;

param_list:
      IDENTIFIER
    | param_list COMMA IDENTIFIER
    ;

/* ---------------- INSTRUCTIONS ---------------- */

block:
      LBRACE statement_list RBRACE
    ;

statement_list:
      /* vide */
    | statement_list statement
    ;

statement:
      set_stmt
    | if_stmt
    | dowhile_stmt
    | dofori_stmt
    | call_stmt SEMICOLON
    | return_stmt SEMICOLON
    ;

/* \SET{x}{expr} */
set_stmt:
      SET LBRACE IDENTIFIER RBRACE LBRACE expression RBRACE
    ;

/* \IF{cond} {block} \FI  |  \IF{cond}{block}\ELSE{block}\FI */
if_stmt:
      IF LBRACE expression RBRACE block FI
    | IF LBRACE expression RBRACE block ELSE block FI
    ;

/* \DOWHILE{cond}{block}\OD */
dowhile_stmt:
      DOWHILE LBRACE expression RBRACE block OD
    ;

/* \DOFORI{x}{a}{b}{block}\OD */
dofori_stmt:
      DOFORI LBRACE IDENTIFIER RBRACE 
              LBRACE expression RBRACE 
              LBRACE expression RBRACE 
              block OD
    ;

/* \CALL{Nom}{args} */
call_stmt:
      CALL LBRACE IDENTIFIER RBRACE LBRACE arg_list_opt RBRACE
    ;

/* \RETURN{expr} */
return_stmt:
      RETURN LBRACE expression RBRACE
    ;

/* dernier appel du fichier */
call_final:
      call_stmt SEMICOLON
    ;

/* ---------------- EXPRESSIONS ---------------- */

arg_list_opt:
      /* vide */
    | arg_list
    ;

arg_list:
      expression
    | arg_list COMMA expression
    ;

expression:
      INT
    | TRUE
    | FALSE
    | IDENTIFIER
    | LPAREN expression RPAREN
    | expression PLUS expression
    | expression MINUS expression
    | expression TIMES expression
    | expression DIV expression
    | expression MOD expression
    | expression EQ expression
    | expression NEQ expression
    | expression LT expression
    | expression LE expression
    | expression GT expression
    | expression GE expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | MINUS expression %prec UMINUS
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erreur syntaxique : %s\n", s);
}

=======
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

/* --- Types de tokens venant du lexer --- */
%token <integer> INT
%token <identifier> IDENTIFIER
%token <bl> TRUE FALSE

%token SET IF ELSE FI DOWHILE DOFORI OD CALL RETURN
%token BEGIN_ALGO END_ALGO

%token EQ NEQ LE GE LT GT
%token AND OR NOT
%token PLUS MINUS TIMES DIV MOD

%token LPAREN RPAREN
%token LBRACE RBRACE
%token COMMA SEMICOLON

%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left TIMES DIV MOD
%right NOT
%right UMINUS

%start program

%%

/* ---------------- PROGRAMME COMPLET ---------------- */

program:
      algo_list call_final
    ;

/* liste d’algorithmes */
algo_list:
      /* vide */
    | algo_list algo
    ;

/* un algo : \begin{algo}{Nom}{params} { instructions } \end{algo} */
algo:
      BEGIN_ALGO LBRACE IDENTIFIER RBRACE LBRACE param_list_opt RBRACE block END_ALGO
    ;

/* paramètres formels */
param_list_opt:
      /* vide */
    | param_list
    ;

param_list:
      IDENTIFIER
    | param_list COMMA IDENTIFIER
    ;

/* ---------------- INSTRUCTIONS ---------------- */

block:
      LBRACE statement_list RBRACE
    ;

statement_list:
      /* vide */
    | statement_list statement
    ;

statement:
      set_stmt
    | if_stmt
    | dowhile_stmt
    | dofori_stmt
    | call_stmt SEMICOLON
    | return_stmt SEMICOLON
    ;

/* \SET{x}{expr} */
set_stmt:
      SET LBRACE IDENTIFIER RBRACE LBRACE expression RBRACE
    ;

/* \IF{cond} {block} \FI  |  \IF{cond}{block}\ELSE{block}\FI */
if_stmt:
      IF LBRACE expression RBRACE block FI
    | IF LBRACE expression RBRACE block ELSE block FI
    ;

/* \DOWHILE{cond}{block}\OD */
dowhile_stmt:
      DOWHILE LBRACE expression RBRACE block OD
    ;

/* \DOFORI{x}{a}{b}{block}\OD */
dofori_stmt:
      DOFORI LBRACE IDENTIFIER RBRACE 
              LBRACE expression RBRACE 
              LBRACE expression RBRACE 
              block OD
    ;

/* \CALL{Nom}{args} */
call_stmt:
      CALL LBRACE IDENTIFIER RBRACE LBRACE arg_list_opt RBRACE
    ;

/* \RETURN{expr} */
return_stmt:
      RETURN LBRACE expression RBRACE
    ;

/* dernier appel du fichier */
call_final:
      call_stmt SEMICOLON
    ;

/* ---------------- EXPRESSIONS ---------------- */

arg_list_opt:
      /* vide */
    | arg_list
    ;

arg_list:
      expression
    | arg_list COMMA expression
    ;

expression:
      INT
    | TRUE
    | FALSE
    | IDENTIFIER
    | LPAREN expression RPAREN
    | expression PLUS expression
    | expression MINUS expression
    | expression TIMES expression
    | expression DIV expression
    | expression MOD expression
    | expression EQ expression
    | expression NEQ expression
    | expression LT expression
    | expression LE expression
    | expression GT expression
    | expression GE expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | MINUS expression %prec UMINUS
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erreur syntaxique : %s\n", s);
}

>>>>>>> 148bc436f9123a86249b80140746afd21be63ba3
