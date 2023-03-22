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

class Init_Declarator_List
    : public Node
{
private:
    NodePtr Declarator;
    NodePtr Declarator_List;

public:
    Init_Declarator_List(const NodePtr _Declarator, const NodePtr _Declarator_List)
        : Declarator(_Declarator), Declarator_List(_Declarator_List)
    {};

    virtual ~Init_Declarator_List()
    {
        delete Declarator;
        delete Declarator_List;
    }

    virtual void print(std::ostream &dst) const override
    {

    }

    std::vector<std::string> get_Id_List() const override {

        //recursively add to a vector each id within a given declarator list
        std::vector<std::string> Id_List = Declarator_List->get_Id_List();
        std::string Declarator_Id = Declarator->getId();
        Id_List.push_back(Declarator_Id);
        return Id_List;

    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        Declarator_List->compile(dst, destReg, context);
        Declarator->compile(dst, destReg, context);

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

    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        std::vector<std::string> Id_List = Init_declarator_list->get_Id_List();

        for( int i=0; i< Id_List.size(); i++){
            if(context.bindings_list.back().count(Id_List[i])){
                context.bindings_list.back().erase(Id_List[i]);
            }
        }

        // //Check if variable already exists, if it does erase it (for scopes)
        Declaration_Specifier->compile(dst, destReg, context);
        Init_declarator_list->compile(dst, destReg, context);

    }
};

#endif
