#include "h/tokenizer.h"
#include "h/util.h"
#include "h/parse.h"
#include "h/lookup.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

node *parse(char *s) {
    tokenizer *t = tokenizer_create(s, 0);

    parser *p = parser_create(t);

    parser_init(p);

    node *n = parser_parse(p);

    parser_destroy(p);
    tokenizer_destroy(t);

    return n;

}

#define LETTER_COUNT (int) (('Z' - 'A') + ('z' - 'a'))

void start_repl() {
    char *buffer = malloc(100+1);
    memset(buffer, '\0', 101);

    lookup_table *table = create_lookup_table(LETTER_COUNT);
    node *n = NULL;

    _Bool exit_repl = false;
    while (!exit_repl) {
        printf(">");

        fgets(buffer, 100, stdin);

        char *input = string_strip(buffer);

        if (!strcmp(input, ".exit")) {
            exit_repl = true;
            goto m_free;
        }

        n = parse(input);
        node_print_recurse(n, 0);

        if (!n)
            goto m_free;

        if (n->type == node_type_assignment) {
            lookup_insert_ast(table, n);
            printf("Appended '%c' to persistent memory.\n", n->assignment.var->variable.variable_name);
            node_destroy(n->assignment.var);
            free(n);
        }

        m_free:
        free(input);

        memset(buffer, '\0', 101);
    }

    free(buffer);
    lookup_destroy(table, node_destroy);
}

int main() {
    // Disable tokenizer warnings
    token_set_warning(false);

    printf("LogiC REPL v1.0\nType '.exit' to exit.\n");

    start_repl();

    return 0;
}
