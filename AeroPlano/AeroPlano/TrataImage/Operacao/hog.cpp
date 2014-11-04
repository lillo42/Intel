#include "hog.h"

HOG::HOG()
{
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
}

void HOG::detecta(Mat frame)//,vector<Rect> &found, vector<Rect> &found_filtered)
{
    found.clear();
    found_filtered.clear();
    //hogDefault.detectMultiScale(Query,found,1);
    hog.detectMultiScale(frame, found, 0, Size(8,8), Size(64,64), 1.05, 2);

    size_t i, j;
    for( i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }
}

void HOG::detecta(Mat frame, vector<Rect> &found_filtered)
{
    found.clear();
    found_filtered.clear();
    //hogDefault.detectMultiScale(Query,found,1);
    hog.detectMultiScale(frame, found, 0, Size(8,8), Size(64,64), 1.05, 2);

    size_t i, j;
    for( i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }
}
