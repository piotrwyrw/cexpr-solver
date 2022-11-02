#include "h/tokenizer.h"
#include "h/util.h"
#include "h/parse.h"
#include "h/lookup.h"
#include "h/settings.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LETTER_COUNT (int) (('Z' - 'A') + ('z' - 'a'))

node *parse(char *s) {
    tokenizer *t = tokenizer_create(s, 0);

    parser *p = parser_create(t);

    parser_init(p);

    node *n = parser_parse(p);

    parser_destroy(p);
    tokenizer_destroy(t);

    return n;

}

void lookup_write(lookup_table *t, node *n) {
    int ix = lookup_get(t, n->assignment.var->variable.variable_name);
    if (lookup_insert_ast(t, n) >= 0) {
        if (ix < 0 && cfg_debug_info)
            printf("Appended '%c' to persistent memory.\n", n->assignment.var->variable.variable_name);
        else if (cfg_debug_info)
            printf("Modified value of '%c'.\n", n->assignment.var->variable.variable_name);
        node_destroy(n->assignment.var);
        free(n);
    } else {
        printf("Failed to write to the lookup table.\n");
        node_destroy(n);
    }
}

void handle_command(char *s) {
    if (!strcmp(s, ".help")) {
        printf("----------\n");
        printf("(.exit) :: Exit the REPL.\n");
        printf("(.show-debug) :: Show extended debug information\n");
        printf("(.hide-debug) :: Hide debug information.\n");
        printf("(.show-ast) :: Show the AST Tree when evaluating. For debugging purposes.\n");
        printf("(.hide-ast) :: Don't display the AST anymore.\n");
        printf("(.help) :: Display this page.\n");
        printf("(.info) :: Info about LogiC.\n");
        printf("----------\n");
        return;
    }

    if (!strcmp(s, ".exit")) {
        stat_exit_repl = true;
        printf("Exiting ..\n");
        return;
    }

    if (!strcmp(s, ".show-debug")) {
        cfg_debug_info = true;
        printf("Debug info will be displayed from now on.\n");
        return;
    }

    if (!strcmp(s, ".hide-debug")) {
        cfg_debug_info = false;
        printf("Debug info won't be displayed anymore.\n");
        return;
    }

    if (!strcmp(s, ".show-ast")) {
        cfg_show_ast = true;
        printf("The AST will be displayed from now on.\n");
        return;
    }

    if (!strcmp(s, ".hide-ast")) {
        cfg_show_ast = false;
        printf("The AST won't be displayed anymore.\n");
        return;
    }

    if (!strcmp(s, ".info")) {
        printf("%s by %s\n", const_pkg_name, const_author);
        printf("Version v%s\n", const_version);
        return;
    }

    printf("Unknown command. Use '.help' for help.\n");

}

void start_repl() {
    char *buffer = malloc(100+1);
    memset(buffer, '\0', 101);

    lookup_table *table = create_lookup_table(LETTER_COUNT, node_destroy);
    node *n = NULL;

    while (!stat_exit_repl) {
        printf(">");

        fgets(buffer, 100, stdin);

        char *input = string_strip(buffer);

        if (input[0] == '.') {
            handle_command(input);
            goto m_free;
        }

        n = parse(input);

        if (cfg_show_ast)
            node_print_recurse(n, 0);

        if (!n)
            goto m_free;

        if (n->type == node_type_assignment)
            lookup_write(table, n);

        m_free:
        free(input);

        memset(buffer, '\0', 101);
    }

    free(buffer);
    lookup_destroy(table);
}

int main() {
    printf("LogiC REPL v1.0\nType '.exit' to exit.\n");

    start_repl();

    return 0;
}
