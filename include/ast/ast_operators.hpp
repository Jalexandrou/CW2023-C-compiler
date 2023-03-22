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

    void compile(std::ostream &dst, std::string destReg, Context &context)const {
        std::string symbol = getOpcode();
        //context PointerOffset is reduced after an operation
        left->compile(dst, "x5", context);
        right->compile(dst, "x6", context);

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        if(symbol == "*"){
            dst << "\tmul     " << destReg << ", x5, x6\n";

        }else if (symbol == "/"){

            dst << "\tdiv     " << destReg << ", x5, x6\n";

        }else if (symbol == "%"){

            dst << "\trem     " << destReg << ", x5, x6\n";

        }else if (symbol == "+"){

            dst << "\tadd     " << destReg << ", x5, x6\n";

        }else if (symbol == "-"){

            dst << "\tsub     " << destReg << ", x5, x6\n";

        }else{
            throw std::runtime_error("Operator not implemented");
        }

        dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4); //As we are loading values change offset
    }
};

class AssignmentOperator
    : public Node
{
private:
    NodePtr left;
    NodePtr right;
    std::string opcode;

public:
    AssignmentOperator(NodePtr _left, std::string _opcode, NodePtr _right)
        : left(_left)
        , opcode(_opcode)
        , right(_right)
    {}

    ~AssignmentOperator()
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

    void compile(std::ostream &dst, std::string destReg, Context &context)const {
        std::string symbol = getOpcode();
        //context PointerOffset is reduced after an operation
        right->compile(dst, destReg, context);

        if(context.bindings_list.back().count(left->getId())){

            if (symbol == "="){
                context.changeOffset(4);
                dst << "\tsw      " << destReg << ", " << context.bindings_list.back()[left->getId()] << "(s0)" << std::endl;
            }else{

                dst << "\tlw      " << "x6" << ", " << context.bindings_list.back()[left->getId()] << "(s0)" << std::endl;
                context.changeOffset(4); //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

                if(symbol == "*="){
                    dst << "\tmul     " << destReg << ", x6, x5\n";

                }else if(symbol == "/="){
                    dst << "\tdiv     " << destReg << ", x6, x5\n";

                }else if(symbol == "%="){
                    dst << "\trem     " << destReg << ", x6, x5\n";

                }else if(symbol == "+="){
                    dst << "\tadd     " << destReg << ", x6, x5\n";

                }else if(symbol == "-="){
                    dst << "\tsub     " << destReg << ", x6, x5\n";

                }else if(symbol == "<<="){
                    dst << "\tsll     " << destReg << ", x6, x5\n";

                }else if(symbol == ">>="){
                    dst << "\tsra     " << destReg << ", x6, x5\n";

                }else if(symbol == "&="){
                    dst << "\tand     " << destReg << ", x6, x5\n";

                }else if(symbol == "^="){
                    dst << "\txor     " << destReg << ", x6, x5\n";

                }else if(symbol == "|="){
                    dst << "\tor      " << destReg << ", x6, x5\n";
                }else {
                    throw std::runtime_error("This Assignment is not implemented");
                }

                dst << "\tsw      " << destReg << ", " << context.bindings_list.back()[left->getId()] << "(s0)" << std::endl;
                dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
                context.changeOffset(-4);
            }

        }else{
            context.changeOffset(4);
            context.bindings_list.back()[left->getId()] = context.pointerOffset;
            context.changeOffset(-4);
        }
    }
};

class LogicalOperator
    : public Node
{
private:
    NodePtr left;
    NodePtr right;
    std::string opcode;

public:
    LogicalOperator(NodePtr _left, std::string _opcode, NodePtr _right)
        : left(_left)
        , opcode(_opcode)
        , right(_right)
    {}

    ~LogicalOperator()
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

    void compile(std::ostream &dst, std::string destReg, Context &context)const {
        std::string symbol = getOpcode();
        //context PointerOffset is reduced after an operation
        left->compile(dst, "x5", context);
        right->compile(dst, "x6", context);

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(4);
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        if (symbol == "=="){

            dst << "\tsub     " << "x5" << ", x6, x5\n";
            dst << "\tseqz    " << "x5, x5\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";


        }else if (symbol == "!="){

            dst << "\tsub     " << "x5" << ", x6, x5\n";
            dst << "\tsnez    " << "x5, x5\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";

        }else if (symbol == "<"){
            dst << "\tslt     " << "x5, x5, x6\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";

        }else if (symbol == ">"){
            dst << "\tsgt     " << "x5, x5, x6\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";

        }else if (symbol == "<="){
            dst << "\tsgt     " << "x5, x5, x6\n";
            dst << "\txori    " << "x5, x5, 1\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";

        }else if (symbol == ">="){
            dst << "\tslt     " << "x5, x5, x6\n";
            dst << "\txori    " << "x5, x5, 1\n";
            dst << "\tandi    " << destReg << ", x5, 0xff\n";

        }else {
            throw std::runtime_error("This Logical Operator is not implemented");
        }

        dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4); //As we are loading values change offset
    }
};

#endif
