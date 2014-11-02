                                                      #ifndef TREINA_H
#define TREINA_H

// Opencv
#include <opencv2/opencv.hpp>
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

#define WIDTH_PADRAO  64//32 // cols
#define HEIGHT_PADRAO 128//36 //rows

#define WIDTH_ROI  64  // cols
#define HEIGHT_ROI 128  // rows

#define ANDA_WIDTH 32
#define ANDA_HEIGHT 64

#define RAIO 3
#define VIZINHAZA 24

//#define WIDTH_DEFAULT 1280//cols
//#define HEIGHT_DEFAULT 720 //rows


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

    void extraiCaracteristicasLBP(Mat &Query);
    void extraiCaracteristicasWavalet(Mat &Query);
    void extraiCaracteristicasWavaletLBP(Mat &Query);
    void extraiCaracteristicasPixel(Mat &Query);
    void extraiCaracteristicasHOG(Mat &Query);


    void extraiInformacaoLBP(Mat &src, Mat &LBP);

    void testeLPB( Mat& Query );
    void testeWavalet( Mat& Query );
    void testeWavaletLBP( Mat& Query );
    void testePixel( Mat& Query );
    void testeHOG( Mat& Query );

    void extraiInformacaoTeste(Mat &src,DetectFace &df,Point &roi);

    void padraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca);
    void wavaletToVecto(Mat &wavalet, vector<float> &retorno, int NIter);

    const string currentDateTime();
    void treino();
    void eliminaRepetidos();
    void desenhaRetangulo(Mat &imagem);
    void desenhaHog(Mat &frame);

    void loadBoost();

    vector<Rect> found, found_filtered;

private:
    QString dirImagens, dir;
    QDir dirNegativas;
    QDir dirTreinoCascade;

    QDir dirPessoa;
    QDir dirNaoPessoa;
    QDir dirTreino;

    CvBoost boost;

    HOGDescriptor hogDefault;

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

    void cvInvHaarWavelet(Mat &src,Mat &dst,int NIter, int SHRINKAGE_TYPE=0, float SHRINKAGE_T=50);

    void cvHaarWavelet(Mat &src,Mat &dst,int NIter);

    void cvHaarWavelet(Mat &src,vector<float> &dst,int NIter);

    void cvHaarWavelet(Mat &src,Mat &dst,int NIter, int escolha);

    void cvHaarWavelet(Mat &src, Mat &mc, Mat &mdd, Mat &mdv, Mat &mdh, int NIter);
};

#endif // TREINA_H
