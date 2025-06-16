%code requires {
  #include <vector>
  #include <string>
  #include "ast.h"
}

%{
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include "ast.h"
#include <fstream>

extern int yylex();
void yyerror(const char* s) { std::cerr << "Error: " << s << std::endl; exit(1); }
extern FILE* yyin;
AST* tree;

std::string generar_programa(AST* root);

std::map<std::string, bool> tabla_simbolos;  // Guarda variables declaradas
%}

%union {
    int intval;
    float floatval;
    char* strval;
    AST* ast;
    std::vector<AST*>* astlist;
    std::vector<std::string>* strlist;
}

%token <intval> NUM
%token <strval> ID STRING
%token <floatval> FLOAT
%token IF ELSE WHILE PRINT FUNCTION RETURN EQ FOR NEQ LEQ GEQ TIPO_INT TIPO_FLOAT TIPO_STRING LEE

%type <ast> expr stmt stmts program func_def func_call return_stmt decl
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
    | FOR '(' decl ';' expr ';' expr ')' stmt  
                                 { $$ = make_for($3, $5, $7, $9); }
    | '{' stmts '}'              { $$ = $2; }
    | func_def                   { $$ = $1; }
    | return_stmt                { $$ = $1; }
    | decl ';'                   { $$ = $1; }
    | LEE ID ';'                 { 
                                    if (tabla_simbolos.count($2) == 0) {
                                        std::cerr << "Error: variable '" << $2 << "' no declarada para input\n";
                                        exit(1);
                                    }
                                    $$ = make_input(make_id($2)); 
                                 }
    ;

decl
    : TIPO_INT ID                {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_decl("int", $2);
                                }
    | TIPO_INT ID '=' expr       {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_seq(make_decl("int", $2), make_assign(make_id($2), $4));
                                }
    | TIPO_FLOAT ID              {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_decl("float", $2);
                                }
    | TIPO_FLOAT ID '=' expr     {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_seq(make_decl("float", $2), make_assign(make_id($2), $4));
                                }
    | TIPO_STRING ID             {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_decl("string", $2);
                                }
    | TIPO_STRING ID '=' expr    {
                                  if (tabla_simbolos.count($2)) {
                                    std::cerr << "Error: variable '" << $2 << "' ya declarada\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$2] = true;
                                  $$ = make_seq(make_decl("string", $2), make_assign(make_id($2), $4));
                                }
    ;

return_stmt
    : RETURN expr ';'            { $$ = make_return($2); }
    ;

func_def
    : FUNCTION ID '(' param_list ')' '{' stmts '}'
                                 { $$ = make_func_def($2, make_params($4), $7); }
    ;

param_list
    : /* vacio */                { $$ = new std::vector<std::string>(); }
    | ID                         {
                                  if (tabla_simbolos.count($1)) {
                                    std::cerr << "Error: parametro '" << $1 << "' ya declarado como variable\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$1] = true;
                                  $$ = new std::vector<std::string>({$1});
                                }
    | param_list ',' ID          {
                                  if (tabla_simbolos.count($3)) {
                                    std::cerr << "Error: parametro '" << $3 << "' ya declarado como variable\n";
                                    exit(1);
                                  }
                                  tabla_simbolos[$3] = true;
                                  $1->push_back($3);
                                  $$ = $1;
                                }
    ;

expr
    : NUM                        { $$ = make_int($1); }
    | FLOAT                      { $$ = make_float($1); }
    | STRING                     { $$ = make_string($1); }
    | ID                         {
                                  if (tabla_simbolos.count($1) == 0) {
                                    std::cerr << "Error sintactico: variable '" << $1 << "' no declarada\n";
                                    exit(1);
                                  }
                                  $$ = make_id($1);
                                }
    | expr '+' expr              { $$ = make_binop(OP_PLUS, $1, $3); }
    | expr '-' expr              { $$ = make_binop(OP_MINUS, $1, $3); }
    | expr '*' expr              { $$ = make_binop(OP_MULT, $1, $3); }
    | expr '/' expr              { $$ = make_binop(OP_DIV, $1, $3); }
    | expr EQ expr               { $$ = make_binop(OP_EQ, $1, $3); }
    | expr '<' expr              { $$ = make_binop(OP_LT, $1, $3); }
    | expr '>' expr              { $$ = make_binop(OP_GT, $1, $3); }
    | expr NEQ expr              { $$ = make_binop(OP_NEQ, $1, $3); }
    | expr LEQ expr              { $$ = make_binop(OP_LEQ, $1, $3); }
    | expr GEQ expr              { $$ = make_binop(OP_GEQ, $1, $3); }
    | ID '=' expr                {
                                  if (tabla_simbolos.count($1) == 0) {
                                    std::cerr << "Error sintactico: variable '" << $1 << "' no declarada para asignacion.\n";
                                    exit(1);
                                  }
                                  $$ = make_assign(make_id($1), $3);
                                }
    | func_call                  { $$ = $1; }
    | '(' expr ')'               { $$ = $2; }
    ;

func_call
    : ID '(' arg_list ')'        { $$ = make_func_call($1, make_args($3)); }
    ;

arg_list
    : /* vacio */                { $$ = new std::vector<AST*>(); }
    | expr                       { $$ = new std::vector<AST*>({$1}); }
    | arg_list ',' expr          { $1->push_back($3); $$ = $1; }
    ;

%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            std::cerr << "No se pudo abrir el archivo: " << argv[1] << std::endl;
            return 1;
        }
        yyin = f;
    } else {
        std::cerr << "Uso: ./chileno_compilador archivo.chileno.txt\n";
        return 1;
    }

    if (yyparse() == 0) {
        std::cout << "--- Arbol de sintaxis generado ---\n";
        print_ast(tree, 0);

        std::cout << "\n--- Ejecucion del programa ---\n";
        eval_ast(tree); // Ejecuta como siempre

        
        std::cout << "\n--- Generando codigo C++ ---\n";
        std::string codigo_cpp = generar_programa(tree);
        std::ofstream out("cpp_chileno.cpp");
        out << codigo_cpp;
        out.close();
        std::cout << "Archivo generado: generado.cpp\n";

    } else {
        std::cerr << "Error durante el parseo.\n";
        return 1;
    }

    return 0;
}