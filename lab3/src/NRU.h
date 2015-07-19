#include "AbstractPageReplacement.h"
#include <vector>
#include "pte.h"

using namespace std;
class NRU : public AbstractPageReplacement {
    private:
        vector<pte> class3;
        vector<pte> class2;
        vector<pte> class1;
        vector<pte> class0;
        vector<int> nonEmptyFlag;

    public:

        NRU(): AbstractPageReplacement(){
            cout << "in the NRU constructor" << endl;
            class3 = vector<pte>(0);
            class2 = vector<pte>(0);
            class1 = vector<pte>(0);
            class0 = vector<pte>(0);
            nonEmptyFlag = vector<int>(4,0);

        } 

        unsigned int getFrame(vector<pte>& pageTable,vector<unsigned int>& frameTable, vector<unsigned int>& fToP){
            cout << "in getFrame of NRU " << endl;
            int framenum;
            int myRand= rand->myrandom();
            int numFrames;
            int classIndex = getLowestClass(pageTable);
            
            
            switch (classIndex) {
                case 0:
                    numFrames = class0.size();
                    break;
                case 1:
                    numFrames = class1.size();
                    break;
                case 2:
                    numFrames = class2.size();
                    break;
                case 3:
                    numFrames = class3.size();
                    break;
            }

            framenum = (myRand % numFrames);
            frameTable.erase(frameTable.begin()+framenum);
            frameTable.push_back(framenum);
            return framenum;
       

        }

        int getLowestClass(vector<pte>& pageTable){

        cout << "in getLowest class function " << endl;

         for(vector<pte>::iterator it = pageTable.begin();it != pageTable.end(); it++) 
            {

            cout << *it << "printing *it " << endl;
                if((it->referenced == 0) && (it->modified == 0) )
                {
                    class0.push_back(*it);
                    nonEmptyFlag[0] = 1;
                }
                else if((it->referenced == 0) && (it->modified == 1))
                {
                    class1.push_back(*it);
                    nonEmptyFlag[1] = 1;
                }
                else if((it->referenced == 1) && (it->modified ==1 ))
                {
                    class2.push_back(*it);
                    nonEmptyFlag[2] = 1;
                }
                else if((it->referenced == 1) && (it->modified == 0))
                {
                    class3.push_back(*it);
                    nonEmptyFlag[3] = 1;
                }
            }
            
            int classIndex = -1;
            for(vector<int>::iterator it = nonEmptyFlag.begin() ; it != nonEmptyFlag.end() ; it++)
            {
                classIndex++;
                if (*it == 1){
                    break;
                }
            }

            return classIndex;

        }

};
