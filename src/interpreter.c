// interpreter.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS     256
#define MAX_ALGOS    128
#define MAX_STACK    128
#define MAX_PARAMS   16

/* -------------------- Types de valeurs -------------------- */

typedef enum {
    VAL_INT,
    VAL_BOOL
} ValueKind;

typedef struct {
    ValueKind kind;
    int       i;    // utilisé pour int et bool (0/1)
} Value;

/* -------------------- Variables & environnement -------------------- */

typedef struct {
    char *name;
    Value value;
} Var;

typedef struct {
    Var vars[MAX_VARS];
    int var_count;
} Environment;

/* Pile d’environnements (pile d’appels) */
static Environment call_stack[MAX_STACK];
static int sp = -1; // stack pointer

/* -------------------- AST minimal (à raccorder au parser plus tard) -------------------- */

typedef enum {
    EXPR_INT,
    EXPR_BOOL,
    EXPR_IDENT,
    EXPR_BINOP,
    EXPR_UNOP
} ExprKind;

typedef enum {
    OP_PLUS, OP_MINUS, OP_TIMES, OP_DIV, OP_MOD,
    OP_EQ, OP_NEQ, OP_LT, OP_LE, OP_GT, OP_GE,
    OP_AND, OP_OR,
    OP_NOT, OP_UMINUS
} OpKind;

typedef struct Expr {
    ExprKind kind;
    union {
        int   i;        // INT
        bool  b;        // TRUE/FALSE
        char *ident;    // IDENTIFIER
        struct {
            OpKind op;
            struct Expr *left;
            struct Expr *right;
        } bin;
        struct {
            OpKind op;
            struct Expr *sub;
        } un;
    } u;
} Expr;

typedef enum {
    STMT_SET,
    STMT_IF,
    STMT_DOWHILE,
    STMT_DOFORI,
    STMT_CALL,
    STMT_RETURN,
    STMT_BLOCK
} StmtKind;

struct Statement;

typedef struct StmtList {
    struct Statement *stmt;
    struct StmtList  *next;
} StmtList;

typedef struct Statement {
    StmtKind kind;
    union {
        struct {              // SET x := expr
            char *name;
            Expr *expr;
        } set;
        struct {              // IF cond then blk [else blk]
            Expr     *cond;
            struct Statement *then_block;
            struct Statement *else_block; // peut être NULL
        } ifs;
        struct {              // DOWHILE cond blk
            Expr     *cond;
            struct Statement *block;
        } dowhile;
        struct {              // DOFORI x {a}{b} blk
            char *var;
            Expr *start;
            Expr *end;
            struct Statement *block;
        } dofori;
        struct {              // CALL f(args)
            char  *name;
            Expr **args;
            int    arg_count;
        } call;
        struct {              // RETURN expr
            Expr *expr;
        } ret;
        struct {              // BLOCK { stmt_list }
            StmtList *list;
        } block;
    } u;
} Statement;

/* -------------------- Algorithmes -------------------- */

typedef struct {
    char  *name;
    char  *params[MAX_PARAMS];
    int    param_count;
    Statement *body;   // bloc principal de l’algo
} Algo;

static Algo algos[MAX_ALGOS];
static int  algo_count = 0;

/* -------------------- Gestion des algos -------------------- */

int find_algo(const char *name) {
    for (int i = 0; i < algo_count; ++i) {
        if (strcmp(algos[i].name, name) == 0)
            return i;
    }
    return -1;
}

/* À appeler depuis le parser quand on termine un \begin{algo}...\end{algo} */
int register_algo(const char *name, char **params, int param_count, Statement *body) {
    if (algo_count >= MAX_ALGOS) {
        fprintf(stderr, "Erreur: trop d’algorithmes\n");
        exit(1);
    }
    algos[algo_count].name = strdup(name);
    algos[algo_count].param_count = param_count;
    for (int i = 0; i < param_count; ++i)
        algos[algo_count].params[i] = strdup(params[i]);
    algos[algo_count].body = body;
    return algo_count++;
}

/* -------------------- Gestion des variables -------------------- */

int find_var(Environment *env, const char *name) {
    for (int i = 0; i < env->var_count; ++i) {
        if (strcmp(env->vars[i].name, name) == 0)
            return i;
    }
    return -1;
}

