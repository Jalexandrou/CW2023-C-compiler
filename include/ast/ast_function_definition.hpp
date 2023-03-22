#ifndef ast_function_definition_hpp
#define ast_function_definition_hpp

typedef const Identifier *IdenPtr;
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

        dst << Declarator->getId() << ':' << std::endl;


        std::string Function_End_Label = makeName("Function_End");
        std::stringstream temp;

        context.Function_End_Label = Function_End_Label;

        Compound_Statement->compile(temp, destReg, context);

        //Set stack size, should be equal to max PointerOffset + StackBuffer
        dst << "\taddi    sp,  sp, -32\n";
        // dst << "\tsw      ra,  28(sp)\n";
        dst << "\tsw      s0,  24(sp)\n";
        dst << "\taddi    s0,  sp, 32\n";

        dst << temp.str();


        dst << Function_End_Label << ":" << std::endl;

        // dst << "\tlw      ra,  28(sp)\n";
        dst << "\tlw      s0,  24(sp)\n";
        dst << "\taddi    sp,  sp, 32\n";
        dst << "\tjr      ra\n";

    }
};


class Function_Definition_Args
    : public Node
{
private:
    NodePtr Type;
    NodePtr Declarator;
    NodePtr Compound_Statement;
    NodePtr Args;

public:
    Function_Definition_Args(NodePtr _Type, NodePtr _Declarator, NodePtr _Compound_Statement, NodePtr _Args)
        : Type(_Type), Declarator(_Declarator), Compound_Statement(_Compound_Statement), Args(_Args)
    {};

    virtual ~Function_Definition_Args()
    {
        delete Type;
        delete Declarator;
        delete Compound_Statement;
        delete Args;
    }

    NodePtr getType() const
    { return Type; }

    NodePtr getDeclarator() const
    { return Declarator; }

    NodePtr getCompound_Statement() const
    { return Compound_Statement; }

    NodePtr getArgs() const
    { return Args; }

    void print(std::ostream &dst) const override
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

    void compile(std::ostream &dst, std::string destReg, Context &context) const {

        dst << ".globl " << Declarator->getId() << std::endl; //Add context checking temp

        dst << Declarator->getId() << ':' << std::endl;


        std::string Function_End_Label = makeName("Function_End");

        std::stringstream temp;

        //Set the function end label within the current context to the end of the current function
        context.Function_End_Label = Function_End_Label;


        //Compile the code into the temp string stream
        Compound_Statement->compile(temp, destReg, context);

        //Set stack size, should be equal to max PointerOffset + StackBuffer
        dst << "\taddi    sp,  sp, -32\n";
        // dst << "\tsw      ra,  28(sp)\n";
        dst << "\tsw      s0,  24(sp)\n";
        dst << "\taddi    s0,  sp, 32\n";

        //compile the args into the normal dst
        Args->compile(dst, destReg, context);

        //print the string stream into the main output
        dst << temp.str();

        //Add an end label to jump to in case of return
        dst << Function_End_Label << ":" << std::endl;

        // dst << "\tlw      ra,  28(sp)\n";
        dst << "\tlw      s0,  24(sp)\n";
        dst << "\taddi    sp,  sp, 32\n";
        dst << "\tjr      ra\n";

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

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const {
        Declaration_list->compile(dst, destReg, context);
        Statement_list->compile(dst, destReg, context);
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
#endif
