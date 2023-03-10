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

class JumpStatement
    : public Expression
{
private:
    ExpressionPtr expr;

protected:
    JumpStatement(const ExpressionPtr _expr)
        : expr(_expr)
    {}

public:

    virtual ~JumpStatement()
    {
        delete expr;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        expr->print(dst);
        dst<<" )";
    }
};


class ReturnStatement 
    : public JumpStatement
{
public:
    ReturnStatement(const ExpressionPtr _Statement)
        : JumpStatement(_Statement)
    {}

    ReturnStatement()
        : JumpStatement(NULL)
    {}
};

class BreakStatement
    : public JumpStatement

{
public:
    BreakStatement()
        : JumpStatement(NULL)
    {}
};

class ContinueStatement
    : public JumpStatement

{
public:
    ContinueStatement()
        : JumpStatement(NULL)
    {}
};


#endif