Value get_var(const char *name) {
    if (sp < 0) {
        fprintf(stderr, "Erreur: accès variable hors de tout environnement: %s\n", name);
        exit(1);
    }
    Environment *env = &call_stack[sp];
    int idx = find_var(env, name);
    if (idx < 0) {
        fprintf(stderr, "Erreur: variable non déclarée: %s\n", name);
        exit(1);
    }
    return env->vars[idx].value;
}

void set_var(const char *name, Value v) {
    if (sp < 0) {
        fprintf(stderr, "Erreur: set_var hors de tout environnement: %s\n", name);
        exit(1);
    }
    Environment *env = &call_stack[sp];
    int idx = find_var(env, name);
    if (idx < 0) {
        if (env->var_count >= MAX_VARS) {
            fprintf(stderr, "Erreur: trop de variables\n");
            exit(1);
        }
        env->vars[env->var_count].name = strdup(name);
        env->vars[env->var_count].value = v;
        env->var_count++;
    } else {
        env->vars[idx].value = v;
    }
}

/* -------------------- Exécution -------------------- */

static bool  returning = false;
static Value return_value;

Value eval_expr(Expr *e);
void  exec_stmt(Statement *s);
void  exec_block(Statement *block);

/* Appel d’un algorithme AlgoSIPRO */
Value call_algo(const char *name, Value *args, int arg_count) {
    int idx = find_algo(name);
    if (idx < 0) {
        fprintf(stderr, "Erreur: algo inconnu: %s\n", name);
        exit(1);
    }

    Algo *a = &algos[idx];

    if (a->param_count != arg_count) {
        fprintf(stderr, "Erreur: nombre d’arguments incorrect pour %s (attendu %d, reçu %d)\n",
                name, a->param_count, arg_count);
        exit(1);
    }

    if (sp + 1 >= MAX_STACK) {
        fprintf(stderr, "Erreur: dépassement de pile d’appels\n");
        exit(1);
    }

    /* Nouveau cadre d’environnement */
    sp++;
    call_stack[sp].var_count = 0;

    /* Lier les paramètres formels aux valeurs effectives */
    for (int i = 0; i < a->param_count; ++i) {
        set_var(a->params[i], args[i]);
    }

    /* Exécuter le corps */
    returning = false;
    exec_block(a->body);

    /* Valeur de retour */
    Value v;
    if (!returning) {
        v.kind = VAL_INT;
        v.i    = 0;
    } else {
        v = return_value;
    }

    /* Sortie de l’environnement */
    sp--;

    return v;
}

/* Parcours d’un bloc { stmt_list } */
void exec_block(Statement *block) {
    if (!block) return;
    if (block->kind != STMT_BLOCK) {
        exec_stmt(block);
        return;
    }
    StmtList *cur = block->u.block.list;
    while (cur && !returning) {
        exec_stmt(cur->stmt);
        cur = cur->next;
    }
}

/* Exécution d’une instruction */
void exec_stmt(Statement *s) {
    if (!s || returning) return;

    switch (s->kind) {
    case STMT_SET: {
        Value v = eval_expr(s->u.set.expr);
        set_var(s->u.set.name, v);
        break;
    }
    case STMT_IF: {
        Value c = eval_expr(s->u.ifs.cond);
        if (c.kind != VAL_BOOL) {
            fprintf(stderr, "Erreur: condition IF non booléenne\n");
            exit(1);
        }
        if (c.i)
            exec_block(s->u.ifs.then_block);
        else if (s->u.ifs.else_block)
            exec_block(s->u.ifs.else_block);
        break;
    }
    case STMT_DOWHILE: {
        do {
            exec_block(s->u.dowhile.block);
            if (returning) break;
            Value c = eval_expr(s->u.dowhile.cond);
            if (c.kind != VAL_BOOL) {
                fprintf(stderr, "Erreur: condition DOWHILE non booléenne\n");
                exit(1);
            }
            if (!c.i) break;
        } while (1);
        break;
    }
    case STMT_DOFORI: {
        Value start = eval_expr(s->u.dofori.start);
        Value end   = eval_expr(s->u.dofori.end);
        if (start.kind != VAL_INT || end.kind != VAL_INT) {
            fprintf(stderr, "Erreur: bornes DOFORI non entières\n");
            exit(1);
        }
        for (int k = start.i; k <= end.i && !returning; ++k) {
            Value vk; vk.kind = VAL_INT; vk.i = k;
            set_var(s->u.dofori.var, vk);
            exec_block(s->u.dofori.block);
        }
        break;
    }
    case STMT_CALL: {
        Value args[MAX_PARAMS];
        for (int i = 0; i < s->u.call.arg_count; ++i)
            args[i] = eval_expr(s->u.call.args[i]);
        (void)call_algo(s->u.call.name, args, s->u.call.arg_count);
        break;
    }
    case STMT_RETURN: {
        return_value = eval_expr(s->u.ret.expr);
        returning = true;
        break;
    }
    case STMT_BLOCK:
        exec_block(s);
        break;
    }
}

