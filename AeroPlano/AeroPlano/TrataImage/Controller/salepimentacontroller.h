#ifndef SALEPIMENTACONTROLLER_H
#define SALEPIMENTACONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../Operacao/salepimenta.h"
#include "../../ImagemController/imagem.h"

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;

class SalEPimentaController : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit SalEPimentaController(QObject *parent = 0);

    void addProcessa(Imagem frame);

    void run();

signals:
    void onTerminouSalEPimenta(Imagem frame);
public slots:

private:
    QList<Imagem> listaProcessa;
    SalEPimenta *sp;

    QWaitCondition sincronizaThread;
    QWaitCondition bufferIsNotEmpty;

    void processa();
    void executaSalPimentaLista();
};

#endif // SALEPIMENTACONTROLLER_H
