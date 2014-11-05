#ifndef WAVELETCONTROLLER_H
#define WAVELETCONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../Operacao/wavelet.h"
#include "../../ImagemController/imagem.h"

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;

class WaveletController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit WaveletController(QObject *parent = 0);
    ~WaveletController();

    void addProcessa(Imagem frame);
    void stopThread();

    void run();

signals:
    void onTerminouWavelet(Imagem frame);
    void onCalculatesPoint(float values);
public slots:

private:
    QList<Imagem> listaProcessa;

    Wavelet *w;

    QWaitCondition sincronizedThread;
    bool finishedThread;

    void processImage();
    void executeWavelet();

};

#endif // WAVELETCONTROLLER_H
