#ifndef AST_H
#define AST_H

#include <string>

enum NodeType {
    NODE_INT,
    NODE_ID,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_BINOP,
    NODE_SEQ
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
void print_ast(AST* tree, int indent);

#endif
