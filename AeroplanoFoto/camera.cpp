#include "camera.h"

Camera::Camera(QObject *parent) :
    QObject(parent)
{
    nameFile = QCoreApplication::applicationDirPath() + "/Test.jpg";
    txtFile = QCoreApplication::applicationDirPath() + "/Port.txt";
}

void Camera::run()
{
    VideoCapture cam;
    startCamera(cam);

    Mat frame;
    QImage image;
    TcpClient t;

    startConnection(t);

    forever
    {
        if(canGetImage())
        {
            cam.read(frame);
            convertMatToQImage(frame,image);
            t.send(image);
        }
        QThread::sleep(2);
    }
}

void Camera::convertMatToQImage(Mat frame, QImage &retorno)
{
    imwrite(nameFile.toStdString(),frame);
    retorno.load(nameFile);
}

bool Camera::canGetImage()
{
    return true;
}

void Camera::startConnection(TcpClient &tcp)
{
    QFile file(txtFile);
    while(!file.open(QIODevice::ReadOnly))
        qDebug() << "Can't open file Image.txt";
    QTextStream text(&file);

    QStringList ipPort = text.readAll().split(";");
    tcp.star(ipPort[0],ipPort[1].toInt());

}

void Camera::startCamera(VideoCapture &cam)
{
    while(!cam.isOpened())
    {
        cam.open(0);
        if(!cam.isOpened())
            qDebug() << "Can't open cam";
        else
            qDebug() << "Open cam";
    }
}
