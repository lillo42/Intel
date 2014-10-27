#ifndef TREINA_H
#define TREINA_H

// Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/ml/ml.hpp>


// STD
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <fstream>

#include <QDir>
#include <QString>
#include <QStringList>
#include <QFile>

#define NONE 0  // no filter
#define HARD 1  // hard shrinkage
#define SOFT 2  // soft shrinkage
#define GARROT 3  // garrot filter

#define WIDTH  32  // cols
#define HEIGHT 36  // rows


using namespace cv;
using namespace std;

struct detF {
    float predicao;     // Armazena a predicao, para comparare elimiar falsos positivos por distância
    Point ponto;        // Localizacao a Face
} typedef DetectFace;

class Treina
{
public:
    Treina();

    // Funções std
    vector<vector <float> > Features;
    vector<DetectFace> faces;

    float PREDICAO;
    unsigned int IMAGENSPOSITIVAS, IMAGENSNEGATIVAS, AMOSTRA;

    void carregaPessoa( QStringList &Lista);
    void carregaNaoPessoa(QStringList &Lista);
    void carregaTeste(QStringList &Lista);

    void extraiCaracteristicas(Mat &Query);

    void padraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca);

    const string currentDateTime();
    void treino();
    void teste( Mat& Query );
    void eliminaRepetidos();
    void desenhaRetangulo(Mat &imagem);

    void loadBoost();

private:
    QString dirImagens, dir;
    QDir dirNegativas;
    QDir dirTreinoCascade;

    QDir dirPessoa;
    QDir dirNaoPessoa;
    QDir dirTreino;

    CvBoost boost;


    string arqCriterio;

    void carregaList(QDir dir, QStringList &Lista);


    // signum
    float sgn(float x);

    // Soft shrinkage
    float soft_shrink(float d,float T);

    // Hard shrinkage
    float hard_shrink(float d,float T);

    // Garrot shrinkage
    float garrot_shrink(float d,float T);

    void cvHaarWavelet(Mat &src,Mat &dst,int NIter, int escolha);
};

#endif // TREINA_H
