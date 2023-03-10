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

class JumpExpressionStatement
    : public Expression
{
private:
    ExpressionPtr expr;

protected:
    JumpExpressionStatement(const ExpressionPtr _expr)
        : expr(_expr)
    {}

public:

    virtual ~JumpExpressionStatement()
    {
        delete expr;
    }

    virtual const char *getStatement() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        dst << getStatement();
        dst << " ";
        expr->print(dst);
        dst<<"; )";
    }
};

class JumpStatement: public Expression
{
public:

    virtual ~JumpStatement()
    {}

    virtual const char *getStatement() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        dst << getStatement();
        dst<<"; )";
    }
};



class ReturnExpressionStatement 
    : public JumpExpressionStatement
{
public:
    ReturnExpressionStatement(const ExpressionPtr _Statement)
        : JumpExpressionStatement(_Statement)
    {}

    virtual const char *getStatement() const override
    { return "return"; }
};

class ReturnStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "return"; }
};

class BreakStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "break"; }
};

class ContinueStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "continue"; }
};


#endif