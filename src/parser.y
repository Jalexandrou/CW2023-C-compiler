%code requires{

  #include "ast.hpp"

  extern const Node *g_root; // A way of getting the AST out

  extern FILE *yyin;

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Node *node;
  double number;
  std::string *string;
}

//List of terminal tokens, given from lexer shown with a T

//--------------------------Operators----------------------

%token T_DIVIDE T_PLUS T_MINUS T_EXPONENT T_INC T_DEC T_MOD T_POW T_PTR

//Logical Operators
%token T_LOGAND T_LOGOR T_NOTEQUAL T_EQUALS T_GREATEREQUALS T_LESSEQUALS
%token T_LOGNOT T_LESS T_GREATER

//Binary Operators
%token T_BOR T_LEFTSHIFT T_RIGHTSHIFT T_BNOT T_XOR

//-------------------------Grammar------------------------
%token T_LBRACKET T_RBRACKET T_RCURBRACKET T_LCURBRACKET T_SEMICOLON
%token T_COLON T_COMMA T_LSQRBRACKET T_RSQRBRACKET T_DOT

//-------------------------Selection Statments------------
%token T_IF T_ELSE T_WHILE T_RETURN T_CASE T_DEFAULT T_SWITCH T_DO
%token T_FOR T_CONTINUE T_BREAK T_QUESTIONMARK

//-------------------------Types--------------------------
%token T_INT T_VOID T_ENUM T_STRUCT T_UNION T_CHAR T_SHORT T_LONG
%token T_SIGNED T_UNSIGNED T_FLOAT T_DOUBLE T_VOLATILE

//-------------------------Assignments--------------------
%token T_MUL_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token T_LEFT_ASSIGN T_RIGHT_ASSIGN T_AND_ASSIGN T_XOR_ASSIGN T_ASSIGN
%token T_OR_ASSIGN

//-------------------------Functions----------------------
%token T_LOG T_EXP T_SQRT T_SIZEOF

//-------------------------Values-------------------------
%token T_NUMBER T_IDENTIFIER T_STRING_LITERAL

//-------------------------Context Dependant--------------
%token T_STAR T_AND

//-------------------------MISC---------------------------
%token T_ELLIPSIS //specifies unknown amount of params
%token T_TYPEDEF //allows you to define your own type
%token T_EXTERN //Set a type as being already assigned in another file
%token T_STATIC
%token T_AUTO
%token T_REGISTER

//Assignment of types for each token

//-----------------------------------------Define AST nodes -------------------------------------
%type <node> PRIMARY_EXPRESSION POSTFIX_EXPRESSION ARGUMENT_EXPRESSION_LIST UNARY_EXPRESSION
%type <node> CAST_EXPRESSION MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION SHIFT_EXPRESSION RELATIONAL_EXPRESSION
%type <node> EQUALITY_EXPRESSION AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION
%type <node> LOGICAL_OR_EXPRESSION CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION EXPRESSION
%type <node> CONSTANT_EXPRESSION DECLARATION DECLARATION_SPECIFIERS INIT_DECLARATOR_LIST INIT_DECLARATOR STORAGE_CLASS_SPECIFIER
%type <node> TYPE_SPECIFIER STRUCT_OR_UNION_SPECIFIER STRUCT_OR_UNION STRUCT_DECLARATION_LIST STRUCT_DECLARATION ENUM_SPECIFIER
%type <node> ENUMERATOR_LIST ENUMERATOR DECLARATOR DIRECT_DECLARATOR POINTER PARAMETER_TYPE_LIST
%type <node> PARAMETER_LIST PARAMETER_DECLARATION IDENTIFIER_LIST TYPE_NAME ABSTRACT_DECLARATOR DIRECT_ABSTRACT_DECLARATOR
%type <node> INITIALIZER INITIALIZER_LIST STATEMENT LABELED_STATEMENT COMPOUND_STATEMENT DECLARATION_LIST STATEMENT_LIST
%type <node> EXPRESSION_STATEMENT SELECTION_STATEMENT ITERATION_STATEMENT JUMP_STATEMENT TRANSLATION_UNIT EXTERNAL_DECLARATION
%type <node> FUNCTION_DEFINITION SPECIFIER_QUALIFIER_LIST STRUCT_DECLARATOR STRUCT_DECLARATOR_LIST

