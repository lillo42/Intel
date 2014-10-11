#ifndef GRAVAIMAGE_H
#define GRAVAIMAGE_H


//QT
#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>


//OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
class GravaImage : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit GravaImage(QObject *parent = 0);

    QString GetPath();
    void SetPath(QString path);

    int GetContador();
    QString getNomePadrao();

    void Grava(Mat frame);
    void Grava(QString nomeImage, Mat frame,int quantidade);

    void run();
signals:
    void OnGravou(Mat frame);
public slots:


private:
    QString path;
    int contador;

    QList<Mat> listaSalva;

    QWaitCondition sincronizaThread;

    QString GetCaminhoImage();
    void GravaNoHD(Mat frame);
    void GravaListaImagem();

    const QString NOME_IMAGEM = "//imagem";
    const QString EXTENSAO= ".jpg";
};

#endif // GRAVAIMAGE_H
