<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h" // Inclure le fichier généré par Bison

extern FILE *yyin; // Fichier d'entrée pour le lexer

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename.algo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    yyparse(); // Appeler le parser

    fclose(yyin);
    return EXIT_SUCCESS;
}
=======
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h" // Inclure le fichier généré par Bison

extern FILE *yyin; // Fichier d'entrée pour le lexer

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename.algo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    yyparse(); // Appeler le parser

    fclose(yyin);
    return EXIT_SUCCESS;
}
>>>>>>> 148bc436f9123a86249b80140746afd21be63ba3
