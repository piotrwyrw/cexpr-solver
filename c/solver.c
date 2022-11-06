//
// Created by Piotr Wyrwas on 11/6/2022.
//

#include "../h/solver.h"
#include "../h/settings.h"

#include <stdio.h>

final_value solve_recurse(lookup_table *t, node *n) {
    node_type type = n->type;
    switch (type) {
        case node_type_binary:
            return solve_binary(t, n);
        case node_type_variable:
        case node_type_immediate:
            return solve_atom(t, n);
        default:
            DEBUG("Solver does not support this node type: %s\n", node_type_to_string(n->type));
            return final_unknown;
    }
}

final_value solve_binary(lookup_table *t, node *n) {
    if (n->type != node_type_binary) {
        DEBUG("Binary expression node expected.\n");
        return final_unknown;
    }

    binary_type op = n->binary_op.type;
    final_value left = solve_recurse(t, n->binary_op.left);
    final_value right = solve_recurse(t, n->binary_op.right);

    if (left == final_unknown || right == final_unknown) {
        DEBUG("Failed to solve binary: Left or right nodes returned undefined final.\n");
        return final_unknown;
    }

    return final_value_logic(left, right, op);
}

final_value solve_variable(lookup_table *t, node *n) {
    char c = n->variable.variable_name;
    int i = lookup_get(t, c);
    if (i < 0) {
        DEBUG("Requested variable is not defined and hence cannot be used: %c\n", n->variable.variable_name);
        return final_unknown;
    }

    final_value val = solve_recurse(t, t->table[i]->val);
    if (val == final_unknown) {
        DEBUG("Solving the variable '%c' yielded unknown final.\n", n->variable.variable_name);
        return final_unknown;
    }

    return val;
}

final_value solve_atom(lookup_table *t, node *n) {
    if (n->type == node_type_immediate) {
        return n->immediate.value;
    }

    if (n->type == node_type_variable) {
        return solve_variable(t, n);
    }

    DEBUG("Failed to solve unsupported atom type: '%s'\n", node_type_to_string(n->type));
    return final_unknown;
}