#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;

class Clock : public AbstractPageReplacement {

    private:

        int ptr;

    public:

        Clock(): AbstractPageReplacement(){

            ptr = 0;

        } 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            int frameNum = -1;
            int vPageNum;

            while(true){   

                frameNum = frameTable[ptr];
                vPageNum = fToP[frameNum];
                pte &page = pageTable[vPageNum];
                if (page.referenced == 0)
                {
                    break;

                }
                page.referenced = 0;
                ptr = (ptr + 1)%(frameTable.size());
            }
            ptr = (ptr + 1)%(frameTable.size());
            return frameNum;

        }

    void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}
};
