#ifndef HOG_H
#define HOG_H


#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;
class HOG
{
public:
    HOG();
    void detecta(Mat frame, vector<Rect> &found_filtered);
    void detecta(Mat frame);
    vector<Rect> found,found_filtered;
private:
    HOGDescriptor hog;

};

#endif // HOG_H
