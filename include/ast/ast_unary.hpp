#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>

class Unary
    : public Node
{
private:
    NodePtr expr;
protected:
    Unary(const NodePtr _expr)
        : expr(_expr)
    {}
public:
    virtual ~Unary()
    {
        delete expr;
    }

    virtual const char *getOpcode() const =0;

    NodePtr getExpr() const
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
    NegUnary(const NodePtr _expr)
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
    PreIncUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "++"; }
};

class PreDecUnary
    : public Unary
{
public:
    PreDecUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "--"; }
};

class ReferenceUnary
    : public Unary
{
public:
    ReferenceUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "&"; }
};

class PointerUnary
    : public Unary
{
public:
    PointerUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "*"; }
};

class PlusUnary
    : public Unary
{
public:
    PlusUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "+"; }
};

class BnotUnary
    : public Unary
{
public:
    BnotUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "~"; }
};

class LognotUnary
    : public Unary
{
public:
    LognotUnary(const NodePtr _expr)
        : Unary(_expr)
    {}

    virtual const char *getOpcode() const override
    { return "!"; }
};
#endif
