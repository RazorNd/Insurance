#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientedit.h"
#include "insurancetypeedit.h"
#include "insurancedealcreate.h"
#include "insurancedealview.h"
#include "models.h"
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
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

    ui->clientsView->setModel(createClientFilter(_clients = createClienModel(this)));
    ui->insuranceTypeView->setModel(_insuranceType = createTypeModel(this));
    ui->InsuranceDealView->setModel(_insuranceDeal = createDealModel(this));
    ui->InsuranceDealView->hideColumn(_insuranceDeal->fieldIndex("param"));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    InsuranceTypeEdit edit(_insuranceType, _insuranceType->index(newRow, 0), true, this);
    if(edit.exec() != QDialog::Accepted) {
        _insuranceType->revertAll();
    }
}

void MainWindow::on_addInsuranceDeal_triggered()
{
    InsuranceDealCreate edit(_insuranceDeal, this);
    edit.exec();
}

void MainWindow::on_InsuranceDealView_doubleClicked(const QModelIndex &index)
{
    InsuranceDealView view(_insuranceDeal, index, this);
    view.exec();
}

QSortFilterProxyModel *MainWindow::createClientFilter(QSqlRelationalTableModel *clients)
{
    MultipleFilterProxyModel *clientFilterModel = new MultipleFilterProxyModel(this);
    clientFilterModel->setSourceModel(clients);

    connect(ui->clientFirstNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("firstName")));
    connect(ui->clientLastNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("lastName")));
    connect(ui->clientPassportFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("passport")));

    return clientFilterModel;
}

std::function<void (QString)> MainWindow::filterSlotsFactory(MultipleFilterProxyModel *model, int columnNumber) const
{
    return [columnNumber, model] (QString filterPattern) -> void {
        model->setFilterWildcard(columnNumber, filterPattern);
    };
}
