#include "busca.h"
#include <fstream>
#include <opencv2/ml/ml.hpp>
#include <QString>

#define WIDTH  25  // cols
#define HEIGHT 30  // rows


Busca::Busca()
{
    IMAGENSPOSITIVAS=0, IMAGENSNEGATIVAS=0, AMOSTRA=0;

    arqCriterio=("criterio.xml");

    if (OS_Linux){
        dir = QString("/home/rsj/ep2/");
        dirImagens.append(dir);
    }
    else {
        //        vector<string> vs;
        //        vsWildCard("c:\\ep2\\an?.jpg",vs);
        //        vsWildCard("c:\\ep2\\ap????.jpg",vs);
    }
}

void Busca::eliminaRepetidos()
{
    unsigned i = 0;

    while (i+1 < faces.size() ) {

        if ((faces[i+1].ponto.x - faces[i].ponto.x <= WIDTH) &&
                (faces[i+1].ponto.y - faces[i].ponto.y <= HEIGHT)){

            if (faces[i+1].predicao > faces[i].predicao)
                faces.erase(faces.begin()+i);
            else
                faces.erase(faces.begin()+1+i);
            IMAGENSNEGATIVAS++;
            IMAGENSPOSITIVAS--;
        }
        else
            i++;
    }

}

void Busca::desenhaRetangulo(Mat &imagem)
{
    Point center;
    int radius, scale= 0.264583333; // Converte para mm

    for(unsigned long i = 0; i < faces.size(); i++) {
        rectangle(imagem,(Rect(faces[i].ponto.x,
                                faces[i].ponto.y,WIDTH+1,HEIGHT+1)),
                  CV_RGB(255, 255, 0), 1.5);
        // comparar distancia entre os rec, e verficar a media.... para eliminar falsos positivos.

        center.x = faces[i].ponto.x+WIDTH/2;
        center.y = faces[i].ponto.y+HEIGHT/2;
        radius = cvRound((WIDTH + HEIGHT)*0.25*scale);
        circle( imagem, center, radius,  CV_RGB(0,255,0), 2);

    }
}

void Busca::loadBoost()
{
    boost.load("criterio.xml");
}

const string Busca::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "( %d-%m-%Y ) %X", &tstruct);

    return buf;
}

void Busca::filterWildCard(QStringList &files, QRegExp rx)
{
    QDir diretorio(dir);
    QFileInfoList fileList = diretorio.entryInfoList();
    foreach (const QFileInfo &info, fileList) {
        if (rx.exactMatch(info.fileName()))
            files.append(QString("%1/%2").arg(diretorio.path()).arg(info.fileName()));
    }
}

// Copyright (c) 2011, philipp <bytefish[at]gmx.de>
void Busca::padraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca)
{
    Vizinhaca = max(min(Vizinhaca,31),1);
    Destino = Mat::zeros(Origem.rows-2*Raio, Origem.cols-2*Raio, CV_32FC1);

    for (int n=0; n<Vizinhaca; n++) {
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
        for (int i=Raio; i < Origem.rows-Raio;i++) {
            for (int j=Raio;j < Origem.cols-Raio;j++) {
                float t = w1*Origem.at<float>(i+fy,j+fx) + w2*Origem.at<float>(i+fy,j+cx) + w3*Origem.at<float>(i+cy,j+fx) + w4*Origem.at<float>(i+cy,j+cx);

                Destino.at<float>(i-Raio,j-Raio) += ((t > Origem.at<float>(i,j)) && (abs(t-Origem.at<float>(i,j)) > numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}

void Busca::extraiCaracteristicas(Mat &Query)
{
    Mat ROI(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi; // Armazena as coordenadas das Features
    int raio=1; int vizinhaca=8;

    // convoluçao para converter uma imagem de 320 x 240 px em faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT ; i=i+6) {
        roi.y = i;

        for(int j = 0; j <= Query.cols - WIDTH ; j=j+6) {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH,HEIGHT)).convertTo(ROI,CV_32FC1,1,0);

            padraoLocal(ROI,LBP,raio,vizinhaca);

            vector<float> temp;

            MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

            for(; it != it_end; ++it) temp.push_back(*it);

            Features.push_back(temp);
            temp.empty();
        }
    }
}

void Busca::treino()
{
    Size DIM(Features[1].size(),Features.size()); // (width, height)

    Mat trainData( DIM, CV_32FC1, Scalar::all(0) );
    Mat responses( Size(1, Features.size()), CV_32SC1, Scalar::all(0) );

    //  --------- Carrega os vetores em cv::Mat para a fase de Treino --------------

    for(std::vector<int>::size_type i = 0; i < Features.size(); i++)

        if ( i < IMAGENSPOSITIVAS )
            responses.at<int>(0,i) = 1;
        else
            responses.at<int>(0,i) = -1;


    for(int k = 0; k < trainData.rows; k++)
        for(int l = 0; l < trainData.cols; l++)
            trainData.at<float>(k,l) = Features[k][l];

    // -----------------------------------------------------------------------------

    Features.clear();

    if ( responses.depth() == CV_32SC1 && trainData.depth() == CV_32FC1 )
    {
        // As amostras estao armazenadas em linhas ...
        boost.train(trainData, CV_ROW_SAMPLE, responses); //, Mat(), Mat(), Mat(), Mat(), BoostParams(CvBoost::REAL, 100, 0.95, 5, false, 0));
        boost.save( arqCriterio.c_str() );
    }
    else
        cout << "Os dados para treino ou resposta estão em formatos errado !";

}

// Busca por padrão
void Busca::teste(Mat& Query)
{
    Mat ROI(Size(WIDTH,HEIGHT),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi;  // Armazena as coordenadas das Features
    int raio=1; int vizinhaca=8;
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    // convoluçao para gerar uma imagem de 320 x 240 px em NAO faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT ; i++) {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH ; j++) {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH,HEIGHT)).convertTo(ROI,CV_32FC1,1,0);


            padraoLocal(ROI,LBP,raio,vizinhaca);

            Mat temp;

            MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

            for(; it != it_end; ++it) temp.push_back(*it);

            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

            QString nome = QString("ROI00%1-%2.jpg").arg(i).arg(j);
            string result2 = nome.toUtf8().constData();


            if ( PREDICAO > 12 ) {
                df.predicao = PREDICAO; df.ponto = roi;
                faces.push_back( df );
                IMAGENSPOSITIVAS++;
            }
            else
            {
                IMAGENSNEGATIVAS++;
            }

        }

    }
}
