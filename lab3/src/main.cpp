
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include "mmu.h"
//#include "pte.h"

using namespace std;

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
    
//    stringstream ss(line);

    int c = 0;
    while((c = getopt(argc, argv, "a:o:f:")) != -1)
    {
        switch(c)
        {
            case 'a':
                aValue = optarg;
                cout << aValue << endl;
                break;

            case 'f':
                fValue = optarg;
                num_of_frames = strtol(fValue,&pEnd,10);
                cout <<  num_of_frames << endl;
                break;

            case 'o':
                oValue = optarg;
                cout << oValue << endl;
                break;

        }
    }
    FILE_NAME = argv[optind];
    R_FILE = argv[optind + 1];

    ifstream fin(FILE_NAME, ios::in | ios::binary);
    //  RandomUtil rand(R_FILE);

    if ((R_FILE == "") || (FILE_NAME == "")) {
        cout << "Input file paths empty";
        return 0;
    }

    if (!fin.is_open())
    {
        cout << "File open failed" << endl ;
        exit(0);
    }

    mmu* mmuObj = new mmu(num_of_frames,aValue,oValue);

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
                
//                cout << " op " << op << "  vepageindex  " << vpageindex << endl;
        }
        }
    }





}

}