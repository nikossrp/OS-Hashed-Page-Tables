#ifndef Address_H_
#define Address_H_

#include <string>

class Address {

    private:
        int pageNumber;
        bool dirty;  
        int processId;   
    
    public:

        Address(int processId, int pageNumber, bool dirty);
        Address() {};
        Address(const Address& address);
        ~Address() {}
        bool getDirty() const { return dirty; }
        int getPageNumber() const { return pageNumber; }
        int getProcessId() const { return processId; }
        void setDirty(const bool dirty) { this->dirty = dirty; }
        bool operator == (const Address& address) const; 
        Address& operator = (const Address& address);
        friend std::ostream &operator<<(std::ostream& left, const Address& ob);
        void print();

};


#endif      //Address_h