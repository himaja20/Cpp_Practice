#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"
#include "randomUtil.h"

using namespace std;



class Random : public AbstractPageReplacement {


    public:

        Random(): AbstractPageReplacement(){} 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){
            int framenum;
            int numFrames = frameTable.size();
            int myRand= rand->myrandom();
            framenum = (myRand % numFrames);
            return framenum;
        }

    void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}
};
