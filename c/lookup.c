//
// Created by Piotr Wyrwas on 11/2/2022.
//

#include "../h/lookup.h"

#include <stdlib.h>

key_val *key_val_create(char key, node *val) {
    key_val *kv = malloc(sizeof(key_val));
    kv->key = key;
    kv->val = val;
    return kv;
}

void key_val_destroy(key_val *kv) {
    free(kv);
}

void key_val_destroy2(key_val *kv, void (*val_free)(void *)) {
    val_free(kv->val);
    key_val_destroy(kv);
}

lookup_table *create_lookup_table(unsigned int size) {
    lookup_table *table = malloc(sizeof(lookup_table));
    table->size = size;
    table->table = malloc(sizeof(key_val) * size);
    for (unsigned i = 0; i < size; i ++)
        table->table[i] = NULL;
    return table;
}

void lookup_destroy(lookup_table *table, void(*f)(void *)) {
    for (unsigned i = 0; i < table->size; i ++)
        if (table->table[i]) {
            if (!f)
                key_val_destroy(table->table[i]);
            else
                key_val_destroy2(table->table[i], f);
            table->table[i] = NULL;
        }

    free(table->table);
    free(table);
}

int lookup_insert(lookup_table *table, key_val *kv) {
    for (unsigned i = 0; i < table->size; i ++)
        if (!table->table[i]) {
            table->table[i] = kv;
            return i;
        }
    return -1;
}

int lookup_insert_ast(lookup_table *table, node *n) {
    if (n->type != node_type_assignment)
        return -2;

    key_val *kv = key_val_create(n->assignment.var->variable.variable_name, n->assignment.val);

    int i = lookup_insert(table, kv);

    if (i < 0) {
        key_val_destroy(kv);
        return -1;
    }

    return i;
}

_Bool lookup_rm(lookup_table *table, int i) {
    for (unsigned i = 0; i < table->size; i ++)
        if (table->table[i]) {
            key_val_destroy(table->table[i]);
            table->table[i] = NULL;
            return true;
        }
    return false;
}

int lookup_get(lookup_table *table, char k) {
    for (unsigned i = 0; i < table->size; i ++)
        if (table->table[i])
            if (table->table[i]->key == k)
                return i;
    return -1;
}

void lookup_each(lookup_table *t, void(*f)(void *)) {
    for (unsigned i = 0; i < t->size; i ++)
        f(t->table[i]);
}

void lookup_clear(lookup_table *t) {
    for (unsigned i = 0; i < t->size; i ++)
        t->table[i] = NULL;
}