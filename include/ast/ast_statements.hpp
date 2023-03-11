#ifndef ast_statements_hpp
#define ast_statements_hpp


class Statement_list
    : public Node
{
private:
    NodePtr Statement_left;
    NodePtr Statement_right;

public:
    Statement_list(NodePtr _Statement_left, NodePtr _Statement_right)
        : Statement_left(_Statement_left), Statement_right(_Statement_right)
    {};

    virtual ~Statement_list()
    {
        delete Statement_left;
        delete Statement_right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Statement_left->print(dst);
        dst<<" ";
        Statement_right->print(dst);
        dst<<" )";
    }
};

class JumpExpressionStatement
    : public Node
{
private:
    NodePtr expr;

protected:
    JumpExpressionStatement(const NodePtr _expr)
        : expr(_expr)
    {}

public:

    virtual ~JumpExpressionStatement()
    {
        delete expr;
    }

    virtual const char *getStatement() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        dst << getStatement();
        dst << " ";
        expr->print(dst);
        dst<<"; )";
    }
};

class JumpStatement: public Node
{
protected:
    JumpStatement()
    {}

public:
    virtual ~JumpStatement()
    {}

    virtual const char *getStatement() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        dst<<getStatement();
        dst<<"; )";
    }
};

class ReturnExpressionStatement 
    : public JumpExpressionStatement
{
public:
    ReturnExpressionStatement(const NodePtr _Statement)
        : JumpExpressionStatement(_Statement)
    {}

    virtual const char *getStatement() const override
    { return "return"; }
};

class ReturnStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "return"; }
};

class BreakStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "break"; }
};

class ContinueStatement: public JumpStatement
{
public:
    virtual const char *getStatement() const override
    { return "continue"; }
};


#endif