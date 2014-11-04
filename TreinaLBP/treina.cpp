#include "treina.h"

Treina::Treina()
{
     QString nome = "criterio.xml";
     arqCriterio = nome.toStdString();

     dir = QString("../Imagens/Imagem");
     dirImagens.append(dir);
     dirPessoa = QString("../Imagens/Pessoa");
     dirNaoPessoa = QString("../Imagens/NPessoa");
     dirTreino = QString("../Imagens/Teste");

     IMAGENSNEGATIVAS = 0;
     IMAGENSPOSITIVAS = 0;
     AMOSTRA = 0;
     PREDICAO = 0;

     hogDefault.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
}

void Treina::carregaPessoa(QStringList &Lista)
{
    carregaList(dirPessoa,Lista);
}

void Treina::carregaNaoPessoa(QStringList &Lista)
{
    carregaList(dirNaoPessoa,Lista);
}

void Treina::carregaTeste(QStringList &Lista)
{
    carregaList(dirTreino,Lista);
}

void Treina::extraiCaracteristicasLBP(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi; // Armazena as coordenadas das Features

    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    // convoluçao da imagens 32 x 36
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i=i+ANDA_HEIGHT)
    {
        roi.y = i;

        for(int j = 0; j <= Query.cols - WIDTH_PADRAO ; j=j+ANDA_WIDTH)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,size);

            padraoLocal(ROI_TRUE,LBP,RAIO,VIZINHAZA);

            vector<float> temp;

            MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

            for(; it != it_end; ++it)
                temp.push_back(*it);

            Features.push_back(temp);
            temp.empty();
        }
    }
}

void Treina::extraiCaracteristicasWavalet(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Point roi; // Armazena as coordenadas das Features

    Size s(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(s,CV_32FC1);
    // convoluçao para converter uma imagem de 320 x 240 px em faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i=i+ANDA_HEIGHT)
    {
        roi.y = i;

        for(int j = 0; j <= Query.cols - WIDTH_PADRAO ; j=j+ANDA_WIDTH)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);


            resize(ROI,ROI_TRUE,s);
            vector<float> temp;

            cvHaarWavelet(ROI_TRUE,temp,HARD);

            Features.push_back(temp);
            temp.empty();
        }
    }
}

void Treina::extraiCaracteristicasWavaletLBP(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0)),LBP;
    Point roi; // Armazena as coordenadas das Features

    Size s(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(s,CV_32FC1);
    Mat mc = Mat::zeros(s,CV_32FC1), mdd = Mat::zeros(s,CV_32FC1), mdv = Mat::zeros(s,CV_32FC1),mdh = Mat::zeros(s,CV_32FC1);
    // convoluçao para converter uma imagem de 320 x 240 px em faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i=i+ANDA_HEIGHT)
    {
        roi.y = i;

        for(int j = 0; j <= Query.cols - WIDTH_PADRAO ; j=j+ANDA_WIDTH)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,s);

            cvHaarWavelet(ROI_TRUE,mc,mdd,mdv,mdh,HARD);

            extraiInformacaoLBP(mc,LBP);
            IMAGENSPOSITIVAS++;
            extraiInformacaoLBP(mdd,LBP);
            IMAGENSPOSITIVAS++;
            extraiInformacaoLBP(mdv,LBP);
            IMAGENSPOSITIVAS++;
            extraiInformacaoLBP(mdh,LBP);

        }
    }
}

void Treina::extraiCaracteristicasPixel(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Point roi; // Armazena as coordenadas das Features
    Size s(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(s,CV_32FC1);
    // convoluçao para converter uma imagem de 320 x 240 px em faces de 25 x 30 px
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i=i+ANDA_HEIGHT)
    {
        roi.y = i;

        for(int j = 0; j <= Query.cols - WIDTH_PADRAO ; j=j+ANDA_WIDTH)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,s);

            vector<float> temp;

            MatConstIterator_<float> it = ROI_TRUE.begin<float>(), it_end = ROI_TRUE.end<float>();

            for(; it != it_end; ++it)
                temp.push_back(*it);

            Features.push_back(temp);
            temp.empty();
        }
    }
}

void Treina::extraiCaracteristicasHOG(Mat &Query)
{

}

void Treina::extraiInformacaoLBP(Mat &src,Mat &LBP)
{
    vector<float> temp;

    padraoLocal(src,LBP,RAIO,VIZINHAZA);

    MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

    for(; it != it_end; ++it)
        temp.push_back(*it);

    Features.push_back(temp);
    temp.empty();
}

