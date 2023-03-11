#ifndef ast_function_definition_hpp
#define ast_function_definition_hpp


class Function_Definition
    : public Node
{
private:
    NodePtr Type;
    NodePtr Declarator;
    NodePtr Compound_Statement;

public:
    Function_Definition(NodePtr _Type, NodePtr _Declarator, NodePtr _Compound_Statement)
        : Type(_Type), Declarator(_Declarator), Compound_Statement(_Compound_Statement)
    {};

    virtual ~Function_Definition()
    {
        delete Type;
        delete Declarator;
        delete Compound_Statement;
    }

    NodePtr getType() const
    { return Type; }

    NodePtr getDeclarator() const
    { return Declarator; }

    NodePtr getCompound_Statement() const
    { return Compound_Statement; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Type->print(dst);
        dst<<" ";
        Declarator->print(dst);
        dst<<"()";
        dst<<" { ";
        Compound_Statement->print(dst);
        dst<<" } )";
    }
};

class Compound_Statement
    : public Node
{
private:
    NodePtr Declaration_list;
    NodePtr Statement_list;

public:
    Compound_Statement(const NodePtr _Declaration_list, const NodePtr _Statement_list)
        : Declaration_list(_Declaration_list), Statement_list(_Statement_list)
    {};

    virtual ~Compound_Statement()
    {
        delete Declaration_list;
        delete Statement_list;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_list->print(dst);
        dst<<" ";
        Statement_list->print(dst);
        dst<<" )";
    }
};

#endif
