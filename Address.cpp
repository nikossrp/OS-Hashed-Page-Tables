#include "Address.h"
#include <iostream>


using namespace std;

Address::Address(int processId, int pageNumber, bool dirty)
{   
    this->pageNumber = pageNumber;
    this->dirty = dirty;
    this->processId = processId;
}


Address::Address(const Address& address)  
{
    pageNumber = address.getPageNumber();
    dirty = address.getDirty();
    processId = address.getProcessId();
}


bool Address::operator == (const Address& address) const
{
    return (pageNumber == address.pageNumber) && (processId == address.processId);
}


ostream &operator << (ostream& left, const Address& ob)
{
    left << ob.getPageNumber() << " " << ob.getDirty();
    return left;
}


Address& Address::operator = (const Address& address)
{
    if (this == &address) return *this;
    pageNumber = address.pageNumber;
    processId = address.processId;
    dirty = address.dirty;
    return *this;
}
