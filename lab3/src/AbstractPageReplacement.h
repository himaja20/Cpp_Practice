#ifndef ABSTRACTPAGEREPLACEMENT_H
#define ABSTRACTPAGEREPLACEMENT_H


#include <vector>
#include "pte.h"



using namespace std;

class AbstractPageReplacement{

public: 

    AbstractPageReplacement(){}
    virtual unsigned int getFrame(vector<pte>&,vector<unsigned int>&,vector<unsigned int>&) = 0;

};
#endif
