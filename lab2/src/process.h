#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>

using namespace std;

class Process {

    private:
        int at,tct,mcb,mib;
        int proc_sp;
        int remainingCPUBurst;
        int remainingCPUTime;
        int allowedCPUTime;
        int lastTransitionTime;

        static int p_counter;

    public:

        int pid;
        enum States{
            CREATED, READY, RUNNING, BLOCKED, DONE
        };

        States state;

        Process(int atP, int tctP, int mcbP, int mibP, int proc_spP){

            pid = p_counter++;
            at = atP;
            tct = tctP;
            remainingCPUTime = tctP;
            mcb = mcbP;
            mib = mibP;
            state = CREATED;
            proc_sp = proc_spP;

            remainingCPUBurst = 0;
            allowedCPUTime = 0;
            lastTransitionTime = 0;
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

        int get_remainingCPUBurst(){
            return remainingCPUBurst;
        }

        int get_remainingCPUTime(){
            return remainingCPUTime;
        }

        int get_allowedCPUTime(){
            return allowedCPUTime;
        }

        void set_lastTransitionTime(int tstamp){
            lastTransitionTime = tstamp;
        }

        void set_remainingCPUBurst(int burst){
            remainingCPUBurst = burst;
        }

        void reduceRemCPUBurst(int time){
            remainingCPUBurst = remainingCPUBurst - time;
            remainingCPUTime -= time;
        }

        States get_state(){
            return state;
        }

        friend ostream& operator<<(ostream& os, const Process& proc){
            cout << "printing " << endl;
            os << proc.at << " " << proc.tct << " " << proc.mcb << " " << proc.mib << "  " << proc.proc_sp << endl;
            return os;
        }
};
int Process::p_counter = 0;

#endif