void Treina::padraoLocal(Mat &Origem, Mat &Destino, int Raio, int Vizinhaca)
{
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

void Treina::wavaletToVecto(Mat &wavalet, vector<float> &retorno, int NIter)
{
    vector<float> vc;
    vector<float> vdh;
    vector<float> vdv;
    vector<float> vdd;

    float c,dh,dv,dd;
    int width = wavalet.cols;
    int height = wavalet.rows;

    for (int k=NIter;k>0;k--)
    {
        for (int y=0;y<(height>>k);y++)
        {
            for (int x=0; x<(width>>k);x++)
            {
                c = wavalet.at<float>(y,x);
                dh = wavalet.at<float>(y,x+(width>>k));
                dv = wavalet.at<float>(y+(height>>k),x);
                dd = wavalet.at<float>(y+(height>>k),x+(width>>k));

                vc.push_back(c);
                vdh.push_back(dh);
                vdv.push_back(dv);
                vdd.push_back(dd);
            }
        }
    }
    retorno.reserve(vc.size() + vdh.size() + vdv.size() + vdd.size());
    retorno.insert(retorno.end(), vc.begin(),  vc.end());
    retorno.insert(retorno.end(), vdh.begin(), vdh.end());
    retorno.insert(retorno.end(), vdv.begin(), vdv.end());
    retorno.insert(retorno.end(), vdd.begin(), vdd.end());

}

const string Treina::currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "( %d-%m-%Y ) %X", &tstruct);

    return buf;
}

void Treina::treino()
{
    if(Features.size() < 1)
        return;
    Size DIM(Features[1].size(),Features.size()); // (width, height)

    Mat trainData( DIM, CV_32FC1, Scalar::all(0) );
    Mat responses( Size(1, Features.size()), CV_32SC1, Scalar::all(0) );

    //  --------- Carrega os vetores em cv::Mat para a fase de Treino --------------

    for(std::vector<int>::size_type i = 0; i < Features.size(); i++)
    {
        if ( i < IMAGENSPOSITIVAS )
            responses.at<int>(0,i) = 1;
        else
            responses.at<int>(0,i) = -1;
    }

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

void Treina::testeLPB(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi;  // Armazena as coordenadas das Features
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i++)
    {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH_PADRAO ; j++)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);
            resize(ROI,ROI_TRUE,size);
            padraoLocal(ROI_TRUE,LBP,RAIO,VIZINHAZA);

            Mat temp;

            MatConstIterator_<float> it = LBP.begin<float>(), it_end = LBP.end<float>();

            for(; it != it_end; ++it)
                temp.push_back(*it);


            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

            QString nome = QString("ROI00%1-%2.jpg").arg(i).arg(j);
            string result2 = nome.toUtf8().constData();


            if ( PREDICAO > 15)
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

void Treina::testeWavalet(Mat &Query)
{
    return;
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Point roi;  // Armazena as coordenadas das Features
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i++)
    {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH_PADRAO ; j++)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,size);

            vector<float> tempv;

            cvHaarWavelet(ROI_TRUE,tempv,HARD);

            Mat temp;

            for(int i = 0; i <= tempv.size();i++)
                temp.push_back(tempv.at(i));

            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

            QString nome = QString("ROI00%1-%2.jpg").arg(i).arg(j);
            string result2 = nome.toUtf8().constData();


            if ( PREDICAO > 5)
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

void Treina::testeWavaletLBP(Mat &Query)
{

    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Mat LBP;
    Point roi;  // Armazena as coordenadas das Features
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i++)
    {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH_PADRAO ; j++)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,size);

            Mat mc = Mat::zeros(size,CV_32FC1), mdd = Mat::zeros(size,CV_32FC1), mdv = Mat::zeros(size,CV_32FC1),mdh = Mat::zeros(size,CV_32FC1);
            cvHaarWavelet(ROI_TRUE,mc,mdd,mdv,mdh,HARD);

            padraoLocal(mc,LBP,RAIO,VIZINHAZA);
            extraiInformacaoTeste(LBP,df,roi);

            padraoLocal(mdd,LBP,RAIO,VIZINHAZA);
            extraiInformacaoTeste(LBP,df,roi);

            padraoLocal(mdv,LBP,RAIO,VIZINHAZA);
            extraiInformacaoTeste(LBP,df,roi);

            padraoLocal(mdh,LBP,RAIO,VIZINHAZA);
            extraiInformacaoTeste(LBP,df,roi);

        }
    }
}

