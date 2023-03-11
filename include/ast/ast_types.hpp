#ifndef ast_types_hpp
#define ast_types_hpp

#include <string>
#include <iostream>

class Type
    : public Node
{
public:
    virtual const char *getType() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<getType();
    }

    virtual void compile(std::ostream &dst, std::string destReg) const override {}
};

class Int
    : public Type
{
protected:
    virtual const char *getType() const override
    { return "int"; }
};


class Void
    : public Type
{
protected:
    virtual const char *getType() const override
    { return "void"; }
};


#endif