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

    AbstractPageReplacement(){
        cout << "in abstract constructor" << endl;
    }
    virtual unsigned int getFrame(vector<pte>&,vector<unsigned int>&,vector<unsigned int>&) = 0;

    void setRandomUtil(RandomUtil* rand){
        this->rand = rand;
    }

};
#endif
