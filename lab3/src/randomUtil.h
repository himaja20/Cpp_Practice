#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include <iostream>
#include <fstream>

using namespace std;

class RandomUtil{

    private:
        int *ranvals;
        int offset;
        int ranvalsSize;

    public:


        RandomUtil(char* rFile){
            ifstream rfin(rFile,ios::in);
            long tmp, i = 0;
            if (!rfin.is_open())
            {
                cout << "Random File open failed" << endl;
                exit(0);
            }

            if(rfin >> tmp){
                ranvals = new int[tmp];
                ranvalsSize = tmp;
            }
            else{
                exit(0);
            }

            while(rfin >> tmp){
                ranvals[i] = tmp;//segmentation field
                i++;
            }

            offset = 0;
        }

        int myrandom() 
        {
            int returnVal;
            if (offset == ranvalsSize - 1){
                offset = 0;
            }
            returnVal = ranvals[offset];
            //returnVal = (ranvals[offset] % frameTableSize);
            offset++;

            return returnVal;
        }
};
#endif
