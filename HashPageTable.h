#ifndef HashPageTable_H_
#define HashPageTable_H_
#include "Node.h"
#include <list>
#include <vector>


class HashPageTable {

    protected:
        std::list <Node>* hpt;      //Node [pageNumber | frame]
        int hptSize;

    public:
        HashPageTable(const int buckets);
        ~HashPageTable();
        void Insert(Address& address);
        void Remove(Address& address);      //for Victimize page 
        unsigned int hash(unsigned int pageNumber);
        bool checkPageInHPT(Address& address);
        Address* getPage(Address& address);
        int getNumberOfPages(); 
        int getFrame(Address& address);  
        virtual int getProcessId() const {return -1;};    
        void print(); 
};



/*Each process has it's own hash page table*/
class HPT_P1: public HashPageTable {

    private:
        int processId;

    public:
        HPT_P1(int buckets): HashPageTable(buckets), processId(1) { }
        int getProcessId() const { return processId; }
        ~HPT_P1() { };
};


class HPT_P2: public  HashPageTable {

    private:
        int processId;
    
    public:
        HPT_P2(int buckets): HashPageTable(buckets), processId(2) { }
        int getProcessId() const { return processId; }
        ~HPT_P2() { }
};


#endif //HashPageTable_H_
