%{
#include "chileno.tab.h"
#include <cstdlib>
#include <cstring>
%}

%%

"si_po"                return IF;
"si_no_po"             return ELSE;
"mientras_la_wa"       return WHILE;
"pa_cada"              return FOR;
"suelta_la_wa"         return PRINT;
"hace_la_pega"         return FUNCTION;
"devuelve_la_wa"       return RETURN;
"igualito"             return EQ;
"igualitont"           return NEQ;
"igualitito"           return LEQ;
"igualitote"           return GEQ;
"numerito"             return TIPO_INT;
"numerito_con_punto"   return TIPO_FLOAT;
"palabrita"            return TIPO_STRING;
"lee_la_wa"            return LEE;
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
[ \t\r\n]+             ;  // ignorar espacios y saltos de linea

.                      { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
