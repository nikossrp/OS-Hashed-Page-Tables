#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(Address& address, int pageFrame) 
{
    this->address = address;
    this->frame = pageFrame;
}

Node::Node(const Node& ob)
{
    address = ob.address;
    frame = ob.frame;
}


bool Node::operator==(const Node& node) const   //H list.remove(.) prepei na mporei na brei to Node 
{
    return address == node.address && frame == node.frame;

}

ostream &operator<<(ostream& left, Node& ob)
{ 
    left << ob.getPageNumber() << "|" << ob.getFrame() << " "; 
    return left; 
}