#include "h/tokenizer.h"
#include "h/util.h"
#include "h/parse.h"

#include <stdlib.h>

int main() {
    const char *text = "a&b|c";
    char *actual_text = string_strip(text);

    tokenizer *t = tokenizer_create(actual_text, 0);

    parser *p = parser_create(t);

    // Disable tokenizer warnings
    token_set_warning(false);

    parser_init(p);

    node *n = parser_parse(p);
    node_print_recurse(n, 0);

    if (n)
        node_destroy(n);

    parser_destroy(p);
    tokenizer_destroy(t);

    free(actual_text);
    return 0;
}
