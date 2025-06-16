#include "ast.h"
#include <iostream>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <variant>

struct VarInfo {
    std::string tipo; // "int", "float", "string"
    Value valor;
};

static std::map<std::string, VarInfo> variables;
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

AST* make_float(float val) {
    AST* node = new AST;
    node->type = NODE_FLOAT;
    node->data.floatval = val;
    node->value = val;    
    return node;
}

AST* make_string(const char* val) {
    AST* node = new AST;
    node->type = NODE_STRING;
    node->data.strval = strdup(val);
    node->value = 0; 
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

AST* make_input(AST* variable) {
    AST* node = new AST;
    node->type = NODE_INPUT;
    node->data.input.variable = variable;
    return node;
}





Value eval_ast(AST* tree) {
    if (!tree) return Value();

    switch (tree->type) {
        case NODE_DECL: {
            std::string var = tree->data.decl.nombre;
            if (variables.count(var)) {
                std::cerr << "Error: variable '" << var << "' ya declarada.\n";
                exit(1);
            }
            variables[var] = VarInfo{tree->data.decl.tipo, Value()};
            return Value();
        }

        case NODE_INT: {
            return Value(tree->data.intval);
        }
        case NODE_FLOAT: {
            return Value(tree->data.floatval);
        }
        case NODE_STRING: {
            return Value(std::string(tree->data.strval));
        }
        case NODE_ID: {
            std::string var = tree->data.id;
            if (variables.count(var) == 0) {
                std::cerr << "Error: variable no definida: " << var << "\n";
                exit(1);
            }
            return variables[var].valor;
        }

        case NODE_ASSIGN: {
            std::string var = tree->data.bin.left->data.id;
            if (variables.count(var) == 0) {
                std::cerr << "Error: asignacion a variable no declarada: " << var << "\n";
                exit(1);
            }

            Value val = eval_ast(tree->data.bin.right);
            std::string tipo = variables[var].tipo;

            // Validar que el tipo del valor que asignamos coincide con el tipo declarado
            if ((tipo == "int" && val.type != Value::INT) ||
                (tipo == "float" && val.type != Value::FLOAT && val.type != Value::INT) || // aceptar int en float
                (tipo == "string" && val.type != Value::STRING)) {
                std::cerr << "Error: tipo incompatible en asignacion a variable '" << var << "'\n";
                exit(1);
            }

            // Para float, si es int lo convertimos a float
            if (tipo == "float" && val.type == Value::INT) {
                variables[var].valor = Value(static_cast<float>(val.asInt()));
            } else {
                variables[var].valor = val;
            }

            return variables[var].valor;
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
                        std::cerr << "Error: Operacion aritmetica no soportada para estos tipos\n";
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
                    bool result = (tree->op == OP_EQ) ? (lhs.val == rhs.val) : (lhs.val != rhs.val);
                    return Value(result ? 1 : 0);
                }
                case OP_LT:
                case OP_LEQ:
                case OP_GT:
                case OP_GEQ: {
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
                        std::cerr << "Error: Comparacion no soportada para estos tipos\n";
                        return Value();
                    }
                }
                default:
                    return Value();
            }
        }
        case NODE_IF: {
            Value cond = eval_ast(tree->data.ctrl.cond);
            bool cond_true = (cond.type == Value::INT && cond.asInt() != 0) ||
                             (cond.type == Value::FLOAT && cond.asFloat() != 0.0f);
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
                bool cond_true = (cond.type == Value::INT && cond.asInt() != 0) ||
                                 (cond.type == Value::FLOAT && cond.asFloat() != 0.0f);
                if (!cond_true) break;
                eval_ast(tree->data.ctrl.then_branch);
            }
            return Value();
        }
        case NODE_FOR: {
            eval_ast(tree->data.for_loop.init);
            while (true) {
                Value cond = eval_ast(tree->data.for_loop.cond);
                bool cond_true = (cond.type == Value::INT && cond.asInt() != 0) ||
                                 (cond.type == Value::FLOAT && cond.asFloat() != 0.0f);
                if (!cond_true) break;
                eval_ast(tree->data.for_loop.body);
                eval_ast(tree->data.for_loop.update);
            }
            return Value();
        }
        case NODE_INPUT: {
            AST* var_node = tree->data.input.variable;
            if (!var_node || var_node->type != NODE_ID) {
                std::cerr << "Error: input espera una variable valida\n";
                exit(1);
            }

            std::string var = var_node->data.id;
            if (variables.count(var) == 0) {
                std::cerr << "Error: variable no declarada: " << var << "\n";
                exit(1);
            }

            std::string tipo = variables[var].tipo;
            std::string input;
            std::getline(std::cin, input);

            try {
                if (tipo == "int") {
                    size_t pos;
                    int i = std::stoi(input, &pos);
                    if (pos != input.size()) throw std::invalid_argument("No es int valido");
                    variables[var].valor = Value(i);
                }
                else if (tipo == "float") {
                    size_t pos;
                    float f = std::stof(input, &pos);
                    if (pos != input.size()) throw std::invalid_argument("No es float valido");
                    variables[var].valor = Value(f);
                }
                else if (tipo == "string") {
                    variables[var].valor = Value(input);
                }
                else {
                    std::cerr << "Tipo desconocido para variable " << var << "\n";
                    exit(1);
                }
            } catch (std::exception& e) {
                std::cerr << "Error: entrada invalida para tipo " << tipo << "\n";
                exit(1);
            }

            return variables[var].valor;
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
                    VarInfo varinfo;
                    // Asignar tipo segun tipo del valor
                    if (val.type == Value::INT) varinfo.tipo = "int";
                    else if (val.type == Value::FLOAT) varinfo.tipo = "float";
                    else if (val.type == Value::STRING) varinfo.tipo = "string";
                    else varinfo.tipo = "unknown"; // o "" para manejar

                    varinfo.valor = val;
                    variables[param_names[i]] = varinfo;
                }

                Value result = eval_ast(body);
                variables = saved_vars; // restaura variables
                return result;
            } else {
                std::cerr << "Error: funcion '" << tree->data.func_call.name << "' no definida.\n";
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
        case NODE_INPUT:
            std::cout << "INPUT\n";
            print_ast(tree->data.input.variable, indent + 1);
            break;
        default:
            std::cout << "Nodo desconocido\n";
    }
}

