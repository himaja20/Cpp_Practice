#include "IO_Req.h"
#include <vector>
#include <limits.h>
#include "AbstractDiskScheduler.h"

using namespace std;

class SSTF : public AbstractDiskScheduler {

    private:
        vector<IO_Req*> IO_queue;

    public:

        SSTF() : AbstractDiskScheduler(){}

        IO_Req* getNewRequest(int curTrackLoc){

            int minDist = INT_MAX, index = 0, trackDist;
            if (IO_queue.size() == 0){
                return NULL;
            }

            for (int i = 0; i<IO_queue.size() ; i++)
            {  
                trackDist = abs(IO_queue[i]->getRequestedTrack() - curTrackLoc);
                if(minDist > trackDist){
                    minDist = trackDist;
                    index = i;
                }
            }
            IO_Req* myReq = IO_queue.at(index);
            IO_queue.erase(IO_queue.begin()+index);
            return myReq;
        }

        void addRequest(IO_Req* req){
            if (req != NULL){
                IO_queue.push_back(req);
            }
        }

};

