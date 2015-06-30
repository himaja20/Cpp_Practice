
#include "AbstractScheduler.h"


class FIFO: public AbstractScheduler {

    private:

        queue<Process*> readyQ;

    public:

        FIFO(int quantum) : AbstractScheduler(quantum) {}

        Process* getNewProcess(){
            if (readyQ.size() == 0) {
                return NULL;
            }
            Process* myProc =  readyQ.front(); 
            readyQ.pop();
            return myProc;
        }

        void addProcess(Process* proc){
            if(proc != NULL){
                readyQ.push(proc);
            }

        }

        string get_schedulerName(){
            return "FCFS";
            }


};


