#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      server(new Server("127.0.0.1",8080)),
      modelTask(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setupUi();

    server->getTask();

    connect(ui->createTaskButton_2, &QPushButton::clicked, this, &MainWindow::clickAddTaskButton);
    connect(ui->deleteTaskButton_2, &QPushButton::clicked, this, &MainWindow::clickDeleteTaskButton);

    connect(ui->getTaskTable, &QPushButton::clicked, this, [this]()
    {
        modelTask->clear();
        server->getTask();
    });

    connect(ui->getLastTaskButton, &QPushButton::clicked, this, [this]()
    {
        modelTask->clear();
        server->getOverdueTask();
    });

    connect(ui->createTaskButton, &QPushButton::clicked, this, [this]()
    {
        ui->tabWidget->setCurrentIndex(2);
    });

    connect(ui-> returnToTableButton, &QPushButton::clicked, this, [this]()
    {
        ui->tabWidget->setCurrentIndex(1);
    });

    connect(ui-> returnToTableButtonD, &QPushButton::clicked, this, [this]()
    {
        ui->tabWidget->setCurrentIndex(1);
    });


    connect(ui->deleteTaskButton, &QPushButton::clicked, this, [this]()
    {
       ui->tabWidget->setCurrentIndex(3);
    });

    connect(server, &Server::onRequesComplite, this, [this](StatusCode statusCode, const QString& message)
    {
        if(statusCode ==ERROR)
            QMessageBox::information(this,"Ошибка",message);
        else
            QMessageBox::information(this,"Уведомление",message);
    });

    connect(server, &Server::onLoadTask, this, &MainWindow::loadDataToTable);


    connect(server, &Server::onLoadOverdueTask, this, &MainWindow::loadDataToTable);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    ui->tabWidget->tabBar()->hide();
    ui->tableViewEmployeeTask->setModel(modelTask);
    QStringList headers = {"Name", "Surname", "Task","Last date","Status"};
    for(int i = 0; i < headers.size(); i++){
        modelTask->setHeaderData(i, Qt::Horizontal, headers[i].toUtf8());
    }
}

void MainWindow::clickDeleteTaskButton()
{
    if(ui->nameEditD->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Имя сотрудника не заполнено");
    else if(ui->surnameEditD->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Фамилия сотрудника не заполнена");
    else if(ui->pantronymicEditD->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Отчество сотрудника не заполнено");
    else if(ui->taskNameEditD->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Наименование задачи не заполнено");
    else
    {

        server->insertTask(Task(ui->nameEditD->displayText(),
                                ui->surnameEditD->displayText(),
                                ui->pantronymicEditD->displayText(),
                                ui->taskNameEditD->displayText()));
    }
}

void MainWindow::clickAddTaskButton()
{
    if(ui->nameEdit->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Имя сотрудника не заполнено");
    else if(ui->surnameEdit->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Фамилия сотрудника не заполнена");
    else if(ui->pantronymicEdit->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Отчество сотрудника не заполнено");
    else if(ui->TaskEdit->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Наименование задачи не заполнено");
    else if(ui->statusEdit->displayText().isEmpty())
        QMessageBox::information(this,"Предупреждение","Статус задачи не заполнен");
    else
    {
        server->insertTask(Task(ui->nameEdit->displayText(),
                                ui->surnameEdit->displayText(),
                                ui->pantronymicEdit->displayText(),
                                ui->TaskEdit->displayText(),
                                ui->Dependency->isChecked(),
                                ui->ParentTaskEdit->displayText(),
                                ui->dateEdit->date(),
                                ui->statusEdit->displayText()));
    }

}

void MainWindow::loadDataToTable(const QList<Task> &data)
{
    for(int i = 0; i < data.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(data[i].getName()));
        row.append(new QStandardItem(data[i].getSurname()));
        row.append(new QStandardItem(data[i].getTaskName()));

        row.append(new QStandardItem(data[i].getDate().toString()));
        row.append(new QStandardItem(data[i].getStatus()));
        modelTask->appendRow(row);
    }
}


