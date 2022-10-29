project(expr_solver C)

add_executable(expr_solver main.c h/ast.h c/ast.c h/token.h c/token.c h/tokenizer.h c/tokenizer.c h/util.h c/util.c h/parse.h c/parser.c)
