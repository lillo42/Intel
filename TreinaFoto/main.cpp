// Importação de Bibliotecas
#include <QFileInfoList>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

#include <QDebug>
#include <QTime>
#include <QRegExp>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>  /* clock_t, clock, CLOCKS_PER_SEC */
#include "busca.cpp"

using namespace cv;
using namespace std;


#define WIDTH  25  // cols
#define HEIGHT 30  // rows

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList filesAP, filesAN, filesQ, filesQP, filesQN;
    QRegExp rxAP, rxAN, rxQ, rxQP, rxQN;

    Mat IMG(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Busca busca;

    int TT = 3;

    rxAP.setPattern("ap(\\d+)[ab].jpg");
    rxAN.setPattern("imagem[1-228].jpg");
    busca.filterWildCard(filesAP, rxAP);
    busca.filterWildCard(filesAN, rxAN);

    clock_t tempoInicial = clock();

    cout << "Extraindo Features de Amostras Positivas ." << endl;

    // Extrai caracteristicas Faces
    for (int i = 0; i < filesAP.length();i++)
    {
        string img=filesAP[i].toUtf8().constData();
        IMG = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
        busca.IMAGENSPOSITIVAS++;  busca.AMOSTRA++;
        busca.extraiCaracteristicas( IMG );
    }

    cout  << "Extraindo Features de Amostras Negativas ";

    // Extrai caracteristicas Nao Faces
    for (int i = 0; i < filesAN.length() ;i++)
    {
        string img=filesAN[i].toUtf8().constData();
        IMG = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
        busca.AMOSTRA++;
        busca.extraiCaracteristicas( IMG );
        cout << ".";
    }

    cout << "\nFaces Positivas -> " << busca.IMAGENSPOSITIVAS  << endl;
    cout << "Faces Negativas -> " << busca.Features.size() - busca.IMAGENSPOSITIVAS  << endl;
    cout << "Total de Amostras -> " << busca.Features.size()  << endl;
    cout << "Features por Amostras -> " << busca.Features[1].size()  << endl;
    cout << "Total de Features [ (Faces e NAO Faces) x Amostras ] -> " <<  busca.Features[1].size() * busca.Features.size() << endl;

    //get current time

    clock_t tempoInicial1 = clock();

    cout << "\nInicio do Treino -> " << busca.currentDateTime() << endl;

    //Realiza Treino
    busca.treino();

    clock_t tempoFinal = clock();

    cout << "Tempo Extracao de Caracteristicas -> " << ((tempoInicial1 - tempoInicial) / CLOCKS_PER_SEC ) << " segundos" << endl;
    cout << "Tempo Treino -> " << ((tempoFinal - tempoInicial1) / CLOCKS_PER_SEC ) << " segundos" << endl;
    cout << "Tempo Total -> " << ((tempoFinal - tempoInicial) / CLOCKS_PER_SEC )  << " segundos" << endl;
    cout << "Fim do Treino -> " << busca.currentDateTime() << endl;
    cout << "Treino Finalizado ! " << endl;


    return a.exec();
}
