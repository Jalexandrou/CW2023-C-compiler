#ifndef ast_types_hpp
#define ast_types_hpp

#include <string>
#include <iostream>

class Type
    : public Expression
{
public:
    virtual const char *getType() const=0;

    virtual void print(std::ostream &dst) const override
    {
        dst<<getType();
    }
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