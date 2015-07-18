
#include "pte.h"
#include <iomanip>
#include <cstring>

//#include "AbstractPageReplacement.h"

using namespace std;

class mmu {

    private:
        vector<pte> pageTable;
        vector<unsigned int> frameTable;
        vector<unsigned int> frameToPageMapping;
        int numFrames;
        char* algVal;
        char* opVals;
        AbstractPageReplacement* apr;
        int instrCounter;
        bool O_flag;
        bool P_flag;
        bool F_flag;
        bool S_flag;
        bool p_flag;
        bool f_flag;
        bool a_flag;


    public:
        mmu(int numOfFrames, char* opVals, AbstractPageReplacement* apr){

            pageTable = vector<pte>(64);
            numFrames = numOfFrames; 
            frameTable = vector<unsigned int>(0);
            frameToPageMapping = vector<unsigned int> (numFrames);
            this->opVals = opVals;
            this->apr = apr;
            instrCounter = 0;
            O_flag = false;
            P_flag = false;
            F_flag = false;
            S_flag = false;
            p_flag = false;
            f_flag = false;
            a_flag = false;
        }

        void printPte(){

            string myPte;
            int pteIndex = 0;
            for(vector<pte>::iterator it = pageTable.begin(); it !=pageTable.end(); it++){
                if(it->present == 0)
                {
                    (it->pagedout == 0)? cout << "* " : cout << "# ";
                }
                else{
                    cout << pteIndex << ":" ; 
                    (it->referenced == 1)? cout << "R" : cout << "-";
                    (it->modified == 1)? cout << "M" : cout << "-";
                    (it->pagedout == 1)? cout << "S " : cout << "- ";
                }
                pteIndex++;
            } 
        }

        void setOptionFlags(){

            int optLen = strlen(opVals);
            while(optLen > 0){
                char c = opVals[optLen - 1];
                switch(c)
                {
                    case 'O':
                        O_flag = true;
                        break;

                    case 'P':
                        P_flag = true;
                        break;

                    case 'F':
                        F_flag = true;
                        break;

                    case 'S':
                        S_flag = true;
                        break;

                    case 'p':///
                        p_flag = true;
                        break;

                    case 'f':
                        f_flag = true;
                        break;

                    case 'a':
                        a_flag = true;
                        break;
                }

                optLen--;

            }


        }

        void printFinalInfo(){
            if (P_flag){
                printPte();
            }
            if (F_flag){
                
            }
    
        }


        void printF2P(){



        }

        void handleInstruction(int op, int pIndex){
            pte &page = pageTable[pIndex];
            unsigned int framenum; 
            unsigned int v_oldPageNum;
            cout << "==> inst: " << op << " " << pIndex << endl;

            if (op == 1) {
                page.modified = 1;
            }
            page.referenced = 1;

            if(page.present == 0) {

                if(frameTable.size() < numFrames){
                    framenum = frameTable.size();
                    frameTable.push_back(framenum);

                    frameToPageMapping[framenum] = pIndex;


                    page.pageFrameNumber = framenum;
                    page.present = 1;
                    cout << instrCounter << ": " << "ZERO" << " " << setw(8) << setfill(' ') << framenum << endl;
                    cout << instrCounter << ": " << "MAP  " << " " << setw(3) << setfill(' ') << pIndex << " " << setw(3) << setfill(' ') << framenum << endl;
                }
                else{

                    framenum = apr->getFrame(pageTable,frameTable,frameToPageMapping);
                    v_oldPageNum = frameToPageMapping[framenum];
                    pte &v_oldPage = pageTable[v_oldPageNum];
                    v_oldPage.present = 0;
                    cout << instrCounter << ": " << "UNMAP" << " " << setw(3) << setfill(' ') << v_oldPageNum << " " << setw(3) << setfill(' ') << framenum << endl; 
                    if (v_oldPage.modified == 1) {
                        v_oldPage.modified = 0;
                        v_oldPage.pagedout = 1;
                        //                cout << "in the if" << endl;
                        cout << instrCounter << ": " << "OUT  " << " " << setw(3) << setfill(' ') <<  v_oldPageNum << " " << setw(3) << setfill(' ') << framenum << endl;
                    }


                    frameToPageMapping[framenum] = pIndex;
                    if(page.pagedout == 1)
                    {
                        cout << instrCounter << ": " << "IN   " << " " << setw(3) << setfill(' ') << pIndex << " " << setw(3) << setfill(' ') << framenum << endl;
                    }
                    else{

                        cout << instrCounter << ": " << "ZERO " << setw(8) << setfill(' ') << framenum << endl;
                    }

                    page.pageFrameNumber = framenum;
                    page.present = 1;
                    cout << instrCounter << ": " << "MAP  " << " " << setw(3) << setfill(' ') << pIndex << " " << setw(3) << setfill(' ') << framenum << endl;


                }

            }

            instrCounter++;
            if (p_flag) {
                printPte();
            }

          }


};
