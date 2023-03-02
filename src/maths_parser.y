%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern const Expression *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Expression *expr;
  double number;
  std::string *string;
}

%token T_TIMES T_DIVIDE T_PLUS T_MINUS T_EXPONENT T_EQUALS
%token T_LOGAND T_LOGOR T_NOTEQUAL 
%token T_LBRACKET T_RBRACKET T_RCURBRACKET T_LCURBRACKET T_SEMICOLON
%token T_IF T_ELSE T_WHILE T_RETURN
%token T_INT T_VOID
%token T_LOG T_EXP T_SQRT
%token T_NUMBER T_IDENTIFIER

%type <expr> EXTERNAL_DECLARATION FUNCTION_DEFINITION TYPE_SPECIFIER  DECLARATOR COMPOUND_STATEMENT STATEMENT EXPR TERM UNARY FACTOR
%type <number> T_NUMBER
%type <string> T_IDENTIFIER T_INT T_LOG T_EXP T_SQRT

%start ROOT

%%

ROOT : EXTERNAL_DECLARATION { g_root = $1; }


EXTERNAL_DECLARATION
	: FUNCTION_DEFINITION  { $$ = $1; }
	;

FUNCTION_DEFINITION
	: TYPE_SPECIFIER DECLARATOR COMPOUND_STATEMENT { $$ = new Function_Definition($1, $2, $3); }
	;

TYPE_SPECIFIER
	: T_INT { $$ = new Int(*$1); }
	;

DECLARATOR 
	: T_IDENTIFIER      { $$ = new Identifier( $1); }
    | DECLARATOR T_LBRACKET T_RBRACKET { $$ = $1; }
	;

COMPOUND_STATEMENT
	: T_LCURBRACKET STATEMENT T_RCURBRACKET  { $$ = $2; }
	;

STATEMENT
    : T_RETURN T_NUMBER T_SEMICOLON { $$ = new Statement($2); }
    ;
/* EXPR : TERM           { $$ = $1;}
     | EXPR T_PLUS TERM { $$ = new AddOperator($1, $3);}
     | EXPR T_MINUS TERM { $$ = new SubOperator($1, $3);}
     ;

TERM : UNARY          { $$ = $1;}
     | TERM T_TIMES UNARY {$$ = new MulOperator($1, $3);}
     | TERM T_DIVIDE UNARY {$$ = new DivOperator($1, $3);}
     ;

UNARY : FACTOR        { $$ = $1; }
      | T_MINUS FACTOR  {$$ = new NegOperator( $2 ); }
      ;

FACTOR : T_NUMBER     { $$ = new Number( $1 ); }
       | T_IDENTIFIER   { $$ = new Identifier( *$1 ); }
       | T_LBRACKET EXPR T_RBRACKET { $$ = $2; }
       | FACTOR T_EXPONENT UNARY {$$ = new ExpOperator($1, $3);}
       | T_LOG T_LBRACKET EXPR T_RBRACKET {$$ = new LogFunction($3); }
       | T_EXP T_LBRACKET EXPR T_RBRACKET {$$ = new ExpFunction($3); }
       | T_SQRT T_LBRACKET EXPR T_RBRACKET {$$ = new SqrtFunction($3); }
       ; */
%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
