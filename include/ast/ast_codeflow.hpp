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

    virtual void compile(std::ostream &dst, std::string destReg) const override {}
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

    virtual void compile(std::ostream &dst, std::string destReg) const override {}
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

    virtual void compile(std::ostream &dst, std::string destReg) const override {}
};

#endif