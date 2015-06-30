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
#include "FIFO.h"
#include "LCFS.h"
#include "SJF.h"
#include "Prio.h"
#include <iomanip>
#include <stdio.h>

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
    int quantum = INT_MAX;
    char c = opArg[0];
    AbstractScheduler* ass;
    switch(c){
        case 'R':
            opArg = opArg + 1;
            quantum = strtol(opArg,&pEnd,10);
            ass = new RRScheduler(quantum);
            break;
        case 'F':
            ass = new FIFO(quantum);
            break;
        case 'L' :
            ass = new LCFS(quantum);
            break;
        case 'S' :
            ass = new SJF(quantum);
            break;
        case 'P' :
            opArg = opArg + 1;
            quantum = strtol(opArg,&pEnd,10);
            ass = new Prio(quantum);
            break;

    }
    return ass;
}


int main(int argc, char* argv[]){

    char* FILE_NAME;
    char* R_FILE;
    int at,tct,mcb,mib;
    int nextCPUFreeTime;
    int cpuBurst;
    int ioInProgress = 0;
    double totalIOTime_noOverlap = 0;
    int iobStart;


    map<int,Process*> procObjMap;
    priority_queue<Event*,vector<Event*>,myComparison> eventQ;

    bool vflag = false;
    char *sValue = NULL;
    int c;
    AbstractScheduler* ass = NULL;

    double CpuUtilization;
    double IoUtilization;
    double AvgTurnAround;
    double AvgCpuWait;
    double throughPut;


    opterr = 0;
    char *pEnd;
    while((c = getopt(argc, argv, "vs:")) != -1)
    {
        switch(c)
        {
            case 'v':
                vflag = true;
                break;

            case 's':

                //sflag = 1;
                sValue = optarg;
                ass = generateQuantum(optarg);
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
        //cout << val  << endl;
        Process* newProc = new Process(at,tct,mcb,mib,val);
        procObjMap.insert(pair<int,Process*>(newProc->get_pid(),newProc));
        Event* newEvent = new Event(newProc->get_pid(),at,Process::CREATED,Process::READY);
        eventQ.push(newEvent);
    }

    int currentTime = 0;
    nextCPUFreeTime = currentTime;
    Process::States finalState;
    int timeInPrevState;
    while(eventQ.size() > 0){

        Event* curEvent =  eventQ.top();
        currentTime = curEvent->get_Tstamp();

        while(eventQ.size() > 0 && (eventQ.top())->get_Tstamp()== currentTime){
            curEvent = eventQ.top();
            eventQ.pop();
            Process* curProc = procObjMap[curEvent->get_pid()];
            curProc->setLastEventId(curEvent->get_eid());
            timeInPrevState = currentTime - curProc->get_lastTransitionTime();

            if (curEvent->get_prevState() == Process::CREATED && curEvent->get_newState() == Process::READY){
                ass->addProcess(curProc);           
                if (vflag){
                    cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "CREATED" << " " << "->" << " "
                        << "READY" << endl;}
            }

            else if (curEvent->get_prevState() == Process::READY && curEvent->get_newState() == Process::RUNNING){

                curProc->set_state(Process::RUNNING);
                curProc->set_totalCpuWaitTime(currentTime,curProc->get_lastTransitionTime());
                curProc->decrement_dp();

                int allowedTime = curProc->get_remainingCPUBurst();
                finalState = Process::BLOCKED;

                if (curProc->get_remainingCPUBurst() == 0){
                    int cpBurst = rand.myrandom(curProc->get_mcb());
                    curProc->set_remainingCPUBurst(cpBurst);
                }
                if(curProc->get_remainingCPUBurst() >= curProc->get_remainingCPUTime()){
                    curProc->set_remainingCPUBurst(curProc->get_remainingCPUTime());
                    finalState = Process::DONE;
                }
                if(vflag){
                    cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "READY" << " " << "->" << " "
                        << "RUNNG" << " cb=" << curProc->get_remainingCPUBurst() << " rem=" << curProc->get_remainingCPUTime() << " prio=" << curProc->get_dp()+1 <<  endl;
                }
                allowedTime = curProc->get_remainingCPUBurst();
                if(allowedTime > ass->get_quantum()){
                    allowedTime = ass->get_quantum();
                    finalState = Process::READY;
                }
                curProc->reduceRemCPUBurst(allowedTime);
                nextCPUFreeTime = currentTime + allowedTime;

                eventQ.push(new Event(curProc->get_pid(),(currentTime + allowedTime),Process::RUNNING,finalState));

            }


            else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::BLOCKED){

                curProc->set_state(Process::BLOCKED);
                curProc->reset_dp();
                ioInProgress++;

                int calculatedIb = rand.myrandom(curProc->get_mib());
                finalState = Process::READY;
                curProc->set_totalIOTime(calculatedIb);
                if(ioInProgress == 1){
                    iobStart = currentTime;
                }
                eventQ.push(new Event(curProc->get_pid(),(currentTime + calculatedIb),curEvent->get_newState(),finalState));

                if (vflag){
                cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "RUNNG" << " " << "->" << " "
                    << "BLOCK" << "  ib=" << calculatedIb << " rem=" << curProc->get_remainingCPUTime() << endl;}

                //calculate ib value
                //add an event for Blocked-Ready, Tstamp = currentTime + calculated Ib
                //
            }

            else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::READY)
            {
               if (vflag){
               cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "RUNNG" << " " << "->" << " "
                    << "READY" << "  cb=" << curProc->get_remainingCPUBurst() << " rem=" << curProc->get_remainingCPUTime() 
                    << " prio=" << curProc->get_dp()+1 << endl;}

                ass->addProcess(curProc);
                curProc->set_state(Process::READY);

                //just print remainingCPUBurst (Cbr)

                //remainingCPUTime
            }

            else if (curEvent->get_prevState() == Process::BLOCKED && curEvent->get_newState() == Process::READY) {

                ioInProgress--;
                if(ioInProgress == 0){
                    totalIOTime_noOverlap += currentTime - iobStart;
                }
                if(vflag){
                cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "BLOCK" << " " << "->" << " "
                    << "READY" << endl;}
                ass->addProcess(curProc);
                curProc->set_state(Process::READY);
                //just push it to ready queue

            }

            else if(curEvent->get_prevState() == Process::RUNNING && curEvent->get_newState() == Process::DONE){
                //print done
                if (vflag){
                cout << currentTime << " " << curProc->get_pid() << " " <<  timeInPrevState <<": "<< "Done" << endl;}
                curProc->set_state(Process::DONE);
            }
            curProc->set_lastTransitionTime(currentTime);
        }

        if(currentTime >= nextCPUFreeTime) {
            Process* newProcess = ass->getNewProcess();
            if (newProcess != NULL) {
                eventQ.push(new Event(newProcess->get_pid(),currentTime,Process::READY,Process::RUNNING));
            }
        }

    }


    cout << ass->get_schedulerName() << endl;
    int FinishTime, TurnaroundTime, IOTime, CPU_waitTime;
    for(map<int,Process*>::iterator it = procObjMap.begin(); it!=procObjMap.end(); it++)
    {
        Process* prc = procObjMap[it->first];
        FinishTime = prc->get_lastTransitionTime();
        TurnaroundTime = FinishTime - prc->get_at();

        cout << setw(4) << setfill('0') << prc->get_pid() << ":" << setw(5) << setfill(' ') << prc->get_at() <<  setw(5) << setfill(' ') << prc->get_tct()<< setw(5) << setfill(' ') << prc->get_mcb() << setw(5) << setfill(' ') << prc->get_mib() << setw(2) << setfill(' ') << prc->get_sp() << " |" ;

        cout << setw(6) << setfill(' ') << FinishTime << setw(6) << setfill(' ') << TurnaroundTime << setw(6) << setfill(' ') << prc->get_totalIOTime() << setw(6) <<setfill(' ') << prc->get_totalCpuWaitTime() << endl;

        CpuUtilization = CpuUtilization + prc->get_tct();
        IoUtilization = IoUtilization + prc->get_totalIOTime();
        AvgTurnAround = AvgTurnAround + TurnaroundTime;
        AvgCpuWait = AvgCpuWait + prc->get_totalCpuWaitTime();
    }     

    double procMapSize = (double)procObjMap.size();
    CpuUtilization = (CpuUtilization/currentTime)*100;
    IoUtilization = (totalIOTime_noOverlap/currentTime)*100;
    AvgTurnAround = AvgTurnAround/procMapSize;
    AvgCpuWait = AvgCpuWait/procMapSize;
    throughPut = (procMapSize/currentTime)*100;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
            currentTime,
            CpuUtilization,
            IoUtilization,
            AvgTurnAround,
            AvgCpuWait,         
            throughPut);

    //cout << "SUM:" << " " << currentTime << " " << CpuUtilization << " " << IoUtilization << " " << AvgTurnAround << " " << AvgCpuWait << endl;
}


