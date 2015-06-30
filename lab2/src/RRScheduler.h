
#include "AbstractScheduler.h"
#include <sstream>

class RRScheduler: public AbstractScheduler {

    private:

        queue<Process*> readyQ;

    public:

        RRScheduler(int quantum) : AbstractScheduler(quantum) {}

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
                proc->reset_dp();
            }

        }

        string get_schedulerName(){
           
           string name;
           std::ostringstream oss;
           
           name = "RR " ;
           oss << name << quantum;
            
            name = oss.str();
            return name;
        }

};

