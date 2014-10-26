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

void Teste(Treina &busca, int tamanho);
void Treino(Treina &busca, int tamanho);
Mat& GeraMatComTamanhoEspecifico(Mat frame, int tamanho);
bool PodeAddImage(Mat frame, int tamanho);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int escolha = 2;

    cout << "Começando treino geral" << endl;
    for(int tamanho = 8;tamanho <=128;tamanho=tamanho*2)
    {
        Treina t(tamanho);

        if(escolha == 1)
            Treino(t,tamanho);
        else if(escolha == 2)
            Teste(t,tamanho);
    }

    cout << "Fim do treino geral" << endl;

    return a.exec();
}


void Teste(Treina &busca, int tamanho)
{
    Mat IMG(Size(tamanho,tamanho),CV_32FC1, Scalar::all(0));
    clock_t tempoInicial3 = clock();

    QStringList filesQ;

    cout << "Inicio do Localiza -> " << busca.currentDateTime() << endl;
    cout << "\nLocalizando Faces .... \n" << endl;

    busca.carregaTeste(filesQ);

    if(!busca.loadBoost())
        return;
    busca.faces.clear();   // Limpa variaveis

    for (int j = 0; j < filesQ.length(); j++)
    {

      string img = filesQ[j].toUtf8().constData();
      Mat imagemQ = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
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

void Treino(Treina &busca, int tamanho)
{
    Mat IMG(Size(tamanho,tamanho),CV_32FC1, Scalar::all(0));

    QStringList filesAP, filesAN;

    busca.carregaPessoa(filesAP);
    busca.carregaNaoPessoa(filesAN);

    clock_t tempoInicial = clock();

    cout << "Extraindo Features de Amostras Positivas ." << endl;

    // Extrai caracteristicas Faces
    for (int i = 0; i < filesAP.length();i++)
    {
        string img=filesAP[i].toUtf8().constData();
        IMG = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
        if(i == 199)
            PodeAddImage(IMG,tamanho);
        if(!PodeAddImage(IMG,tamanho))
            continue;
        Mat mascara = GeraMatComTamanhoEspecifico(IMG,tamanho);
        busca.IMAGENSPOSITIVAS++;  busca.AMOSTRA++;
        busca.extraiCaracteristicas( mascara );
    }

    if(busca.IMAGENSPOSITIVAS == 0)
        return;

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
}



Mat& GeraMatComTamanhoEspecifico(Mat frame, int tamanho)
{
    Mat retorno = Mat::zeros(tamanho,tamanho,CV_32FC1);

    int coluna = (tamanho/2) - (frame.cols /  2), linhas = (tamanho/2) - (frame.rows / 2);

    for(int i = 0;i < frame.cols;i++)
    {
        int linhas2 = linhas;
        for(int j =0;j < frame.rows;j++)
        {
            frame.col(i).row(j).copyTo(retorno.col(coluna).row(linhas2));
            linhas2++;
        }
        coluna++;
    }
    return retorno;
}

bool PodeAddImage(Mat frame, int tamanho)
{
    bool retorno = false;
    if(frame.empty())
        retorno = false;
    else if(tamanho <= 8)
        retorno = (frame.cols <= tamanho && frame.rows <= tamanho);
    else
        retorno = ((frame.cols >= tamanho/2 && frame.cols <= tamanho) && (frame.rows >= tamanho/2 && frame.rows <= tamanho));
    return retorno;
}
