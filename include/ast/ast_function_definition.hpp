#ifndef ast_function_definition_hpp
#define ast_function_definition_hpp

#include "ast_expression.hpp"

class Function_Definition
    : public Expression
{
private:
    ExpressionPtr Type;
    ExpressionPtr Declarator;
    ExpressionPtr Compound_Statement;
protected:
    Function_Definition(ExpressionPtr _Type, ExpressionPtr _Declarator, ExpressionPtr _Compound_Statement)
        : Type(_Type), Declarator(_Declarator), Compound_Statement(_Compound_Statement)
    {};
public:
    virtual ~Function_Definition()
    {
        delete Type;
        delete Declarator;
        delete Compound_Statement;
    }

    ExpressionPtr getType() const
    { return Type; }

    ExpressionPtr getDeclarator() const
    { return Declarator; }

    ExpressionPtr getCompound_Statement() const
    { return Compound_Statement; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Type->print(dst);
        dst<<" ";
        Declarator->print(dst);
        dst<<" ";
        Compound_Statement->print(dst);
        dst<<" )";
    }
};

class Statement
    : public Expression
{
private:
    ExpressionPtr Value;

protected:
    Statement(ExpressionPtr _value)
        : Value(_value)
    {};
public:
    virtual ~Statement()
    {
        delete Value;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( Return";
        Value->print(dst);
        dst<<";";
    }
};

#endif
