#ifndef PIXEL_H
#define PIXEL_H


#include <QString>

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#define WIDTH_PADRAO  64 // cols
#define HEIGHT_PADRAO 128//rows

#define WIDTH_ROI  64  // cols
#define HEIGHT_ROI 128  // rows

#define ANDA_WIDTH 32
#define ANDA_HEIGHT 64

#define RAIO 3
#define VIZINHAZA 24

using namespace cv;
using namespace std;
class Pixel
{
public:
    Pixel();
    void detected(Mat &frame, vector<Point> &point);
    vector<Point> point;
private:
    void load();
    CvBoost boost;
    float PREDICAO;
    unsigned int IMAGENSPOSITIVAS, IMAGENSNEGATIVAS, AMOSTRA;
};

#endif // PIXEL_H
