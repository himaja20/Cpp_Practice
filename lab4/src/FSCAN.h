#include "IO_Req.h"
#include <vector>
#include <limits.h>
#include "AbstractDiskScheduler.h"

using namespace std;

class FSCAN : public AbstractDiskScheduler {


private:

vector<IO_Req*>* IO_queue;
vector<IO_Req*>* buffer_list;
vector<IO_Req*> list_one;
vector<IO_Req*> list_two;
bool positive;


public:

FSCAN(){
    positive = true;
    buffer_list = &list_one;
    IO_queue = &list_two;
}


IO_Req* getNewRequest(int curTrackLoc){
    if(IO_queue->size() == 0 && buffer_list->size() == 0){
        return NULL;
    }
    if(IO_queue->size() == 0){
        vector<IO_Req*>* temp = IO_queue;
        IO_queue = buffer_list;
        buffer_list = temp;
        positive = true;
    }
    IO_Req *minToLeft = NULL, *minToRight = NULL, *ioToReturn = NULL;
    int locToErase = -1;
    int minLeftDistance = INT_MAX, minRightDistance = INT_MAX, leftLoc = -1, rightLoc = -1;
    unsigned int track_distance;
    bool skip = false;
    for(int i=0; i < IO_queue->size(); i++){
        track_distance = abs(IO_queue->at(i)->getRequestedTrack() - curTrackLoc);
        if(track_distance == 0){
            skip = true;
            locToErase = i;
            ioToReturn = IO_queue->at(i);
            break;
        }
        if(IO_queue->at(i)->getRequestedTrack() < curTrackLoc){
            if(track_distance < minLeftDistance){
                minToLeft = IO_queue->at(i);
                leftLoc = i;
                minLeftDistance = track_distance;
            }
        }else{
            if(track_distance < minRightDistance){
                minToRight = IO_queue->at(i);
                rightLoc = i;
                minRightDistance = track_distance;
            }
        }
    }
    if(!skip){
        if(positive && minToRight == NULL){
            ioToReturn = minToLeft;
            locToErase = leftLoc;
            positive = !positive;
        }else if(positive && minToRight != NULL){
            ioToReturn = minToRight;
            locToErase = rightLoc;
        }else if(!positive && minToLeft == NULL){
            ioToReturn = minToRight;
            locToErase = rightLoc;
            positive = !positive;
        }else{
            ioToReturn = minToLeft;
            locToErase = leftLoc;
        }
    }
    IO_queue->erase(IO_queue->begin() + locToErase);
    return ioToReturn;
}

void addRequest(IO_Req* req){
    int loc = buffer_list->size();
    for(int i = 0; i < buffer_list->size(); i++){
        if(req->getRequestedTrack() < buffer_list->at(i)->getRequestedTrack()){
            loc = i;
            break;
        }
    }
    buffer_list->insert(buffer_list->begin() + loc, req);
}
};
