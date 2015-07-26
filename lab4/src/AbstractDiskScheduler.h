
#ifndef ABSTRACTDISKSCHEDULER_H
#define ABSTRACTDISKSCHEDULER_H

class AbstractDiskScheduler{


public:

        AbstractDiskScheduler(){}

        
        virtual IO_Req* getNewRequest() = 0;
        virtual void addRequest(IO_Req* req) = 0;
        //virtual string get_schedulerName() = 0;


};
#endif


