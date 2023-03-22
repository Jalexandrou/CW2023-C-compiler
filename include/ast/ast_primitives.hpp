#ifndef ast_primitives_hpp
#define ast_primitives_hpp

#include <string>
#include <iostream>

class Identifier
    : public Node
{
private:
    std::string id;
public:
    Identifier(const std::string &_id)
        : id(_id)
    {}

    const std::string getId() const override
    { return id; }

    void print(std::ostream &dst) const override
    {
        dst<<id;
    }

    void compile(std::ostream &dst, std::string destReg, Context &context) const{
        //Load the Immediate Value then store it into the stack, as the stack is larger change the context offset
        if(context.bindings_list.back().count(getId())){
            dst << "\tlw      " << destReg << ", " << context.bindings_list.back()[getId()] << "(s0)" << std::endl;
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.changeOffset(-4);
        }else{
            //SHOULD NOT OCCOUR JUST ACTS AS A BACKUP
            dst << "\tli      " << destReg << ", " << 0 << std::endl;
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.bindings_list.back()[getId()] = context.pointerOffset;
            context.changeOffset(-4);
        }
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // TODO-B : Run bin/eval_expr with a variable binding to make sure you understand how this works.
        // If the binding does not exist, this will throw an error
        return bindings.at(id);
    }
};

class Number
    : public Node
{
private:
    double value;
public:
    Number(double _value)
        : value(_value)
    {}

    double getValue() const
    { return value; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<value;
    }

    void compile(std::ostream &dst, std::string destReg, Context &context) const {
        //Load the Immediate Value then store it into the stack, as the stack is larger change the context offset
        dst << "\tli      " << destReg << ", " << getValue() << std::endl;
        dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4);
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // TODO-A : Run bin/eval_expr with a numeric Node to make sure you understand how this works.
        return value;
    }
};

#endif
