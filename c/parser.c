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

#define SYNTAX_ERR(p, x, ...) \
        printf("[Col. %d] Syntax error: ", p->t->i); \
        printf(x, ##__VA_ARGS__);


node *parser_parse(parser *p) {
    if (!p->c) {
        return NULL;
    }

    node *tmp = NULL;

    // Assignment
    if (parser_cmp(p, token_letter) && parser_cmp_next(p, token_equals)) {
        tmp = parser_parse_assignment(p);
        goto do_return;
    }

    tmp = parser_parse_first_degree(p);

    do_return:
    if (p->n) {
        SYNTAX_ERR(p, "Expected a single statement per line.\n");
        if (tmp) node_destroy(tmp);
        return NULL;
    }
    return tmp;
}

node *parser_parse_assignment(parser *p) {
    if (!parser_cmp(p, token_letter)) {
        SYNTAX_ERR(p, "Expected an identifier.\n")
        return NULL;
    }

    char id = p->c->c;

    parser_consume(p);

    if (!parser_cmp(p, token_equals)) {
        SYNTAX_ERR(p, "Expected an '=' operator.\n");
        return NULL;
    }

    parser_consume(p);

    node *expr = parser_parse_first_degree(p);

    if (!expr) {
        return NULL;
    }

    return node_create_assignment(node_create_variable(id), expr);
}

node *parser_parse_first_degree(parser *p) {
    node *left = parser_parse_second_degree(p);

    if (!left)
        return NULL;

    if (parser_cmp_var(p, NEXT_TOKEN, 3, token_or, token_xor, token_imply))
        parser_consume(p);

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

        node *right = parser_parse_second_degree(p);
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
        if (!expr) {
            return NULL;
        }
        parser_consume(p);
        if (!parser_cmp(p, token_rparen)) {
            char *tok_s = token_string(p->c);
            SYNTAX_ERR(p, "Expected closing parentheses after the expression, got '%s' instead.\n", tok_s);
            free(tok_s);
            return NULL;
        }
        return expr;
    }

    char *tok_s = token_string(p->c);
    printf("[Col. %d] Syntax error: Invalid atom: '%s'\n", p->t->i,  tok_s);
    free(tok_s);

    return NULL;

}