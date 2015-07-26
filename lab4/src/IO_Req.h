
#ifndef IO_REQ_H
#define IO_REQ_H

class IO_Req {

    private:

        int arrTime;
        int issueTime;
        int finishTime;
        static int Req_Id;
        int Track;
        int requestedTrack;

    public:

        enum States{
            ADD,ISSUE,FINISH
        };

        States state;

        IO_Req(int arrTime, int Track){

            this->arrTime = arrTime;
            this->Track = Track;
            this->Req_Id = Req_Id++;
            this->finishTime = 0;
            this->issueTime = 0;
            this->state = ADD;
            this->requestedTrack = Track;
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
};
int IO_Req::Req_Id = -1;
#endif
