#include "AbstractScheduler.h"


class prio : public AbstractScheduler {

private:

    vector<Process*> active_q(4);
    vector<Process*> expired_q(4);

public:

    Prio(int quantum) : AbstractScheduler(quantum) {}

    



};
