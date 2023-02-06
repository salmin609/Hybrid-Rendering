//#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "Affine.h"
#include "PPMReader.h"

using namespace std;


void readImage(std::string fname, std::vector<Color_Val>& colorVec, int& width, int& height, int& maxVal)
{
    ifstream f;
    f.open(fname.c_str(), ifstream::in | ifstream::binary);
    cout << fname.c_str() << endl;
    if (!f.is_open()) {
        cout << "Cannot open the file" << endl;
        return;
    }
    else
        cout << "opened" << endl;

    string temp;
    string temp2;

    std::getline(f, temp);
    std::getline(f, temp2);


    f >> width;
    f >> height;
    f >> maxVal;

    unsigned int value;
    //int value;

    while (!f.eof())
    {
        Color_Val colorVal;
        f >> value;
        colorVal.r = value;

        f >> value;
        //colorVal.y = (float)value / (float)maxVal;
        colorVal.g = value;

        f >> value;
        //colorVal.z = (float)value / (float)maxVal;
        colorVal.b = value;

        colorVec.push_back(colorVal);

    }
    const int a = 0;

}