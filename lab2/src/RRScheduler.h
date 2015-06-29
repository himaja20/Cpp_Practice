
#include "AbstractScheduler.h"


class RRScheduler: public AbstractScheduler {

    private:

        queue<Process*> readyQ;

    public:

        RRScheduler(int quantum) : AbstractScheduler(quantum) {}

        Process* getNewProcess(){
            Process* myProc =  readyQ.front(); 
            readyQ.pop();
            return myProc;

        }

        void addProcess(Process* proc){
            readyQ.push(proc);

        }

};

