
#ifndef IO_REQ_H
#define IO_REQ_H

class IO_Req {

    private:

        int arrTime;
        int issueTime;
        int finishTime;
        static int Req_Counter;
        int Track;
        int requestedTrack;
        int startingTime;

    public:
        int Req_Id;
        enum States{
            ADD,ISSUE,FINISH
        };

        States state;

        IO_Req(int arrTime, int Track){

            this->arrTime = arrTime;
            this->Track = Track;
            this->Req_Id = Req_Counter++;
            this->finishTime = 0;
            this->issueTime = 0;
            this->state = ADD;
            this->requestedTrack = Track;
            this->startingTime = 0;
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

        int getTrack(){
            return Track;
        }

        int getRequestedTrack(){
            return requestedTrack;
        }

        int getArrTime(){
            return arrTime;
        }

        void setStartingTime(int time){
             startingTime = time;
        }

        int getStartingTime(){
            return startingTime;
        }

};
int IO_Req::Req_Counter= 0;
#endif
