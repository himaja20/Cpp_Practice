
#include "AbstractPageReplacement.h"
#include "pte.h"
#include <vector>

using namespace std;

class LRU : public AbstractPageReplacement {

public:

    LRU() : AbstractPageReplacement(){}

    unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){
            
            unsigned int framenum;
            framenum = frameTable.front();
            frameTable.erase(frameTable.begin());
            frameTable.push_back(framenum);
            return framenum;   


    }


    void updateFrameTable(vector<unsigned int>& frameTable,int framenum){
           unsigned int fNum = framenum;
           for(unsigned int i=0 ; i < frameTable.size() ; i++){
                if (frameTable[i] == fNum){
                    frameTable.erase(frameTable.begin()+i);
                    frameTable.push_back(fNum);
                    break;
                }
           }
    }


};
