//
// Created by Piotr Wyrwas on 11/2/2022.
//

#include "../h/lookup.h"
#include "../h/settings.h"

#include <stdlib.h>
#include <stdio.h>

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

lookup_table *create_lookup_table(unsigned int size, void (*kv_val_free)(void *)) {
    lookup_table *table = malloc(sizeof(lookup_table));
    table->size = size;
    table->table = malloc(sizeof(key_val) * size);
    for (unsigned i = 0; i < size; i ++)
        table->table[i] = NULL;
    table->kv_val_free = kv_val_free;

    return table;
}

void lookup_destroy(lookup_table *table) {
    DEBUG("Freeing lookup table.\n");
    for (unsigned i = 0; i < table->size; i ++)
        if (table->table[i]) {
            if (!table->kv_val_free)
                key_val_destroy(table->table[i]);
            else
                key_val_destroy2(table->table[i], table->kv_val_free);
            table->table[i] = NULL;
        }

    free(table->table);
    free(table);
}

int lookup_insert(lookup_table *table, key_val *kv) {
    DEBUG("Inserting '%c' into a lookup table.\n", kv->key);
    int n = lookup_get(table, kv->key);
    if (n >= 0) {
        DEBUG("The item is already present in the lookup table and will be replaced (%c).\n", kv->key);
    }
    for (unsigned i = 0; i < table->size; i ++) {
        if (!n) {
            if (table->table[i]->key == kv->key) {
                if (table->kv_val_free)
                    (table->kv_val_free)(table->table[i]->val);
                key_val_destroy(table->table[i]);
                table->table[i] = kv;
            }
        }
        else if (!table->table[i]) {
            table->table[i] = kv;
        }
        else continue;

        return i;
    }

    DEBUG("Failed to alter the lookup table.\n");
    return -1;
}

int lookup_insert_ast(lookup_table *table, node *n) {
    if (n->type != node_type_assignment) {
        DEBUG("Node is not an assignment node.\n");
        return -2;
    }

    DEBUG("Inserting into a lookup table using an assignment node.\n");

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
    DEBUG("Searching for '%c' in a lookup table.\n", k);
    for (unsigned i = 0; i < table->size; i ++)
        if (table->table[i])
            if (table->table[i]->key == k) {
                DEBUG("'%c' found at index '%d'.\n", k, i);
                return i;
            }

    DEBUG("Could not find '%c' in the table.\n", k);
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