//give the terminal leaf node number the TYPE of number
%type <number> T_NUMBER

//give the terminal leaf node of string to IDENTIFIER
%type <string> T_IDENTIFIER T_STRING_LITERAL

%start TRANSLATION_UNIT

%%

PRIMARY_EXPRESSION
	: T_IDENTIFIER															{ $$ = new Identifier(*$1); }
	| T_NUMBER 																{ $$ = new Number($1); }
	| T_STRING_LITERAL
	| T_LBRACKET EXPRESSION T_RBRACKET 										{ $$ = $2; }
	;

POSTFIX_EXPRESSION
	: PRIMARY_EXPRESSION													{ $$ = $1; } //misc
	| POSTFIX_EXPRESSION T_LSQRBRACKET EXPRESSION T_RSQRBRACKET				//indexing x[y]
	| POSTFIX_EXPRESSION T_LBRACKET T_RBRACKET 								{ $$ = $1; } //function call x()
	| POSTFIX_EXPRESSION T_LBRACKET ARGUMENT_EXPRESSION_LIST T_RBRACKET		//function call with params??
	| POSTFIX_EXPRESSION T_DOT T_IDENTIFIER									//fetching struct value
	| POSTFIX_EXPRESSION T_PTR T_IDENTIFIER									//fetching pointer value
	| POSTFIX_EXPRESSION T_INC												//post-increment node x++
	| POSTFIX_EXPRESSION T_DEC												//post-decrement node x--
	;

ARGUMENT_EXPRESSION_LIST
	: ASSIGNMENT_EXPRESSION													{ $$ = $1; }
	| ARGUMENT_EXPRESSION_LIST T_COMMA ASSIGNMENT_EXPRESSION				//list of assignments to be used as params
	;

UNARY_EXPRESSION
	: POSTFIX_EXPRESSION													{ $$ = $1; }
	| T_INC UNARY_EXPRESSION												{ $$ = new Unary($2, "++");  }
	| T_DEC UNARY_EXPRESSION												{ $$ = new Unary($2, "--");  }
	| T_AND CAST_EXPRESSION													{ $$ = new Unary($2, "&");  }
	| T_STAR CAST_EXPRESSION												{ $$ = new Unary($2, "*");  }
	| T_PLUS CAST_EXPRESSION												{ $$ = new Unary($2, "+");  }
	| T_MINUS CAST_EXPRESSION												{ $$ = new Unary($2, "-");  }
	| T_BNOT CAST_EXPRESSION												{ $$ = new Unary($2, "~");  }
	| T_LOGNOT CAST_EXPRESSION												{ $$ = new Unary($2, "!");  }
	| T_SIZEOF UNARY_EXPRESSION												//Create SizeOf Function Node
	| T_SIZEOF T_LBRACKET TYPE_NAME T_RBRACKET								//Create SizeOf Function Node
	;

CAST_EXPRESSION
	: UNARY_EXPRESSION														{ $$ = $1; }
	;

MULTIPLICATIVE_EXPRESSION
	: CAST_EXPRESSION														{ $$ = $1; }
	| MULTIPLICATIVE_EXPRESSION T_STAR CAST_EXPRESSION						{ $$ = new Operator($1, "*", $3); }
	| MULTIPLICATIVE_EXPRESSION T_DIVIDE CAST_EXPRESSION					{ $$ = new Operator($1, "/", $3); }
	| MULTIPLICATIVE_EXPRESSION T_MOD CAST_EXPRESSION						{ $$ = new Operator($1, "%", $3); }
	;

ADDITIVE_EXPRESSION
	: MULTIPLICATIVE_EXPRESSION												{ $$ = $1; }
	| ADDITIVE_EXPRESSION T_PLUS MULTIPLICATIVE_EXPRESSION					{ $$ = new Operator($1, "+", $3); }
	| ADDITIVE_EXPRESSION T_MINUS MULTIPLICATIVE_EXPRESSION					{ $$ = new Operator($1, "-", $3); }
	;

