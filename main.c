#include "h/tokenizer.h"
#include "h/util.h"
#include "h/parse.h"

#include <stdlib.h>

int main() {
    const char *text = "a/b";
    char *actual_text = string_strip(text);

    tokenizer *t = tokenizer_create(actual_text, 0);

    parser *p = parser_create(t);

    // Disable tokenizer warnings
    token_set_warning(false);

    parser_init(p);

    node *n = parser_parse_first_degree(p);
    node_print_recurse(n, 0);

    parser_destroy(p);
    tokenizer_destroy(t);

    free(actual_text);
    return 0;
}
