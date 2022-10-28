//
// Created by Piotr Wyrwas on 10/24/2022.
//

#include "../h/util.h"

#include <string.h>
#include <stdlib.h>

char *string_strip(char *s) {
    unsigned le = strlen(s);
    char *_s = malloc(le + 1);
    memset(_s, '\0', le);
    char *p = _s;
    for (unsigned i = 0; i < strlen(s); i ++)
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
            *(p ++) = s[i];
    le = strlen(s);
    _s = realloc(_s, le + 1);
    _s[le] = '\0';
    return _s;
}

char *string_rep(char **dst, char c, int n) {
    if (*dst == NULL)
        *dst = malloc(n+1);
    else
        *dst = realloc(*dst, n+1);

    memset(*dst, c, n);
    (*dst)[n] = '\0';
    return *dst;
}

void string_clean(char *s) {
    if (!s) return;
    unsigned le = strlen(s);
    memset(s, '\0', le + 1);
}