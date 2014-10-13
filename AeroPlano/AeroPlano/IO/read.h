#ifndef READ_H
#define READ_H

#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;
class Read
{
public:
    Read();

    Mat readImage(string path);
};

#endif // READ_H
