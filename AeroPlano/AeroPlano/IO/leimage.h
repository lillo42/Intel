#ifndef LEIMAGE_H
#define LEIMAGE_H

//QT
#include <QObject>
#include <QList>


//OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
class LeImage : public QObject
{
    Q_OBJECT
public:
    explicit LeImage(QObject *parent = 0);

    //QList<Mat> LeImagens(QString caminho);
    Mat leImagem(QString caminho);
    Mat leProximaImagem();

    void setPath(QString path);
    QString getPath();
signals:

public slots:

private:
    int contador;
    QString path;

    QString getCaminho();

    const QString NOME_IMAGEM = "//imagem";
    const QString EXTENSAO= ".jpg";

};

#endif // LEIMAGE_H
