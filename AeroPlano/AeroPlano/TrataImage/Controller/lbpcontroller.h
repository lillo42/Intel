#ifndef LBPCONTROLLER_H
#define LBPCONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "../Operacao/lbp.h"
#include "../../ImagemController/imagem.h"

#include <opencv/cv.h>
using namespace cv;

class LBPController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit LBPController(QObject *parent = 0);

    void addProcesa(Imagem frame);
    void stopThread();

    void run();

signals:
    void onTerminouLBP(Imagem frame,int quantidades);

public slots:
    void onCalculatesLBP(float values);
private:
    QList<Imagem> listaProcessa;
    LBP *lbp;

    QWaitCondition sincronizaThread;
    bool finishedThread;

    void processa();
    void executaLBP();

};

#endif // LBPCONTROLLER_H
