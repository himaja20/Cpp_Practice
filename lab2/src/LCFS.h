
#include "AbstractScheduler.h"
#include <stack>


class LCFS: public AbstractScheduler {

    private:

        stack<Process*> readyQ;

    public:

        LCFS(int quantum) : AbstractScheduler(quantum) {}

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
            return "LCFS";
        }


} ;




