%code requires {
  #include <vector>
  #include <string>
  #include "ast.h"   // acá debe estar la definición de AST
}

%{
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "ast.h"

extern int yylex();
void yyerror(const char* s) { std::cerr << "Error: " << s << std::endl; }

AST* tree;
%}

%union {
    int intval;
    char* strval;
    AST* ast;
    std::vector<AST*>* astlist;
    std::vector<std::string>* strlist;
}

%token <intval> NUM
%token <strval> ID
%token IF ELSE WHILE PRINT FUNCTION RETURN EQ FOR

%type <ast> expr stmt stmts program func_def func_call return_stmt
%type <astlist> arg_list
%type <strlist> param_list

%%

program
    : stmts                      { tree = $1; }
    ;

stmts
    : stmt                       { $$ = $1; }
    | stmts stmt                 { $$ = make_seq($1, $2); }
    ;

stmt
    : expr ';'                   { $$ = $1; }
    | PRINT expr ';'             { $$ = make_print($2); }
    | IF '(' expr ')' stmt       { $$ = make_if($3, $5, nullptr); }
    | IF '(' expr ')' stmt ELSE stmt
                                 { $$ = make_if($3, $5, $7); }
    | WHILE '(' expr ')' stmt    { $$ = make_while($3, $5); }
    | FOR '(' expr ';' expr ';' expr ')' stmt  
                                 { $$ = make_for($3, $5, $7, $9); }
    | '{' stmts '}'              { $$ = $2; }
    | func_def                   { $$ = $1; }
    | return_stmt                { $$ = $1; }

return_stmt
    : RETURN expr ';'            { $$ = make_return($2); }
    ;

func_def
    : FUNCTION ID '(' param_list ')' '{' stmts '}'
                                 { $$ = make_func_def($2, make_params($4), $7); }
    ;

param_list
    : /* vacío */                { $$ = new std::vector<std::string>(); }
    | ID                         { $$ = new std::vector<std::string>({$1}); }
    | param_list ',' ID          { $1->push_back($3); $$ = $1; }
    ;

expr
    : NUM                        { $$ = make_int($1); }
    | ID                         { $$ = make_id($1); }
    | expr '+' expr              { $$ = make_binop(OP_PLUS, $1, $3); }
    | expr '-' expr              { $$ = make_binop(OP_MINUS, $1, $3); }
    | expr '*' expr              { $$ = make_binop(OP_MULT, $1, $3); }
    | expr '/' expr              { $$ = make_binop(OP_DIV, $1, $3); }
    | expr EQ expr               { $$ = make_binop(OP_EQ, $1, $3); }
    | expr '<' expr              { $$ = make_binop(OP_LT, $1, $3); }
    | expr '>' expr              { $$ = make_binop(OP_GT, $1, $3); }
    | ID '=' expr                { $$ = make_assign(make_id($1), $3); }
    | func_call                  { $$ = $1; }
    | '(' expr ')'               { $$ = $2; }
    ;

func_call
    : ID '(' arg_list ')'        { $$ = make_func_call($1, make_args($3)); }
    ;

arg_list
    : /* vacío */                { $$ = new std::vector<AST*>(); }
    | expr                       { $$ = new std::vector<AST*>({$1}); }
    | arg_list ',' expr          { $1->push_back($3); $$ = $1; }
    ;

%%

int main() {
    yyparse();
    std::cout << "--- Arbol de sintaxis generado---\n";
    print_ast(tree, 0); 
    std::cout << "\n--- Ejecucion del programa ---\n";
    eval_ast(tree);
    return 0;
}
