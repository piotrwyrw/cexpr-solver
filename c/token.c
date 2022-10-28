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

token *token_create(char c, token_type type, unsigned col) {
    token *t = malloc(sizeof(token));
    t->c = c;
    t->type = type;
    t->col = col;
    return t;
}

token *token_auto(char c, unsigned col) {
    token_type type = classify(c);
    if (type == token_unknown)
        printf("Warning: Failed to classify '%c'.\n", c);

    return token_create(c, type, col);
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