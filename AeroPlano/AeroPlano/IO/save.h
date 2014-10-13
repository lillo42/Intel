#ifndef SAVE_H
#define SAVE_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;
using namespace cv;
class Save
{
public:
    Save();

    void save(QString path, QString imageName ,int count);
    void save(string path,Mat frame);
private:

};

#endif // SAVE_H
