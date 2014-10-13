#include "save.h"

Save::Save()
{
}

void Save::save(QString path, QString imageName, int count)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QString text;
    text =  imageName + "\t Count:" + QString::number(count) + "\n";
    out << text;
    file.close();
}

void Save::save(string path, Mat frame)
{
    imwrite(path,frame);
}
