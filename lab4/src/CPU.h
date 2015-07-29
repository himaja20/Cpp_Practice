#include "IO_Req.h"
#include "Event.h"
#include "myComparison.h"
#include "AbstractDiskScheduler.h"
#include <iomanip>
#include <vector>

using namespace std;


class CPU {

    private:
        int totalTime;
        int totalHeadMovements;
        int totalTurnAroundTime;
        int totalWaitTime;
        bool CpuLock;
        vector<int> waitTimes;


    public:


        CPU(){
            this->totalTime = 0;
            this->totalHeadMovements = 0;
            this->totalTurnAroundTime = 0;
            this->totalWaitTime = 0;
            this->CpuLock = false;
            waitTimes = vector<int>(0);
        }

        void start_IO(priority_queue<Event*,vector<Event*>,myComparison>& eventQ, map<int,IO_Req*>& IO_ReqMap,AbstractDiskScheduler* ds){
            cout << "TRACE" << endl;
            int nextCPUFreeTime = 0;
            int curTrackNum = 0;
            int curTimeStamp = 0;
            while(eventQ.size() > 0){ 
                Event* curEvent = eventQ.top();
                eventQ.pop();
                int curReqId = curEvent->getRid();
                curTimeStamp = curEvent->getTstamp();
                IO_Req* curReq = IO_ReqMap[curReqId];
                if(curEvent->getState() == IO_Req::ADD){
                    cout << curTimeStamp  << ":  " << setw(4) << setfill(' ') << curEvent->getRid() 
                        << " " << "add " << curReq->getRequestedTrack() << endl;
                    curReq->setState(IO_Req::ISSUE);
                    curReq->setReqAddTime(curTimeStamp);
                    ds->addRequest(curReq);
                }   
                else if (curEvent->getState() == IO_Req::ISSUE) {
                    int nextEventTime;
                    curReq->setDiskStartTime(curTimeStamp);
                    totalWaitTime += curReq->getDiskStartTime() - curReq->getReqAddTime();
                    waitTimes.push_back(curReq->getDiskStartTime() - curReq->getReqAddTime());
                    nextEventTime = curTimeStamp + abs(curTrackNum - curReq->getRequestedTrack());
                    nextCPUFreeTime = curTimeStamp + abs(curTrackNum - curReq->getRequestedTrack());
                    cout << curTimeStamp << ":  " << setw(4) << setfill(' ') << 
                        curEvent->getRid() << " " << "issue" << " " << curReq->getRequestedTrack() << " "
                        << curTrackNum << endl;
                    curReq->setState(IO_Req::FINISH);
                    eventQ.push(new Event(nextEventTime,curReq->getRid(),curReq->getState()));
                    totalHeadMovements += abs(curTrackNum - curReq->getRequestedTrack());
                    curTrackNum = curReq->getRequestedTrack();
                }   
                else if(curEvent->getState() == IO_Req::FINISH){
                    cout << curTimeStamp << ":  " << setw(4) << setfill(' ') << curEvent->getRid() << " " << "finish" << " " 
                        << curTimeStamp - curReq->getReqAddTime() << endl;
                    curReq->setDiskEndTime(curTimeStamp);
                    totalTurnAroundTime += curTimeStamp - curReq->getReqAddTime();

                }

                //if ((curTimeStamp >= nextCPUFreeTime) && (CpuLock == false)){
                if(curTimeStamp >= nextCPUFreeTime) {
                    IO_Req* newReq = ds->getNewRequest(curTrackNum);
                    if (newReq != NULL ) {
                        eventQ.push(new Event(curTimeStamp,newReq->getRid(),newReq->getState()));
                        nextCPUFreeTime = curTimeStamp + abs(newReq->getRequestedTrack() - curTrackNum);
                    }   

                }
                }
                totalTime = curTimeStamp;
            }

            void printFinalInfo(map<int,IO_Req*>& IO_ReqMap){

                int ReqId, arrTime, diskStartTime, diskEndTime;
                double IO_ReqMapSize = (double)IO_ReqMap.size();
                double avgTurnAroundTime = 0.0;
                double avgWaitTime = 0.0;
                int maxWaitTime;

                avgTurnAroundTime = totalTurnAroundTime/IO_ReqMapSize; 
                avgWaitTime = totalWaitTime/IO_ReqMapSize;

                for(vector<int>::iterator it = waitTimes.begin(); it!=waitTimes.end();it++)
                { 

                    (maxWaitTime < *it)?(maxWaitTime = *it):(maxWaitTime = maxWaitTime);

                }

                cout << "IOREQS INFO" << endl;
                for(map<int,IO_Req*>::iterator it = IO_ReqMap.begin(); it!=IO_ReqMap.end(); it++)
                {
                    IO_Req* req = IO_ReqMap[it->first];
                    ReqId = req->getRid();
                    arrTime = req->getArrTime();
                    diskStartTime = req->getDiskStartTime();
                    diskEndTime = req->getDiskEndTime();
                    cout << setw(5) << setfill(' ') << ReqId << ":" << " " << setw(5) << setfill(' ')  
                        << arrTime << " " << setw(5) << setfill(' ') << diskStartTime << " " << setw(5) << setfill(' ') << diskEndTime << endl;

                } 

                cout << "SUM: " << totalTime << " " << totalHeadMovements << " " << fixed << setprecision(2) << avgTurnAroundTime 
                    << " " << fixed << setprecision(2) << avgWaitTime << " " << maxWaitTime << endl;


            }

        };
