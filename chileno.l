%{
#include "chileno.tab.h"
#include <cstdlib>
#include <cstring>
%}

%%

"si_po"                return IF;
"si_no"                return ELSE;
"mientras_la_wa"       return WHILE;
"pa_cada"              return FOR;
"imprime_la_wa"        return PRINT;
"pega"                 return FUNCTION;
"devuelve_la_wa"       return RETURN;
"=="                   return EQ;
"!="                   return NEQ;
"<="                   return LEQ;
">="                   return GEQ;
"int"                  return TIPO_INT;
"float"                return TIPO_FLOAT;
"string"               return TIPO_STRING;
[0-9]+\.[0-9]+          { yylval.floatval = atof(yytext); return FLOAT; }     // Flotantes
[0-9]+                  { yylval.intval = atoi(yytext); return NUM; }         // Enteros
\"([^\"\\]|\\.)*\"      {
                            int len = yyleng - 2; // largo sin las comillas
                            char* str = (char*)malloc(len + 1);
                            strncpy(str, yytext + 1, len);                     //Cadenas
                            str[len] = '\0';
                            yylval.strval = str;
                            return STRING;
                        }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.strval = strdup(yytext); return ID; }        // Identificadores


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
"//".*                 ;
[ \t\r\n]+             ;  // ignorar espacios y saltos de línea

.                      { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
