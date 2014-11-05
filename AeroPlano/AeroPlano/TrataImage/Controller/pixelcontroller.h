#ifndef PIXELCONTROLLER_H
#define PIXELCONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../../ImagemController/imagem.h"
#include "../Operacao/pixel.h"

#include <opencv/cv.h>
using namespace cv;

class PixelController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit PixelController(QObject *parent = 0);
    ~PixelController();

    void run();
    void addProcessa(Imagem frame);
    void stopThread();

signals:
    void onFinishedPixel(Imagem frame, vector<Point> &point);
public slots:

private:
    QList<Imagem> listaProcessa;
    Pixel *pixel;

    QWaitCondition sincronizedThread;
    bool finishedThread;

    void processImage();
    void executeFilter();
    void removeRepeated(vector<Point> &points);

};

#endif // PIXELCONTROLLER_H
