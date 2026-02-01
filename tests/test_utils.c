void test_allocate_string() {
    const char *original = "Bonjour le monde";
    char *copy = allocate_string(original);

    if (copy == NULL) {
        printf("Échec : allocation mémoire\n");
        return;
    }

    // Vérification du contenu
    if (strcmp(original, copy) == 0) {
        printf("Succès : la chaîne a été correctement copiée\n");
    } else {
        printf("Échec : la chaîne copiée est différente\n");
    }

    // Vérification que ce n'est pas la même adresse mémoire
    if (original != copy) {
        printf("Succès : les adresses mémoire sont différentes\n");
    } else {
        printf("Échec : même adresse mémoire\n");
    }

    // Libération de la mémoire
    free(copy);
}
