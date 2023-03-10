#ifndef ast_if_hpp
#define ast_if_hpp

#include "ast_expression.hpp"

class If
    : public Expression
{
private:
    ExpressionPtr arg;
    ExpressionPtr compoundStatement;

public:
    If(const ExpressionPtr _arg,const  ExpressionPtr _compoundStatement)
        : arg(_arg), compoundStatement(_compoundStatement)
    {}

    virtual ~If()
    {
        delete arg;
        delete compoundStatement;
    }

    ExpressionPtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"if ( ";
        arg->print(dst);
        dst<<" ) ";
        dst << "{";
        compoundStatement->print(dst);
        dst << "} ";
    }
};

class If_Else
    : public Expression
{
private:
    ExpressionPtr arg;
    ExpressionPtr compoundStatement;
    ExpressionPtr elseCompoundStatement;

public:
    If_Else(const ExpressionPtr _arg,const  ExpressionPtr _compoundStatement, const ExpressionPtr _elseCompoundStatement)
        : arg(_arg), compoundStatement(_compoundStatement), elseCompoundStatement(_elseCompoundStatement)
    {}

    virtual ~If_Else()
    {
        delete arg;
        delete compoundStatement;
        delete elseCompoundStatement;
    }

    ExpressionPtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"if ( ";
        arg->print(dst);
        dst<<" ) ";
        dst << "{";
        compoundStatement->print(dst);
        dst << "} ";
        dst << "else";
        dst << " { ";
        elseCompoundStatement->print(dst);
        dst << "} ";
    }
};

#endif