#include "IO_Req.h"
#include "Event.h"
#include "myComparison.h"
#include "AbstractDiskScheduler.h"

using namespace std;


class CPU {


    

public:

        void test(int Tstamp, int Rid, IO_Req::States state){

            cout << Tstamp << "  " << Rid << "  " << state << "   "  << endl;


        }

        void start_IO(priority_queue<Event*,vector<Event*>,myComparison>& eventQ, map<int,IO_Req*>& IO_ReqMap,AbstractDiskScheduler* ds){

            int nextCPUFreeTime = 0;
            int curTrackNum = 0;
            while(eventQ.size() > 0){ 
                Event* curEvent = eventQ.top();
                int curReqId = curEvent->getRid();
                int curTimeStamp = curEvent->getTstamp();
                IO_Req* curReq = IO_ReqMap[curReqId];
                if(curEvent->getState() == IO_Req::ADD){

                     //Insert into IO_queue;
                    cout << curTimeStamp << ":  " << curEvent->getRid() << "  " << "ADD  " << curReq->getRequestedTrack() << endl;
                    curReq->setState(IO_Req::ISSUE);
                    ds->addRequest(curReq);
                 }   
            
                 else if (curEvent->getState() == IO_Req::ISSUE) {

                    int nextEventTime;
                    
                    bool diskCheck = curTrackNum > curReq->getRequestedTrack();
                   
                   if(diskCheck){
                        nextEventTime = curTimeStamp + (curTrackNum - curReq->getRequestedTrack());
                    }
                    else{
                        nextEventTime = curTimeStamp + (curReq->getRequestedTrack() - curTrackNum);
                    }
                   
                   cout << curTimeStamp << ":  " << curEvent->getRid() << "  " << "ISSUE" << "  " << curReq->getRequestedTrack() << "  "
                    << curTrackNum << "  " << endl;

                    curReq->setState(IO_Req::FINISH);
                    eventQ.push(new Event(nextEventTime,curReq->getRid(),curReq->getState()));
                    nextCPUFreeTime = curTimeStamp + curReq->getTrack();
                    
                 }   

                else if(curEvent->getState() == IO_Req::FINISH){
                    curTrackNum = curReq->getRequestedTrack();
                    cout << "In finish state" << endl;

                }
                cout << curTimeStamp << "  " << nextCPUFreeTime << endl;
                if (curTimeStamp >= nextCPUFreeTime){
                    IO_Req* newReq = ds->getNewRequest();
                    if (newReq != NULL ) {
                        eventQ.push(new Event(curTimeStamp,curReq->getRid(),curReq->getState()));
                    }

                }
                eventQ.pop();
        }
    }

};
