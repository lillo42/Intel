#ifndef TRATAIMAGECONTROLLER_H
#define TRATAIMAGECONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
#include <QThreadPool>

#include "Controller/lbpcontroller.h"
#include "Controller/filtercontroller.h"
#include "Controller/waveletcontroller.h"
#include "../ImagemController/imagem.h"
#include "Controller/hogcontroller.h"
#include "Controller/pixelcontroller.h"

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
    ~TrataImageController();
    void addImage(Imagem frame);
    void stopThread();

    void run();

signals:
    void onTerminouContagem(Imagem frame);

public slots:

private slots:
    void OnTerminouLBP(Imagem frame,int quantidade);
    void OnTerminouSalEPimenta(Imagem salPimenta);
    void OnTerminouWavelet(Imagem frame);
    void OnTerminouHOG(Imagem frame, vector<Rect> found);
    void OnTerminou(Imagem frame, vector<Point> &point);

private:
    QList<Imagem> listaProcessa;
    QWaitCondition sincronizaThread;
    bool finishedThread;

    LBPController *lbp;
    FilterController *filter;
    WaveletController *wavelet;
    HOGController *hog;
    PixelController *pixel;



    void criaLBP();
    void criaSalEPimenta();
    void criaWavelet();
    void criaPixel();
    void criaHOG();

    void processa();
    void comecaProcessar();

    void addProcessoThreadPool();
};

#endif // TRATAIMAGECONTROLLER_H
