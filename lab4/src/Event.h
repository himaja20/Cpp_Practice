#ifndef EVENT_H
#define EVENT_H

#include "IO_Req.h"

class Event{

    private:

        static int e_counter;
        int Tstamp;
        int Rid;
        IO_Req::States state;
        int eid;
       // int curTimeStamp;
       

    public:
        
        Event(int Tstamp, int Rid, IO_Req::States state){
            this->Tstamp = Tstamp;
            this->Rid = Rid;
            this->state = state;
            this->eid = e_counter++;
    
        }

        int getTstamp(){
            return Tstamp;
        }

        int getRid(){
            return Rid;
        }

        IO_Req::States getState(){
            return state;
        }

        int getEid(){
            return eid;
        }


};
int Event::e_counter = 0;
#endif
