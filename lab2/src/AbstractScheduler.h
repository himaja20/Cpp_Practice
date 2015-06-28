#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

class AbstractScheduler{

    private:

        int quantum;

    public:

        AbstractScheduler(int quantum){
           this->quantum = quantum;
        }

        virtual int getNewProcess() = 0;
        virtual void addProcess() = 0;


};
#endif



