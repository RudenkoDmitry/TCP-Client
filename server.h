#ifndef SERVER_H
#define SERVER_H

#include "types.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QObject>
#include <task.h>

class Server : public QObject
{
    Q_OBJECT

public:
    Server(const QString& ip, int port, QObject* parent = nullptr);

    void removeTask(Task task);
    void getTask();
    void getOverdueTask();
    void insertTask(Task task);

signals:
    void onRequesComplite(StatusCode statusCode, const QString& message);
    void onLoadTask(const QList<Task>& data);
    void onLoadOverdueTask(const QList<Task>& data);

private:
    void socketDisconnect();
    void socketConnect();
    void showMessage(const QString& sucsessful, const QString& error, StatusCode statusCode);
    QList<Task> fillData(const QJsonArray& arrayData);
private:
    QTcpSocket *socket;
};

#endif // SERVER_H
