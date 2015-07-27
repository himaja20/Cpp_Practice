#include "IO_Req.h"
#include "Event.h"
#include "myComparison.h"
#include "AbstractDiskScheduler.h"
#include <iomanip>

using namespace std;


class CPU {

    public:

        void test(int Tstamp, int Rid, IO_Req::States state){
            cout << Tstamp << "  " << Rid << "  " << state << "   "  << endl;
        }

        void start_IO(priority_queue<Event*,vector<Event*>,myComparison>& eventQ, map<int,IO_Req*>& IO_ReqMap,AbstractDiskScheduler* ds){
            cout << "TRACE" << endl;
            int nextCPUFreeTime = 0;
            int curTrackNum = 0;
            while(eventQ.size() > 0){ 
                Event* curEvent = eventQ.top();
                eventQ.pop();
                int curReqId = curEvent->getRid();
                int curTimeStamp = curEvent->getTstamp();
                IO_Req* curReq = IO_ReqMap[curReqId];
                if(curEvent->getState() == IO_Req::ADD){
                    cout << curTimeStamp  << ":  " << setw(4) << setfill(' ') << curEvent->getRid() 
                    << " " << "add " << curReq->getRequestedTrack() << endl;
                    curReq->setState(IO_Req::ISSUE);
                    curReq->setStartingTime(curTimeStamp);
                    ds->addRequest(curReq);
                }   
                else if (curEvent->getState() == IO_Req::ISSUE) {
                    int nextEventTime;
                    nextEventTime = curTimeStamp + abs(curTrackNum - curReq->getRequestedTrack());
                    nextCPUFreeTime = curTimeStamp + abs(curTrackNum - curReq->getRequestedTrack());
                    cout << curTimeStamp << ":  " << setw(4) << setfill(' ') << 
                    curEvent->getRid() << " " << "issue" << " " << curReq->getRequestedTrack() << " "
                        << curTrackNum << endl;
                    curReq->setState(IO_Req::FINISH);
                    eventQ.push(new Event(nextEventTime,curReq->getRid(),curReq->getState()));
                }   
                else if(curEvent->getState() == IO_Req::FINISH){
                    cout << curTimeStamp << ":  " << setw(4) << setfill(' ') << curEvent->getRid() << " " << "finish" << " " 
                        << curTimeStamp - curReq->getStartingTime() << endl;
                    curTrackNum = curReq->getRequestedTrack();
                }
                if (curTimeStamp >= nextCPUFreeTime){
                    IO_Req* newReq = ds->getNewRequest();
                    if (newReq != NULL ) {
                        eventQ.push(new Event(curTimeStamp,newReq->getRid(),newReq->getState()));
                    }

                }
            }
        }

};
