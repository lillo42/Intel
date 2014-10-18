#include "filter.h"

Filter::Filter()
{
}

void Filter::saltPepper(Mat frame)
{
    Mat saltpepper_noise = Mat::zeros(frame.rows, frame.cols,CV_8U);
    randu(saltpepper_noise,0,255);

    Mat black = saltpepper_noise < 30;
    Mat white = saltpepper_noise > 225;

    frame.setTo(255,white);
    frame.setTo(0,black);
}
