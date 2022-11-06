cmake_minimum_required(VERSION 3.23)
project(expr_solver C)

set(CMAKE_C_STANDARD 23)
add_executable(expr_solver main.c src/ast.h src/ast.c src/token.h src/token.c src/tokenizer.h src/tokenizer.c src/util.h src/util.c src/parse.h src/parser.c src/lookup.h src/lookup.c src/settings.h src/settings.c src/solver.h src/solver.c)
