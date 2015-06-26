#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>

using namespace std;

class Process {

    private:
        int at,tct,mcb,mib;
        static int p_counter;

    public:

        int pid;
        enum States{
            CREATED, READY, RUNNING, BLOCKED, DONE
        };

        States state;

        Process(int atP, int tctP, int mcbP, int mibP){
            pid = p_counter++;
            at = atP;
            tct = tctP;
            mcb = mcbP;
            mib = mibP;
            state = CREATED;
        }

        int get_at()
        {
            return at;
        }

        int get_tct() {

            return tct;
        }

        int get_mcb()
        {
            return mcb;
        }

        int get_mib(){
            return mib;
        }

        int get_pid(){
            return pid;
        }

        States get_state(){
            return state;
        }

        friend ostream& operator<<(ostream& os, const Process& proc){
            cout << "printing " << endl;
            os << proc.at << " " << proc.tct << " " << proc.mcb << " " << proc.mib << endl;
            return os;
        }
};
int Process::p_counter = 0;
#endif