void Treina::testePixel(Mat &Query)
{
    Mat ROI(Size(WIDTH_PADRAO,HEIGHT_PADRAO),CV_32FC1, Scalar::all(0));
    Point roi;  // Armazena as coordenadas das Features
    DetectFace df;
    faces.clear();
    IMAGENSPOSITIVAS=0;IMAGENSNEGATIVAS=0;
    Size size(WIDTH_ROI,HEIGHT_ROI);
    Mat ROI_TRUE = Mat::zeros(size,CV_32FC1);
    for(int i = 0; i <= Query.rows - HEIGHT_PADRAO ; i++)
    {
        roi.y = i;

        for(int j =0; j <= Query.cols - WIDTH_PADRAO ; j++)
        {
            roi.x = j;

            Query.operator ()(Rect(roi.x,roi.y,WIDTH_PADRAO,HEIGHT_PADRAO)).convertTo(ROI,CV_32FC1,1,0);

            resize(ROI,ROI_TRUE,size);

            Mat temp;

            MatConstIterator_<float> it = ROI_TRUE.begin<float>(), it_end = ROI_TRUE.end<float>();

            for(; it != it_end; ++it)
                temp.push_back(*it);


            PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

            QString nome = QString("ROI00%1-%2.jpg").arg(i).arg(j);
            string result2 = nome.toUtf8().constData();


            if ( PREDICAO > 5)
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

void Treina::testeHOG(Mat &Query)
{
    found.clear();
    found_filtered.clear();
    //hogDefault.detectMultiScale(Query,found,1);
    hogDefault.detectMultiScale(Query, found, 0, Size(8,8), Size(64,64), 1.05, 2);

    size_t i, j;
    for( i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }



    IMAGENSPOSITIVAS = IMAGENSPOSITIVAS + found_filtered.size();
}

void Treina::extraiInformacaoTeste(Mat &src,DetectFace &df,Point &roi)
{
    Mat temp;

    MatConstIterator_<float> it = src.begin<float>(), it_end = src.end<float>();

    for(; it != it_end; ++it) temp.push_back(*it);


    PREDICAO = boost.predict( temp, Mat(),Range::all(),false,true);

    if ( PREDICAO > 5)
    {
        df.predicao = PREDICAO; df.ponto = roi;
        faces.push_back( df );
        IMAGENSPOSITIVAS++;
    }
    else
        IMAGENSNEGATIVAS++;
}

void Treina::eliminaRepetidos()
{
    unsigned i = 0;

    while (i+1 < faces.size() )
    {
        if ((faces[i+1].ponto.x - faces[i].ponto.x <= WIDTH_ROI) &&
                (faces[i+1].ponto.y - faces[i].ponto.y <= HEIGHT_ROI))
        {

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

void Treina::desenhaRetangulo(Mat &imagem)
{
    Point center;
    int radius, scale= 0.264583333; // Converte para mm

    for(unsigned long i = 0; i < faces.size(); i++)
    {
        rectangle(imagem,(Rect(faces[i].ponto.x,
                                faces[i].ponto.y,WIDTH_PADRAO+1,HEIGHT_PADRAO+1)),
                  CV_RGB(255, 255, 0), 1.5);
        // comparar distancia entre os rec, e verficar a media.... para eliminar falsos positivos.

        center.x = faces[i].ponto.x+WIDTH_PADRAO/2;
        center.y = faces[i].ponto.y+HEIGHT_PADRAO/2;
        radius = cvRound((WIDTH_PADRAO + HEIGHT_PADRAO)*0.25*scale);
        circle( imagem, center, radius,  CV_RGB(0,255,0), 2);

    }
}

void Treina::desenhaHog(Mat &frame)
{
    size_t i;
    for( i = 0; i < found_filtered.size(); i++ )
    {
        Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}

void Treina::loadBoost()
{
    boost.load(arqCriterio.c_str());
}

void Treina::carregaList(QDir dir, QStringList &Lista)
{
    QDir diretorio(dir);
    QFileInfoList fileList = diretorio.entryInfoList();
    foreach (const QFileInfo &info, fileList)
        Lista.append(QString("%1/%2").arg(diretorio.path()).arg(info.fileName()));
}

float Treina::sgn(float x)
{
    float res=0;
    if(x == 0)
        res = 0;
    else if(x > 0)
        res = 1;
    else if(x < 0)
        res = -1;
    return res;
}

float Treina::soft_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res= sgn(d) * (fabs(d) - T);
    else
        res=0;
    return res;
}

float Treina::hard_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res = d;
    else
        res = 0;
    return res;

}

float Treina::garrot_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res= d - ( (T * T) / d);
    else
        res=0;
    return res;
}

void Treina::cvInvHaarWavelet(Mat &src, Mat &dst, int NIter, int SHRINKAGE_TYPE, float SHRINKAGE_T)
{
   float c,dh,dv,dd;
   int width = src.cols;
   int height = src.rows;
   //--------------------------------
   // NIter - number of iterations
   //--------------------------------
   for (int k=NIter;k>0;k--)
   {
       for (int y=0;y<(height>>k);y++)
       {
           for (int x=0; x<(width>>k);x++)
           {
               c=src.at<float>(y,x);
               dh=src.at<float>(y,x+(width>>k));
               dv=src.at<float>(y+(height>>k),x);
               dd=src.at<float>(y+(height>>k),x+(width>>k));

              // (shrinkage)
               switch(SHRINKAGE_TYPE)
               {
                   case HARD:
                       dh=hard_shrink(dh,SHRINKAGE_T);
                       dv=hard_shrink(dv,SHRINKAGE_T);
                       dd=hard_shrink(dd,SHRINKAGE_T);
                       break;
                   case SOFT:
                       dh=soft_shrink(dh,SHRINKAGE_T);
                       dv=soft_shrink(dv,SHRINKAGE_T);
                       dd=soft_shrink(dd,SHRINKAGE_T);
                       break;
//                   case GARROT:
//                       dh=Garrot_shrink(dh,SHRINKAGE_T);
//                       dv=Garrot_shrink(dv,SHRINKAGE_T);
//                       dd=Garrot_shrink(dd,SHRINKAGE_T);
//                       break;
               }

               //-------------------
               dst.at<float>(y*2,x*2)=0.5*(c+dh+dv+dd);
               dst.at<float>(y*2,x*2+1)=0.5*(c-dh+dv-dd);
               dst.at<float>(y*2+1,x*2)=0.5*(c+dh-dv-dd);
               dst.at<float>(y*2+1,x*2+1)=0.5*(c-dh-dv+dd);
           }
       }
       Mat C=src(Rect(0,0,width>>(k-1),height>>(k-1)));
       Mat D=dst(Rect(0,0,width>>(k-1),height>>(k-1)));
       D.copyTo(C);
   }
}

void Treina::cvHaarWavelet(Mat &src, Mat &dst, int NIter)
{
    float c,dh,dv,dd;
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y,x) = c;

                dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y, x + (width >> (k+1))) = dh;

                dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y+( height >> (k+1)),x) = dv;

                dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y+(height >> (k+1)),x+(width >> (k+1))) = dd;
            }
        }
        dst.copyTo(src);
    }
}

