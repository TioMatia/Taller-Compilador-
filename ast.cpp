#include "ast.h"
#include <iostream>
#include <cstring>
#include <map>
#include <string>
#include <vector>

std::map<std::string, int> variables;
static std::map<std::string, std::pair<AST*, std::vector<std::string>>> funciones;

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

AST* make_func_def(const char* name, AST* params, AST* body) {
    AST* node = new AST;
    node->type = NODE_FUNC_DEF;
    node->data.func_def.name = strdup(name);
    node->data.func_def.params = params;
    node->data.func_def.body = body;
    return node;
}

AST* make_func_call(const char* name, AST* args) {
    AST* node = new AST;
    node->type = NODE_FUNC_CALL;
    node->data.func_call.name = strdup(name);
    node->data.func_call.args = args;
    return node;
}

AST* make_args(std::vector<AST*>* values) {
    AST* node = new AST;
    node->type = NODE_ARGS;
    node->data.args.values = values;
    return node;
}

AST* make_params(std::vector<std::string>* names) {
    AST* node = new AST;
    node->type = NODE_PARAMS;
    node->data.params.names = names;
    return node;
}

AST* make_return(AST* expr) {
    AST* node = new AST;
    node->type = NODE_RETURN;
    node->data.ret.expr = expr;
    return node;
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

        case NODE_FUNC_DEF: {
            std::vector<std::string>* names = tree->data.func_def.params ? tree->data.func_def.params->data.params.names : new std::vector<std::string>();
            funciones[tree->data.func_def.name] = {tree->data.func_def.body, *names};
            return 0;
        }

        case NODE_FUNC_CALL: {
            if (funciones.count(tree->data.func_call.name)) {
                auto [body, param_names] = funciones[tree->data.func_call.name];

                std::map<std::string, int> saved_vars = variables;

                std::vector<AST*>* args = tree->data.func_call.args ? tree->data.func_call.args->data.args.values : new std::vector<AST*>();

                for (size_t i = 0; i < param_names.size(); ++i) {
                    int val = i < args->size() ? eval_ast((*args)[i]) : 0;
                    variables[param_names[i]] = val;
                }

                int result = eval_ast(body);

                variables = saved_vars;

                return result;
            } else {
                std::cerr << "Error: función '" << tree->data.func_call.name << "' no definida.\n";
                return 0;
            }
        }

        case NODE_RETURN:
            return eval_ast(tree->data.ret.expr);

        default:
            return 0;
    }
}

// Funciones para imprimir el AST

void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}

void print_ast(AST* tree, int indent) {
    if (!tree) return;

    print_indent(indent);

    switch (tree->type) {
        case NODE_INT:
            std::cout << "INT: " << tree->data.intval << "\n";
            break;

        case NODE_ID:
            std::cout << "ID: " << tree->data.id << "\n";
            break;

        case NODE_ASSIGN:
            std::cout << "ASSIGN\n";
            print_ast(tree->data.bin.left, indent + 1);
            print_ast(tree->data.bin.right, indent + 1);
            break;

        case NODE_PRINT:
            std::cout << "PRINT\n";
            print_ast(tree->data.bin.left, indent + 1);
            break;

        case NODE_BINOP:
            std::cout << "BINOP (" << tree->op << ")\n";
            print_ast(tree->data.bin.left, indent + 1);
            print_ast(tree->data.bin.right, indent + 1);
            break;

        case NODE_IF:
            std::cout << "IF\n";
            print_ast(tree->data.ctrl.cond, indent + 1);
            print_ast(tree->data.ctrl.then_branch, indent + 1);
            if (tree->data.ctrl.else_branch)
                print_ast(tree->data.ctrl.else_branch, indent + 1);
            break;

        case NODE_WHILE:
            std::cout << "WHILE\n";
            print_ast(tree->data.ctrl.cond, indent + 1);
            print_ast(tree->data.ctrl.then_branch, indent + 1);
            break;

        case NODE_SEQ:
            std::cout << "SEQ\n";
            print_ast(tree->data.seq.first, indent + 1);
            print_ast(tree->data.seq.second, indent + 1);
            break;

        case NODE_FUNC_DEF:
            std::cout << "FUNC_DEF: " << tree->data.func_def.name << "\n";
            print_ast(tree->data.func_def.params, indent + 1);
            print_ast(tree->data.func_def.body, indent + 1);
            break;

        case NODE_FUNC_CALL:
            std::cout << "FUNC_CALL: " << tree->data.func_call.name << "\n";
            print_ast(tree->data.func_call.args, indent + 1);
            break;

        case NODE_ARGS:
            std::cout << "ARGS\n";
            if (tree->data.args.values) {
                for (AST* arg : *(tree->data.args.values)) {
                    print_ast(arg, indent + 1);
                }
            }
            break;

        case NODE_PARAMS:
            std::cout << "PARAMS\n";
            if (tree->data.params.names) {
                for (const std::string& param : *(tree->data.params.names)) {
                    print_indent(indent + 1);
                    std::cout << param << "\n";
                }
            }
            break;

        case NODE_RETURN:
            std::cout << "RETURN\n";
            print_ast(tree->data.ret.expr, indent + 1);
            break;

        default:
            std::cout << "Nodo desconocido\n";
    }
}
