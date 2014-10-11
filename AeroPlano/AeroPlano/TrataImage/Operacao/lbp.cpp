#include "lbp.h"

LBP::LBP()
{
}


int LBP::Detecta(Mat frame)
{
    return 0;
}

void LBP::eliminaRepetidos()
{

}

void LBP::Procura(Mat &Query)
{
    Mat ROI(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi;  // Armazena as coordenadas das Features
    int raio=1; int vizinhaca=8;
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;

    // convoluçao para gerar uma imagem de 320 x 240 px em NAO faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT ; i++)
    {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH ; j++)
        {
            roi.x = j;
            Query.operator ()(Rect(roi.x,roi.y,WIDTH,HEIGHT)).convertTo(ROI,CV_32FC1,1,0);

            PadraoLocal(ROI,LBP,raio,vizinhaca);

            Mat temp;
            MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

            for(; it != it_end; ++it) temp.push_back(*it);

            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);
            //Bboost.predict( const float* row_sample, int row_len, bool returnDFVal=false ) const;
            //svm.predict()
            QString nome = QString("ROI00%1-%2.jpg").arg(i).arg(j);
            string result2 = nome.toUtf8().constData();

            if ( PREDICAO > 12 )
            {
                df.predicao = PREDICAO; df.ponto = roi;
                faces.push_back( df );
                IMAGENSPOSITIVAS++;
            }
            else
                IMAGENSNEGATIVAS++;
        }
    }
}

void LBP::loadBoost()
{
    boost.load("");
}

void LBP::PadraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca)
{
    // Copyright (c) 2011, philipp <bytefish[at]gmx.de>
    Vizinhaca = max(min(Vizinhaca,31),1);
    Destino = Mat::zeros(Origem.rows-2*Raio, Origem.cols-2*Raio, CV_32FC1);

    for (int n=0; n<Vizinhaca; n++)
    {
        // Pontos de amostragem
        float x = static_cast<float>(Raio) * cos(2.0*M_PI*n/static_cast<float>(Vizinhaca));
        float y = static_cast<float>(Raio) * -sin(2.0*M_PI*n/static_cast<float>(Vizinhaca));

        // Indices relativos
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));

        // Normalizacao
        float ty = y - fy;
        float tx = x - fx;

        // Interpolacao dos pesos
        float w1 = (1 - tx) * (1 - ty);
        float w2 =      tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 =      tx  *      ty;

        // Iterecao
        for (int i=Raio; i < Origem.rows-Raio;i++)
        {
            for (int j=Raio;j < Origem.cols-Raio;j++)
            {
                float t = w1*Origem.at<float>(i+fy,j+fx) + w2*Origem.at<float>(i+fy,j+cx) + w3*Origem.at<float>(i+cy,j+fx) + w4*Origem.at<float>(i+cy,j+cx);

                Destino.at<float>(i-Raio,j-Raio) += ((t > Origem.at<float>(i,j)) && (abs(t-Origem.at<float>(i,j)) > numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}
