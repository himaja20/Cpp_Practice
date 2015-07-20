#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;

class ClockVirtual : public AbstractPageReplacement {

    private:

        int ptr;

    public:

        ClockVirtual(): AbstractPageReplacement(){

            ptr = 0;

        } 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            int frameNum = -1;

            while(true){   

                pte &page = pageTable[ptr];
                frameNum = page.pageFrameNumber;
                if (page.present == 1){
                    if (page.referenced == 0)
                     {   
                        break;
                     }
                    page.referenced = 0;
                }
                ptr = (ptr + 1)%(pageTable.size());
            }
            ptr = (ptr + 1)%(pageTable.size());
            return frameNum;

        }

    void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}

};
