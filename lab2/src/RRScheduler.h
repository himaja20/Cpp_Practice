
#include "AbstractScheduler.h"


class RRScheduler: public AbstractScheduler {

    public:

        RRScheduler(int quantum) : AbstractScheduler(quantum) {}

        int getNewProcess(){
            return 1;
        }

        void addProcess(){
        }

};

