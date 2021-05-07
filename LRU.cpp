#include "LRU.h"
#include <iostream>
#include <algorithm>

using namespace std;

/*
* If element exist in list
* upgrading the priority to the highest,
* Otherwise insert element with highest priority
*/
void LRU::Insert(Address& address)
{
    list <Address>::iterator it;
    it = find(stack.begin(), stack.end(), address);

    if(it != stack.end()) {     //element found
        stack.remove(address);  //remove and upgrade priority
        stack.push_front(address);
    } 
    else {
        stack.push_front(address);
    }
}


Address LRU::getVictim()
{
    Address victim = stack.back();
    stack.pop_back();
    return victim;      
}


void LRU::print() 
{
    cout << "~~~~ Most recent calls ~~~~\n\n";
    for(list <Address>::iterator it = stack.begin(); it != stack.end(); it++) {
        cout << (*it).getPageNumber() << " ";
    }
    // cout << "X";
    cout << "\n\n";
}