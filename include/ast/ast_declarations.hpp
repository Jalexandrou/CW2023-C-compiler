#ifndef ast_declarations_hpp
#define ast_declarations_hpp


class Declaration_list
    : public Node
{
private:
    NodePtr Declaration_left;
    NodePtr Declaration_right;

public:
    Declaration_list(const NodePtr _Declaration_left, const NodePtr _Declaration_right)
        : Declaration_left(_Declaration_left), Declaration_right(_Declaration_right)
    {};

    virtual ~Declaration_list()
    {
        delete Declaration_left;
        delete Declaration_right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_left->print(dst);
        dst<<" ";
        Declaration_right->print(dst);
        dst<<" )";
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        Declaration_left->compile(dst, destReg, context);
        Declaration_right->compile(dst, destReg, context);
    }
};

class Declaration
    : public Node
{
private:
    NodePtr Declaration_Specifier;
    NodePtr Init_declarator_list;

public:
    Declaration(const NodePtr _Declaration_Specifier, const NodePtr _Init_declarator_list)
        : Declaration_Specifier(_Declaration_Specifier), Init_declarator_list(_Init_declarator_list)
    {};

    virtual ~Declaration()
    {
        delete Declaration_Specifier;
        delete Init_declarator_list;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        Declaration_Specifier->print(dst);
        dst<<" ";
        Init_declarator_list->print(dst);
        dst<<"; )";
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        //Check if variable already exists, if it does erase it (for scopes)
        if(context.bindings_list.back().count(Init_declarator_list->getId())){
            context.bindings_list.back().erase(Init_declarator_list->getId());
        }

        Declaration_Specifier->compile(dst, destReg, context);
        Init_declarator_list->compile(dst, destReg, context);

    }
};

#endif
