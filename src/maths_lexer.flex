%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include "maths_parser.tab.hpp"
%}

D               [0-9]
L               [a-zA-Z_]
%%

[*]             { return T_TIMES; }
[+]             { return T_PLUS; }
[\^]            { return T_EXPONENT; }
[\/]            { return T_DIVIDE; }
[-]             { return T_MINUS; }
[=]             { return T_EQUALS; }
(&&)            { return T_LOGAND; }
(\|\|)          { return T_LOGOR; }
(!=)            { return T_NOTEQUAL; }

[(]             { return T_LBRACKET; }
[)]             { return T_RBRACKET; }
[{]             { return T_LCURBRACKET; }
[}]             { return T_RCURBRACKET; }  
[;]             { return T_SEMICOLON; }

if              { return T_IF; }
else            { return T_ELSE; }
while           { return T_WHILE; }
return          { return T_RETURN; }

log             { return T_LOG; }
exp             { return T_EXP; }
sqrt            { return T_SQRT; }

int             { return T_INT; }
void            { return T_VOID; }


{L}({L}|{D})*   { yylval.string=new std::string(yytext); return T_IDENTIFIER; }
{D}+([.]{D}*)? { yylval.number=strtod(yytext, 0); return T_NUMBER; }

[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }
%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}
