#include "save.h"

Save::Save()
{
    txtPath = QCoreApplication::applicationDirPath() + "/result.txt";
}

void Save::save(QString pathName,QString nameImage, Mat frame, int count)
{
    save(pathName.toStdString(),frame);
    save(nameImage,count);
}

void Save::save( QString imageName, int count)
{
    QFile file(txtPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QString text;
    text =  imageName + " Count:" + QString::number(count) + "\n";
    out << text;
    file.close();
}

void Save::save(string path, Mat frame)
{
    imwrite(path,frame);
}
