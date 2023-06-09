#ifndef ast_function_definition_hpp
#define ast_function_definition_hpp

typedef std::vector<NodePtr> Arglist;
typedef Arglist *ArglistPtr;

inline ArglistPtr initArglist(NodePtr arg){
    ArglistPtr arglist = new Arglist();
    arglist->push_back(arg);
    return arglist;
}

inline ArglistPtr ArglistAppend(ArglistPtr arglist, NodePtr arg){
  arglist->push_back(arg);
  return arglist;
}

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

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        dst << ".globl " << Declarator->getId() << std::endl; //Add context checking temp

        //copy the current scope into a new vector element
        std::map<std::string, double> copy = context.bindings_list.back();
        context.bindings_list.push_back(copy);

        dst << Declarator->getId() << ':' << std::endl;


        std::string Function_End_Label = makeName("Function_End");

        std::stringstream temp;

        //Set the function end label within the current context to the end of the current function
        context.Function_End_Label = Function_End_Label;

        Declarator->compile(temp, destReg, context);
        //Compile the code into the temp string stream
        Compound_Statement->compile(temp, destReg, context);

        //Get the current minimum needed stack size for the function
        //This is a negative number so make it positive
        int currentStackSize = -context.getStackSize() + 4;

        //Set stack size, should be equal to max PointerOffset + StackBuffer
        dst << "\taddi    sp,  sp, " << -currentStackSize << std::endl;

        //Store return address one below the end of the current stack
        dst << "\tsw      ra,  " << currentStackSize - 4 << "(sp)\n";
        //Store the Frame pointer into an address 2 below the bottom of the stack
        dst << "\tsw      s0,  " << currentStackSize - 8 << "(sp)\n";
        //Set the Frame pointer to the top of the last stack
        dst << "\taddi    s0,  sp, " << currentStackSize << std::endl;

        //print the string stream into the main output
        dst << temp.str();

        //Add an end label to jump to in case of return
        dst << Function_End_Label << ":" << std::endl;

        dst << "\tlw      ra,  " << currentStackSize - 4 << "(sp)\n";
        dst << "\tlw      s0,  " << currentStackSize - 8 << "(sp)\n";
        dst << "\taddi    sp,  sp, " << currentStackSize << std::endl;
        dst << "\tjr      ra\n";

        //pop off the last scope of the vector once we leave this current scope
        context.bindings_list.pop_back();
    }
};

class Null_Declarator
    : public Node
{
private:
    NodePtr Identifier;

public:
    Null_Declarator(const NodePtr _Identifier)
        : Identifier(_Identifier)
    {};

    virtual ~Null_Declarator()
    {
        delete Identifier;
    }

    const std::string getId() const override
    { return Identifier->getId(); }

    virtual void print(std::ostream &dst) const override
    {}

     std::vector<std::string> get_Id_List() const override
    {
        //Return empty vector so if called memory can be dealocated successfully
        return {};
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        //Do nothing
    }

};

class Param_Declarator
    : public Node
{
private:
    NodePtr Direct_Declarator;
    NodePtr Parameter_Type_List;

public:
    Param_Declarator(const NodePtr _Direct_Declarator, const NodePtr _Parameter_Type_List)
        : Direct_Declarator(_Direct_Declarator), Parameter_Type_List(_Parameter_Type_List)
    {};

    virtual ~Param_Declarator()
    {
        delete Direct_Declarator;
        delete Parameter_Type_List;
    }

    const std::string getId() const override
    { return Direct_Declarator->getId(); }

    std::vector<std::string> get_Id_List() const override
    {
        //Return empty vector so if called memory can be dealocated successfully
        return {};
    }

    virtual void print(std::ostream &dst) const override
    {}

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

        //compiler param list, adding variables to scope
        Parameter_Type_List->compile(dst, destReg, context);

        //Store each param register value into the memory location of each param
        for(int i = 0; i<context.param_num; i++){
            dst << "\tsw      a" << i  << ", " << context.bindings_list.back()[context.param_map[i]] << "(s0)" << std::endl;
        }

        //reset param values in context ready for next function call
        context.param_map.clear();
        context.param_num = 0;
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
    {}

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const override{
        //copy the current scope into a new vector element
        std::map<std::string, double> copy = context.bindings_list.back();
        context.bindings_list.push_back(copy);

        //compile the current compound statement
        Declaration_list->compile(dst, destReg, context);
        Statement_list->compile(dst, destReg, context);

        //pop off the last scope of the vector once we leave this current scope
        context.bindings_list.pop_back();
    }
};

class Empty_Compound_Statement
    : public Node
{

public:
    Empty_Compound_Statement()
    {};

    virtual void print(std::ostream &dst) const override
    {

    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {

    }
};

class Function_Call
    : public Node
{
private:
    NodePtr Direct_Declarator;

public:
    Function_Call(const NodePtr Direct_Declarator)
        : Direct_Declarator(Direct_Declarator)
    {};

    virtual ~Function_Call()
    {
        delete Direct_Declarator;
    }

    virtual void print(std::ostream &dst) const override
    {

    }

    const std::string getId() const override {
        return Direct_Declarator->getId();
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        dst << "\tcall    " << getId() << std::endl;
        dst << "\tmv      " << destReg << ", a0" << std::endl;
        dst << "\tsw      " << destReg <<", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4);
    }
};

class Function_Call_Args
    : public Node
{
private:
    NodePtr Direct_Declarator;
    Arglist arglist;

public:
    Function_Call_Args(const NodePtr _Direct_Declarator, const Arglist _arglist)
        : Direct_Declarator(_Direct_Declarator), arglist(_arglist)
    {};

    virtual ~Function_Call_Args()
    {
        delete Direct_Declarator;
    }

    virtual void print(std::ostream &dst) const override
    {}

    const std::string getId() const override {
        return Direct_Declarator->getId();
    }

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        for(int i = 0; i<arglist.size(); i++){
            arglist[i]->compile(dst, "x7", context);
            dst << "\tmv      " << "a" << i << ", " << "x7" << std::endl;
        }

        dst << "\tcall    " << getId() << std::endl;
        dst << "\tmv      " << destReg << ", a0" << std::endl;
        dst << "\tsw      " << destReg <<", " << context.pointerOffset << "(s0)" << std::endl;
        context.changeOffset(-4);
    }
};

class Translation_Unit
    : public Node
{
private:
    NodePtr translation_unit;
    NodePtr external_declaration;

public:
    Translation_Unit(const NodePtr _translation_unit, const NodePtr _external_declaration)
        : translation_unit(_translation_unit), external_declaration(_external_declaration)
    {};

    virtual ~Translation_Unit()
    {
        delete translation_unit;
        delete external_declaration;
    }


    virtual void print(std::ostream &dst) const override
    {

    }


    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        translation_unit->compile(dst, destReg, context);
        external_declaration->compile(dst, destReg, context);
    }
};

#endif
