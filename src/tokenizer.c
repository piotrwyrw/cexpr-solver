//
// Created by Piotr Wyrwas on 10/24/2022.
//

#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

tokenizer *tokenizer_create(char *str, unsigned i) {
    tokenizer *t = malloc(sizeof(tokenizer));
    t->s = str;
    t->i = i;
    t->le = strlen(str);
    return t;
}

void tokenizer_destroy(tokenizer *t) {
    free(t);
}

token *tokenizer_next(tokenizer *t) {
    if (t->i >= t->le)
        return NULL;

    char c = t->s[t->i];

    return token_auto(c, ++ t->i);
}

_Bool tokenizer_has_next(tokenizer *t) {
    return t->i < t->le;
}