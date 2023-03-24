%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);


#include "parser.tab.hpp"
%}

D               [0-9]
L               [a-zA-Z_]
%%

[\*]            { return T_STAR; }
[%]             { return T_MOD; }
[+]             { return T_PLUS; }
[\^]            { return T_XOR; }
[\/]            { return T_DIVIDE; }
[-]             { return T_MINUS; }
(\~)            { return T_BNOT; }
(\+\+)          { return T_INC; }
(\-\-)          { return T_DEC; }
(\&)            { return T_AND; }
(\!)            { return T_LOGNOT; }
(\|)            { return T_BOR; }

(=)             { return T_ASSIGN; }
(\*=)           { return T_MUL_ASSIGN; }
(\/=)           { return T_DIV_ASSIGN; }
(\%=)           { return T_MOD_ASSIGN; }
(\+=)           { return T_ADD_ASSIGN; }
(\-=)           { return T_SUB_ASSIGN; }
(\-=)           { return T_SUB_ASSIGN; }
(\<\<=)         { return T_LEFT_ASSIGN; }
(\>\>=)         { return T_RIGHT_ASSIGN; }
(\&=)           { return T_AND_ASSIGN; }
(\^=)           { return T_XOR_ASSIGN; }
(\|=)           { return T_OR_ASSIGN; }

(&&)            { return T_LOGAND; }
(\|\|)          { return T_LOGOR; }
(!=)            { return T_NOTEQUAL; }
(==)            { return T_EQUALS; }
(<=)            { return T_LESSEQUALS; }
(>=)            { return T_GREATEREQUALS; }
(<)             { return T_LESS; }
(>)             { return T_GREATER; }
(<<)            { return T_LEFTSHIFT; }
(>>)            { return T_RIGHTSHIFT; }


[(]             { return T_LBRACKET; }
[)]             { return T_RBRACKET; }
[{]             { return T_LCURBRACKET; }
[}]             { return T_RCURBRACKET; }
[;]             { return T_SEMICOLON; }
[,]             { return T_COMMA;}
[:]             { return T_COLON; }

if              { return T_IF; }
else            { return T_ELSE; }
while           { return T_WHILE; }
return          { return T_RETURN; }
continue        { return T_CONTINUE; }
break           { return T_BREAK; }
switch          { return T_SWITCH; }
case            { return T_CASE;}


for             { return T_FOR; }

log             { return T_LOG; }
exp             { return T_EXP; }
sqrt            { return T_SQRT; }

int             { return T_INT; }
void            { return T_VOID; }
unsigned        { return T_UNSIGNED; }

sizeof          { return T_SIZEOF; }

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
