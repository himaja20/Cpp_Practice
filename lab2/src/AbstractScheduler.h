#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

class AbstractScheduler{

    protected:
        int quantum;

    public:

        AbstractScheduler(int quantum){
           this->quantum = quantum;
        }

        int get_quantum(){
            
            return quantum;
        
        }
        
        virtual Process* getNewProcess() = 0;
        virtual void addProcess(Process* proc) = 0;
        virtual string get_schedulerName() = 0;


};
#endif