SHIFT_EXPRESSION
	: ADDITIVE_EXPRESSION													{ $$ = $1; }
	| SHIFT_EXPRESSION T_LEFTSHIFT ADDITIVE_EXPRESSION						{ $$ = new Operator($1, "<<", $3); }
	| SHIFT_EXPRESSION T_RIGHTSHIFT ADDITIVE_EXPRESSION						{ $$ = new Operator($1, ">>", $3); }
	;

RELATIONAL_EXPRESSION
	: SHIFT_EXPRESSION														{ $$ = $1; }
	| RELATIONAL_EXPRESSION T_LESS SHIFT_EXPRESSION							{ $$ = new LogicalOperator($1, "<", $3); }//Less than Logical Node <
	| RELATIONAL_EXPRESSION T_GREATER SHIFT_EXPRESSION						{ $$ = new LogicalOperator($1, ">", $3); }//Greater Than 			 >
	| RELATIONAL_EXPRESSION T_LESSEQUALS SHIFT_EXPRESSION					{ $$ = new LogicalOperator($1, "<=", $3); }//Less than Equals Node <=
	| RELATIONAL_EXPRESSION T_GREATEREQUALS SHIFT_EXPRESSION				{ $$ = new LogicalOperator($1, ">=", $3); }//Greater than Equals Node >=
	;

EQUALITY_EXPRESSION
	: RELATIONAL_EXPRESSION													{ $$ = $1; }
	| EQUALITY_EXPRESSION T_EQUALS RELATIONAL_EXPRESSION					{ $$ = new LogicalOperator($1, ">=", $3); }//Logical Equals Node ==
	| EQUALITY_EXPRESSION T_NOTEQUAL RELATIONAL_EXPRESSION					{ $$ = new LogicalOperator($1, ">=", $3); }//Logical Not Equals Node !=
	;

AND_EXPRESSION
	: EQUALITY_EXPRESSION													{ $$ = $1; }
	| AND_EXPRESSION T_AND EQUALITY_EXPRESSION								{ $$ = new Operator($1, "-", $3); }
	;

EXCLUSIVE_OR_EXPRESSION
	: AND_EXPRESSION														{ $$ = $1; }
	| EXCLUSIVE_OR_EXPRESSION T_XOR AND_EXPRESSION							{ $$ = new Operator($1, "-", $3); }
	;

INCLUSIVE_OR_EXPRESSION
	: EXCLUSIVE_OR_EXPRESSION												{ $$ = $1; }
	| INCLUSIVE_OR_EXPRESSION T_BOR EXCLUSIVE_OR_EXPRESSION					{ $$ = new Operator($1, "-", $3); }
	;

LOGICAL_AND_EXPRESSION
	: INCLUSIVE_OR_EXPRESSION												{ $$ = $1; }
	| LOGICAL_AND_EXPRESSION T_LOGAND INCLUSIVE_OR_EXPRESSION				{ $$ = new LogicalOperator($1, ">=", $3); }
	;

LOGICAL_OR_EXPRESSION
	: LOGICAL_AND_EXPRESSION												{ $$ = $1; }
	| LOGICAL_OR_EXPRESSION T_LOGOR LOGICAL_AND_EXPRESSION					{ $$ = new LogicalOperator($1, ">=", $3); }
	;

CONDITIONAL_EXPRESSION
	: LOGICAL_OR_EXPRESSION													{ $$ = $1; }
	| LOGICAL_OR_EXPRESSION T_QUESTIONMARK EXPRESSION T_COLON CONDITIONAL_EXPRESSION	//conditional statement x ? y : z
	;

ASSIGNMENT_EXPRESSION
	: CONDITIONAL_EXPRESSION												{ $$ = $1; }
	| UNARY_EXPRESSION T_ASSIGN ASSIGNMENT_EXPRESSION						{ $$ = new AssignmentOperator($1, "=", $3); }//Assignment x = y , x+=y, x*=y ...
	| UNARY_EXPRESSION T_MUL_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "*=", $3); }
	| UNARY_EXPRESSION T_DIV_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "/=", $3); }
	| UNARY_EXPRESSION T_MOD_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "%=", $3); }
	| UNARY_EXPRESSION T_ADD_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "+=", $3); }
	| UNARY_EXPRESSION T_SUB_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "-=", $3); }
	| UNARY_EXPRESSION T_LEFT_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "<<=", $3); }
	| UNARY_EXPRESSION T_RIGHT_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, ">>=", $3); }
	| UNARY_EXPRESSION T_AND_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "&=", $3); }
	| UNARY_EXPRESSION T_XOR_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "^=", $3); }
	| UNARY_EXPRESSION T_OR_ASSIGN ASSIGNMENT_EXPRESSION					{ $$ = new AssignmentOperator($1, "|=", $3); }
	;