void Treina::cvHaarWavelet(Mat &src, vector<float> &dst, int NIter)
{
    float c,dh,dv,dd;
    vector<float> vc,vdh,vdv,vdd;
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                vc.push_back(c);

                dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                vdh.push_back(dh);

                dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                vdv.push_back(dv);

                dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                vdd.push_back(dd);
            }
        }
    }

    dst.reserve(vc.size() + vdh.size() + vdv.size() + vdd.size());
    dst.insert(dst.end(), vc.begin(),  vc.end());
    dst.insert(dst.end(), vdh.begin(), vdh.end());
    dst.insert(dst.end(), vdv.begin(), vdv.end());
    dst.insert(dst.end(), vdd.begin(), vdd.end());
}

void Treina::cvHaarWavelet(Mat &src, Mat &dst, int NIter, int escolha)
{
    float c,dh,dv,dd;
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                if(escolha == 0)
                {
                    c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                    dst.at<float>(y,x) = c;
                }
                else if(escolha == 1)
                {
                    dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                    dst.at<float>(y, x) = dh;
                }
                else if(escolha == 2)
                {
                    dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                    dst.at<float>(y,x) = dv;
                }
                else
                {
                    dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                    dst.at<float>(y,x) = dd;
                }
            }
        }
       // dst.copyTo(src);
    }
}

void Treina::cvHaarWavelet(Mat &src, Mat &mc, Mat &mdd, Mat &mdv, Mat &mdh, int NIter)
{
    float c,dh,dv,dd;
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                mc.at<float>(y,x) = c;

                dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                mdd.at<float>(y, x) = dh;

                dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                mdv.at<float>(y,x) = dv;

                dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                mdh.at<float>(y,x) = dd;
            }
        }
    }
}
