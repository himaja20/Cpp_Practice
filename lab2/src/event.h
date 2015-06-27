#include <iostream>
#include "process.h"

using namespace std;

class Event{

    private:

        static int e_counter;
        int Tstamp;
        int pid;
        Process::States prevState;
        Process::States newState;

    public:

        int eid;
        Event(int pidP, int TstampP, Process::States prevStateP, Process::States newStateP){
            eid = e_counter++;
            pid = pidP;
            Tstamp = TstampP;
            prevState = prevStateP;
            newState = newStateP;
        }

        int get_pid(){
            return pid;
        }

        int get_eid(){
            return eid;
        }


        int get_Tstamp(){
            return Tstamp;
        }

        Process::States get_prevState(){
            return prevState;
        }

        Process::States get_newState(){
            return newState;
        }
        
        friend ostream& operator<<(ostream& os, const Event& eve){
            cout << "printing events " << endl;
            os << eve.eid << " " << eve.pid << " " << eve.Tstamp << " " << eve.prevState << " " << eve.newState << endl;
            return os;
        }

};
int Event::e_counter = 0;