EXPRESSION
	: ASSIGNMENT_EXPRESSION													{ $$ = $1; }
	| EXPRESSION T_COMMA ASSIGNMENT_EXPRESSION								//list of assignments and expressions into one line
	;

CONSTANT_EXPRESSION
	: CONDITIONAL_EXPRESSION												{ $$ = $1; }//used for const keyword does not need implementation
	;

DECLARATION
	: DECLARATION_SPECIFIERS T_SEMICOLON									{ $$ = $1; }
	| DECLARATION_SPECIFIERS INIT_DECLARATOR_LIST T_SEMICOLON				{ $$ = new Declaration($1, $2); }
	;

DECLARATION_SPECIFIERS
	: STORAGE_CLASS_SPECIFIER												{ $$ = $1; }
	| STORAGE_CLASS_SPECIFIER DECLARATION_SPECIFIERS						//
	| TYPE_SPECIFIER														{ $$ = $1; }
	| TYPE_SPECIFIER DECLARATION_SPECIFIERS									//Type followed by type/storage specifier
	;

INIT_DECLARATOR_LIST
	: INIT_DECLARATOR														{ $$ = $1; }
	| INIT_DECLARATOR_LIST T_COMMA INIT_DECLARATOR							//concatenate declarators
	;

INIT_DECLARATOR
	: DECLARATOR															{ $$ = $1; }
	| DECLARATOR T_ASSIGN INITIALIZER										{ $$ = new AssignmentOperator($1, "=", $3); }
	;

STORAGE_CLASS_SPECIFIER
	: T_TYPEDEF																//weird lexer thingy
	| T_EXTERN																//assume it is already declared
	| T_STATIC
	| T_AUTO
	| T_REGISTER
	;

TYPE_SPECIFIER
	: T_VOID																{ $$ = new Void(); }
	| T_CHAR
	| T_SHORT
	| T_INT 																{ $$ = new Int(); }
	| T_LONG
	| T_FLOAT
	| T_DOUBLE
	| T_SIGNED
	| T_UNSIGNED
	| STRUCT_OR_UNION_SPECIFIER												{ $$ = $1; }
	| ENUM_SPECIFIER														{ $$ = $1; }
	| TYPE_NAME																{ $$ = $1; }
	;

STRUCT_OR_UNION_SPECIFIER
	: STRUCT_OR_UNION T_IDENTIFIER T_LCURBRACKET STRUCT_DECLARATION_LIST T_RCURBRACKET
	| STRUCT_OR_UNION T_LCURBRACKET STRUCT_DECLARATION_LIST T_RCURBRACKET
	| STRUCT_OR_UNION T_IDENTIFIER
	;

STRUCT_OR_UNION
	: T_STRUCT
	| T_UNION
	;

STRUCT_DECLARATION_LIST
	: STRUCT_DECLARATION													{ $$ = $1; }
	| STRUCT_DECLARATION_LIST STRUCT_DECLARATION
	;

STRUCT_DECLARATION
	: SPECIFIER_QUALIFIER_LIST STRUCT_DECLARATOR_LIST T_SEMICOLON
	;

SPECIFIER_QUALIFIER_LIST
	: TYPE_SPECIFIER SPECIFIER_QUALIFIER_LIST
	| TYPE_SPECIFIER														{ $$ = $1; }
	;

STRUCT_DECLARATOR_LIST
	: STRUCT_DECLARATOR														{ $$ = $1; }
	| STRUCT_DECLARATOR_LIST T_COMMA STRUCT_DECLARATOR
	;

STRUCT_DECLARATOR
	: DECLARATOR															{ $$ = $1; }
	| T_COLON CONSTANT_EXPRESSION
	| DECLARATOR T_COLON CONSTANT_EXPRESSION
	;

ENUM_SPECIFIER
	: T_ENUM T_LCURBRACKET ENUMERATOR_LIST T_RCURBRACKET
	| T_ENUM T_IDENTIFIER T_LCURBRACKET ENUMERATOR_LIST T_RCURBRACKET
	| T_ENUM T_IDENTIFIER
	;

