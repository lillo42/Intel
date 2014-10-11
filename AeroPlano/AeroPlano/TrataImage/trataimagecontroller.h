#ifndef TRATAIMAGECONTROLLER_H
#define TRATAIMAGECONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
#include <QThreadPool>

#include "Controller/lbpcontroller.h"
#include "Controller/salepimentacontroller.h"
#include "Controller/waveletcontroller.h"
#include "../ImagemController/imagem.h"

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv/cv.h>

using namespace cv;
class TrataImageController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit TrataImageController(QObject *parent = 0);

    void addImage(Imagem frame);

    void run();

signals:
    void onTerminouContagem(Imagem frame,int quantidade);

public slots:

private slots:
    void OnTerminouLBP(Imagem frame,int quantidade);
    void OnTerminouSalEPimenta(Imagem salPimenta);
    void OnTerminouWavelet(Imagem frame);

private:
    QList<Imagem> listaProcessa;
    QWaitCondition sincronizaThread;

    LBPController *lbp;
    SalEPimentaController *sp;
    WaveletController *wavelet;

    void criaLBP();
    void criaSalEPimenta();
    void criaWavelet();

    void processa();
    void comecaProcessar();


    void addProcessoThreadPool();

    gpu::GpuMat convertMatToGpuMat(Mat frame);
    Mat convertGpuMaToMat(gpu::GpuMat frame);
};

#endif // TRATAIMAGECONTROLLER_H
