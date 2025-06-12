%{
#include <cstdlib>
#include <cstring>
#include "ast.h"
#include "chileno.tab.h"


%}

%%

"si_po"                return IF;
"si_no"                return ELSE;
"mientras_la_wa"       return WHILE;
"pa_cada"              return FOR;
"pega"                 return FUNCTION;
"devuelve_la_wa"       return RETURN;
"numerito"             return TYPE_INT;
"muestra_la_wa"        return PRINT;
"pide_la_wa"           return READ;
"es"                   return ASSIGN;
"igualito"             return EQ;

[0-9]+                 { yylval.intval = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.id = strdup(yytext); return IDENTIFIER; }

[ \t\n]+               ; // ignorar espacios
.                      return *yytext;

%%

extern "C" int yywrap() {
    return 1;
}
