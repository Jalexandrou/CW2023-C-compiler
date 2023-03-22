#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <memory>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}
class Context{
    public:
        int pointerOffset;
        int stackSize;
        std::string Function_End_Label;
        std::vector<std::map<std::string,double>> bindings_list;


        Context(){
            //initalise each value
            pointerOffset = -8;
            stackSize = 0;
            Function_End_Label = "";
            //Add an empty map to the vector
            std::map<std::string, double> current_map;
            bindings_list.push_back(current_map);
        }

        void changeOffset(int change){
            pointerOffset += change;
            if(pointerOffset < stackSize){
                stackSize = pointerOffset;
            }
        }

        int getStackSize(){
            int temp = stackSize;
            stackSize = 0;
            return temp;
        }
};

class Node;

typedef const Node *NodePtr;

class Node
{
public:
    virtual ~Node()
    {}

    //! Tell a node to print itself to the given stream
    virtual void print(std::ostream &dst) const = 0;

    virtual void compile(std::ostream &dst, std::string destReg, Context &context) const = 0;

    virtual const std::string getId() const { throw std::runtime_error("Not implemented getId() here"); }

    //! Evaluate the tree using the given mapping of variables to numbers
    // Don't need but will keep for now
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const
    { throw std::runtime_error("Not implemented."); }
};


#endif
