#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
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

        bool operator()(Event* e1,Event* e2){
            /*
             * first check timestamp
             * equal
             *  check eids
             *      return eid1 < eid2
             *  not equal
             *      reutrn ets1 < ets2
             *  
             */
            if (e1->get_Tstamp() == e2->get_Tstamp()){
                return (e1->get_eid() > e2->get_eid());
            }
            return (e1->get_Tstamp() > e2->get_Tstamp());
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
    AbstractScheduler* as = new RRScheduler(quantum);
    return as;
}

int main(int argc, char* argv[]){

    char* FILE_NAME;
    char* R_FILE;
    int at,tct,mcb,mib;
    int nextCPUFreeTime;
    int cpuBurst;

    map<int,Process*> procObjMap;
    priority_queue<Event*,vector<Event*>,myComparison> eventQ;

    int vflag = 0;
    int sflag = 0;
    char *sValue = NULL;
    int quantum = INT_MAX;
    int c;
    AbstractScheduler* as = NULL;

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
                as = generateQuantum(optarg);
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
        Process* newProc = new Process(at,tct,mcb,mib,val);
        procObjMap.insert(pair<int,Process*>(newProc->get_pid(),newProc));
        Event* newEvent = new Event(newProc->get_pid(),at,Process::CREATED,Process::READY);
        eventQ.push(newEvent);
    }

    int currentTime = 0;
    while(!eventQ.empty()){

        Event* curEvent =  eventQ.top();
        eventQ.pop();
        currentTime = curEvent->get_Tstamp();
        Process* curProc = procObjMap[curEvent->get_pid()];

        if (curEvent->get_prevState() == Process::CREATED && curEvent->get_newState() == Process::READY){
            //push it to ready queue
        }

        else if (curEvent->get_prevState() == Process::READY && curEvent->get_newState() == Process::RUNNING){
            int allowedTime = curProc->get_remainingCPUBurst();
            curEvent->set_newState(Process::BLOCKED);

            if (curProc->get_remainingCPUBurst() == 0){
                int cpBurst = rand.myrandom(curProc->get_mcb());
                if(cpBurst > curProc->get_remainingCPUTime()){
                    cpBurst = curProc->get_remainingCPUTime();
                    curEvent->set_newState(Process::DONE);
                }
                curProc->set_remainingCPUBurst(cpBurst);
            }
            allowedTime = curProc->get_remainingCPUBurst();
            if(allowedTime > as->get_quantum()){
                allowedTime = as->get_quantum();
                curEvent->set_newState(Process::RUNNING);
            }
            curProc->reduceRemCPUBurst(allowedTime);
            
            eventQ.push(new Event(curProc->get_pid(),(currentTime + allowedTime),Process::RUNNING,curEvent->get_newState()));
        }

        else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::BLOCKED){

            //calculate ib value
            //TotalCPUTime - TotalTimeSpentInCpu(adding times of cpu Bursrts) = remainingCPUTime
            //
            //
        }

        else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::READY)
        {

            //just print remainingCPUBurst (Cbr)
            //TotalCPUTime - TotalTimeSpentInCpu(adding times of cpu Bursrts) = remainingCPUTime

        }

        else if (curEvent->get_prevState() == Process::BLOCKED && curEvent->get_newState() == Process::READY) {
            //just push it to ready queue

        }

        else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::DONE){
            //print done

        }

        curProc->set_lastTransitionTime(currentTime);
    }


    /*    for(vector<Process>::iterator it = procObjList.begin(); it!=procObjList.end(); it++)
          {
    //cout << procObjMap[it->second.get_pid()] << endl;
    cout << "iterating " << *it << endl;
    } */

}
