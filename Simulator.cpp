#include <iostream>
#include <string.h>
#include <fstream>
// #include <cstdlib>
// #include <cstring>
// #include <unistd.h>

#include "Simulator.h" 

using namespace std;


int mySqrt(int num) {

    float temp, sqrt;
    int number;
    number = num;
    sqrt = number / 2;
    temp = 0;

    while(sqrt != temp) {
        temp = sqrt;
        sqrt = ( number/temp + temp) / 2;
    }
   return (sqrt);
}


Simulator::Simulator(string replacementAlg, int frames, int frameSize, int quantum, int maxReferences): processId(1)
{
    this->replacementAlg = replacementAlg;
    this->frames = frames;
    this->frameSize = frameSize;
    this->quantum = quantum;
    this->maxReferences = maxReferences;
    
    int buckets = mySqrt(frames);
    buckets = (buckets < 1 ? 1 : buckets);

    hpt_p1 = new HPT_P1(buckets);
    hpt_p2 = new HPT_P2(buckets);

    this->init_Stats();

    if (!(this->replacementAlg.compare("lru"))) {
        lru = new LRU();
    }

    if (!(replacementAlg.compare("sc"))) 
        sca = new SecondChance();
}


Simulator::~Simulator()
{
    if (!replacementAlg.compare("lru"))
        delete lru;
    
    if (!replacementAlg.compare("sc"))
        delete sca;

    delete hpt_p1;
    delete hpt_p2;
}



/* Print page fault and disk write */
void printMemory(Address* address, Address* victim, int pf)
{
    if (!pf) {  
        cout <<  "Page " << address->getPageNumber() << " already in Memory\n\n";
    } 
    else if (pf && victim == NULL) {   
        cout << "Page fault on requesting page " << address->getPageNumber() << " from process " << address->getProcessId() << "\n\n";
    } 
    else if(pf && victim != NULL) { 
        cout << "Memory full, replacing page " << victim->getPageNumber() << " from process " << victim->getProcessId() <<" "<< ((victim->getDirty()) ? ". Disk WRITE replacement page" : " ") << "\n\n";
    }
}


void printRequest(Address* address) 
{
    cout << "Process "  << address->getProcessId() << " requested page " << address->getPageNumber() << " for " << ((address->getDirty()) ? "WRITE" : "READ") << endl;
}



void Simulator::run()
{
    ifstream trace1;
    ifstream trace2;
    Address* address;

    trace1.open("traces/bzip.trace", ios::in);
    trace2.open("traces/gcc.trace", ios::in);

    cout << endl;
    
    /* Starting with processId = 1, and trace1 */
    while((address = this->getTrace(trace1, trace2))) {
        statistics.pageRequests++;

        printRequest(address);

        if(!replacementAlg.compare("lru")) {
            this->runLRU(*address);
        } 
        else if(!replacementAlg.compare("sc")) {
            this->runSC(*address);
        }
        
        delete address;
    }

    trace1.close();
    trace2.close();

    this->printStatistics();
}




Address* Simulator::getTrace(ifstream& trace1, ifstream& trace2)
{
    static int currQ = quantum;
    static int referenceNumber = 0;
    static istream* trace = &trace1;
    string line;

    if (referenceNumber == maxReferences || getline(*trace, line).eof() || currQ == 0) {
        return NULL;
    }

    char* c_line = new char[line.length()+1];

    strcpy(c_line, line.c_str());

    // cout << "TRACE: "  << line << endl;

    char* hexAddress = strtok(c_line, " "); 
    char* write = strtok(NULL, " \n\0");

    string Address_str (hexAddress, strlen(hexAddress));

    /*log_2(4096) = 12, -> 12/4 = 3 (digitals) offset*/
    string hexAddress_str = Address_str.substr(0, Address_str.length() - 3);
    string hexOffset  = Address_str.substr(Address_str.length() - 3);

    long decAddress = strtoul(hexAddress_str.c_str(), NULL, 16);
    //long decOffset = strtoul(hexOffset.c_str(), NULL, 16);

    int pageNumber = decAddress; 


    bool dirty = false;     //for 'R'ead
    
    if(*write == 'W') {
        dirty = true;
    }

    Address* address = new Address(processId, pageNumber, dirty);

    delete [] c_line;

    /*swap trace and process*/
    if(!--currQ) {
        currQ = quantum;
        this->swapProcess();
        this->swapTrace(&trace, trace1, trace2);
    }
    
    referenceNumber++;

    return address;
}