ENUMERATOR_LIST
	: ENUMERATOR															{ $$ = $1; }
	| ENUMERATOR_LIST T_COMMA ENUMERATOR
	;

ENUMERATOR
	: T_IDENTIFIER
	| T_IDENTIFIER T_ASSIGN CONSTANT_EXPRESSION
	;

DECLARATOR
	: POINTER DIRECT_DECLARATOR
	| DIRECT_DECLARATOR														{ $$ = $1; }
	;

DIRECT_DECLARATOR
	: T_IDENTIFIER															{ $$ = new Identifier(*$1); }
	| T_LBRACKET DECLARATOR T_RBRACKET										{ $$ = $2; }
	| DIRECT_DECLARATOR T_LSQRBRACKET CONSTANT_EXPRESSION T_RSQRBRACKET		//array
	| DIRECT_DECLARATOR T_LSQRBRACKET T_RSQRBRACKET							//array with unknown size?
	| DIRECT_DECLARATOR T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET			//function call with params?
	| DIRECT_DECLARATOR T_LBRACKET IDENTIFIER_LIST T_RBRACKET				//function call with variable params?
	| DIRECT_DECLARATOR T_LBRACKET T_RBRACKET 								{ $$ = $1; } //function call without params?
	;

POINTER
	: T_STAR
	| T_STAR POINTER
	;


PARAMETER_TYPE_LIST
	: PARAMETER_LIST														{ $$ = $1; }
	| PARAMETER_LIST T_COMMA T_ELLIPSIS
	;

PARAMETER_LIST
	: PARAMETER_DECLARATION													{ $$ = $1; }
	| PARAMETER_LIST T_COMMA PARAMETER_DECLARATION
	;

PARAMETER_DECLARATION
	: DECLARATION_SPECIFIERS DECLARATOR
	| DECLARATION_SPECIFIERS ABSTRACT_DECLARATOR
	| DECLARATION_SPECIFIERS												{ $$ = $1; }
	;

IDENTIFIER_LIST
	: T_IDENTIFIER
	| IDENTIFIER_LIST T_COMMA T_IDENTIFIER
	;

TYPE_NAME
	: SPECIFIER_QUALIFIER_LIST												{ $$ = $1; }
	| SPECIFIER_QUALIFIER_LIST ABSTRACT_DECLARATOR
	;

ABSTRACT_DECLARATOR
	: POINTER																{ $$ = $1; }
	| DIRECT_ABSTRACT_DECLARATOR
	| POINTER DIRECT_ABSTRACT_DECLARATOR
	;

DIRECT_ABSTRACT_DECLARATOR
	: T_LBRACKET ABSTRACT_DECLARATOR T_RBRACKET								{ $$ = $2; }
	| T_LSQRBRACKET T_RSQRBRACKET
	| T_LSQRBRACKET CONSTANT_EXPRESSION T_RSQRBRACKET
	| DIRECT_ABSTRACT_DECLARATOR T_LSQRBRACKET T_RSQRBRACKET
	| DIRECT_ABSTRACT_DECLARATOR T_LSQRBRACKET CONSTANT_EXPRESSION T_RSQRBRACKET
	| T_LBRACKET T_RBRACKET
	| T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET
	| DIRECT_ABSTRACT_DECLARATOR T_LBRACKET T_RBRACKET
	| DIRECT_ABSTRACT_DECLARATOR T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET
	;

INITIALIZER
	: ASSIGNMENT_EXPRESSION													{ $$ = $1; }
	| T_LCURBRACKET INITIALIZER_LIST T_RCURBRACKET
	| T_LCURBRACKET INITIALIZER_LIST T_COMMA T_RCURBRACKET
	;

INITIALIZER_LIST
	: INITIALIZER															{ $$ = $1; }
	| INITIALIZER_LIST T_COMMA INITIALIZER
	;

STATEMENT
	: LABELED_STATEMENT														{ $$ = $1; }
	| COMPOUND_STATEMENT													{ $$ = $1; }
	| EXPRESSION_STATEMENT													{ $$ = $1; }
	| SELECTION_STATEMENT													{ $$ = $1; }
	| ITERATION_STATEMENT													{ $$ = $1; }
	| JUMP_STATEMENT														{ $$ = $1; }
	;

