#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientedit.h"
#include "insurancetypeedit.h"
#include <QSqlError>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _settingDialog(new SettingsDialog(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Insuarance");
    db.setUserName("root");
    db.setPassword("root");

    if(!db.open("root", "root")) {
        qDebug() << db.lastError().text();
    }
    ui->clientsView->setModel(_clients = createModel("Clients", {"id", "Имя", "Фамилия", "Паспорт"}));
    ui->insuranceTypeView->setModel(_insuranceType = createModel("InsuranceType", {"id", "вид страхования"}));
    ui->InsuranceDealView->setModel(_insuranceDeal = createModel("InsuranceDeal",
                                    {"id", "вид страхования", "имя клиента", "фамилия клиента", "параметры", "действует с", "дейчтвительна до"},
                                    {
                                        std::make_tuple(1, QString("InsuranceType"), QString("InsTypeID"), QString("name")),
                                        std::make_tuple(2, QString("Clients"), QString("CID"), QString("firstName, lastName"))
                                    }));



}

MainWindow::~MainWindow()
{
    delete ui;
}

QSqlRelationalTableModel *MainWindow::createModel(const QString &tableName, std::initializer_list<QString> headers,
                                                  std::initializer_list<std::tuple<int, QString, QString, QString> > relations)
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
    model->setTable(tableName);

    if(headers.size()) {
        auto headerIterator = headers.begin();
        for(int i = 0; headerIterator != headers.end(); headerIterator++, i++) {
            model->setHeaderData(i, Qt::Horizontal, *headerIterator);
        }
    }

    for(const std::tuple<int, QString, QString, QString> &relation : relations) {
        int column;
        QString table, index, display;
        std::tie(column, table, index, display) = relation;
        model->setRelation(column, QSqlRelation(table, index, display));
    }
    model->select();
    return model;
}

void MainWindow::on_settingsAction_triggered()
{
    int result = _settingDialog->exec();
    if(result == QDialog::Accepted) {
        //TODO: accept setting
    }
}

void MainWindow::on_clientsView_doubleClicked(const QModelIndex &index)
{
    ClientEdit edit(_clients, index, false, this);
    edit.exec();
}

void MainWindow::on_addClientAction_triggered()
{
    int newRow = _clients->rowCount();
    _clients->insertRow(newRow);
    ClientEdit edit(_clients, _clients->index(newRow, 0), true, this);
    if(edit.exec() != QDialog::Accepted) {
        _clients->revertAll();
    }
}

void MainWindow::on_insuranceTypeView_doubleClicked(const QModelIndex &index)
{
    InsuranceTypeEdit edit(_insuranceType, index, false, this);
    edit.exec();
}

void MainWindow::on_addInsuranceType_triggered()
{
    int newRow = _insuranceType->rowCount();
    _insuranceType->insertRow(newRow);
    InsuranceTypeEdit edit(_insuranceType, _insuranceDeal->index(newRow, 0), true, this);
    if(edit.exec() != QDialog::Accepted) {
        _insuranceType->revertAll();
    }
}
