//
// Created by Piotr Wyrwas on 10/24/2022.
//

#ifndef EXPR_SOLVER_TOKEN_H
#define EXPR_SOLVER_TOKEN_H

#include <stdbool.h>

#define IS_LETTER(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IS_IMMEDIATE(c) (c == '0' || c == '1')

typedef enum {
    token_lparen,
    token_rparen,

    token_or,
    token_and,
    token_xor,
    token_imply,
    token_not,

    token_letter,
    token_immediate,

    token_colon,
    token_equals,
    token_semi,

    token_lsquare,
    token_rsquare,

    token_unknown
} token_type;

token_type classify(char);
const char *token_type_to_string(token_type);

typedef struct {
    char c;
    token_type type;
    unsigned col;
} token;

token *token_create(char, token_type, unsigned);

token *token_auto(char, unsigned);

void token_destroy(token *);

_Bool token_compare(token *, token *);
_Bool token_quick_compare(token *, token_type);

char *token_string(token *);

#endif //EXPR_SOLVER_TOKEN_H
