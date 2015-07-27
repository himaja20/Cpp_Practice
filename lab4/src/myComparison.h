#ifndef MYCOMPARISON_H
#define MYCOMPARISON_H

#include "Event.h"

using namespace std;

class myComparison{

    public:

        bool operator()(Event* e1,Event* e2){
            /*
             * first check timestamp
             * equal
             *  check eids
             *      return eid1 < eid2
             *  not equal
             *      reutrn ets1 < ets2
             *  
             */
            if (e1->getTstamp() == e2->getTstamp()){
               return (e1->getEid() > e2->getEid());
            }
            return (e1->getTstamp() > e2->getTstamp());

        }
};
#endif
