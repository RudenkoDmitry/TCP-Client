#pragma once

#include "server.h"
#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QStandardItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUi();
    void clickDeleteTaskButton();
    void clickSelectTable();
    void clickSelectOverdueTable();
    void clickAddTaskButton();
    void loadDataToTable(const QList<Task>& data);

private:
    Ui::MainWindow *ui;
    Server *server;
    QStandardItemModel* modelTask;
};
