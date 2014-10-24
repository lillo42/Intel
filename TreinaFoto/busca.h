#ifndef Busca_H
#define Busca_H

// Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

// STD
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>

#include <QDir>
#include <QString>
#include <QStringList>
#include <QRegExp>

using namespace cv;
using namespace std;

#ifdef __unix__                    /* Linux */
#define OS_Linux 1
   #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32) /* Windows 32 ou 64 bits*/
  #define OS_Linux 0
#endif

struct detF {
    float predicao;     // Armazena a predicao, para comparare elimiar falsos positivos por distância
    Point ponto;        // Localizacao a Face
} typedef DetectFace;


class Busca
{
public:

    // Funções Framework QT
    QString dirImagens, dir;
    QDir dirNegativas;
    QDir dirTreinoCascade;

    CvBoost boost;

    // Funções std
    vector<vector <float> > Features;
    vector<DetectFace> faces;

    string arqCriterio;

    float PREDICAO;
    unsigned int IMAGENSPOSITIVAS, IMAGENSNEGATIVAS, AMOSTRA;

    Busca();
    void eliminaRepetidos();
    void desenhaRetangulo(Mat &imagem);
    const string currentDateTime();
    void resizeMatriz( Mat &Origem, Mat &Destino );
    void filterWildCard( QStringList &Lista, QRegExp Padrao);
    void padraoLocal( Mat &Origem, Mat &Destino, int Raio, int Vizinhaca);
    void extraiCaracteristicas( Mat &Query );
    void treino();
    void teste( Mat& Query );
    void loadBoost();

};

#endif // Busca_H
