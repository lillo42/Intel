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

    void addProcessa(Imagem frame);

    void run();

signals:
    void onTerminouWavelet(Imagem frame);
public slots:

private:
    QList<Imagem> listaProcessa;

    Wavelet *w;

    QWaitCondition sincronizaThread;
    QWaitCondition bufferIsNotEmpty;

    void processa();
    void executaWavelet();

};

#endif // WAVELETCONTROLLER_H