LABELED_STATEMENT
	: T_IDENTIFIER T_COLON STATEMENT
	| T_CASE CONSTANT_EXPRESSION T_COLON STATEMENT
	| T_DEFAULT T_COLON STATEMENT
	;

COMPOUND_STATEMENT
	: T_LCURBRACKET T_RCURBRACKET
	| T_LCURBRACKET STATEMENT_LIST T_RCURBRACKET 							{ $$ = $2; }
	| T_LCURBRACKET DECLARATION_LIST T_RCURBRACKET                          { $$ = $2; }
	| T_LCURBRACKET DECLARATION_LIST STATEMENT_LIST T_RCURBRACKET           { $$ = new Compound_Statement($2, $3); }
	;

DECLARATION_LIST
	: DECLARATION															{ $$ = $1; }
	| DECLARATION_LIST DECLARATION                                          { $$ = new Declaration_list($1, $2); } //one or more declarators with or without values int x, y=7, z;
	;

STATEMENT_LIST
	: STATEMENT																{ $$ = $1; }
	| STATEMENT_LIST STATEMENT                                              { $$ = new Statement_list($1, $2); }
	;

EXPRESSION_STATEMENT
	: T_SEMICOLON
	| EXPRESSION T_SEMICOLON												{ $$ = $1; }
	;

SELECTION_STATEMENT
	: T_IF T_LBRACKET EXPRESSION T_RBRACKET STATEMENT						{ $$ = new If($3, $5); }
	| T_IF T_LBRACKET EXPRESSION T_RBRACKET STATEMENT T_ELSE STATEMENT		{ $$ = new If_Else($3, $5, $7); }
	| T_SWITCH T_LBRACKET EXPRESSION T_RBRACKET STATEMENT					//Switch Node
	;

ITERATION_STATEMENT
	: T_WHILE T_LBRACKET EXPRESSION T_RBRACKET STATEMENT					{ $$ = new while_Node($3, $5); }//While Statement Node
	| T_DO STATEMENT T_WHILE T_LBRACKET EXPRESSION T_RBRACKET T_SEMICOLON	//While Statement Node (Overloaded Do)
	| T_FOR T_LBRACKET EXPRESSION_STATEMENT EXPRESSION_STATEMENT T_RBRACKET STATEMENT	//For Statement Node for( ; ; )
	| T_FOR T_LBRACKET EXPRESSION_STATEMENT EXPRESSION_STATEMENT EXPRESSION T_RBRACKET STATEMENT //For Statement Node (Overloaded i++)
	;

JUMP_STATEMENT
	: T_CONTINUE T_SEMICOLON												{ $$ = new JumpStatement("continue"); }
	| T_BREAK T_SEMICOLON													{ $$ = new JumpStatement("break"); }
	| T_RETURN T_SEMICOLON													{ $$ = new JumpStatement("return"); }
	| T_RETURN EXPRESSION T_SEMICOLON 		  								{ $$ = new JumpExpressionStatement($2, "return"); }
	;

TRANSLATION_UNIT
	: EXTERNAL_DECLARATION                    								{ g_root = $1; }
	| TRANSLATION_UNIT EXTERNAL_DECLARATION
	;

EXTERNAL_DECLARATION
	: FUNCTION_DEFINITION 					  								{ $$ = $1; }
	| DECLARATION															{ $$ = $1; }
	;

FUNCTION_DEFINITION
	: DECLARATION_SPECIFIERS DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT { $$ = new Function_Definition_Args($1, $2, $4, $3); }
	| DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT 					{ $$ = new Function_Definition($1, $2, $3); }
	| DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT
	| DECLARATOR COMPOUND_STATEMENT
	;
%%

const Node *g_root; // DEFINITION of variable (to match DECLARATION earlier)

const Node *parseAST()
{
  g_root = NULL;
  yyparse();
  return g_root;
}

/* Node *parseAST(std::string source)
{
  yyin = fopen(source.c_str(), "r");
  if (yyin == NULL)
  {
    perror("Could not open source file");
    return 1;
  }
  g_root = NULL;
  yyparse();
  return g_root;
} */


