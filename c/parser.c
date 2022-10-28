//
// Created by Piotr Wyrwas on 10/24/2022.
//

#include "../h/parse.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

parser *parser_create(tokenizer *t) {
    parser *p = malloc(sizeof(parser));
    p->t = t;
    p->c = NULL;
    p->n = NULL;
    return p;
}

void parser_destroy(parser *p) {
    if (p->c != NULL)
        token_destroy(p->c);

    if (p->n != NULL)
        token_destroy(p->n);

    free(p);
}

void parser_init(parser *p) {
    parser_consume(p);
    parser_consume(p);
}

_Bool parser_consume(parser *p) {
    _Bool state = true;
    if (p->c != NULL)
        token_destroy(p->c);
    p->c = p->n;
    if (p->c == NULL)
        state = false;
    if (tokenizer_has_next(p->t))
        p->n = tokenizer_next(p->t);
    else
        p->n = NULL;

    return state;
}

_Bool parser_cmp(parser *p, token_type t) {
    return token_quick_compare(p->c, t);
}

_Bool parser_cmp_next(parser *p, token_type t) {
    return token_quick_compare(p->n, t);
}

_Bool parser_cmp_var(parser *p, char _c, int n, ...) {
    if ((_c == CURRENT_TOKEN && p->c == NULL) || (_c == NEXT_TOKEN && p->n == NULL))
        return false;

    va_list l;
    va_start(l, n);;

    _Bool state = false;

    for (int i = 0; i < n; i ++)
        if (_c == 'c')
            state = p->c->type == va_arg(l, token_type) ? true : state;
        else
            state = p->n->type == va_arg(l, token_type) ? true : state;

    va_end(l);

    return state;
}

node *parser_parse(parser *p) {
    return parser_parse_first_degree(p);
}

// a |^> b
node *parser_parse_first_degree(parser *p) {
    node *left = parser_parse_second_degree(p);

    if (!left)
        return NULL;

    if (parser_cmp_var(p, NEXT_TOKEN, 3, token_or, token_xor, token_imply))
        parser_consume(p);
    else if (p->n) {
        printf("Syntax error: Unknown operator: %s\n", token_type_to_string(p->n->type));
        return NULL;
    }

    while (parser_cmp_var(p, CURRENT_TOKEN, 3, token_or, token_xor, token_imply)) {
        binary_type bop = binary_type_from(p->c->type);

        parser_consume(p);

        node *right = parser_parse_first_degree(p);

        if (!right) {
            node_destroy(left);
            return NULL;
        }

        node *tmp = node_create_binary(left, right, bop);

        left = tmp;

        if (parser_cmp_var(p, NEXT_TOKEN, 3, token_or, token_xor, token_imply))
            parser_consume(p);

    }

    return left;
}

node *parser_parse_second_degree(parser *p) {
    node *left = parser_parse_atom(p);

    if (!left)
        return NULL;

    if (parser_cmp_next(p, token_and))
        parser_consume(p);

    while (parser_cmp(p, token_and)) {
        binary_type bop = binary_type_from(p->c->type);

        parser_consume(p);

        node *right = parser_parse_first_degree(p);
        if (!right) {
            node_destroy(left);
            return NULL;
        }

        node *tmp = node_create_binary(left, right, bop);


        left = tmp;

        if (parser_cmp_next(p, token_and))
            parser_consume(p);
    }

    return left;
}

node *parser_parse_atom(parser *p) {
    if (parser_cmp(p, token_letter))
        return node_create_variable(p->c->c);

    if (parser_cmp(p, token_immediate))
        return node_create_immediate(immediate_value_from(p->c->c));

    if (parser_cmp(p, token_lparen)) {
        parser_consume(p);
        node *expr = parser_parse_first_degree(p);
        parser_consume(p);
        if (!parser_cmp(p, token_rparen)) {
            printf("Expected closing parentheses after the expression.\n");
            return NULL;
        }
        return expr;
    }

    return NULL;

}