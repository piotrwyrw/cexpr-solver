#include "h/tokenizer.h"
#include "h/util.h"
#include "h/parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parse(char *s) {
    tokenizer *t = tokenizer_create(s, 0);

    parser *p = parser_create(t);

    parser_init(p);

    node *n = parser_parse(p);
    node_print_recurse(n, 0);

    if (n)
        node_destroy(n);

    parser_destroy(p);
    tokenizer_destroy(t);
}

void start_repl() {
    char *buffer = malloc(100+1);
    _Bool exit_repl = false;
    while (!exit_repl) {
        string_clean(buffer);

        printf(">");

        fgets(buffer, 100, stdin);

        char *input = string_strip(buffer);

        if (!strcmp(input, ".exit")) {
            exit_repl = true;
            goto m_free;
        }

        parse(input);

        m_free:
        free(input);
    }
    free(buffer);
}

int main() {
    // Disable tokenizer warnings
    token_set_warning(false);

    printf("LogiC REPL v1.0\nType '.exit' to exit.\n");

    start_repl();

    return 0;
}
