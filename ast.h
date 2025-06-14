#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum NodeType {
    NODE_INT,
    NODE_ID,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_BINOP,
    NODE_SEQ,
    NODE_FUNC_DEF,
    NODE_FUNC_CALL,
    NODE_RETURN,
    NODE_ARGS,
    NODE_PARAMS
};

enum BinOp {
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    OP_EQ,
    OP_LT,
    OP_GT
};

struct AST {
    NodeType type;
    int op; 

    union {
        int intval;
        char* id;

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
    } data;
};


AST* make_int(int val);
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


void print_ast(AST* tree, int indent = 0);
int eval_ast(AST* tree);

#endif
