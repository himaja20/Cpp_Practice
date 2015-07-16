#ifndef MMU_H
#define MMU_H

#include "pte.h"

using namespace std;

class mmu {

private:
    vector<pte> pageTable;
    vector<unsigned int> frameTable;
    vector<unsigned int> frameToPageMapping;
    int numFrames;
    char* algVal;
    char* opVals;

public:
    mmu(int numOfFrames, char* algVal, char* opVals){

    pageTable = vector<pte>(64);
    numFrames = numOfFrames; 
    frameTable = vector<unsigned int>(numFrames);
    frameToPageMapping = vector<unsigned int> (numFrames);
    this->algVal = algVal;
    this->opVals = opVals;

    }

    void handleInstruction(int op, int pIndex){
    cout << "mmu class" << endl;
    cout << op << " operation" << "  " << pIndex << "  page Index " << endl; 

    }
};
#endif
