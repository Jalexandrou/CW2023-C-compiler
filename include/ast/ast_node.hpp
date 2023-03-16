#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>
#include <sstream>

#include <memory>

class Node;

typedef const Node *NodePtr;

class Node
{
public:
    virtual ~Node()
    {}

    //! Tell a node to print itself to the given stream
    virtual void print(std::ostream &dst) const = 0;

    virtual void compile(std::ostream &dst, std::string destReg) const = 0;

    virtual const std::string getId() const { throw std::runtime_error("Not implemented getId() here"); }

    //! Evaluate the tree using the given mapping of variables to numbers
    // Don't need but will keep for now
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const
    { throw std::runtime_error("Not implemented."); }
};


#endif
