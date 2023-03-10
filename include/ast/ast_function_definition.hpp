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

public:
    Function_Definition(ExpressionPtr _Type, ExpressionPtr _Declarator, ExpressionPtr _Compound_Statement)
        : Type(_Type), Declarator(_Declarator), Compound_Statement(_Compound_Statement)
    {};

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
        dst<<" { ";
        Compound_Statement->print(dst);
        dst<<" } )";
    }
};

class Compound_Statement
    : public Expression
{
private:
    ExpressionPtr Declaration_list;
    ExpressionPtr Statement_list;

public:
    Compound_Statement(const ExpressionPtr _Declaration_list, const ExpressionPtr _Statement_list)
        : Declaration_list(_Declaration_list), Statement_list(_Statement_list)
    {};

    virtual ~Compound_Statement()
    {
        delete Declaration_list;
        delete Statement_list;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_list->print(dst);
        dst<<" ";
        Statement_list->print(dst);
        dst<<" )";
    }
};

#endif
