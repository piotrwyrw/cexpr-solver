//
// Created by Piotr Wyrwas on 10/21/2022.
//

#ifndef EXPR_SOLVER_AST_H
#define EXPR_SOLVER_AST_H

#include "token.h"

#define __ignore [[nodiscard("Return value must not be ignored.")]]

typedef enum {
    node_type_binary,
    node_type_unary,
    node_type_variable,
    node_type_immediate,
    node_type_assignment,

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
    immediate_on,
    immediate_off,

    immediate_unknown
} immediate_value;

const char *immediate_value_to_string(immediate_value);
immediate_value immediate_value_from(char);

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
            immediate_value value;
        } immediate;
        struct {
            node *var;
            node *val;
        } assignment;
    };
};

void node_print_recurse(node *, int);

__ignore node *node_create_base(node_type);
void node_destroy(node *);

__ignore node *node_create_binary(node *, node *, binary_type);
void node_destroy_binary(node *);

__ignore node *node_create_unary(node *, unary_type);
void node_destroy_unary(node *);

__ignore node *node_create_variable(char);

__ignore node *node_create_immediate(immediate_value);

__ignore node *node_create_assignment(node *, node *);
void node_destroy_assignment(node *);

node *node_create_immediate_auto(token *);

#endif //EXPR_SOLVER_AST_H
