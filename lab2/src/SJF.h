
#include "AbstractScheduler.h"


class myComp{

    public:

        bool operator()(Process* p1,Process* p2){
            /*
             * first check timestamp
             * equal
             *  check eids
             *      return eid1 < eid2
             *  not equal
             *      reutrn ets1 < ets2
             *  
             */
            if (p1->get_remainingCPUTime() == p2->get_remainingCPUTime()){
                return (p1->get_pid() > p2->get_pid());
            }
            return (p1->get_remainingCPUTime() > p2->get_remainingCPUTime());

        }
};


class SJF: public AbstractScheduler {

    private:

        priority_queue<Process*,vector<Process*>,myComp> readyQ;

    public:

        SJF(int quantum) : AbstractScheduler(quantum) {}

        Process* getNewProcess(){
            if (readyQ.size() == 0) {
                return NULL;
            }
            Process* myProc =  readyQ.top(); 
            readyQ.pop();
            return myProc;
        }

        void addProcess(Process* proc){
            if(proc != NULL){
                readyQ.push(proc);
            }

        }

        string get_schedulerName(){
            return "SJF";
        }

};



