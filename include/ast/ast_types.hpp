#ifndef ast_types_hpp
#define ast_types_hpp

#include <string>
#include <iostream>

class Int
    : public Expression
{
public:
    std::string getValue() const
    { return "int"; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<getValue();
    }
};

#endif