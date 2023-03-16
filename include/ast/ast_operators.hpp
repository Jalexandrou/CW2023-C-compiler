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
    std::string opcode;

public:
    Operator(NodePtr _left, std::string _opcode, NodePtr _right)
        : left(_left)
        , opcode(_opcode)
        , right(_right)
    {}

    ~Operator()
    {
        delete left;
        delete right;
    }

    NodePtr getLeft() const
    { return left; }

    NodePtr getRight() const
    { return right; }

    std::string getOpcode() const
    { return opcode; }

    void print(std::ostream &dst) const
    {
        dst<<"( ";
        left->print(dst);
        dst<<" ";
        dst<<getOpcode();
        dst<<" ";
        right->print(dst);
        dst<<" )";
    }

    void compile(std::ostream &dst, std::string destReg)const {
        std::string symbol = getOpcode();
        //context??
        left->compile(dst, "a1");
        right->compile(dst, "a2");

        if(symbol == "*"){
            dst << "\tmul     " << destReg << ", a1, a2\n";

        }else if (symbol == "/"){
            dst << "\tdiv     " << destReg << ", a1, a2\n";

        }else if (symbol == "%"){
            dst << "\tmod     " << destReg << ", a1, a2\n";

        }else if (symbol == "+"){
            dst << "\tadd     " << destReg << ", a1, a2\n";

        }else if (symbol == "-"){
            dst << "\tsub     " << destReg << ", a1, a2\n";

        }else if (symbol == "="){
            dst << "\tseqz    a1, a1, a2\n";

        }else if (symbol == "=="){
            dst << "\tsub     " << destReg << ", a1, a2\n";

        }else if (symbol == "*="){
            dst << "\tmod     a1, a1, a2\n";

        }else if (symbol == "/="){
            dst << "\tadd     a1, a1, a2\n";

        }else if (symbol == "%="){
            dst << "\tsub     a1, a1, a2\n";

        }else if (symbol == "+="){
            dst << "\tseqz    a1, a1, a2\n";

        }else if (symbol == "-="){
            dst << "\tdiv     a1, a1, a2\n";

        }else if (symbol == "<<="){
            dst << "\tmod     a1, a1, a2\n";

        }else if (symbol == ">>="){
            dst << "\tadd     a1, a1, a2\n";

        }else if (symbol == "&="){
            dst << "\tsub     a1, a1, a2\n";

        }else if (symbol == "^="){
            dst << "\tseqz    a1, a1, a2\n";

        }else if (symbol == "|="){
            dst << "\tdiv     a1, a1, a2\n";

        }else {
            throw std::runtime_error("This Opcode is not implemented");
        }
    }
};

#endif
