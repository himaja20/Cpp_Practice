
#include "AbstractPageReplacement.h"
#include "pte.h"
#include <vector>

using namespace std;

class Aging : public AbstractPageReplacement {

   private:

        vector<unsigned int> counter;
   
   public:

        Aging() : AbstractPageReplacement() {}
        
        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){
                
               if(counter.size() == 0) {
                    counter = vector<unsigned int>(frameTable.size(),0);
               }
                int framenum = -1;
                int vPageIndex;
                unsigned int rBit;
                unsigned int currentCounter;
                unsigned int smallest = 0xffffffff;
                int smallestIndex;
                for(unsigned int i = 0; i < frameTable.size() ; i++){
                    vPageIndex = fToP[i];
                    pte &page = pageTable[vPageIndex];
                    rBit = page.referenced;
                    currentCounter = counter[i];
                    currentCounter = currentCounter >> 1 | rBit << 31;
                    counter[i] = currentCounter;
                    page.referenced = 0;
                }

                for(unsigned int i = 0; i < counter.size() ; i++)
                {
                    if (counter[i] < smallest){
                        smallest = counter[i];
                        smallestIndex = i;
                    }
                }

                counter[smallestIndex] = 0;
                
                
                framenum = frameTable[smallestIndex];
                return framenum;

               
        }   

    void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}
};
