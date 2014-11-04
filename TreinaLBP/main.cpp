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

#define LBP 1
#define WAVELET 2
#define LBP_WAVELET 3
#define PIXEL 4
#define HOG 5

using namespace cv;
using namespace std;

void Teste(Treina &busca, int tipoTeste);
void Treino(Treina &busca, int tipoTreino);
void TipoTreino(Treina &busca,Mat &frame, int tipoTreino);
void TipoTeste(Treina &busca,Mat &frame, int tipoTeste);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int tipo = PIXEL;
    for(int escolha = 1;escolha <=2;escolha++)
    {
        Treina t;
        if(escolha == 1)
            Treino(t,tipo);
        else if(escolha == 2)
            Teste(t,tipo);
    }
    return a.exec();
}


void Teste(Treina &busca,int tipoTeste)
{
    //Mat IMG(Size(WIDTH_PADRAO,HEIGHT),CV_32FC1, Scalar::all(0));
    clock_t tempoInicial3 = clock();

    QStringList filesQ;

    cout << "\nTipo Teste ...." << tipoTeste << endl;
    cout << "Inicio do Localiza -> " << busca.currentDateTime() << endl;
    cout << "\nLocalizando Pessoas .... \n" << endl;

    busca.carregaTeste(filesQ);

    busca.loadBoost();
    busca.faces.clear();   // Limpa variaveis.jpg
    //Size s(WIDTH_PADRAO_DEFAULT,HEIGHT_DEFAULT);
    for (int j = 0; j < filesQ.length(); j++)
    {

      string img = filesQ[j].toUtf8().constData();
      Mat imagemQ = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
      if(imagemQ.empty())
          continue;
      busca.faces.clear();

      busca.AMOSTRA++;
      busca.IMAGENSNEGATIVAS=0;
      busca.IMAGENSPOSITIVAS=0;

      TipoTeste(busca,imagemQ,tipoTeste);
      busca.eliminaRepetidos();

      Mat imagemS = imread( img);
      if(tipoTeste == HOG)
          busca.desenhaHog(imagemS);
      else
      busca.desenhaRetangulo(imagemS);

      QString nome = QString("./ImagensProcessada/p%1_").arg(j+1);
      QString result2 = nome + QString::number(tipoTeste) + ".jpg";
      imwrite(result2.toStdString(),imagemS);
      cout << img << "\t pos = " << busca.IMAGENSPOSITIVAS << "\t neg = " << busca.IMAGENSNEGATIVAS << endl;

    }

    clock_t tempoFinal3 = clock();

    cout << "\nTempo Total -> " << ((tempoFinal3 - tempoInicial3) / CLOCKS_PER_SEC ) << " segundos" << endl;
    cout << "\nFim do Localiza -> " << busca.currentDateTime() << endl;
    cout << endl;
}

void Treino(Treina &busca, int tipoTreino)
{
    Mat IMG(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));

    QStringList filesAP, filesAN;

    busca.carregaPessoa(filesAP);
    busca.carregaNaoPessoa(filesAN);

    clock_t tempoInicial = clock();

    cout << "\nTipo Treino ...." << tipoTreino << endl;

    cout << "Extraindo Features de Amostras Positivas ." << endl;
    Size size(WIDTH_PADRAO,HEIGHT_PADRAO);
    // Extrai caracteristicas
    for (int i = 0; i < filesAP.length();i++)
    {
        string img=filesAP[i].toUtf8().constData();
        IMG = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
        if(IMG.empty())
            continue;
        //VerificaImagemTreino(IMG,IMG);
        resize(IMG,IMG,size);
        busca.IMAGENSPOSITIVAS++;  busca.AMOSTRA++;
        TipoTreino(busca,IMG,tipoTreino);
    }

    cout  << "Extraindo Features de Amostras Negativas ";

    // Extrai caracteristicas Nao Pessoa
    //Size s(WIDTH_PADRAO_DEFAULT,HEIGHT_DEFAULT);
    for (int i = 0; i < filesAN.length() ;i++)
    {
        string img=filesAN[i].toUtf8().constData();
        IMG = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
        if(IMG.empty())
            continue;
        busca.AMOSTRA++;
        TipoTreino(busca,IMG,tipoTreino);
        cout << ".";
    }

    cout << "\nFaces Positivas -> " << busca.IMAGENSPOSITIVAS  << endl;
    cout << "Faces Negativas -> " << busca.Features.size() - busca.IMAGENSPOSITIVAS  << endl;
    cout << "Total de Amostras -> " << busca.Features.size()  << endl;
    if(busca.Features.size() > 1)
    {
        cout << "Features por Amostras -> " << busca.Features[1].size()  << endl;
        cout << "Total de Features [ (Faces e NAO Faces) x Amostras ] -> " <<  busca.Features[1].size() * busca.Features.size() << endl;
    }

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

void TipoTreino(Treina &busca,Mat &frame, int tipoTreino)
{
    if(tipoTreino == 1)
        busca.extraiCaracteristicasLBP(frame);
    else if(tipoTreino == 2)
        busca.extraiCaracteristicasWavalet(frame);
    else if(tipoTreino == 3)
        busca.extraiCaracteristicasWavaletLBP(frame);
    else if(tipoTreino == 4)
        busca.extraiCaracteristicasPixel(frame);
    else if(tipoTreino == 5)
        busca.extraiCaracteristicasHOG(frame);
}

void TipoTeste(Treina &busca,Mat &frame, int tipoTeste)
{
    if(tipoTeste == 1)
        busca.testeLPB(frame);
    else if(tipoTeste == 2)
        busca.testeWavalet(frame);
    else if(tipoTeste == 3)
        busca.testeWavaletLBP(frame);
    else if(tipoTeste == 4)
        busca.testePixel(frame);
    else if(tipoTeste == 5)
        busca.testeHOG(frame);
}
