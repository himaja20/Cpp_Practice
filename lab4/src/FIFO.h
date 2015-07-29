
#include "IO_Req.h"
#include "AbstractDiskScheduler.h"

class FIFO : public AbstractDiskScheduler {

private:
    queue<IO_Req*> IO_queue;

public:

    FIFO() : AbstractDiskScheduler(){}

    IO_Req* getNewRequest(int curTrackLoc){
        
        if (IO_queue.size() == 0){
            return NULL;
        }
        IO_Req* myReq = IO_queue.front();
        IO_queue.pop();
        return myReq;
    }

    void addRequest(IO_Req* req){
        if (req != NULL){
            IO_queue.push(req);
         }
    }

};

