#include "ast.h"
#include <iostream>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <variant>

std::map<std::string, Value> variables;
static std::map<std::string, std::pair<AST*, std::vector<std::string>>> funciones;

const char* op_to_str(int op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MULT: return "*";
        case OP_DIV: return "/";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LEQ: return "<=";
        case OP_GEQ: return ">=";
        default: return "UNKNOWN_OP";
    }
}


AST* make_int(int val) {
    AST* node = new AST;
    node->type = NODE_INT;
    node->data.intval = val;
    node->value = val;    
    return node;
}

AST* make_id(const char* id) {
    AST* node = new AST;
    node->type = NODE_ID;
    node->data.id = strdup(id);
    node->value = 0; 
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

AST* make_for(AST* init, AST* cond, AST* update, AST* body) {
    AST* node = new AST;
    node->type = NODE_FOR;
    node->data.for_loop.init = init;
    node->data.for_loop.cond = cond;
    node->data.for_loop.update = update;
    node->data.for_loop.body = body;
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

AST* make_decl(const char* tipo, const char* nombre) {
    AST* node = new AST();
    node->type = NODE_DECL;
    node->data.decl.tipo = strdup(tipo);
    node->data.decl.nombre = strdup(nombre);
    return node;
}

AST* make_string(const char* val) {
    AST* node = new AST;
    node->type = NODE_STRING;
    node->data.strval = strdup(val);
    node->value = 0; 
    return node;
}

AST* make_float(float val) {
    AST* node = new AST;
    node->type = NODE_FLOAT;
    node->data.floatval = val;
    node->value = val;    
    return node;
}


Value eval_ast(AST* tree) {
    if (!tree) return Value();

    switch (tree->type) {
        case NODE_DECL: {
            std::string var = tree->data.decl.nombre;
            variables[var] = Value(); // Variable declarada sin valor (NONE)
            tree->value = 0;
            return Value();
        }
        case NODE_INT: {
            Value v(tree->data.intval);
            tree->value = 0; // Este campo ya no se usa
            return v;
        }
        case NODE_FLOAT: {
            Value v(tree->data.floatval);
            tree->value = 0;
            return v;
        }
        case NODE_STRING: {
            Value v(std::string(tree->data.strval));
            tree->value = 0;
            return v;
        }
        case NODE_ID: {
            std::string var = tree->data.id;
            if (variables.count(var))
                return variables[var];
            else {
                std::cerr << "Variable no definida: " << var << "\n";
                return Value();
            }
        }
        case NODE_ASSIGN: {
            std::string var = tree->data.bin.left->data.id;
            Value val = eval_ast(tree->data.bin.right);
            variables[var] = val;
            return val;
        }
        case NODE_PRINT: {
            Value val = eval_ast(tree->data.bin.left);
            switch (val.type) {
                case Value::INT: std::cout << val.asInt(); break;
                case Value::FLOAT: std::cout << val.asFloat(); break;
                case Value::STRING: std::cout << val.asString(); break;
                default: std::cout << "null";
            }
            std::cout << std::endl;
            return Value();
        }
        case NODE_BINOP: {
            Value lhs = eval_ast(tree->data.bin.left);
            Value rhs = eval_ast(tree->data.bin.right);

            switch (tree->op) {
                case OP_PLUS: {
                    if (lhs.type == Value::STRING || rhs.type == Value::STRING) {
                        std::string s1 = (lhs.type == Value::STRING) ? lhs.asString() :
                                        (lhs.type == Value::INT) ? std::to_string(lhs.asInt()) :
                                        (lhs.type == Value::FLOAT) ? std::to_string(lhs.asFloat()) :
                                        "";

                        std::string s2;
                        if (rhs.type == Value::STRING) {
                            s2 = rhs.asString();
                        } else if (rhs.type == Value::INT) {
                            s2 = std::to_string(rhs.asInt());
                        } else if (rhs.type == Value::FLOAT) {
                            s2 = std::to_string(rhs.asFloat());
                        } else {
                            std::cerr << "Error: No se puede convertir RHS a string\n";
                            return Value();
                        }

                        return Value(s1 + s2);
                    } else if ((lhs.type == Value::INT || lhs.type == Value::FLOAT) &&
                            (rhs.type == Value::INT || rhs.type == Value::FLOAT)) {
                        float res = (lhs.type == Value::FLOAT ? lhs.asFloat() : lhs.asInt()) +
                                    (rhs.type == Value::FLOAT ? rhs.asFloat() : rhs.asInt());
                        if (lhs.type == Value::INT && rhs.type == Value::INT && (int)res == res)
                            return Value((int)res);
                        else
                            return Value(res);
                    } else {
                        std::cerr << "Error: Operacion suma no soportada para estos tipos\n";
                        return Value();
                    }
                }
                case OP_MINUS:
                case OP_MULT:
                case OP_DIV: {
                    
                    if (!((lhs.type == Value::INT || lhs.type == Value::FLOAT) &&
                        (rhs.type == Value::INT || rhs.type == Value::FLOAT))) {
                        std::cerr << "Error: Operacion aritmética no soportada para estos tipos\n";
                        return Value();
                    }

                    float l = (lhs.type == Value::FLOAT) ? lhs.asFloat() : lhs.asInt();
                    float r = (rhs.type == Value::FLOAT) ? rhs.asFloat() : rhs.asInt();
                    float res = 0;
                    switch (tree->op) {
                        case OP_MINUS: res = l - r; break;
                        case OP_MULT:  res = l * r; break;
                        case OP_DIV:   res = (r != 0) ? (l / r) : 0; break;
                    }
                    if (lhs.type == Value::INT && rhs.type == Value::INT && (int)res == res)
                        return Value((int)res);
                    else
                        return Value(res);
                }
                
                case OP_EQ:
                case OP_NEQ: {
                    // Comparación general usando la variante directamente
                    bool result = (tree->op == OP_EQ) ? (lhs.val == rhs.val) : (lhs.val != rhs.val);
                    return Value(result ? 1 : 0);
                }
                case OP_LT:
                case OP_LEQ:
                case OP_GT:
                case OP_GEQ: {
                    // Comparaciones sólo permitidas entre INT o FLOAT
                    if ((lhs.type == Value::INT || lhs.type == Value::FLOAT) &&
                        (rhs.type == Value::INT || rhs.type == Value::FLOAT)) {
                        
                        float l = (lhs.type == Value::FLOAT) ? lhs.asFloat() : lhs.asInt();
                        float r = (rhs.type == Value::FLOAT) ? rhs.asFloat() : rhs.asInt();
                        bool result = false;

                        switch (tree->op) {
                            case OP_LT:  result = l < r; break;
                            case OP_LEQ: result = l <= r; break;
                            case OP_GT:  result = l > r; break;
                            case OP_GEQ: result = l >= r; break;
                            default: break;
                        }

                        return Value(result ? 1 : 0);
                    } else {
                        std::cerr << "Error: Comparación no soportada para estos tipos\n";
                        return Value();
                    }
                }

                default:
                    return Value();
            }
        }
        case NODE_IF: {
            Value cond = eval_ast(tree->data.ctrl.cond);
            bool cond_true = false;
            if (cond.type == Value::INT)
                cond_true = cond.asInt() != 0;
            else if (cond.type == Value::FLOAT)
                cond_true = cond.asFloat() != 0.0f;
            else
                cond_true = false;

            if (cond_true)
                return eval_ast(tree->data.ctrl.then_branch);
            else if (tree->data.ctrl.else_branch)
                return eval_ast(tree->data.ctrl.else_branch);
            else
                return Value();
        }
        case NODE_WHILE: {
            while (true) {
                Value cond = eval_ast(tree->data.ctrl.cond);
                bool cond_true = false;
                if (cond.type == Value::INT)
                    cond_true = cond.asInt() != 0;
                else if (cond.type == Value::FLOAT)
                    cond_true = cond.asFloat() != 0.0f;
                else
                    cond_true = false;

                if (!cond_true)
                    break;

                eval_ast(tree->data.ctrl.then_branch);
            }
            return Value();
        }
        case NODE_FOR: {
            eval_ast(tree->data.for_loop.init);
            while (true) {
                Value cond = eval_ast(tree->data.for_loop.cond);
                bool cond_true = false;
                if (cond.type == Value::INT)
                    cond_true = cond.asInt() != 0;
                else if (cond.type == Value::FLOAT)
                    cond_true = cond.asFloat() != 0.0f;
                else
                    cond_true = false;

                if (!cond_true)
                    break;

                eval_ast(tree->data.for_loop.body);
                eval_ast(tree->data.for_loop.update);
            }
            return Value();
        }
        case NODE_SEQ: {
            eval_ast(tree->data.seq.first);
            return eval_ast(tree->data.seq.second);
        }
        case NODE_FUNC_DEF: {
            std::vector<std::string> names;
            if (tree->data.func_def.params && tree->data.func_def.params->data.params.names)
                names = *(tree->data.func_def.params->data.params.names);
            funciones[tree->data.func_def.name] = {tree->data.func_def.body, names};
            return Value();
        }
        case NODE_FUNC_CALL: {
            if (funciones.count(tree->data.func_call.name)) {
                auto [body, param_names] = funciones[tree->data.func_call.name];
                auto saved_vars = variables;

                std::vector<AST*> args;
                if (tree->data.func_call.args && tree->data.func_call.args->data.args.values)
                    args = *(tree->data.func_call.args->data.args.values);

                for (size_t i = 0; i < param_names.size(); ++i) {
                    Value val = (i < args.size()) ? eval_ast(args[i]) : Value();
                    variables[param_names[i]] = val;
                }

                Value result = eval_ast(body);
                variables = saved_vars;
                return result;
            } else {
                std::cerr << "Error: función '" << tree->data.func_call.name << "' no definida.\n";
                return Value();
            }
        }
        case NODE_RETURN: {
            return eval_ast(tree->data.ret.expr);
        }
        default:
            return Value();
    }
}

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
            std::cout << "BINOP (" << op_to_str(tree->op) << ")\n";
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

        case NODE_FOR:
            std::cout << "FOR\n";
            print_indent(indent + 1); std::cout << "Init:\n";
            print_ast(tree->data.for_loop.init, indent + 2);
            print_indent(indent + 1); std::cout << "Cond:\n";
            print_ast(tree->data.for_loop.cond, indent + 2);
            print_indent(indent + 1); std::cout << "Update:\n";
            print_ast(tree->data.for_loop.update, indent + 2);
            print_indent(indent + 1); std::cout << "Body:\n";
            print_ast(tree->data.for_loop.body, indent + 2);
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

        case NODE_DECL:
            std::cout << "DECLARACION: " << tree->data.decl.nombre 
                    << " Tipo: " << tree->data.decl.tipo << "\n";
            break;
            
        case NODE_RETURN:
            std::cout << "RETURN\n";
            print_ast(tree->data.ret.expr, indent + 1);
            break;
        case NODE_FLOAT:
            std::cout << "FLOAT: " << tree->data.floatval << "\n";
            break;

        case NODE_STRING:
            std::cout << "STRING: " << tree->data.strval << "\n";
            break;

        default:
            std::cout << "Nodo desconocido\n";
    }
}
