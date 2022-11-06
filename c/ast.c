//
// Created by Piotr Wyrwas on 10/21/2022.
//

#include "../h/ast.h"
#include "../h/util.h"
#include "../h/settings.h"

#include <stdlib.h>
#include <stdio.h>

#define AUTO(c) case c: return #c;
#define STR_UNKNOWN "(unknown)"

const char *node_type_to_string(node_type type) {
    switch (type) {
        default: return STR_UNKNOWN;
        AUTO(node_type_binary);
        AUTO(node_type_unary);
        AUTO(node_type_variable)
        AUTO(node_type_immediate)
        AUTO(node_type_assignment)
        AUTO(node_type_unknown)
    }
}

const char *binary_type_to_string(binary_type type) {
    switch (type) {
        default: return STR_UNKNOWN;
        AUTO(binary_type_implication)
        AUTO(binary_type_and)
        AUTO(binary_type_or)
        AUTO(binary_type_xor)
        AUTO(binary_type_unknown)
    }
}

binary_type binary_type_from(token_type type) {
    switch (type) {
        case token_imply: return binary_type_implication;
        case token_and: return binary_type_and;
        case token_or: return binary_type_or;
        case token_xor: return binary_type_xor;
        default: return binary_type_unknown;
    }
}

const char *final_value_to_string(final_value val) {
    switch (val) {
        default: return STR_UNKNOWN;
        AUTO(final_on)
        AUTO(final_off)
        AUTO(final_unknown)
    }
}

_Bool final_value_bool(final_value v) {
    switch (v) {
        case final_off: return false;
        case final_on: return true;
        default:
            DEBUG("Unknown final passed. Assuming [false].\n");
            return false;
    }
}

final_value final_value_from(char c) {
    switch (c) {
        case '1':
        case (char) 1:
            return final_on;
        case '0':
        case (char) 0:
            return final_off;
        default: return final_unknown;
    }
}

final_value final_value_logic(final_value a, final_value b, binary_type bin) {
    if (a == final_unknown || b == final_unknown) {
        DEBUG("Could not perform logical operation: Either of the two finals are unknown.\n");
        return final_unknown;
    }

    if (bin == binary_type_unknown) {
        DEBUG("Could not perform logical operation: The operation type must not be unknown.\n");
        return final_unknown;
    }

    _Bool bool_a = final_value_bool(a);
    _Bool bool_b = final_value_bool(b);
    _Bool bool_c;

    switch (bin) {
        case binary_type_implication:
            bool_c = !bool_a || bool_b;
            break;
        case binary_type_xor:
            bool_c = (bool_a && !bool_b) || (!bool_a && bool_b);
            break;
        case binary_type_and:
            bool_c = bool_a && bool_b;
            break;
        case binary_type_or:
            bool_c = bool_a || bool_b;
            break;
    }

    return final_value_from((char) bool_c);
}

const char *unary_to_string(unary_type type) {
    switch (type) {
        AUTO(unary_not)
        default: return STR_UNKNOWN;
    }
}

#undef STR_UNKNOWN
#undef AUTO

unary_type unary_type_from(token_type type) {
    switch (type) {
        default: return unary_unknown;
        case token_not: return unary_not;
    }
}

#define _PRINT(x, ...) \
        string_rep(&padding, ' ', d*3); \
        printf("%s", padding);               \
        printf(x, ##__VA_ARGS__);

void node_print_recurse(node *n, int d) {
    char *padding = NULL;

    if (n == NULL) {
        _PRINT("<Null Node>\n");
        goto end;
    }

    switch (n->type) {
        case node_type_binary:
            _PRINT("<Binary Operation> :: %s\n", binary_type_to_string(n->binary_op.type));
            d ++;
                _PRINT("Left ->\n");
                d ++;
                    node_print_recurse(n->binary_op.left, d);
                d --;
                _PRINT("Right ->\n");
                d ++;
                    node_print_recurse(n->binary_op.right, d);
                d --;
            d --;
            break;
        case node_type_variable:
            _PRINT("<Variable> :: %c\n", n->variable.variable_name);
            break;
        case node_type_immediate:
            _PRINT("<Immediate> :: %s\n", final_value_to_string(n->immediate.value));
            break;
        case node_type_unary:
            _PRINT("<Unary> :: %s\n", unary_to_string(n->unary_op.type));
            d ++;
                _PRINT("Expression ->\n");
                d ++;
                    node_print_recurse(n->unary_op.node, d);
                d --;
            d --;
            break;
        case node_type_assignment:
            _PRINT("<Assignment>\n");
            d ++;
                _PRINT("Destination ->\n");
                d ++;
                    node_print_recurse(n->assignment.var, d);
                d --;
                _PRINT("Source ->\n");
                d ++;
                    node_print_recurse(n->assignment.val, d);
                d --;
            d --;
            break;
        case node_type_unknown:
            _PRINT("<Unknown>\n");
            break;
        case node_type_solve:
            _PRINT("<Solve>\n");
            d ++;
                _PRINT("Expression ->\n");
                d ++;
                    node_print_recurse(n->solve.val, d);
                d --;
            d --;
            break;
        default:
            _PRINT("<Unsupported>\n");
            break;
    }

    end:

    free(padding);
}

#undef _PRINT

node *node_create_base(node_type type) {
    node *n = malloc(sizeof(node));
    n->type = type;
    return n;
}

void node_destroy(node *node) {
    switch (node->type) {
        case node_type_unary:
            node_destroy_unary(node);
            break;
        case node_type_binary:
            node_destroy_binary(node);
            break;
        case node_type_assignment:
            node_destroy_assignment(node);
            break;
        case node_type_solve:
            node_destroy_solve(node);
            break;
        default:
            break;
    }
    free(node);
}

node *node_create_binary(node *left, node *right, binary_type type) {
    node *n = node_create_base(node_type_binary);
    n->binary_op.left = left;
    n->binary_op.right = right;
    n->binary_op.type = type;
}

void node_destroy_binary(node *n) {
    node_destroy(n->binary_op.left);
    node_destroy(n->binary_op.right);
}

node *node_create_unary(node *n, unary_type type) {
    node *node = node_create_base(node_type_unary);
    node->unary_op.type = type;
    node->unary_op.node = n;
    return node;
}

void node_destroy_unary(node *node) {
    node_destroy(node->unary_op.node);
}

node *node_create_variable(char name) {
    node *n = node_create_base(node_type_variable);
    n->variable.variable_name = name;
    return n;
}

node *node_create_immediate(final_value v) {
    node *n = node_create_base(node_type_immediate);
    n->immediate.value = v;
    return n;
}

node *node_create_assignment(node *var, node *val) {
    node *n = node_create_base(node_type_assignment);
    n->assignment.var = var;
    n->assignment.val = val;
    return n;
}

void node_destroy_assignment(node *n) {
    node_destroy(n->assignment.var);
    node_destroy(n->assignment.val);
}

node *node_create_solve(node *expr) {
    node *n = node_create_base(node_type_solve);
    n->solve.val = expr;
    return n;
}

void node_destroy_solve(node *n) {
    node_destroy(n->solve.val);
}

node *node_create_immediate_auto(token *t) {
    if (t == NULL)
        return NULL;

    if (t->type != token_immediate)
        return NULL;

    final_value v;
    if (t->c == '1') v = final_on;
    else if (t->c == '0') v = final_off;
    else { printf("Invalid immediate value: '%c'.\n", t->c); return NULL; }

    return node_create_immediate(v);
}
