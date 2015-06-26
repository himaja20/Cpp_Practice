#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <functional>
#include "process.h"
#include "event.h"

using namespace std;

class myComparison{



}


int main(int argc, char* argv[]){

    char* FILE_NAME;
    int at,tct,mcb,mib;
    vector<Process> procObjList;
    priority_queue<Event> eventQ;
    FILE_NAME = argv[1];
    ifstream fin(FILE_NAME, ios::in | ios::binary);
    if ((argc <= 1) || (FILE_NAME == "")) {
        cout << "Expected argument after options";
        return 0;
    }

    if (!fin.is_open())
    {
        cout << "File open failed" << endl ;
        exit(0);
    }

    while(fin >> at >> tct >> mcb >> mib){
        Process newProc(at,tct,mcb,mib);
        procObjList.push_back(newProc);
        Event newEvent(newProc.get_pid(),at,Process::CREATED,Process::READY);
        eventQ.push(newEvent);

    }

    for(vector<Process>::iterator it = procObjList.begin(); it!=procObjList.end(); it++)
    {
        //cout << procObjMap[it->second.get_pid()] << endl;
        cout << "iterating " << *it << endl;
    }

}
