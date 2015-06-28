#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include <iostream>
#include <fstream>


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

            while(rfin >> tmp){
               ranvals[i] = tmp;//segmentation field
                i++;
            }

            offset = 0;
        }

        int getPriority(){
            return myrandom(4);
        }



        int myrandom(int burst) 
        {
            int returnVal;
            if (offset == ranvalsSize - 1){
                offset = set_ofs();
            }
            returnVal =  1 + (ranvals[offset] % burst);
            offset++;
            
            return returnVal;
        }

        int set_ofs(){
            offset = 0;
            return offset;
        }




};
#endif
