//
// Created by Piotr Wyrwas on 11/2/2022.
//

#ifndef EXPR_SOLVER_SETTINGS_H
#define EXPR_SOLVER_SETTINGS_H

#define DEBUG(x, ...) if (cfg_debug_info) { printf("[Debug @ %s] ", __func__); printf(x, ##__VA_ARGS__); }

// Informative Constants
extern const char const_version[];
extern const char const_author[];
extern const char const_pkg_name[];

// Configuration
extern _Bool cfg_debug_info;
extern _Bool cfg_show_ast;

// Status monitoring
extern _Bool stat_exit_repl;

#endif //EXPR_SOLVER_SETTINGS_H
