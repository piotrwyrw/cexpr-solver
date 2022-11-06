//
// Created by Piotr Wyrwas on 11/6/2022.
//

#ifndef EXPR_SOLVER_SOLVER_H
#define EXPR_SOLVER_SOLVER_H

#include "ast.h"
#include "lookup.h"

// Intended solver entry point
final_value solve_recurse(lookup_table *, node *);

// Called internally [part of recursive call chain]
final_value solve_binary(lookup_table *, node *);

// Called internally [part of recursive call chain]
final_value solve_atom(lookup_table *, node *);

#endif //EXPR_SOLVER_SOLVER_H
