//
// Created by Piotr Wyrwas on 11/2/2022.
//

#ifndef EXPR_SOLVER_LOOKUP_H
#define EXPR_SOLVER_LOOKUP_H

#include "ast.h"

typedef struct {
    char key;
    node *val;
} key_val;

key_val *key_val_create(char, node *);
void key_val_destroy(key_val *);
void key_val_destroy2(key_val *, void (*)(void *));

typedef struct {
    key_val **table;
    unsigned int size;
} lookup_table;

lookup_table *create_lookup_table(unsigned int);
void lookup_destroy(lookup_table *, void(*)(void *));

int lookup_insert(lookup_table *, key_val *);
int lookup_insert_ast(lookup_table *, node *);
_Bool lookup_rm(lookup_table *, int);
int lookup_get(lookup_table *, char);
void lookup_each(lookup_table *, void(*)(void *));
void lookup_clear(lookup_table *);

#endif //EXPR_SOLVER_LOOKUP_H