void Simulator::swapProcess()
{
    if (processId == 1) {
        processId = 2;
    }
    else if(processId == 2) {
        processId = 1;
    }
}


void Simulator::swapTrace(istream** currtrace, ifstream& trace1, ifstream& trace2)
{
    if (*currtrace == &trace1) {
        *currtrace = &trace2;

    } else {

        *currtrace = &trace1;
    }
}


/*
* Check if page is already in memory (hash page table)
*       -Upgrade the priority
*       -check for dirty bit 
*
* otherwise upgrade the page fault counter
*
* Check if memory is full 
*       - take the victim 
*       - remove victim from page table (and memory)            
*       - add the new page to hash page table (replace page in frame)
* otherwise add the new page to hash page table
*
*/
void Simulator::runLRU(Address& address)
{
    HashPageTable* hpt;
    
    if (address.getProcessId() == 1) {
        hpt = hpt_p1;
    } 
    else if(address.getProcessId() == 2) {
        hpt = hpt_p2;
    }

    //An uparxei sto pt alla den einai se morfh write
    //Kanto se morfh write
    if (hpt->checkPageInHPT(address)) {

        printMemory(&address, NULL, 0);
        lru->Insert(address);

        if (address.getDirty())
            this->checkDirty(address);    
    }
    else {

        statistics.pageFaults++;    
        statistics.diskReads++;     

        int PagesInMemory = hpt_p1->getNumberOfPages() + hpt_p2->getNumberOfPages();

        if(frames <= PagesInMemory) {       //Memory is full

            Address victim = lru->getVictim();  

            if(victim.getProcessId() == 1) {
                hpt_p1->Remove(victim);
            }
            else {
                hpt_p2->Remove(victim);
            }
           
            hpt->Insert(address);
            lru->Insert(address); 
            printMemory(&address, &victim, 1);    

            if (victim.getDirty()) {
                statistics.diskWrites++;
            } 
        } 
        else {
            lru->Insert (address);
            hpt->Insert(address);
            printMemory(&address, NULL, 1);
        }
    }
}




void Simulator::runSC(Address& address)
{
    HashPageTable* hpt;

    if (address.getProcessId() == 1) {
        hpt = hpt_p1;
    } 
    else if(address.getProcessId() == 2) {
        hpt = hpt_p2;
    }


    if(hpt->checkPageInHPT(address)) {

        sca->Insert(address);

        if (address.getDirty())
            this->checkDirty(address);

        printMemory(&address, NULL, 0);
  
    }
    else {  /*Page not in memory*/
        statistics.pageFaults++;
        statistics.diskReads++;

        int PagesInMemory = hpt_p1->getNumberOfPages() + hpt_p2->getNumberOfPages();

        if(frames <= PagesInMemory) {  //Memory is full

            Address victim = sca->getVictim();
            printMemory(&address, &victim, 1);

            if(victim.getProcessId() == 1) {
                hpt_p1->Remove(victim);
            }
            else {
                hpt_p2->Remove(victim);
            }

            hpt->Insert(address);
            sca->Insert(address); 

            if (victim.getDirty()) {
                statistics.diskWrites++;
            } 
        } 
        else {
            sca->Insert (address);
            hpt->Insert(address);
            printMemory(&address, NULL, 1);
        }
    }
}


void Simulator::checkDirty(Address& address)
{
    Address* page;

    /*check address in hash page table*/
    if (address.getProcessId() == 1) {
        page = hpt_p1->getPage(address);
    } 
    else if(address.getProcessId() == 2) {
        page = hpt_p2->getPage(address);
    }

    if(address.getDirty() == true && page->getDirty() == false) {
        page->setDirty(true);
        cout << "Page " << page->getPageNumber() << " change to dirty from process " << address.getProcessId() << endl;
    }
}



void Simulator::init_Stats() 
{
    statistics.diskReads = 0;
    statistics.diskWrites = 0;
    statistics.pageFaults = 0;
    statistics.pageRequests = 0;
    statistics.frames = frames;
}



void Simulator::printStatistics()
{
    hpt_p1->print();
    hpt_p2->print();

    if (!replacementAlg.compare("lru"))
        lru->print();
    else
        sca->print();

    cout << "\n\n********** Statistics **********\n\n";
    cout << "Frames: " << statistics.frames << endl;
    cout << "Page Requests: " << statistics.pageRequests << endl;
    cout << "Page Faults: " << statistics.pageFaults << endl;
    cout << "Disk Writes: " << statistics.diskWrites << endl;
    cout << "Disk Reads: " << statistics.diskReads << "\n\n";
}






