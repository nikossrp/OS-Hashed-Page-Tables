#ifndef SecondChance_H_
#define SecondChance_H_


#include <list>
#include "Address.h"

class NodeSC {
    private:
        Address address;
        int bit;    

    public:
        NodeSC(Address address);
        NodeSC(const NodeSC& ob);
        NodeSC() {}; 
        ~NodeSC() {}
        int getPageNumber() const { return address.getPageNumber(); }
        void setBit_one() { bit = 1; }
        void setBit_zero() { bit = 0; }
        Address& getAddress() { return address; }
        int getBit() const { return bit; }
        friend std::ostream &operator<<(std::ostream& left, const NodeSC& ob);
        NodeSC& operator = (const NodeSC& node);
        bool operator == (const NodeSC& node);     
};



class SecondChance{
    private:
        std::list <NodeSC> rlist;       
        // std::list <NodeSC> nslist;

    public:
        SecondChance() {}
        ~SecondChance() {}
        Address getVictim();  
        int Insert(Address& address);
        void print();
};



#endif //SecondChance_H_