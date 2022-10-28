//
// Created by Piotr Wyrwas on 10/24/2022.
//

#include "../h/token.h"

#include <stdlib.h>
#include <stdio.h>

token_type classify(char c) {
    if (IS_LETTER(c))
        return token_letter;

    if (IS_IMMEDIATE(c))
        return token_immediate;

    if (c == '(') return token_lparen;
    if (c == ')') return token_rparen;
    if (c == '|') return token_or;
    if (c == '&') return token_and;
    if (c == '^') return token_xor;
    if (c == '>') return token_imply;
    if (c == '!') return token_not;

    return token_unknown;
}

#define AUTO(c) case c: return #c;
#define STR_UNKNOWN "(unknown)"

char *token_type_to_string(token_type t) {
    switch (t) {
        default: return STR_UNKNOWN;
        AUTO(token_lparen)
        AUTO(token_rparen)
        AUTO(token_or)
        AUTO(token_and)
        AUTO(token_xor)
        AUTO(token_imply)
        AUTO(token_not)
        AUTO(token_letter)
        AUTO(token_immediate)
        AUTO(token_unknown)
    }
}

#undef AUTO
#undef STR_UNKNOWN

token *token_create(char c, token_type type, unsigned col) {
    token *t = malloc(sizeof(token));
    t->c = c;
    t->type = type;
    t->col = col;
    return t;
}

_Bool token_show_warn_classification = true;

token *token_auto(char c, unsigned col) {
    token_type type = classify(c);
    if (type == token_unknown && token_show_warn_classification)
        printf("Warning: Failed to classify '%c'.\n", c);

    return token_create(c, type, col);
}

void token_set_warning(_Bool w) {
    token_show_warn_classification = w;
}

void token_destroy(token *t) {
    free(t);
}

_Bool token_compare(token *a, token *b) {
    if (a == NULL || b == NULL)
        return false;
    return (a->type == b->type && a->c == b->c);
}

_Bool token_quick_compare(token *t, token_type _t) {
    if (t == NULL)
        return false;
    return _t == t->type;
}