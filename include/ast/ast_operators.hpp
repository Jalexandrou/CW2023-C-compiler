#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include <iostream>
#include <math.h>

class Operator
    : public Expression
{
private:
    ExpressionPtr left;
    ExpressionPtr right;
protected:
    Operator(ExpressionPtr _left, ExpressionPtr _right)
        : left(_left)
        , right(_right)
    {}
public:
    virtual ~Operator()
    {
        delete left;
        delete right;
    }

    virtual const char *getOpcode() const =0;

    ExpressionPtr getLeft() const
    { return left; }

    ExpressionPtr getRight() const
    { return right; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        left->print(dst);
        dst<<" ";
        dst<<getOpcode();
        dst<<" ";
        right->print(dst);
        dst<<" )";
    }
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }
public:
    AddOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override 
    {
        // TODO-C : Run bin/eval_expr with something like 5+a, where a=10, to make sure you understand how this works
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl+vr;
    }
};

class SubOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "-"; }
public:
    SubOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override 
    {
        // TODO-D : Implement this, based on AddOperator::evaluate
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl-vr;
    }
};


class MulOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "*"; }
public:
    MulOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl*vr;    
    }
};

class DivOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "/"; }
public:
    DivOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl/vr;
    }
};

class ModOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "%"; }
public:
    ModOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return fmod(vl,vr);
    }
};

class ExpOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "^"; }
public:
    ExpOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        double value = pow(vl, vr);
        return value;
    }
};


class AssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "=";
    }
    public:
    AssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class MulAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "*=";
    }
    public:
    MulAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class DivAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "/=";
    }
    public:
    DivAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class ModAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "%=";
    }
    public:
    ModAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class AddAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "+=";
    }
    public:
    AddAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class SubAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "-=";
    }
    public:
    SubAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class LeftAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "<<=";
    }
    public:
    LeftAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class RightAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return ">>=";
    }
    public:
    RightAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class AndAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "&=";
    }
    public:
    AndAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class XorAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "^=";
    }
    public:
    XorAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class OrAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "|=";
    }
    public:
    OrAssignOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};
#endif
