#ifndef ast_codeflow_hpp
#define ast_codeflow_hpp


class If
    : public Node
{
private:
    NodePtr arg;
    NodePtr compoundStatement;

public:
    If(const NodePtr _arg,const  NodePtr _compoundStatement)
        : arg(_arg), compoundStatement(_compoundStatement)
    {}

    virtual ~If()
    {
        delete arg;
        delete compoundStatement;
    }

    NodePtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"if ( ";
        arg->print(dst);
        dst<<" ) ";
        dst << "{";
        compoundStatement->print(dst);
        dst << "} ";
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        arg->compile(dst, "x5", context);
        std::string falseLabel = makeName("False");

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        dst << "\tbeq     " << "x5, " << "x0, " <<  falseLabel << std::endl;

        compoundStatement->compile(dst, destReg, context);

        dst << falseLabel << ":" << std::endl;
    }
};

class If_Else
    : public Node
{
private:
    NodePtr arg;
    NodePtr compoundStatement;
    NodePtr elseCompoundStatement;

public:
    If_Else(const NodePtr _arg,const  NodePtr _compoundStatement, const NodePtr _elseCompoundStatement)
        : arg(_arg), compoundStatement(_compoundStatement), elseCompoundStatement(_elseCompoundStatement)
    {}

    virtual ~If_Else()
    {
        delete arg;
        delete compoundStatement;
        delete elseCompoundStatement;
    }

    NodePtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"if ( ";
        arg->print(dst);
        dst<<" ) ";
        dst << "{";
        compoundStatement->print(dst);
        dst << "} ";
        dst << "else";
        dst << " { ";
        elseCompoundStatement->print(dst);
        dst << "}";
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        arg->compile(dst, "x5", context);
        std::string falseLabel = makeName("False");
        std::string endLabel = makeName("If_End");

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;

        dst << "\tbeq     " << "x5, " << "x0, " <<  falseLabel << std::endl;

        //Compile the true section
        compoundStatement->compile(dst, destReg, context);

        //add a jump to "skip" the false section
        dst << "\tj       " << endLabel <<std::endl;

        //add a label for the jump section
        dst << falseLabel << ":" << std::endl;

        //compile the else section
        elseCompoundStatement->compile(dst, destReg, context);

        //Add a label for the end to allow to jump to if true is finished
        dst <<endLabel << ":" << std::endl;
    }
};


class while_Node
    : public Node
{
private:
    NodePtr arg;
    NodePtr compoundStatement;

public:
    while_Node(const NodePtr _arg, const NodePtr _compoundStatement)
        : arg(_arg), compoundStatement(_compoundStatement)
    {}

    virtual ~while_Node()
    {
        delete arg;
        delete compoundStatement;
    }

    NodePtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"while ( ";
        arg->print(dst);
        dst<<" ) ";
        dst << "{";
        compoundStatement->print(dst);
        dst << "} ";
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        std::string startLabel = makeName("While_Start");
        std::string endLabel = makeName("While_End");


        dst << startLabel << ":" << std::endl;

        arg->compile(dst, "x5", context);

        context.changeOffset(4); //As we are loading values change offset
        dst << "\tlw      " << "x5, " << context.pointerOffset << "(s0)" << std::endl;
        dst << "\tbeq     " << "x5, " << "x0, " << endLabel << std::endl;

        compoundStatement->compile(dst, destReg, context);

        dst << "\tj       " << startLabel <<std::endl;

        dst << endLabel << ":" << std::endl;

    }
};

#endif