/* Évaluation d’une expression */
Value eval_expr(Expr *e) {
    Value v;
    switch (e->kind) {
    case EXPR_INT:
        v.kind = VAL_INT;
        v.i    = e->u.i;
        return v;
    case EXPR_BOOL:
        v.kind = VAL_BOOL;
        v.i    = e->u.b ? 1 : 0;
        return v;
    case EXPR_IDENT:
        return get_var(e->u.ident);
    case EXPR_UNOP: {
        Value sub = eval_expr(e->u.un.sub);
        v.kind = VAL_INT;
        switch (e->u.un.op) {
        case OP_NOT:
            if (sub.kind != VAL_BOOL) {
                fprintf(stderr, "Erreur: NOT sur non-bool\n");
                exit(1);
            }
            v.kind = VAL_BOOL;
            v.i = !sub.i;
            return v;
        case OP_UMINUS:
            if (sub.kind != VAL_INT) {
                fprintf(stderr, "Erreur: - unaire sur non-int\n");
                exit(1);
            }
            v.i = -sub.i;
            return v;
        default:
            fprintf(stderr, "Erreur: opérateur unaire inconnu\n");
            exit(1);
        }
    }
    case EXPR_BINOP: {
        Value a = eval_expr(e->u.bin.left);
        Value b = eval_expr(e->u.bin.right);
        v.kind = VAL_INT;
        switch (e->u.bin.op) {
        case OP_PLUS:  v.i = a.i + b.i; return v;
        case OP_MINUS: v.i = a.i - b.i; return v;
        case OP_TIMES: v.i = a.i * b.i; return v;
        case OP_DIV:
            if (b.i == 0) {
                fprintf(stderr, "Erreur: division par zéro\n");
                exit(1);
            }
            v.i = a.i / b.i; return v;
        case OP_MOD:
            if (b.i == 0) {
                fprintf(stderr, "Erreur: modulo par zéro\n");
                exit(1);
            }
            v.i = a.i % b.i; return v;
        case OP_EQ:  v.kind = VAL_BOOL; v.i = (a.i == b.i); return v;
        case OP_NEQ: v.kind = VAL_BOOL; v.i = (a.i != b.i); return v;
        case OP_LT:  v.kind = VAL_BOOL; v.i = (a.i <  b.i); return v;
        case OP_LE:  v.kind = VAL_BOOL; v.i = (a.i <= b.i); return v;
        case OP_GT:  v.kind = VAL_BOOL; v.i = (a.i >  b.i); return v;
        case OP_GE:  v.kind = VAL_BOOL; v.i = (a.i >= b.i); return v;
        case OP_AND:
            v.kind = VAL_BOOL; v.i = (a.i && b.i); return v;
        case OP_OR:
            v.kind = VAL_BOOL; v.i = (a.i || b.i); return v;
        default:
            fprintf(stderr, "Erreur: opérateur binaire inconnu\n");
            exit(1);
        }
    }
    }
    fprintf(stderr, "Erreur: expression inconnue\n");
    exit(1);
}

/* -------------------- Point d’entrée d’exécution -------------------- */

void run_final_call(const char *name, Value *args, int arg_count) {
    Value res = call_algo(name, args, arg_count);
    if (res.kind == VAL_BOOL)
        printf("%s\n", res.i ? "true\n" : "false\n");
    else
        printf("%d\n", res.i);
}
