#ifndef ast_declarations_hpp
#define ast_declarations_hpp

#include "ast_expression.hpp"

class Declaration_list
    : public Expression
{
private:
    ExpressionPtr Declaration_left;
    ExpressionPtr Declaration_right;

public:
    Declaration_list(ExpressionPtr _Declaration_left, ExpressionPtr _Declaration_right)
        : Declaration_left(_Declaration_left), Declaration_right(_Declaration_right)
    {};

    virtual ~Declaration_list()
    {
        delete Declaration_left;
        delete Declaration_right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_left->print(dst);
        dst<<" ";
        Declaration_right->print(dst);
        dst<<" )";
    }
};

class Declaration
    : public Expression
{
private:
    ExpressionPtr Declaration_Specifier;
    ExpressionPtr Init_declarator_list;

public:
    Declaration(ExpressionPtr _Declaration_Specifier, ExpressionPtr _Init_declarator_list)
        : Declaration_Specifier(_Declaration_Specifier), Init_declarator_list(_Init_declarator_list)
    {};

    virtual ~Declaration()
    {
        delete Declaration_Specifier;
        delete Init_declarator_list;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_Specifier->print(dst);
        dst<<" ";
        Init_declarator_list->print(dst);
        dst<<"; )";
    }
};

#endif
