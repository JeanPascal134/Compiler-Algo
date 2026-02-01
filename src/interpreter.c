#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

// Fonction pour interpréter les instructions
void interpret_statement(Statement *stmt) {
    switch (stmt->type) {
        case SET:
            // Code pour gérer l'assignation
            printf("SET %s = %d\n", stmt->identifier, stmt->value);
            break;
        case CALL:
            // Code pour gérer l'appel d'algorithme
            printf("CALL %s\n", stmt->identifier);
            break;
        case RETURN:
            // Code pour gérer le retour
            printf("RETURN %d\n", stmt->value);
            break;
        // Ajouter d'autres cas selon les types d'instructions
        default:
            break;
    }
}

// Fonction principale d'interprétation
void interpret_program(Program *program) {
    for (int i = 0; i < program->statement_count; i++) {
        interpret_statement(&program->statements[i]);
    }
}
