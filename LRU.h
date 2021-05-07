#ifndef LRU_H_
#define LRU_H_

#include <list>
#include "Address.h"

class LRU {

    private:
        std::list<Address> stack;
    
    public:
        LRU() {}
        ~LRU() {}
        void Insert(Address& address);  //Insert / Prioritize
        Address getVictim();
        void print();
};



#endif  //LRU_H_