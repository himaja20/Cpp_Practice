#include "AbstractScheduler.h"
#include <sstream>
#include <queue>

class Prio: public AbstractScheduler {

    private:

        queue<Process*>* activeQueue;
        queue<Process*>* expiredQueue;


        Process* fetchFromQueue(queue<Process*>* q){
            Process* p = NULL;
            for(int i = 3; i >= 0; i--){
                if(q[i].size() == 0){
                    continue;
                }
                p = q[i].front();
                q[i].pop();
                break;
            }
            return p;
        }

    public:

        Prio(int quantum) : AbstractScheduler(quantum) {
            activeQueue = new queue<Process*>[4];
            expiredQueue = new queue<Process*>[4];
        }


        void addProcess(Process* p){
            int dp = p->get_dp();

            queue<Process*>* queueToAdd = activeQueue; 

            if(dp < 0){
                queueToAdd = expiredQueue;
                p->reset_dp();
            }

            queueToAdd[p->get_dp()].push(p);
        };

        Process* getNewProcess(){
            Process* p = fetchFromQueue(activeQueue);

            if(p == NULL){
                queue<Process*>* temp = activeQueue; 
                activeQueue  = expiredQueue;
                expiredQueue = temp;
                p = fetchFromQueue(activeQueue);
            }
            return p;
        }
        
        string get_schedulerName(){
           string name;
           std::ostringstream oss;
           
           name = "PRIO " ;
           oss << name << quantum;
            
            name = oss.str();
            return name;
        }
        
};
