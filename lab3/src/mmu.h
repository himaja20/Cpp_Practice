
#include "pte.h"
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


public:
    mmu(int numOfFrames, char* algVal, char* opVals, AbstractPageReplacement* apr){

    pageTable = vector<pte>(64);
    numFrames = numOfFrames; 
    frameTable = vector<unsigned int>(0);
    frameToPageMapping = vector<unsigned int> (numFrames);
    this->algVal = algVal;
    this->opVals = opVals;
    this->apr = apr;

    }

    void handleInstruction(int op, int pIndex){
    
    pte page = pageTable[pIndex];
    unsigned int framenum; 
    unsigned int v_oldPageNum;
    cout << "inst---> " << op << " " << pIndex << endl;

    if(page.present == 0) {

        if(framenum <= numFrames){
            framenum = frameTable.size();
            frameTable.push_back(framenum);

            frameToPageMapping[framenum] = pIndex;
            
            if (op == 1) {
                page.modified  = 1;
            }
            page.referenced = 1;
            page.pageFrameNumber = framenum;
            page.present = 1;
            cout << "ZERO" << endl;
            cout << "MAP" << " " << pIndex << " " << framenum << endl;
        }
        else{

            framenum = apr->getFrame();
            v_oldPageNum = frameToPageMapping[framenum];
            pte v_oldPage = pageTable[v_oldPageNum];
            v_oldPage.present = 0;
            cout << "UNMAP" << v_oldPageNum << " " << framenum << endl; 
            if (v_oldPage.modified == 1) {
                v_oldPage.modified = 0;
                v_oldPage.pagedout = 1;
                cout << "OUT" << v_oldPageNum << " " << framenum << endl;
            }

            if(op = 1){
                page.modified = 1;
             }
             page.referenced = 1;

             if(page.pagedout == 1)
             {
                cout << "IN" << pIndex << " " << framenum << endl;
             }
             else{

                cout << "ZERO" << " " << " " << framenum << endl;
             }

             page.present = 1;
             page.pageFrameNumber = framenum;


        }

    }
//    cout << "mmu class" << endl;
  //  cout << op << " operation" << "  " << pIndex << "  page Index " << endl; 

    }


};
