//
// Created by Piotr Wyrwas on 10/24/2022.
//

#ifndef EXPR_SOLVER_TOKENIZER_H
#define EXPR_SOLVER_TOKENIZER_H

#include "token.h"

#include <stdbool.h>

typedef struct {
    char *s;
    unsigned i;
    unsigned le;
} tokenizer;

tokenizer *tokenizer_create(char *, unsigned);
void tokenizer_destroy(tokenizer *);

token *tokenizer_next(tokenizer *);

_Bool tokenizer_has_next(tokenizer *);

#endif //EXPR_SOLVER_TOKENIZER_H
