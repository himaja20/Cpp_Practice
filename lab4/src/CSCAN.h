#include "IO_Req.h"
#include <vector>
#include <limits.h>
#include "AbstractDiskScheduler.h"

using namespace std;

class CSCAN: public AbstractDiskScheduler{
    private:
        vector<IO_Req*> IO_queue;
    
    public:
        
        IO_Req* getNewRequest(int curTrackLoc){
            if(IO_queue.size() == 0){
                return NULL;
            }
            int locToErase = 0;
            IO_Req* ioToReturn = NULL;
            
            for(int i=0; i < IO_queue.size(); i++){
                if(IO_queue[i]->getRequestedTrack() < curTrackLoc){
                    continue;
                }
                locToErase = i;
                break;
            }
            
            ioToReturn = IO_queue[locToErase];
            IO_queue.erase(IO_queue.begin() + locToErase);
            return ioToReturn;
        }
        void addRequest(IO_Req* req){
            int loc = IO_queue.size();
            for(int i = 0; i < IO_queue.size(); i++){
                if(req->getRequestedTrack() < IO_queue[i]->getRequestedTrack()){
                    loc = i;
                    break;
                }
            }
            IO_queue.insert(IO_queue.begin() + loc, req);
        }

};
