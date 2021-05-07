#ifndef Node_H_
#define Node_H_
#include "Address.h"
/*Node for Hash page table*/


class Node {
    private:
        Address address;
        int frame;      //physical

    public:
        Node(Address& address, int frame);
        Node(const Node& ob);
        ~Node() {}
        int getPageNumber() const { return address.getPageNumber(); }
        Address& getAddress() { return address; }
        bool getDirty() const { return address.getDirty(); }
        int getFrame() const { return frame; }
        // bool checkEquality(int pageNumber) const { return address.getPageNumber() == pageNumber; }
        friend std::ostream &operator<< (std::ostream& left, Node& ob);
        bool operator == (const Node& node) const;    //H list.remove(.) prepei na mporei na brei to Node 

};




#endif //Node_H_
