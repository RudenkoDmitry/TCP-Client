#include "server.h"

Server::Server(const QString& ip, int port, QObject* parent ): QObject(parent)
{

    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &Server::socketConnect);
    connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnect);

    socket->connectToHost(ip,port);
}

void Server::removeTask(Task task)
{
    if(!socket->isOpen())
        return;

    QJsonObject result;
    result.insert("type", REMOVE_TASK);
    result.insert("params",task.toJson());
    socket->write(QJsonDocument(result).toJson());
    socket->waitForBytesWritten(1000);
}

void Server::getTask()
{
    if(!socket->isOpen())
        return;

    QJsonObject result;
    result.insert("type", SELECT_TASK);
    socket->write(QJsonDocument(result).toJson());
    socket->waitForBytesWritten(1000);
}

void Server::getOverdueTask()
{
    if(!socket->isOpen())
        return;

    QJsonObject result;
    result.insert("type", SELECT_OVERDUE_TASK);
    socket->write(QJsonDocument(result).toJson());
    socket->waitForBytesWritten(1000);
}

void Server::insertTask(Task task)
{
    if(!socket->isOpen())
        return;

    QJsonObject result;
    result.insert("type", INSERT_TASK);
    result.insert("params",task.toJson());
    socket->write(QJsonDocument(result).toJson());
    socket->waitForBytesWritten(1000);
}

void Server::socketDisconnect()
{
    socket->deleteLater();
}

void Server::showMessage(const QString &sucsessful, const QString &error, StatusCode statusCode){
    switch(statusCode)
    {
    case StatusCode::ERROR:
    {
        emit onRequesComplite(statusCode, error);
        break;
    }
    case StatusCode::SUCCESSFULY:
    {
        emit onRequesComplite(statusCode, sucsessful);
        break;
    }
    }
}

QList<Task> Server::fillData(const QJsonArray &arrayData)
{
    QList<Task> data;

    for(int i = 0; i < arrayData.size(); i++)
    {
        data.push_back(Task::fromJson(arrayData.at(i).toObject()));
    }
    return data;
}

void Server::socketConnect()
{
    if(!socket->waitForConnected(1000))
        return;

    socket->waitForReadyRead(1000);
    QJsonParseError docError;
    QJsonDocument doc = QJsonDocument::fromJson(socket->readAll(), &docError);

    if(docError.errorString().toInt()==QJsonParseError::NoError)
    {

        switch (doc.object().value("type").toInt())
        {
        case SELECT_TASK:
        {
            emit onLoadTask(fillData(doc.object().value("result").toArray()));
            break;
        }

        case SELECT_OVERDUE_TASK:
        {
            emit onLoadOverdueTask(fillData(doc.object().value("result").toArray()));
            break;
        }

        case INSERT_TASK:
        {
            showMessage("Добавление задачи сотрудника успешно",
                        "Ошибка при добавлении сотрудника",
                        (StatusCode)doc.object().value("result").toInt()
                        );
            break;
        }
        case REMOVE_TASK:
        {
            showMessage("Удаление задачи сотрудника успешно",
                        "Ошибка при удалении задачи сотрудника",
                        (StatusCode)doc.object().value("result").toInt()
                        );
            break;
        }
        default:
        {
            showMessage("", "Соединение не установлено", ERROR);
        }
        }
    } else
        showMessage("", "Ошибка с форматом при передаче данных"+docError.errorString(), ERROR);
}
