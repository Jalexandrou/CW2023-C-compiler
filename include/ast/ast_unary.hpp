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
        expr->compile(dst, "x5", context);

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        if(opcode == "-"){
            dst << "\tneg     " << destReg << ", " << "x5" << "\n";
        }else if(opcode == "++"){
            dst << "\taddi    " << destReg << ", " << "x5" << ", 1"<< "\n";

            dst << "\tsw      " << destReg << ", " << context.bindings_list.back()[expr->getId()] << "(s0)" << std::endl;


        }else if(opcode == "--"){
            dst << "\taddi    " << destReg << ", " << "x5" << ", -1"<< "\n";

            dst << "\tsw      " << destReg << ", " << context.bindings_list.back()[expr->getId()] << "(s0)" << std::endl;

        }else if(opcode == "&"){
            //fetch from context
        }else if(opcode == "*"){
            //fetch from context
        }else if(opcode == "+"){
            //Does this do anything?
        }else if(opcode == "~"){
            dst << "\tnot     " << destReg << ", " << "x5" << "\n";
        }else if(opcode == "!"){
            dst << "\tseqz    " << "x5, x5\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";
        }else {
            throw std::runtime_error("This Unary Operator is not implemented");
        }
        dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4); //As we are loading values change offset
    }
};

class Postfix_Expression
    : public Node
{
private:
    NodePtr expr;
    std::string Opcode;

public:

    Postfix_Expression(NodePtr _expr, std::string _Opcode)
        : expr(_expr)
        , Opcode(_Opcode)
    {}

    ~Postfix_Expression()
    {
        delete expr;
    }

    std::string getOpcode() const
    { return Opcode; }

    NodePtr getExpr() const
    { return expr; }

    void print(std::ostream &dst) const
    {

    }

    void compile(std::ostream &dst, std::string destReg, Context &context) const {
        std::string opcode = getOpcode();
        expr->compile(dst, "x5", context);

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        if(opcode == "-"){
            dst << "\tneg     " << "x5" << ", " << destReg << "\n";

            dst << "\tsw      " << "x5" << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.changeOffset(-4); //As we are loading values change offset
        }else if(opcode == "++"){

            dst << "\taddi    " << "x5" << ", " << "x5" << ", 1"<< "\n";

            dst << "\tsw      " << "x5" << ", " << context.bindings_list.back()[expr->getId()] << "(s0)" << std::endl;

        }else if(opcode == "--"){

            dst << "\taddi    " << "x5" << ", " << "x5" << ", -1"<< "\n";

            dst << "\tsw      " << "x5" << ", " << context.bindings_list.back()[expr->getId()] << "(s0)" << std::endl;

        }else if(opcode == "&"){
            //fetch from context
        }else if(opcode == "*"){
            //fetch from context
        }else if(opcode == "+"){
            //Does this do anything?
        }else if(opcode == "~"){

            dst << "\tnot     " << "x5" << ", " << "x5" << "\n";

            dst << "\tsw      " << "x5" << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.changeOffset(-4); //As we are loading values change offset
        }else if(opcode == "!"){

            dst << "\tseqz    " << "x5, x5\n";
            dst << "\tandi    " << "x5" << ", x5, 0xff\n";

            dst << "\tsw      " << "x5" << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.changeOffset(-4); //As we are loading values change offset
        }else {
            throw std::runtime_error("This Unary Operator is not implemented");
        }

    }
};

#endif
