#include "trataimagecontroller.h"

TrataImageController::TrataImageController(QObject *parent) :
    QObject(parent)
{
//    criaLBP();
//    criaSalEPimenta();
//    criaWavelet();
//    criaPixel();
    criaHOG();
    this->setAutoDelete(true);
}

TrataImageController::~TrataImageController()
{
    this->stopThread();
    if(filter)
    {
        filter->stopThread();
    }
    if(lbp)
        lbp->stopThread();
    if(wavelet)
        wavelet->stopThread();
    if(pixel)
        pixel->stopThread();
    if(hog)
        hog->stopThread();

    QThreadPool::globalInstance()->waitForDone(100);

    if(filter)
    {
        disconnect(filter,SIGNAL(onTerminouSalEPimenta(Imagem)));
        delete filter;
    }
    if(lbp)
    {
        disconnect(lbp,SIGNAL(onTerminouLBP(Imagem,int)));
        delete lbp;
    }
    if(wavelet)
    {
        disconnect(wavelet,SIGNAL(onTerminouWavelet(Imagem)));
        delete wavelet;
    }
    if(pixel)
    {
        disconnect(pixel,SIGNAL(onFinishedPixel(Imagem,vector<Point>&)));
        delete pixel;
    }
    if(hog)
    {
        disconnect(hog,SIGNAL(onFinishedHOG(Imagem,vector<Rect>)));
        delete hog;
    }
}

void TrataImageController::addImage(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    //frame.gpuFrame = convertMatToGpuMat(frame.frame);
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}


void TrataImageController::stopThread()
{
    finishedThread = true;
    sincronizaThread.wakeOne();
}

void TrataImageController::run()
{
    addProcessoThreadPool();
    processa();
}

void TrataImageController::OnTerminouLBP(Imagem frame,int quantidade)
{
    frame.count = quantidade;
    emit onTerminouContagem(frame);
}

void TrataImageController::OnTerminouSalEPimenta(Imagem salPimenta)
{
    wavelet->addProcessa(salPimenta);
}

void TrataImageController::OnTerminouWavelet(Imagem frame)
{
}

void TrataImageController::OnTerminouHOG(Imagem frame, vector<Rect> found)
{
    frame.frame = frame.frameNotProcess;
    drawImage(frame.frame,found);
    int size = found.size();
    emit onTerminouContagemHOG(frame,size);
}

void TrataImageController::OnTerminouPixel(Imagem frame, vector<Point> &point)
{
    frame.frame = frame.frameNotProcess;
    drawImage(frame.frame,point);
    int size = point.size();
    emit onTerminouContagemPixel(frame,size);
}

void TrataImageController::criaLBP()
{
    lbp = new LBPController(this);
    connect(lbp,SIGNAL(onTerminouLBP(Imagem,int)),this,SLOT(OnTerminouLBP(Imagem,int)));
}

void TrataImageController::criaSalEPimenta()
{
    filter = new FilterController();
    connect(filter,SIGNAL(onTerminouSalEPimenta(Imagem)),this,SLOT(OnTerminouSalEPimenta(Imagem)));
}

void TrataImageController::criaWavelet()
{
    wavelet = new WaveletController();
    connect(wavelet,SIGNAL(onTerminouWavelet(Imagem)),this,SLOT(OnTerminouWavelet(Imagem)));
}

void TrataImageController::criaPixel()
{
    pixel = new PixelController(this);
    connect(pixel,SIGNAL(onFinishedPixel(Imagem,vector<Point>&)),this,SLOT(OnTerminouPixel(Imagem,vector<Point>&)));
}

void TrataImageController::criaHOG()
{
    hog = new HOGController(this);
    connect(hog,SIGNAL(onFinishedHOG(Imagem,vector<Rect>)),this,SLOT(OnTerminouHOG(Imagem,vector<Rect>)));
}

void TrataImageController::processa()
{
    QMutex lock;
    while(!finishedThread)
    {
        comecaProcessar();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizaThread.wait(&lock);
        lock.unlock();
    }
}

void TrataImageController::comecaProcessar()
{
    Imagem frame;
    while(!listaProcessa.empty())
    {
        frame = listaProcessa.first();
        hog->addProcessa(frame);
        //pixel->addProcessa(frame);
        listaProcessa.removeFirst();
    }
}

void TrataImageController::addProcessoThreadPool()
{
//    QThreadPool::globalInstance()->start(wavelet);
//    QThreadPool::globalInstance()->start(filter);
//    QThreadPool::globalInstance()->start(lbp);
//    QThreadPool::globalInstance()->start(pixel);
      QThreadPool::globalInstance()->start(hog);

}

void TrataImageController::drawImage(Mat &frame, vector<Point> &points)
{
    Point center;
    int radius, scale= 0.264583333; // Converte para mm

    foreach (Point p, points)
    {
        rectangle(frame,(Rect(p.x,p.y,WIDTH_PADRAO+1,HEIGHT_PADRAO+1)),CV_RGB(255, 255, 0), 1.5);
        // comparar distancia entre os rec, e verficar a media.... para eliminar falsos positivos.
        center.x = p.x+WIDTH_PADRAO/2;
        center.y = p.y+HEIGHT_PADRAO/2;
        radius = cvRound((WIDTH_PADRAO + HEIGHT_PADRAO)*0.25*scale);
        circle( frame, center, radius,  CV_RGB(0,255,0), 2);
    }
}

void TrataImageController::drawImage(Mat &frame, vector<Rect> &points)
{
    foreach (Rect r, points)
    {
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}
