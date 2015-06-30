
#include "AbstractScheduler.h"


class myComp{

    public:

        bool operator()(Process* p1,Process* p2){
            if (p1->get_remainingCPUTime() != p2->get_remainingCPUTime()){
                return (p1->get_remainingCPUTime() >p2->get_remainingCPUTime());
            }
            if (p1->get_lastTransitionTime() !=  p2->get_lastTransitionTime()){
                return (p1->get_lastTransitionTime() >  p2->get_lastTransitionTime());
            }
            return (p1->getLastEventId() > p2->getLastEventId());
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
                        proc->reset_dp();
                    }

                }

                string get_schedulerName(){
                    return "SJF";
                }

        };



