#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>

class Unary
    : public Node
{
private:
    NodePtr expr;
    std::string Opcode;

public:

    Unary(NodePtr _expr, std::string _Opcode)
        : expr(_expr)
        , Opcode(_Opcode)
    {}

    ~Unary()
    {
        delete expr;
    }

    std::string getOpcode() const
    { return Opcode; }

    NodePtr getExpr() const
    { return expr; }

    void print(std::ostream &dst) const
    {
        dst << "( ";
        dst << getOpcode();
        dst << " ";
        expr->print(dst);
        dst << " )";
    }

    void compile(std::ostream &dst, std::string destReg, Context &context) const {
        std::string opcode = getOpcode();
        expr->compile(dst, destReg, context);
        if(opcode == "-"){
            dst << "\tneg     " << destReg << ", " << destReg << "\n";
        }else if(opcode == "++"){
            dst << "\taddi    " << destReg << ", " << destReg << ", 1"<< "\n";
        }else if(opcode == "--"){
            dst << "\taddi    " << destReg << ", " << destReg << ", -1"<< "\n";
        }else if(opcode == "&"){
            //fetch from context
        }else if(opcode == "*"){
            //fetch from context
        }else if(opcode == "+"){
            //Does this do anything?
        }else if(opcode == "~"){
            dst << "\tnot     " << destReg << ", " << destReg << "\n";
        }else if(opcode == "!"){
            //Weird ASM
        }else {
            throw std::runtime_error("This Unary Operator is not implemented");
        }
    }
};

#endif
