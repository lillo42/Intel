#include <QCoreApplication>
#include <QFileInfoList>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include <QDebug>
#include <QTime>
#include <QRegExp>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>  /* clock_t, clock, CLOCKS_PER_SEC */

#include "treina.h"

using namespace cv;
using namespace std;

void Teste(Treina &busca);
void Treino(Treina &busca);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int escolha = 1;

    Treina t;
    if(escolha == 1)
        Treino(t);
    else if(escolha == 2)
        Teste(t);

    return a.exec();
}


void Teste(Treina &busca)
{
    Mat IMG(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    clock_t tempoInicial3 = clock();

    QStringList filesQ;

    cout << "Inicio do Localiza -> " << busca.currentDateTime() << endl;
    cout << "\nLocalizando Pessoas .... \n" << endl;

    busca.carregaTeste(filesQ);

    busca.loadBoost();
    busca.faces.clear();   // Limpa variaveis

    for (int j = 0; j < filesQ.length(); j++)
    {

      string img = filesQ[j].toUtf8().constData();
      Mat imagemQ = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
      if(imagemQ.empty())
          continue;
      Mat imagemS = imread( img);

      busca.faces.clear();

      busca.AMOSTRA++;
      busca.IMAGENSNEGATIVAS=0;
      busca.IMAGENSPOSITIVAS=0;

      busca.teste( imagemQ );
      busca.eliminaRepetidos();

      busca.desenhaRetangulo(imagemS);

      QString nome = QString("p%1.jpg").arg(j+1);
      string result2 = nome.toUtf8().constData();
      imwrite(result2,imagemS);
      cout << img << "\t pos = " << busca.IMAGENSPOSITIVAS << "\t neg = " << busca.IMAGENSNEGATIVAS << endl;

    }

    clock_t tempoFinal3 = clock();

    cout << "\nTempo Total -> " << ((tempoFinal3 - tempoInicial3) / CLOCKS_PER_SEC ) << " segundos" << endl;
    cout << "\nFim do Localiza -> " << busca.currentDateTime() << endl;
    cout << endl;
}

void Treino(Treina &busca)
{
    Mat IMG(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));

    QStringList filesAP, filesAN;

    busca.carregaPessoa(filesAP);
    busca.carregaNaoPessoa(filesAN);

    clock_t tempoInicial = clock();

    cout << "Extraindo Features de Amostras Positivas ." << endl;

    // Extrai caracteristicas
    for (int i = 0; i < filesAP.length();i++)
    {
        string img=filesAP[i].toUtf8().constData();
        IMG = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
        if(IMG.empty())
            continue;
        Size size(32,36);
        Mat retorno = Mat::zeros(size,CV_32FC1);
        resize(IMG,retorno,size);
        busca.IMAGENSPOSITIVAS++;  busca.AMOSTRA++;
        busca.extraiCaracteristicas( retorno);
    }

    cout  << "Extraindo Features de Amostras Negativas ";

    // Extrai caracteristicas Nao Pessoa
    for (int i = 0; i < filesAN.length() ;i++)
    {
        string img=filesAN[i].toUtf8().constData();
        IMG = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
        if(IMG.empty())
            continue;
        busca.AMOSTRA++;
        busca.extraiCaracteristicas( IMG );
        //cout << ".";
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
}
