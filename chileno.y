%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ast.h"

extern int yylex();
void yyerror(const char *s) {
    std::cerr << "Error: " << s << std::endl;
}
AST* tree; // arbol general
%}

%union {
    int intval;
    char* id;
    AST* ast;
}

%start program

%token <intval> NUMBER
%token <id> IDENTIFIER
%token IF ELSE WHILE FOR FUNCTION RETURN
%token TYPE_INT PRINT READ ASSIGN EQ

%type <ast> program statements statement expr

%left EQ

%%

program:
    statements              { tree = $1; }
;

statements:
    statements statement    { $$ = make_seq($1, $2); }
  | statement               { $$ = $1; }
;

statement:
    TYPE_INT IDENTIFIER ';'           { $$ = nullptr; } // declaración ignorada
  | IDENTIFIER ASSIGN expr ';'        { $$ = make_assign(make_id($1), $3); }
  | PRINT expr ';'                    { $$ = make_print($2); }
  | IF expr '{' statements '}' ELSE '{' statements '}' {
        $$ = make_if($2, $4, $8);
    }
  | WHILE expr '{' statements '}'     { $$ = make_while($2, $4); }
;

expr:
    NUMBER             { $$ = make_int($1); }
  | IDENTIFIER         { $$ = make_id($1); }
  | expr EQ expr       { $$ = make_binop(EQ, $1, $3); }
;

%%

int main() {
    if (yyparse() == 0 && tree != nullptr) {
        std::cout << " Árbol de sintaxis generado:\n";
        print_ast(tree, 0);
    }
    return 0;
}

