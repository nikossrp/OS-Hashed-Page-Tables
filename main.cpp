#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "Simulator.h"


#define FRAME_SIZE 4096

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 7) {
        cout << "Failure: usage: -a <algorithm (lru or sc) > -f <frames> -q <quantum> -m <max references>" << endl;
        exit(EXIT_FAILURE);
    }

    string replacementAlg = "";
    int frames = 0;
    int quantum = 0;
    int maxReferences = -1;
    
    for (int i = 0; i < argc; i++) {

        if (!strcmp(argv[i], "-a")) {
            replacementAlg = argv[++i];
            if (replacementAlg.compare("lru") != 0 && replacementAlg.compare("sc") != 0) {
                cout <<"Failure: usage:./main -a <algorithm: (lru or sc) > -f <frames> -q <quantum> -m <max references>" << endl;
                cout << "You must use LRU (lru) or Second Chance (sc) algorithm\n";
                exit(EXIT_FAILURE);
            } 
        }

        if (!strcmp(argv[i], "-f")) {
            frames = atoi(argv[++i]);
        }

        if (!strcmp(argv[i], "-q")) {
            quantum = atoi(argv[++i]);
        }

        if(!strcmp(argv[i], "-m")) {
            if (argc == 8) {
                cout << "Failure: usage:./main -a <algorithm: (lru or sc) > -f <frames> -q <quantum> -m <max references>\n";
                exit(EXIT_FAILURE);
            }
            maxReferences = atoi(argv[++i]);
        }
    }

    if (frames <= 0) {
        printf("Not enough frames\n");
        exit(EXIT_FAILURE);
    }


    Simulator sim(replacementAlg, frames, FRAME_SIZE, quantum, maxReferences);
    sim.run();
}