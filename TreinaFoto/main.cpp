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


#define WIDTH  128  // cols
#define HEIGHT 128  // rows


void Teste();
void Treino();

QStringList filesAP, filesAN,filesQ;
QRegExp rxAP, rxAN,rxQ;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int TT = 3;

    Treino();


    return a.exec();
}

void Teste()
{
    Mat IMG(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Busca busca;
    Busca busca3;



      clock_t tempoInicial3 = clock();

      cout << "Inicio do Localiza -> " << busca.currentDateTime() << endl;
      cout << "\nLocalizando Faces .... \n" << endl;

      rxQ.setPattern("q[1-5].jpg");
      busca3.carregaTeste(filesQ);

      busca3.loadBoost();
      busca3.faces.clear();   // Limpa variaveis

      for (int j = 0; j < filesQ.length(); j++)
      {

          string img = filesQ[j].toUtf8().constData();
          Mat imagemQ = imread( img, CV_LOAD_IMAGE_GRAYSCALE);
          Mat imagemS = imread( img);

          busca3.faces.clear();

          busca3.AMOSTRA++;busca3.IMAGENSNEGATIVAS=0;
          busca3.IMAGENSPOSITIVAS=0;

          busca3.teste( imagemQ );
          busca3.eliminaRepetidos();

          busca3.desenhaRetangulo(imagemS);

          QString nome = QString("p%1.jpg").arg(j+1);
          string result2 = nome.toUtf8().constData();
          imwrite(result2,imagemS);
          cout << img << "\t pos = " << busca3.IMAGENSPOSITIVAS << "\t neg = " << busca3.IMAGENSNEGATIVAS << endl;

      }

      clock_t tempoFinal3 = clock();

      cout << "\nTempo Total -> " << ((tempoFinal3 - tempoInicial3) / CLOCKS_PER_SEC ) << " segundos" << endl;
      cout << "\nFim do Localiza -> " << busca.currentDateTime() << endl;        cout << endl;
}

void Treino()
{
    Mat IMG(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Busca busca;
    //rxAP.setPattern("ap(\\d+)[ab].jpg");
    //rxAN.setPattern("imagem[32-228].jpg");
    busca.carregaPessoas(filesAP);
    busca.carregaNaoPessoa(filesAN);

    clock_t tempoInicial = clock();

    cout << "Extraindo Features de Amostras Positivas ." << endl;

    // Extrai caracteristicas Faces
    for (int i = 0; i < filesAP.length();i++)
    {
        string img=filesAP[i].toUtf8().constData();
        IMG = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
        if(IMG.cols > 128 || IMG.rows > 128)
            continue;
        Mat mascara = Mat::zeros(128,128,0);
        int coluna = 64 - (IMG.cols /  2), linhas = 64 - (IMG.rows / 2);
        for(int i = 0;i < IMG.cols;i++)
        {
            int linhas2 = linhas;
            for(int j =0;j < IMG.rows;j++)
            {
                IMG.col(i).row(j).copyTo(mascara.col(coluna).row(linhas2));
                linhas2++;
            }
            coluna++;
        }

        busca.IMAGENSPOSITIVAS++;  busca.AMOSTRA++;
        busca.extraiCaracteristicas( mascara );
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
}
