

#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;



class SecondChance : public AbstractPageReplacement {


    public:

        SecondChance(): AbstractPageReplacement(){} 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            int framenum = -1;
            int vPagenum;
            while(true)
            {
                framenum = frameTable.front();
                vPagenum = fToP[framenum];
                pte &page = pageTable[vPagenum];
                frameTable.erase(frameTable.begin());
                frameTable.push_back(framenum);
                if(page.referenced == 0){
                    break;
                }
                page.referenced = 0;
            }                
           
    
                       
            return framenum;
        }

};
