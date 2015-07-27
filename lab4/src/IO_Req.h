
#ifndef IO_REQ_H
#define IO_REQ_H

class IO_Req {

    private:

        int arrTime;
        int diskEndTime;
        static int Req_Counter;
        int requestedTrack;
        int diskStartTime;
        int reqAddTime;

    public:
        int Req_Id;
        enum States{
            ADD,ISSUE,FINISH
        };

        States state;

        IO_Req(int arrTime, int Track){

            this->arrTime = arrTime;
            this->Req_Id = Req_Counter++;
            this->diskEndTime = 0;
            this->state = ADD;
            this->requestedTrack = Track;
            this->diskStartTime = 0;
            this->reqAddTime = 0;
        }

       int getRid(){
            return Req_Id;
        }

        States getState(){
            return state;
        }

        void setState(States state){
            this->state = state;
        }

        int getRequestedTrack(){
            return requestedTrack;
        }

        int getArrTime(){
            return arrTime;
        }

        void setDiskStartTime(int time){
             diskStartTime = time;
        }

        int getDiskStartTime(){
            return diskStartTime;
        }

        int getDiskEndTime(){
            return diskEndTime;
        }

        void setDiskEndTime(int time){
            diskEndTime = time;
        }

        int getReqAddTime(){
            return reqAddTime;
        }

        void setReqAddTime(int time){
            reqAddTime = time;
        }


};
int IO_Req::Req_Counter= 0;
#endif
