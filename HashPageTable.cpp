#include <iostream>
#include <algorithm>
#include "HashPageTable.h"

using namespace std;
int i = 0;



HashPageTable::HashPageTable(int buckets)
{
    hptSize = buckets;
    hpt = new list <Node> [buckets];  
}




HashPageTable::~HashPageTable()
{
    delete [] hpt;
}



/*
* Find pageNumber
* Use hash function to find the right bucket
* Insert at the end of list
*/
void HashPageTable::Insert(Address& address)
{

    if(!(this->checkPageInHPT(address))) {

        // if(i == -1) while(1);
        Node node(address, i++);
        int pageNumber = address.getPageNumber();
        int hash_value = this->hash(pageNumber);
        hpt[hash_value].push_back(node);
    } 
    else 
        cout << "Page with page number: " 
            << address.getPageNumber() << " already exist\n";
}






/*Remove Node from Hash page table based on address*/
void HashPageTable::Remove(Address& address)
{
    int pageNumber = address.getPageNumber();
    int hash_value = this->hash(pageNumber);
    int frame = this->getFrame(address);
    Node node(address, frame);
    hpt[hash_value].remove(node);
    i = frame;  //etsi wste to i na kineite ston arithmo twn frames
    // if(i == -1) while(1);

}



/*Return a pointer to the page, if there is this page in HPT*/
Address* HashPageTable::getPage(Address& address)
{   
    // if(this->checkPageInHPT(address)){
        
    int pageNumber = address.getPageNumber();
    int hash_value = this->hash(pageNumber);
    int frame = this->getFrame(address);
    Node node(address, frame);
    list<Node>::iterator it;
    it = find(hpt[hash_value].begin(), hpt[hash_value].end(), node);
    if(it != hpt->end())
        return &((*it).getAddress());

    return NULL;
}




/*Get the Number of pages in hash page table*/
int HashPageTable::getNumberOfPages()
{
    int counter = 0;

    for (int i = 0; i < hptSize; i++){
        counter += hpt[i].size();
    }
    return counter;
}




/*Get Frame for physical address  ( rm: frame + offset = physical addresss)*/
int HashPageTable::getFrame(Address& address)
{
    int pageNumber = address.getPageNumber();
    int hash_value = this->hash(pageNumber);
    for( list<Node>::iterator it = hpt[hash_value].begin(); it != hpt[hash_value].end(); it++) 
        if((*it).getAddress() == address)
            return (*it).getFrame();

    return -1;      //Se periptwsh pou h selida den einai sto Hash Page Table
}




/*
    Check if the page is in a hash page table
    *only the page Number
*/
bool HashPageTable::checkPageInHPT(Address& address)
{   
    int pageNumber = address.getPageNumber();
    int hash_value = this->hash(pageNumber);
    for( list<Node>::iterator it = hpt[hash_value].begin(); it != hpt[hash_value].end(); it++)
        if((*it).getAddress() == address)   //operator == check only pageNumber & processId
            return true;
    
    return false;        
}



void HashPageTable::print()
{
    cout << endl; 
    cout << "#### HashPageTable "<< this->getProcessId() <<" ####\n" << endl;
    // cout <<"PageNumber | Frame\n" <<endl;

    for(int i = 0; i < hptSize; i++) {

        cout << i << ") ";

        if(!hpt[i].empty()){

            for(list<Node>::iterator it = hpt[i].begin(); it != hpt[i].end(); it++)
                cout << *it << "-> " ;     
            cout << "X";  
        }
        else cout << "NULL";

        cout << endl;
    }

    cout << "\n\n";
}




/*Hash function*/
/*source: https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key*/
unsigned int HashPageTable::hash(unsigned int PageNumber)
{
    PageNumber = ((PageNumber >> 16) ^ PageNumber) * 0x45d9f3b;
    PageNumber = ((PageNumber >> 16) ^ PageNumber) * 0x45d9f3b;
    PageNumber = (PageNumber >> 16) ^ PageNumber;

    return PageNumber % hptSize;
}