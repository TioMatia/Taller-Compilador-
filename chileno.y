%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ast.h"

extern int yylex();
void yyerror(const char *s) {
    std::cerr << "Error: " << s << std::endl;
}
AST* tree;
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
%token TYPE_INT PRINT READ ASSIGN EQ LT GT PLUS MINUS MULT DIV

%type <ast> program statements statement expr

%left EQ
%left LT GT
%left PLUS MINUS
%left MULT DIV

%%

program:
    statements              { tree = $1; }
;

statements:
    statements statement    { $$ = make_seq($1, $2); }
  | statement               { $$ = $1; }
;

statement:
    TYPE_INT IDENTIFIER ';'           { $$ = nullptr; }
  | IDENTIFIER ASSIGN expr ';'        { $$ = make_assign(make_id($1), $3); }
  | PRINT expr ';'                    { $$ = make_print($2); }
  | IF expr '{' statements '}' ELSE '{' statements '}' {
        $$ = make_if($2, $4, $8);
    }
  | WHILE expr '{' statements '}'     { $$ = make_while($2, $4); }
  | FOR TYPE_INT IDENTIFIER ASSIGN expr ';' expr ';' IDENTIFIER ASSIGN expr '{' statements '}' {
      AST* init = make_assign(make_id($3), $5);
      AST* cond = $7;
      AST* update = make_assign(make_id($9), $11);
      AST* body_with_update = make_seq($13, update);
      $$ = make_seq(init, make_while(cond, body_with_update));
  }

;

expr:
    NUMBER             { $$ = make_int($1); }
  | IDENTIFIER         { $$ = make_id($1); }
  | expr PLUS expr     { $$ = make_binop(OP_PLUS, $1, $3); }
  | expr MINUS expr    { $$ = make_binop(OP_MINUS, $1, $3); }
  | expr MULT expr     { $$ = make_binop(OP_MULT, $1, $3); }
  | expr DIV expr      { $$ = make_binop(OP_DIV, $1, $3); }
  | expr EQ expr       { $$ = make_binop(OP_EQ, $1, $3); }
  | expr LT expr       { $$ = make_binop(OP_LT, $1, $3); }
  | expr GT expr       { $$ = make_binop(OP_GT, $1, $3); }
;

%%

int main() {
    if (yyparse() == 0 && tree != nullptr) {
        std::cout << "Arbol de sintaxis generado:\n";
        print_ast(tree, 0);
        std::cout << "\n--- Ejecución del programa ---\n";
        eval_ast(tree);
    }
    return 0;
}
