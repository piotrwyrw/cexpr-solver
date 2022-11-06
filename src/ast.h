//
// Created by Piotr Wyrwas on 10/21/2022.
//

#ifndef EXPR_SOLVER_AST_H
#define EXPR_SOLVER_AST_H

#include "token.h"

typedef enum {
    node_type_binary,
    node_type_unary,
    node_type_variable,
    node_type_immediate,
    node_type_assignment,
    node_type_solve,

    node_type_unknown
} node_type;

const char *node_type_to_string(node_type);

typedef enum {
    binary_type_implication,
    binary_type_and,
    binary_type_or,
    binary_type_xor,

    binary_type_unknown
} binary_type;

const char *binary_type_to_string(binary_type);
binary_type binary_type_from(token_type);

typedef enum {
    final_on,
    final_off,

    final_unknown
} final_value;

const char *final_value_to_string(final_value);
_Bool final_value_bool(final_value);
final_value final_value_from(char);
final_value final_value_logic(final_value, final_value, binary_type);

typedef enum {
    unary_not,
    unary_unknown
} unary_type;

const char *unary_to_string(unary_type);
unary_type unary_type_from(token_type);

typedef struct node node;

struct node {
    node_type type;
    union {
        struct {
            node *left;
            node *right;
            binary_type type;
        } binary_op;
        struct {
            node *node;
            unary_type type;
        } unary_op;
        struct {
            char variable_name;
        } variable;
        struct {
            final_value value;
        } immediate;
        struct {
            node *var;
            node *val;
        } assignment;
        struct {
            node *val;
        } solve;
    };
};

void node_print_recurse(node *, int);

node *node_create_base(node_type);
void node_destroy(node *);

node *node_create_binary(node *, node *, binary_type);
void node_destroy_binary(node *);

node *node_create_unary(node *, unary_type);
void node_destroy_unary(node *);

node *node_create_variable(char);

node *node_create_immediate(final_value);

node *node_create_assignment(node *, node *);
void node_destroy_assignment(node *);

node *node_create_solve(node *);
void node_destroy_solve(node *);

node *node_create_immediate_auto(token *);

#endif //EXPR_SOLVER_AST_H
