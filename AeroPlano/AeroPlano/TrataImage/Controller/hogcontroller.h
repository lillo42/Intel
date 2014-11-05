#ifndef HOGCONTROLLER_H
#define HOGCONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../../ImagemController/imagem.h"
#include "../Operacao/hog.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

class HOGController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit HOGController(QObject *parent = 0);
    ~HOGController();

    void run();
    void addProcessa(Imagem frame);
    void stopThread();

signals:
    void onFinishedHOG(Imagem frame, vector<Rect> found);
public slots:

private:
    QList<Imagem> listaProcessa;
    HOG *hog;

    QWaitCondition sincronizedThread;
    bool finishedThread;

    void processImage();
    void executeFilter();

};

#endif // HOGCONTROLLER_H
