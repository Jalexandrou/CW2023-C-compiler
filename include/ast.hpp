#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_node.hpp"
#include "ast/ast_primitives.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_unary.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_statements.hpp"
#include "ast/ast_declarations.hpp"
#include "ast/ast_types.hpp"
#include "ast/ast_codeflow.hpp"

extern const Node *parseAST();

#endif
