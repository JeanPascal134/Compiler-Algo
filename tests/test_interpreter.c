<<<<<<< HEAD
void test_interpret_program() {
    // Création de quelques instructions
    Statement statements[3];

    // SET x = 10
    statements[0].type = SET;
    statements[0].identifier = "x";
    statements[0].value = 10;

    // CALL algo1
    statements[1].type = CALL;
    statements[1].identifier = "algo1";

    // RETURN 0
    statements[2].type = RETURN;
    statements[2].value = 0;

    // Création du programme
    Program program;
    program.statements = statements;
    program.statement_count = 3;

    // Interprétation
    interpret_program(&program);
}
=======
void test_interpret_program() {
    // Création de quelques instructions
    Statement statements[3];

    // SET x = 10
    statements[0].type = SET;
    statements[0].identifier = "x";
    statements[0].value = 10;

    // CALL algo1
    statements[1].type = CALL;
    statements[1].identifier = "algo1";

    // RETURN 0
    statements[2].type = RETURN;
    statements[2].value = 0;

    // Création du programme
    Program program;
    program.statements = statements;
    program.statement_count = 3;

    // Interprétation
    interpret_program(&program);
}
>>>>>>> 148bc436f9123a86249b80140746afd21be63ba3
