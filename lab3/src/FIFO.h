

//#include "AbstractPageReplacement.h"
#include <list>

using namespace std;

class FIFO : public AbstractPageReplacement {

private:

    list<unsigned int> physicalFrames;

public:

    FIFO() : AbstractPageReplacement(){}

    int getFrame(){

    return 1;
    }

};
