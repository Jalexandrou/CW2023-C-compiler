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

    virtual void compile(std::ostream &dst, std::string destReg) const override {
        Statement_left->compile(dst, destReg);
        Statement_right->compile(dst, destReg);
    }
};

class JumpExpressionStatement
    : public Node
{
private:
    NodePtr expr;
    std::string statement;

public:

    JumpExpressionStatement(NodePtr _expr, std::string _statement)
        : expr(_expr)
        , statement(_statement)
    {}


    ~JumpExpressionStatement()
    {
        delete expr;
    }

    std::string getStatement() const {
        return statement;
    };

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        dst << getStatement();
        dst << " ";
        expr->print(dst);
        dst<<"; )";
    }

    virtual void compile(std::ostream &dst, std::string destReg) const override {
        std::string statement = getStatement();
        if (statement == "return"){
            expr->compile(dst, destReg);
        }
    }
};

class JumpStatement: public Node
{
private:
    std::string type;

public:

    JumpStatement(std::string _type)
        : type (_type)
    {}

    ~JumpStatement()
    {}

    std::string getStatement() const{
        return type;
    }

    void print(std::ostream &dst) const {
        dst<<"( ";
        dst<<getStatement();
        dst<<"; )";
    }

    void compile(std::ostream &dst, std::string destReg)const {
        std::string statement = getStatement();
        if (statement == "return"){

        }else if(statement == "break"){

        }else if(statement == "continue"){

        }else{
            throw std::runtime_error("This JumpStatement is not implemented");
        }
    }

};



#endif
