#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>

class Unary
    : public Expression
{
private:
    ExpressionPtr expr;
protected:
    Unary(const ExpressionPtr _expr)
        : expr(_expr)
    {}
public:
    virtual ~Unary()
    {
        delete expr;
    }

    virtual const char *getOpcode() const =0;

    ExpressionPtr getExpr() const
    { return expr; }

    virtual void print(std::ostream &dst) const override
    {
        dst << "( ";
        dst << getOpcode();
        dst << " ";
        expr->print(dst);
        dst << " )";
    }
};

class NegUnary
    : public Unary
{
public:
    NegUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "-"; }

    virtual double evaluate(
        const std::map<std::string, double> &bindings
    ) const override
    {
        // TODO-F: Implement this similar to how AddOperator was implemented.
        double value = getExpr()->evaluate(bindings);
        return -value;
    }
};

class PreIncUnary
    : public Unary
{
public:
    PreIncUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "++"; }
};

class PreDecUnary
    : public Unary
{
public:
    PreDecUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "--"; }
};

class ReferenceUnary
    : public Unary
{
public:
    ReferenceUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "&"; }
};

class PointerUnary
    : public Unary
{
public:
    PointerUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "*"; }
};

class PlusUnary
    : public Unary
{
public:
    PlusUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "+"; }
};

class BnotUnary
    : public Unary
{
public:
    BnotUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "~"; }
};

class LognotUnary
    : public Unary
{
public:
    LognotUnary(const ExpressionPtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "!"; }
};
#endif
