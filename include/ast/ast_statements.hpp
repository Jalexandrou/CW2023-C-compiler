#ifndef ast_statements_hpp
#define ast_statements_hpp

#include "ast_expression.hpp"

class Statement_list
    : public Expression
{
private:
    ExpressionPtr Statement_left;
    ExpressionPtr Statement_right;

public:
    Statement_list(ExpressionPtr _Statement_left, ExpressionPtr _Statement_right)
        : Statement_left(_Statement_left), Statement_right(_Statement_right)
    {};

    virtual ~Statement_list()
    {
        delete Statement_left;
        delete Statement_right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Statement_left->print(dst);
        dst<<" ";
        Statement_right->print(dst);
        dst<<" )";
    }
};

#endif