#include <fstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <queue>
#include <map>
#include <functional>
#include <vector>
#include "IO_Req.h"
#include "Event.h"
#include "CPU.h"
#include "myComparison.h"
#include "AbstractDiskScheduler.h"
#include "FIFO.h"

using namespace std;

AbstractDiskScheduler* algObj;

void getAlgObj(char* opArg){
    
    char c = opArg[0];
    switch(c){
    
        case 'i':
            algObj = new FIFO();
            break;
    }
}




int main(int argc, char* argv[]){
    char* FILE_NAME;
    char* algArg = NULL;
    int time =0;
    int track = 0; 
    string line;

    
    priority_queue<Event*,vector<Event*>,myComparison> eventQ;
    map<int,IO_Req*> IO_ReqMap;

    int c = 0;
    if((c = getopt(argc, argv, "s:")) != -1)
    {
         getAlgObj(optarg);
    }

    FILE_NAME = argv[optind];

    ifstream fin(FILE_NAME, ios::in | ios::binary);
    if (!fin.is_open())
    {
        cout << "File open failed" << endl ;
        exit(0);
    }

    CPU* cpuObj = new CPU();

    if(!fin.eof()){
        while(getline(fin,line)){
            stringstream ss(line);
            if(line[0] != '#'){
                while(ss >> time >> track){
                IO_Req* newReq = new IO_Req(time,track);
                IO_ReqMap.insert(pair<int,IO_Req*>(newReq->getRid(),newReq));
                Event* newEvent = new Event(time,newReq->getRid(),IO_Req::ADD);
                eventQ.push(newEvent);
                }   
                    
                }
            }
        }
        cpuObj->start_IO(eventQ,IO_ReqMap,algObj);
}










