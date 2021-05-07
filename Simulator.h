#ifndef Simulator_H_
#define Simulator_H_

#include <string>
#include <vector>
#include "HashPageTable.h"
#include "LRU.h"
#include "SecondChance.h"
#include "Statistics.h"


class Simulator {

    private:
        HPT_P1* hpt_p1;
        HPT_P2* hpt_p2;
        int frames;
        int frameSize;
        int quantum;
        int maxReferences;
        std::string replacementAlg;
        LRU* lru;
        SecondChance* sca;      
        int processId;
        Stats statistics;
        // int* Memory;

    public:
        Simulator(std::string replacementAlg, int frames, int frameSize, int quantum, int maxReferences);
        ~Simulator();
        void run();
        void runLRU(Address& address);
        void runSC(Address& adrress);
	    void checkDirty(Address& address);
        Address* getTrace(std::ifstream& trace1, std::ifstream& trace2);
        void swapProcess();
        void swapTrace(std::istream** currtrace, std::ifstream& trace1, std::ifstream& trace2);
        void init_Stats();
        void printStatistics();
        // void printRequest(Address* address);
        // void printMemory(Address* address, Address* victim, int pf); //print page faults, disk writes/read 
};



#endif