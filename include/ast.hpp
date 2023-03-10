#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_expression.hpp"
#include "ast/ast_primitives.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_unary.hpp"
#include "ast/ast_functions.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_statements.hpp"
#include "ast/ast_types.hpp"

extern const Expression *parseAST();

#endif
