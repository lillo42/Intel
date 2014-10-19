#ifndef SAVE_H
#define SAVE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;
using namespace cv;
class Save
{
public:
    Save();

    void save(QString pathName, QString nameImage, Mat frame , int count);
    void save(QString imageName , int count);
    void save(string path,Mat frame);
private:
    QString txtPath;

};

#endif // SAVE_H
