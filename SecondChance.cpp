#include <iostream>
#include <algorithm>        
#include "SecondChance.h"   

using namespace std;

NodeSC::NodeSC(Address address): bit(0)
{
    this->address = address;
}

NodeSC::NodeSC(const NodeSC& ob)
{
    address = ob.address;
    bit = ob.bit;
}



bool NodeSC::operator==(const NodeSC& node)  
{
    return address == node.address && bit == node.bit;

}




ostream &operator<<(ostream& left, NodeSC& ob)
{ 
    left << ob.getPageNumber() << "|" << ob.getBit() << " "; 
    return left; 
}


NodeSC& NodeSC::operator = (const NodeSC& node)
{    
    if (this == &node) return *this;
    address = node.address;
    bit = node.bit;
    return *this;
}



/*
*  if the page is in the queue
*       Find the node and give a second chance (Set bit to 1) 
* otherwise insert the node at the end of queue 
*        (at the end of queue are the most recently called elements) 
*/
int SecondChance::Insert(Address& address)
{
    NodeSC node(address);
    list<NodeSC>::iterator it;

    for (it = rlist.begin(); it != rlist.end(); it++) {

        if ((*it).getAddress() == address) {
            (*it).setBit_one();    
            return 0;
        }
    }

    rlist.push_back(node);          
    return 1;
}


/*
*   when you find a node with second chance,
*        change reference bit to 0.
*        insert node at the back to queue
*   continue in circular queue until find a node with reference bit 0
*/
Address SecondChance::getVictim()
{
    list<NodeSC>::iterator it1;
    Address victim;
    NodeSC node;

    for(it1 = rlist.begin();/* (*it1).getBit() != 0 */; it1++) {

        if(it1 == rlist.end())
            it1 = rlist.begin();

        if((*it1).getBit() == 1) {   //This page has a second chance 
            node = (*it1);   
            rlist.erase(it1--);      //rlist.remove(*it) is invalid
            node.setBit_zero();
            rlist.push_back(node);
            continue;     
        }

        if ((*it1).getBit() == 0) {  //get the first victim from list
            victim = (*it1).getAddress();
            rlist.remove(victim);
            return victim;
        }
    }
}




void SecondChance::print() 
{
    cout << "~~~~ Most recsent calls ~~~~\n" << endl;
    list<NodeSC>::iterator it;
    for(it = rlist.begin(); it != rlist.end(); it++) {
        cout << (*it) << " ";
    }
    cout << "\n\n";
}