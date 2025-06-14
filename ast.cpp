#include "ast.h"
#include <iostream>
#include <cstring>
#include <map>
#include <string>

std::map<std::string, int> variables;

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
    node->op = op;
    node->data.bin.left = lhs;
    node->data.bin.right = rhs;
    return node;
}

AST* make_seq(AST* first, AST* second) {
    if (!first) return second;
    if (!second) return first;
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
            std::cout << "BINOP ";
            switch (tree->op) {
                case OP_PLUS: std::cout << "(+)"; break;
                case OP_MINUS: std::cout << "(-)"; break;
                case OP_MULT: std::cout << "(*)"; break;
                case OP_DIV: std::cout << "(/)"; break;
                case OP_EQ: std::cout << "(==)"; break;
                case OP_LT: std::cout << "(<)"; break;
                case OP_GT: std::cout << "(>)"; break;
                default: std::cout << "(unknown)"; break;
            }
            std::cout << ":\n";
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
int eval_ast(AST* tree) {
    if (!tree) return 0;

    switch (tree->type) {
        case NODE_INT:
            return tree->data.intval;

        case NODE_ID:
            return variables[tree->data.id];

        case NODE_ASSIGN: {
            std::string var = tree->data.bin.left->data.id;
            int val = eval_ast(tree->data.bin.right);
            variables[var] = val;
            return val;
        }

        case NODE_PRINT: {
            int val = eval_ast(tree->data.bin.left);
            std::cout << val << std::endl;
            return 0;
        }

        case NODE_BINOP: {
            int lhs = eval_ast(tree->data.bin.left);
            int rhs = eval_ast(tree->data.bin.right);
            switch (tree->op) {
                case OP_EQ: return lhs == rhs;
                case OP_PLUS: return lhs + rhs;
                case OP_MINUS: return lhs - rhs;
                case OP_MULT: return lhs * rhs;
                case OP_DIV: return rhs != 0 ? lhs / rhs : 0;
                case OP_LT: return lhs < rhs;
                case OP_GT: return lhs > rhs;
                default: return 0;
            }
        }

        case NODE_IF: {
            if (eval_ast(tree->data.ctrl.cond))
                return eval_ast(tree->data.ctrl.then_branch);
            else if (tree->data.ctrl.else_branch)
                return eval_ast(tree->data.ctrl.else_branch);
            return 0;
        }

        case NODE_WHILE: {
            while (eval_ast(tree->data.ctrl.cond))
                eval_ast(tree->data.ctrl.then_branch);
            return 0;
        }

        case NODE_SEQ:
            eval_ast(tree->data.seq.first);
            return eval_ast(tree->data.seq.second);
    }

    return 0;
}

