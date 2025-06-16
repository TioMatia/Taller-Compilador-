#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <variant>

enum NodeType {
    NODE_INT,
    NODE_FLOAT,
    NODE_STRING,
    NODE_ID,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_BINOP,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_SEQ,
    NODE_FUNC_DEF,
    NODE_FUNC_CALL,
    NODE_ARGS,
    NODE_PARAMS,
    NODE_RETURN,
    NODE_DECL,
    NODE_INPUT  
};

enum BinOp {
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_GT,
    OP_LEQ,
    OP_GEQ
};

struct Value {
    enum Type { INT, FLOAT, STRING, NONE } type;
    std::variant<int, float, std::string> val;

    Value() : type(NONE) {}
    Value(int v) : type(INT), val(v) {}
    Value(float v) : type(FLOAT), val(v) {}
    Value(const std::string& v) : type(STRING), val(v) {}

    int asInt() const { return std::get<int>(val); }
    float asFloat() const { return std::get<float>(val); }
    std::string asString() const { return std::get<std::string>(val); }
};

struct AST {
    NodeType type;
    int op;
    double value;

    union {
        int intval;
        char* id;
        float floatval;
        char* strval;

        struct {
            AST* left;
            AST* right;
        } bin;

        struct {
            AST* cond;
            AST* then_branch;
            AST* else_branch;
        } ctrl;

        struct {
            AST* first;
            AST* second;
        } seq;

        struct {
            char* name;
            AST* params;
            AST* body;
        } func_def;

        struct {
            char* name;
            AST* args;
        } func_call;

        struct {
            std::vector<std::string>* names;
        } params;

        struct {
            std::vector<AST*>* values;
        } args;

        struct {
            AST* expr;
        } ret;

        struct {
            AST* init;
            AST* cond;
            AST* update;
            AST* body;
        } for_loop;

        struct {
            char* tipo;
            char* nombre;
        } decl;

        struct {
            AST* variable; 
        } input;
    } data;
};

AST* make_int(int val);
AST* make_float(float val);
AST* make_string(const char* val);
AST* make_id(const char* id);
AST* make_assign(AST* id, AST* val);
AST* make_print(AST* expr);
AST* make_if(AST* cond, AST* then_b, AST* else_b);
AST* make_while(AST* cond, AST* body);
AST* make_binop(int op, AST* lhs, AST* rhs);
AST* make_seq(AST* first, AST* second);
AST* make_func_def(const char* name, AST* params, AST* body);
AST* make_func_call(const char* name, AST* args);
AST* make_args(std::vector<AST*>* values);
AST* make_params(std::vector<std::string>* names);
AST* make_return(AST* expr);
AST* make_for(AST* init, AST* cond, AST* update, AST* body);
AST* make_decl(const char* tipo, const char* nombre);
AST* make_input(AST* variable); 

// Funciones existentes
void print_ast(AST* tree, int indent = 0);
Value eval_ast(AST* tree);

// Nuevas funciones para generación de código
std::string generar_programa(AST* tree);          // Genera el archivo completo (void, escribe archivo)
std::string generate_code_funcs(AST* tree);
std::string generate_print_expr(AST* expr);
std::string generate_code_main(AST* tree, bool in_for_header = false);
#endif