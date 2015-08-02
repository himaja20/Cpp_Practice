#include "IO_Req.h"
#include <vector>
#include <limits.h>
#include "AbstractDiskScheduler.h"

using namespace std;

class SCAN : public AbstractDiskScheduler {

    private:
        int up_direction;
        vector<IO_Req*> IO_queue;

    public:

        SCAN() : AbstractDiskScheduler()  {
            up_direction = 1;
        }

        IO_Req* getNewRequest(int curTrackLoc){

            if(IO_queue.size() == 0){
                return NULL;
            }
            
            IO_Req* nextDown = NULL;
            IO_Req* nextUp = NULL;  
            IO_Req* returnReq = NULL;
            int trackDiff,leftTrackLoc = -1,rightTrackLoc = -1,locToErase = -1;
            int minLeft = INT_MAX; 
            int minRight = INT_MAX;
            bool skip = false;

            for (int i = 0; i<IO_queue.size(); i++){

                trackDiff = abs(IO_queue[i]->getRequestedTrack() - curTrackLoc);

                if (IO_queue[i]->getRequestedTrack() == curTrackLoc){
                    locToErase = i;
                    returnReq = IO_queue[i];
                    skip = true;
                    break;
                }

                if (IO_queue[i]->getRequestedTrack() < curTrackLoc){
                    if(trackDiff < minLeft){
                        minLeft = trackDiff;
                        leftTrackLoc = i;
                        nextDown = IO_queue[i];
                    }
                }
                else{
                    if(trackDiff < minRight){
                        minRight = trackDiff;
                        rightTrackLoc = i;
                        nextUp = IO_queue[i];
                    }

                }
            }

            if(!skip){

                if((up_direction == 1) && (nextUp == NULL)){
                    up_direction = 0;
                    returnReq = nextDown;
                    locToErase = leftTrackLoc;
                }   

                if((up_direction == 1) && (nextUp != NULL)){
                    returnReq = nextUp;
                    locToErase = rightTrackLoc;
                }

                if((up_direction == 0 ) && (nextDown != NULL)){
                    returnReq = nextDown;
                    locToErase = leftTrackLoc;
                }

                if((up_direction == 0) && (nextDown == NULL)){
                    up_direction = 1;
                    returnReq = nextUp;
                    locToErase = rightTrackLoc;
                }

            }

            IO_queue.erase(IO_queue.begin() + locToErase);
            return returnReq;
        }

        void addRequest(IO_Req* req){
            if (req == NULL){
                return;
            }

            int i = 0;
            while(i < IO_queue.size() && IO_queue[i]->getRequestedTrack() <= req->getRequestedTrack()){
                i++;
            }

            IO_queue.insert(IO_queue.begin() + i , req);
        }

};
