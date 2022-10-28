//
// Created by Piotr Wyrwas on 10/24/2022.
//

#ifndef EXPR_SOLVER_PARSE_H
#define EXPR_SOLVER_PARSE_H

#include "tokenizer.h"
#include "ast.h"

#define CURRENT_TOKEN 'c'
#define NEXT_TOKEN 'n'

typedef struct {
    tokenizer *t;
    token *c;
    token *n;
} parser;

parser *parser_create(tokenizer *);
void parser_destroy(parser *);
void parser_init(parser *);

_Bool parser_consume(parser *);

_Bool parser_cmp(parser *, token_type);
_Bool parser_cmp_next(parser *, token_type);
_Bool parser_cmp_var(parser *, char, int, ...);

node *parser_parse(parser *);

node *parser_parse_assignment(parser *);

node *parser_parse_first_degree(parser *);  // Low precedence
node *parser_parse_second_degree(parser *); // Higher precedence
node *parser_parse_atom(parser *);          // Highest precedence

void parser_panic(parser *);

#endif //EXPR_SOLVER_PARSE_H
