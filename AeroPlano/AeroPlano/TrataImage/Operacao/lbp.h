#ifndef LBP_H
#define LBP_H

#include <QString>

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;


#define WIDTH  240  // cols
#define HEIGHT 320  // rows
struct detF {
    float predicao;     // Armazena a predica, para comparare elimiar falsos positivos por distância
    Point ponto;        // Localizacao a Face
} typedef DetectFace;


class LBP
{
public:
    LBP();
    int Detecta(Mat frame);
    int Detected(float values);

    float PREDICAO;
    unsigned int IMAGENSPOSITIVAS, IMAGENSNEGATIVAS, AMOSTRA;

private:
    CvBoost boost;
    SVM sv;
    vector<vector <float> > Features;
    vector<DetectFace> faces;

    void eliminaRepetidos();
    void Procura(Mat& Query);
    void loadBoost();
    void PadraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca);
};

#endif // LBP_H
