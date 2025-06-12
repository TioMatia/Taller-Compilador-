#include "ast.h"
#include <iostream>
#include <cstring>

AST* make_int(int val) {
    AST* node = new AST;
    node->type = NODE_INT;
    node->data.intval = val;
    return node;
}

AST* make_id(const char* id) {
    AST* node = new AST;
    node->type = NODE_ID;
    node->data.id = strdup(id);
    return node;
}

AST* make_assign(AST* id, AST* val) {
    AST* node = new AST;
    node->type = NODE_ASSIGN;
    node->data.bin.left = id;
    node->data.bin.right = val;
    return node;
}

AST* make_print(AST* expr) {
    AST* node = new AST;
    node->type = NODE_PRINT;
    node->data.bin.left = expr;
    return node;
}

AST* make_if(AST* cond, AST* then_b, AST* else_b) {
    AST* node = new AST;
    node->type = NODE_IF;
    node->data.ctrl.cond = cond;
    node->data.ctrl.then_branch = then_b;
    node->data.ctrl.else_branch = else_b;
    return node;
}

AST* make_while(AST* cond, AST* body) {
    AST* node = new AST;
    node->type = NODE_WHILE;
    node->data.ctrl.cond = cond;
    node->data.ctrl.then_branch = body;
    node->data.ctrl.else_branch = nullptr;
    return node;
}

AST* make_binop(int op, AST* lhs, AST* rhs) {
    AST* node = new AST;
    node->type = NODE_BINOP;
    node->data.bin.left = lhs;
    node->data.bin.right = rhs;
    node->data.intval = op;
    return node;
}

AST* make_seq(AST* first, AST* second) {
    AST* node = new AST;
    node->type = NODE_SEQ;
    node->data.seq.first = first;
    node->data.seq.second = second;
    return node;
}

void print_ast(AST* tree, int indent) {
    if (!tree) return;
    for (int i = 0; i < indent; ++i) std::cout << "  ";

    switch (tree->type) {
        case NODE_INT:
            std::cout << "INT: " << tree->data.intval << std::endl;
            break;
        case NODE_ID:
            std::cout << "ID: " << tree->data.id << std::endl;
            break;
        case NODE_ASSIGN:
            std::cout << "ASSIGN:\n";
            print_ast(tree->data.bin.left, indent + 1);
            print_ast(tree->data.bin.right, indent + 1);
            break;
        case NODE_PRINT:
            std::cout << "PRINT:\n";
            print_ast(tree->data.bin.left, indent + 1);
            break;
        case NODE_IF:
            std::cout << "IF:\n";
            print_ast(tree->data.ctrl.cond, indent + 1);
            std::cout << "THEN:\n";
            print_ast(tree->data.ctrl.then_branch, indent + 1);
            if (tree->data.ctrl.else_branch) {
                std::cout << "ELSE:\n";
                print_ast(tree->data.ctrl.else_branch, indent + 1);
            }
            break;
        case NODE_WHILE:
            std::cout << "WHILE:\n";
            print_ast(tree->data.ctrl.cond, indent + 1);
            print_ast(tree->data.ctrl.then_branch, indent + 1);
            break;
        case NODE_BINOP:
            std::cout << "BINOP (" << tree->data.intval << "):\n";
            print_ast(tree->data.bin.left, indent + 1);
            print_ast(tree->data.bin.right, indent + 1);
            break;
        case NODE_SEQ:
            std::cout << "SEQ:\n";
            print_ast(tree->data.seq.first, indent + 1);
            print_ast(tree->data.seq.second, indent + 1);
            break;
    }
}
