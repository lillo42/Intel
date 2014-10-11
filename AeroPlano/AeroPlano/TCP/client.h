#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>

class Client : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void run();

signals:

public slots:

private slots:
    void readRead();
    void disconnect();

private:
    QTcpSocket *socket;
    bool deleteThread;

    void criaSocket();
};

#endif // CLIENT_H
