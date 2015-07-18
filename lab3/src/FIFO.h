

#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;



class FIFO : public AbstractPageReplacement {


    public:

        FIFO(): AbstractPageReplacement(){} 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            int framenum;
            framenum = frameTable.front();
            frameTable.erase(frameTable.begin());
            frameTable.push_back(framenum);
            return framenum;
        }

};