std::string generar_programa(AST* tree) {
    std::string codigo = "#include <iostream>\n#include <string>\nusing namespace std;\n\n";

    // Generar funciones fuera del main
    codigo += generate_code_funcs(tree);

    // Abrir main
    codigo += "int main() {\n";

    // Generar código que no sean funciones dentro del main
    codigo += generate_code_main(tree);

    // Cerrar main
    codigo += "return 0;\n}\n";

    return codigo;
}

std::string generate_code_funcs(AST* tree) {
    if (!tree) return "";

    std::string codigo;

    switch (tree->type) {
        case NODE_FUNC_DEF: {
            std::string nombre = tree->data.func_def.name;
            std::string params_code = ""; 
            if (tree->data.func_def.params) {
                for (size_t i = 0; i < tree->data.func_def.params->data.params.names->size(); i++) {
                    if (i > 0) params_code += ", ";
                    params_code += "auto " + tree->data.func_def.params->data.params.names->at(i);
                }
            }
            std::string body_code = generate_code_main(tree->data.func_def.body);
            codigo += "auto " + nombre + "(" + params_code + ") {\n" + body_code + "}\n\n";
            break;
        }
        case NODE_SEQ: {
            codigo += generate_code_funcs(tree->data.seq.first);
            codigo += generate_code_funcs(tree->data.seq.second);
            break;
        }
        default:
            // No es función, no genera nada aquí
            break;
    }
    return codigo;
}

