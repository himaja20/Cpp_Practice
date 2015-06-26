#include <iostream>
#include "process.h"


using namespace std;

class Event{


    private:

        int Tstamp;
        int pid;
        Process::States prevState;
        Process::States newState;

    public:

        Event(int pidP, int TstampP, Process::States prevStateP, Process::States newStateP){
            pid = pidP;
            Tstamp = TstampP;
            prevState = prevStateP;
            newState = newStateP;
        }

};
