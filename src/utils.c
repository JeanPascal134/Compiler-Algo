
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour allouer de la mémoire pour une chaîne
char* allocate_string(const char *str) {
    char *new_str = malloc(strlen(str) + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

// Autres fonctions utilitaires peuvent être ajoutées ici
