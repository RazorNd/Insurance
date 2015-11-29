#include "clientedit.h"
#include "ui_clientedit.h"
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QDebug>

ClientEdit::ClientEdit(QSqlRelationalTableModel *model, const QModelIndex &index, bool insert, QWidget *parent) :
    QDialog(parent),
    _model(model),
    ui(new Ui::ClientEdit)
{
    ui->setupUi(this);

    _mapper = new QDataWidgetMapper(this);
    _mapper->setModel(model);
    _mapper->addMapping(ui->clientFirstNameEdit, model->fieldIndex("firstName"));
    _mapper->addMapping(ui->clientLastNameEdit, model->fieldIndex("lastName"));
    _mapper->addMapping(ui->clientPassportEdit, model->fieldIndex("passport"));

    _mapper->setCurrentModelIndex(index);
    _mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    connect(this, SIGNAL(accepted()), _mapper, SLOT(submit()));

    bool isHaveCID = false;
    CID = model->index(index.row(),
                           model->fieldIndex("CID"))
              .data().toInt(&isHaveCID);
    if(!insert) {
        setWindowTitle("Просмотр/Редактирование клиента");
        _dealModel = new QSqlRelationalTableModel(this);
        _dealModel->setTable("InsuranceDeal");
        _dealModel->setRelation(1, QSqlRelation("InsuranceType", "InsTypeID", "name"));
        setFilters();

        ui->dealView->setModel(_dealModel);
        ui->dealView->hideColumn(_dealModel->fieldIndex("CID"));

    } else {
        setWindowTitle("Создание клиента");
        ui->dealHistory->hide();
    }
}

ClientEdit::~ClientEdit()
{
    delete ui;
}

void ClientEdit::setFilters(bool filterDateEnabled)
{
    if(filterDateEnabled) {
        _dealModel->setFilter(QString("(createdAt <= %1 or validTo >= %2) and CID %3")
                              .arg(ui->dealFilterAfterDate->date().toString(Qt::ISODate))
                              .arg(ui->dealFilterBeforeDate->date().toString(Qt::ISODate))
                              .arg(CID));
    } else {
        _dealModel->setFilter(QString("CID = %1").arg(CID));
    }
    _dealModel->select();
}

void ClientEdit::on_filterApply_clicked()
{
    setFilters(true);
}

void ClientEdit::on_filterClear_clicked()
{
    setFilters();
}
