#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
//#include "AbstractPageReplacement.h"
#include "FIFO.h"
#include "mmu.h"
#include "randomUtil.h"
#include "Random.h"
#include "SecondChance.h"
#include "NRU.h"
#include "Clock.h"
#include "ClockVirtual.h"
#include "LRU.h"
#include "Aging.h"
#include "AgingVirtual.h"
//#include "pte.h"

using namespace std;


AbstractPageReplacement *algObj;

void getAlgObj(char* opArg){

    char c;
    c = opArg[0];
    switch(c) 
    {
        case 'f':
            algObj = new FIFO();
            break;

        case 'r':
            algObj = new Random();
            break;
        
        case 's':
            algObj = new SecondChance();
            break;

        case 'N':
            algObj = new NRU();
            break;

        case 'c':
            algObj = new Clock();
            break;

        case 'X':
            algObj = new ClockVirtual();
            break;

        case 'l':
            algObj = new LRU();
            break;

        case 'Y':
            algObj = new AgingVirtual();
            break;

        case 'a':
            algObj = new Aging();
            break;
    }
}



int main(int argc, char* argv[]){
    char* FILE_NAME;
    char* R_FILE;
    char* aValue = NULL;
    char* fValue = NULL;
    char* oValue = NULL;
    int num_of_frames = 0;
    char* pEnd;
    string line;
    int op;
    int vpageindex;
    int vPageTableSize = 64;

    //    stringstream ss(line);

    int c = 0;
    while((c = getopt(argc, argv, "a:o:f:")) != -1)
    {
        switch(c)
        {
            case 'a':
                aValue = optarg;
                getAlgObj(aValue);
                break;

            case 'f':
                fValue = optarg;
                num_of_frames = strtol(fValue,&pEnd,10);
                break;

            case 'o':
                oValue = optarg;
                break;

        }
    }
    FILE_NAME = argv[optind];
    R_FILE = argv[optind + 1];

    if ((R_FILE == "") || (FILE_NAME == "")) {
        cout << "Input file paths empty";
        return 0;
    }

    ifstream fin(FILE_NAME, ios::in | ios::binary);
    if (!fin.is_open())
    {
        cout << "File open failed" << endl ;
        exit(0);
    }

    //RandomUtil rand(R_FILE);
    RandomUtil* rand = new RandomUtil(R_FILE);

    algObj->setRandomUtil(rand);

    if(num_of_frames == 0) 
    {
        num_of_frames = 32; 
    }
    mmu* mmuObj = new mmu(num_of_frames,oValue,algObj,vPageTableSize);
    mmuObj->setOptionFlags();

    if(!fin.eof()){
        while(getline(fin,line)){
            stringstream ss(line);
            if(line[0] != '#'){
                if (ss >> op >> vpageindex){
                    if (vpageindex > 63){
                        cout << "virtual page index more than 63" << endl;
                        exit(0);
                    }
                    mmuObj->handleInstruction(op,vpageindex);
                }
            }
        }
        mmuObj->printFinalInfo(); 
    }

}
