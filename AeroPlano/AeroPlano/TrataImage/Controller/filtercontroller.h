#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../Operacao/filter.h"
#include "../../ImagemController/imagem.h"

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;

class FilterController : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit FilterController(QObject *parent = 0);

    void addProcessa(Imagem frame);
    void stopThread();

    void run();

signals:
    void onTerminouSalEPimenta(Imagem frame);
public slots:

private:
    QList<Imagem> listaProcessa;
    Filter *filter;

    QWaitCondition sincronizedThread;
    bool finishedThread;

    void processImage();
    void executeFilter();
};

#endif // FILTERCONTROLLER_H
