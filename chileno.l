%{
#include "chileno.tab.h"
#include <cstdlib>
#include <cstring>
%}

%%

"si_po"                return IF;
"si_no"                return ELSE;
"mientras_la_wa"       return WHILE;
"imprime_la_wa"        return PRINT;
"pega"                 return FUNCTION;
"devuelve_la_wa"       return RETURN;
"=="                   return EQ;

[0-9]+                 { yylval.intval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return ID; }

"="                    return '=';
"("                    return '(';
")"                    return ')';
"{"                    return '{';
"}"                    return '}';
","                    return ',';
";"                    return ';';
"<"                    return '<';
">"                    return '>';
"+"                    return '+';
"-"                    return '-';
"*"                    return '*';
"/"                    return '/';

[ \t\r\n]+             ;  // ignorar espacios y saltos de línea

.                      { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
