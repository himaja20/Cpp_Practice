#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;
class NRU : public AbstractPageReplacement {
    private:

        int pageRequestCounter;

    public:

        NRU(): AbstractPageReplacement(){
            pageRequestCounter = 0;
        } 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            vector<pte> *classes = new vector<pte>[4];
            int vPageIndex;
            int frameNum;
            int myRand= rand->myrandom();
            int numFrames;

            pageRequestCounter++;

            for(vector<pte>::iterator it = pageTable.begin();it != pageTable.end(); it++) 
            {
                if(it->present == 1) {
                    if((it->referenced == 0) && (it->modified == 0) )
                    {
                        classes[0].push_back(*it);
                    }
                    else if((it->referenced == 0) && (it->modified == 1))
                    {
                        classes[1].push_back(*it);
                    }
                    else if((it->referenced == 1) && (it->modified ==1 ))
                    { 
                        classes[3].push_back(*it);
                    }
                    else if((it->referenced == 1) && (it->modified == 0))
                    { 
                        classes[2].push_back(*it);
                    }
                }
            }

            int classIndex = -1; 
            while(true){
                classIndex++;
                if (classes[classIndex].size() > 0) {
                    numFrames = classes[classIndex].size();
                    break;
                }
            }
            vPageIndex = myRand % numFrames;
            pte &page = (classes[classIndex])[vPageIndex];
            frameNum = page.pageFrameNumber;
            
            if (pageRequestCounter == 10) {
               pageRequestCounter = 0;
                for (vector<pte>::iterator it = pageTable.begin() ; it !=pageTable.end(); it++)
                {
                    if(it->present == 1){
                        it->referenced = 0;
                    }
                }
            }
            
            return frameNum;
        }
        
        void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}
};
