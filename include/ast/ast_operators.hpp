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

        if(symbol == "*"){
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
            dst << "\tmul     " << destReg << ", x6, x5\n";
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset -= 4; //As we are loading values change offset

        }else if (symbol == "/"){
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset += 4;
            dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
            dst << "\tdiv     " << destReg << ", x6, x5\n";
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset -= 4; //As we are loading values change offset

        }else if (symbol == "%"){
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset += 4;
            dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
            dst << "\trem     " << destReg << ", x6, x5\n";
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset -= 4; //As we are loading values change offset

        }else if (symbol == "+"){
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset += 4;
            dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
            dst << "\tadd     " << destReg << ", x6, x5\n";
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset -= 4; //As we are loading values change offset

        }else if (symbol == "-"){
            context.pointerOffset += 4; //As we are loading values change offset
            dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset += 4;
            dst << "\tlw      " << "x6, " << context.pointerOffset << "(s0)" << std::endl;
            dst << "\tsub     " << destReg << ", x6, x5\n";
            dst << "\tsw      " << destReg << ", " << context.pointerOffset << "(s0)" << std::endl;
            context.pointerOffset -= 4; //As we are loading values change offset

        }else{
            throw std::runtime_error("Operator not implemented");
        }
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
        right->compile(dst, "x6", context);

        if (symbol == "="){

            //we use x6 as we are storing the right side of the assignment, which was compiled into x6
            //check if x exists then overwrite using bindings or define new val
            if(context.bindings.count(left->getId())){
                context.pointerOffset += 4;
                dst << "\tsw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
            }else{
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "*="){


            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tmul     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }


        }else if (symbol == "/="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tdiv     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "%="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\trem     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "+="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tadd     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "-="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tsub     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "<<="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tsll     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == ">>="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tsra     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "&="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tand     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "^="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\txor     " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else if (symbol == "|="){

            if(context.bindings.count(left->getId())){
                dst << "\tlw      " << "x6" << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset += 4; //As we are loading values change offset
                dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
                dst << "\tor      " << destReg << ", x6, x5\n";
                dst << "\tsw      " << destReg << ", " << context.bindings[left->getId()] << "(s0)" << std::endl;
                context.pointerOffset -= 4;

            }else{
                //SHOULD NEVER RUN
                context.pointerOffset += 4;
                context.bindings[left->getId()] = context.pointerOffset;
                context.pointerOffset -= 4;
            }

        }else {
            throw std::runtime_error("This Assignment is not implemented");
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

        if (symbol == "=="){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else if (symbol == "!="){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else if (symbol == "<"){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else if (symbol == ">"){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else if (symbol == "<="){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else if (symbol == ">="){
            throw std::runtime_error("This Logical Operator is not implemented");

        }else {
            throw std::runtime_error("This Logical Operator is not implemented");
        }
    }
};

#endif
