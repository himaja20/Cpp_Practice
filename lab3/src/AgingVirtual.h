#include "AbstractPageReplacement.h"
#include "pte.h"
#include <vector>

using namespace std;

class AgingVirtual : public AbstractPageReplacement {

    private:

        vector<unsigned int> counter;

    public:

        AgingVirtual() : AbstractPageReplacement() {}

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){

            if(counter.size() == 0) {
                counter = vector<unsigned int>(pageTable.size(),0);
            }
            int framenum = -1;
            int vPageIndex;
            unsigned int rBit;
            unsigned int currentCounter;
            unsigned int smallest = 0xffffffff;
            int smallestIndex;
            for(int i = 0; i < pageTable.size() ; i++){
                pte &page = pageTable[i];
                rBit = page.referenced;
                currentCounter = counter[i];
                currentCounter = currentCounter >> 1 | rBit << 31;
                counter[i] = currentCounter;
            //cout << counter[i] << "   counter Values " << i << endl;
            }

            for(int i = 0; i < pageTable.size() ; i++)
            {

                pte &page = pageTable[i];
                if(page.present == 1) {
                    page.referenced = 0;
                
                    if (counter[i] < smallest){
                        smallest = counter[i];
                        smallestIndex = i;
                       // cou << smallestIndex << " smallestIndex" << endl;
                    }
                }
            }


            pte &page = pageTable[smallestIndex];
            framenum = page.pageFrameNumber;
            counter[smallestIndex] = 0;
            return framenum;


        }   

        void updateFrameTable(vector<unsigned int>& frameTable, int framenum){}
};
