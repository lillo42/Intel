#include "pixelcontroller.h"

PixelController::PixelController(QObject *parent) :
    QObject(parent)
{
    pixel = new Pixel();
    this->setAutoDelete(true);
    finishedThread = false;
}

PixelController::~PixelController()
{
    if(pixel)
        delete pixel;
}

void PixelController::run()
{

}

void PixelController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    if(acorda)
    {
        listaProcessa.append(frame);
        sincronizedThread.wakeOne();
    }
}

void PixelController::stopThread()
{
    finishedThread = true;
    sincronizedThread.wakeOne();
}

void PixelController::processImage()
{
    QMutex lock;
    while(!finishedThread)
    {
        executeFilter();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizedThread.wait(&lock);
        lock.unlock();
    }
}

void PixelController::executeFilter()
{
    Imagem image;
    vector<Point> point;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        pixel->detected(image.frame,point);
        emit onFinishedPixel(image,point);
        listaProcessa.removeFirst();

    }
}

void PixelController::removeRepeated(vector<Point> &points)
{
    unsigned i = 0;
    Point pAtual;
    Point pProximo;
    while (i+1 < points.size() )
    {

        pAtual = points[i];
        pProximo = points[i+1];
        if ((pProximo.x - pAtual.x <= WIDTH_ROI) &&
                (pProximo.y - pAtual.y <= HEIGHT_ROI))
        {

//            if (faces[i+1].predicao > faces[i].predicao)
//                faces.erase(faces.begin()+i);
//            else
//                faces.erase(faces.begin()+1+i);
            //IMAGENSNEGATIVAS++;
            //IMAGENSPOSITIVAS--;
        }
        else
            i++;
    }
}
