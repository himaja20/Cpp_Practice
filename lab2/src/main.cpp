#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <functional>
#include "process.h"
#include "event.h"
#include "randomUtil.h"
#include <limits.h>
#include "AbstractScheduler.h"
#include "RRScheduler.h"

using namespace std;

class myComparison{

    public:

        bool operator()(Event e1,Event e2){
            /*
             * first check timestamp
             * equal
             *  check eids
             *      return eid1 < eid2
             *  not equal
             *      reutrn ets1 < ets2
             *  
             */
            if (e1.get_Tstamp() == e2.get_Tstamp()){
                return (e1.get_eid() > e2.get_eid());
            }
            return (e1.get_Tstamp() > e2.get_Tstamp());
        }
};

AbstractScheduler* generateQuantum(char* opArg){

    char* pEnd;
    int quantum;
    if (opArg[0] != 'R')
    { 
        cout << "invalid argument" << endl;
        exit(0);
    }
    opArg = opArg + 1;
    quantum = strtol(opArg,&pEnd,10);
    AbstractScheduler * as = new RRScheduler(quantum);
    return as;
}

int main(int argc, char* argv[]){

    char* FILE_NAME;
    char* R_FILE;
    int at,tct,mcb,mib;

    vector<Process> procObjList;
    priority_queue<Event,vector<Event>,myComparison> eventQ;

    int vflag = 0;
    int sflag = 0;
    char *sValue = NULL;
    int quantum = INT_MAX;
    int c,

        opterr = 0;
    char *pEnd;
    while((c = getopt(argc, argv, "vs:")) != -1)
    {
        switch(c)
        {
            case 'v':
                vflag = 1;
                break;

            case 's':

                sflag = 1;
                sValue = optarg;
                generateQuantum(optarg);
                break;

        }
    }
    FILE_NAME = argv[optind];
    R_FILE = argv[optind + 1];

    ifstream fin(FILE_NAME, ios::in | ios::binary);
    RandomUtil rand(R_FILE);

    if ((R_FILE == "") || (FILE_NAME == "")) {
        cout << "Input file paths empty";
        return 0;
    }

    if (!fin.is_open())
    {
        cout << "File open failed" << endl ;
        exit(0);
    }


    while(fin >> at >> tct >> mcb >> mib){

        int val =  rand.getPriority();
        cout << val  << endl;
        Process newProc(at,tct,mcb,mib,val);
        procObjList.push_back(newProc);
        Event newEvent(newProc.get_pid(),at,Process::CREATED,Process::READY);
        eventQ.push(newEvent);
    }

    while(!eventQ.empty()){
        //        cout << eventQ.top() << endl;

        event curEvent =  eventQ.pop();

        if (curEvent.prevState == CREATED && curEvent.newState == READY){
            //push it to ready queue
        }

        else if (curEvent.prevState == READY && curEvent.newState == RUNNING){

        }

        else if(curEvent.prevState == RUNNING && curEvent.newState == BLOCKED){

        }

        else if(curEvent.prevState == RUNNING && curEvent.newState == READY){

        }

        else if (curEvent.prevState == BLOCKED && curEvent.newState == READY) {

        }

        else if(curEvent.prevState == RUNNING && curEvent.newState == DONE){

        }

    }


    /*    for(vector<Process>::iterator it = procObjList.begin(); it!=procObjList.end(); it++)
          {
    //cout << procObjMap[it->second.get_pid()] << endl;
    cout << "iterating " << *it << endl;
    } */

}