std::string generate_code_main(AST* tree, bool in_for_header) {
    if (!tree) return "";

    switch (tree->type) {
        case NODE_FUNC_DEF:
            return "";
        case NODE_SEQ: {
            return generate_code_main(tree->data.seq.first, in_for_header) + generate_code_main(tree->data.seq.second, in_for_header);
        }
        case NODE_DECL: {
            std::string tipo = tree->data.decl.tipo;
            std::string nombre = tree->data.decl.nombre;
            // Solo poner ; si NO estamos dentro de header de for
            return tipo + " " + nombre + (in_for_header ? "" : ";\n");
        }
        case NODE_ASSIGN: {
            std::string var = tree->data.bin.left->data.id;
            std::string expr = generate_code_main(tree->data.bin.right, in_for_header);
            return var + " = " + expr + (in_for_header ? "" : ";\n");
        }
        case NODE_PRINT: {
            // Cambiar para que no incluya endl y ; en for header
            if (in_for_header) {
                return ""; // no se espera print en header for
            } else {
                return generate_print_expr(tree->data.bin.left);
            }
        }
        case NODE_INPUT: {
            std::string var = tree->data.input.variable->data.id;
            return "cin >> " + var + ";\n";
        }
        case NODE_INT: {
            return std::to_string(tree->data.intval);
        }
        case NODE_FLOAT: {
            return std::to_string(tree->data.floatval);
        }
        case NODE_STRING: {
            return "\"" + std::string(tree->data.strval) + "\"";
        }
        case NODE_ID: {
            return tree->data.id;
        }
        case NODE_BINOP: {
            std::string lhs = generate_code_main(tree->data.bin.left, in_for_header);
            std::string rhs = generate_code_main(tree->data.bin.right, in_for_header);
            std::string op;
            switch (tree->op) {
                case OP_PLUS: op = "+"; break;
                case OP_MINUS: op = "-"; break;
                case OP_MULT: op = "*"; break;
                case OP_DIV: op = "/"; break;
                case OP_EQ: op = "=="; break;
                case OP_NEQ: op = "!="; break;
                case OP_LT: op = "<"; break;
                case OP_LEQ: op = "<="; break;
                case OP_GT: op = ">"; break;
                case OP_GEQ: op = ">="; break;
                default: op = "/* unknown */";
            }
            return "(" + lhs + " " + op + " " + rhs + ")";
        }
        case NODE_IF: {
            std::string cond = generate_code_main(tree->data.ctrl.cond);
            std::string then_branch = generate_code_main(tree->data.ctrl.then_branch);
            std::string else_branch = tree->data.ctrl.else_branch ? generate_code_main(tree->data.ctrl.else_branch) : "";
            std::string code = "if (" + cond + ") {\n" + then_branch + "}\n";
            if (!else_branch.empty())
                code += "else {\n" + else_branch + "}\n";
            return code;
        }
        case NODE_WHILE: {
            std::string cond = generate_code_main(tree->data.ctrl.cond);
            std::string body = generate_code_main(tree->data.ctrl.then_branch);
            return "while (" + cond + ") {\n" + body + "}\n";
        }
        case NODE_FUNC_CALL: {
            std::string nombre = tree->data.func_call.name;
            std::string args_code = "";
            if (tree->data.func_call.args) {
                for (size_t i = 0; i < tree->data.func_call.args->data.args.values->size(); i++) {
                    if (i > 0) args_code += ", ";
                    args_code += generate_code_main(tree->data.func_call.args->data.args.values->at(i));
                }
            }
            return nombre + "(" + args_code + ")" + ";\n";
        }
        case NODE_RETURN: {
            std::string expr = generate_code_main(tree->data.ret.expr);
            return "return " + expr + ";\n";
        }
        case NODE_FOR: {
            std::string var_name = "i";  // Forzamos que sea i para que coincida con cond/post

            std::string init = generate_code_main(tree->data.for_loop.init, true);
            std::string cond = generate_code_main(tree->data.for_loop.cond, true);
            std::string post = generate_code_main(tree->data.for_loop.update, true);

            // Ahora reemplazamos el nombre original en init (posiblemente ii) por var_name ("i")
            // Por ejemplo, si init es "int ii = 0" lo cambiamos a "int i = 0"
            auto replace_init_var = [&](std::string& str, const std::string& new_var) {
                // Encontrar el nombre actual (después de "int ") y reemplazarlo por new_var
                size_t pos = str.find("int ");
                if (pos != std::string::npos) {
                    pos += 4; // pos al inicio del nombre variable
                    size_t end_pos = str.find_first_of("= ;", pos);
                    if (end_pos != std::string::npos) {
                        str.replace(pos, end_pos - pos, new_var);
                    }
                }
            };

            replace_init_var(init, var_name);

            // Reemplazamos "i" en cond y post por var_name (por si acaso)
            auto replace_var = [&](std::string& str, const std::string& from, const std::string& to) {
                size_t pos = 0;
                while ((pos = str.find(from, pos)) != std::string::npos) {
                    // Verificar que sea palabra completa, como antes
                    bool valid = true;
                    if (pos > 0) {
                        char before = str[pos - 1];
                        if ((before >= 'a' && before <= 'z') || (before >= 'A' && before <= 'Z') || (before >= '0' && before <= '9') || before == '_')
                            valid = false;
                    }
                    if (pos + from.size() < str.size()) {
                        char after = str[pos + from.size()];
                        if ((after >= 'a' && after <= 'z') || (after >= 'A' && after <= 'Z') || (after >= '0' && after <= '9') || after == '_')
                            valid = false;
                    }
                    if (valid) {
                        str.replace(pos, from.size(), to);
                        pos += to.size();
                    } else {
                        pos += from.size();
                    }
                }
            };

            replace_var(cond, "i", var_name);
            replace_var(post, "i", var_name);

            std::string body = generate_code_main(tree->data.for_loop.body);
            return "for (" + init + "; " + cond + "; " + post + ") {\n" + body + "}\n";
        }


        default:
            return "/* Nodo no implementado */\n";
    }
}

void gen_print_parts(AST* node, std::string& codigo) {
    if (!node) return;

    if (node->type == NODE_BINOP && node->op == OP_PLUS) {
        gen_print_parts(node->data.bin.left, codigo);
        gen_print_parts(node->data.bin.right, codigo);
    } else {
        codigo += " << " + generate_code_main(node);
    }
}

std::string generate_print_expr(AST* expr) {
    if (!expr) return "";

    if (expr->type == NODE_BINOP && expr->op == OP_PLUS) {
        std::string codigo = "cout";
        gen_print_parts(expr, codigo);
        return codigo + ";\n";
    } else {
        return "cout << " + generate_code_main(expr) + ";\n";
    }
}