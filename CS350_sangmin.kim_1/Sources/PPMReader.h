//unsigned char* readPPM(const char* const fileName, unsigned int* const width, unsigned int* const height, unsigned int* const maximum);
//unsigned char* readPPM(const char* fileName, char* pSix, int* width, int* height, int* maximum);

#include <string>
#include <fstream>
#include "Texture.h"


//void ignoreComment(std::ifstream& f);
void readImage(std::string fname, std::vector<Color_Val>& colorVec, int& width, int& height, int& maxVal);