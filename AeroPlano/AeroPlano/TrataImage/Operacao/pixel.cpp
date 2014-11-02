#include "pixel.h"

Pixel::Pixel()
{
}

void Pixel::detected(Mat &frame)//,vector<Point> point)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Point roi;  // Armazena as coordenadas das Features
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    point.clear();
    for(int i = 0; i <= frame.rows - HEIGHT_PADRAO ; i++)
    {
        roi.y = i;

        for(int j =0; j <= frame.cols - WIDTH_PADRAO ; j++)
        {
            roi.x = j;

            frame.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,size);

            Mat temp;

            MatConstIterator_<float> it = ROI_TRUE.begin<float>(), it_end = ROI_TRUE.end<float>();

            for(; it != it_end; ++it)
                temp.push_back(*it);


            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

            if ( PREDICAO > 12)
            {
                point.push_back(roi);
                IMAGENSPOSITIVAS++;
            }
            else
                IMAGENSNEGATIVAS++;
        }
    }
}

void Pixel::load()
{
    boost.load("Pixel.xml");
}
