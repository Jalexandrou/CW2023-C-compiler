#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include <iostream>
#include <math.h>

class Operator
    : public Node
{
private:
    NodePtr left;
    NodePtr right;
protected:
    Operator(NodePtr _left, NodePtr _right)
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

    NodePtr getLeft() const
    { return left; }

    NodePtr getRight() const
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

    virtual void compile(std::ostream &dst, std::string destReg) const override {}
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }
public:
    AddOperator(NodePtr _left, NodePtr _right)
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
    SubOperator(NodePtr _left, NodePtr _right)
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
    MulOperator(NodePtr _left, NodePtr _right)
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
    DivOperator(NodePtr _left, NodePtr _right)
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
    ModOperator(NodePtr _left, NodePtr _right)
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
    ExpOperator(NodePtr _left, NodePtr _right)
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
    AssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class MulAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "*=";
    }
    public:
    MulAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class DivAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "/=";
    }
    public:
    DivAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class ModAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "%=";
    }
    public:
    ModAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class AddAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "+=";
    }
    public:
    AddAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class SubAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "-=";
    }
    public:
    SubAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class LeftAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "<<=";
    }
    public:
    LeftAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class RightAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return ">>=";
    }
    public:
    RightAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class AndAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "&=";
    }
    public:
    AndAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class XorAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "^=";
    }
    public:
    XorAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};

class OrAssignOperator: public Operator {
    protected:
    virtual const char *getOpcode() const override {
        return "|=";
    }
    public:
    OrAssignOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

};
#endif
