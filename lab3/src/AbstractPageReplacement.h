#ifndef ABSTRACTPAGEREPLACEMENT_H
#define ABSTRACTPAGEREPLACEMENT_H


#include <vector>
#include "pte.h"
#include "randomUtil.h"


using namespace std;

class AbstractPageReplacement{

protected:
    RandomUtil* rand;


public: 

    AbstractPageReplacement(){}
    virtual unsigned int getFrame(vector<pte>&,vector<unsigned int>&,vector<unsigned int>&) = 0;
    virtual void updateFrameTable(vector<unsigned int>&,int) = 0;
    void setRandomUtil(RandomUtil* rand){
        this->rand = rand;
    }

};
#endif
