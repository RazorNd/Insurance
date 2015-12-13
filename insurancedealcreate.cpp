#include "insurancedealcreate.h"
#include "ui_insurancedealedit.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

InsuranceDealCreate::InsuranceDealCreate(QSqlRelationalTableModel *model, QWidget *parent) :
    QDialog(parent),    
    _model(model),            
    ui(new Ui::InsuranceDealEdit)
{        
    clients = new QSqlQueryModel(this);
    clients->setQuery(QString("SELECT CID, CONCAT_WS(' ', firstName, lastName) as name FROM Clients"));
    init();
}

InsuranceDealCreate::InsuranceDealCreate(QSqlRelationalTableModel *model, int CID, QWidget *parent) :
    QDialog(parent),
    _model(model),
    ui(new Ui::InsuranceDealEdit)
{
    clients = new QSqlQueryModel(this);
    clients->setQuery(QString("SELECT CID, CONCAT_WS(' ', firstName, lastName) as name FROM Clients WHERE CID = %1").arg(CID));
    init();
}

InsuranceDealCreate::~InsuranceDealCreate()
{
    delete ui;
}

void InsuranceDealCreate::calculateCost()
{
    int countDay = ui->createdAtEdit->date().daysTo(ui->validToEdit->date());
    if(countDay < 0) {
        return ui->validToEdit->setDate(ui->createdAtEdit->date());
    }

    double priceForDay = insType->index(ui->insTypeEdit->currentIndex(),
                                        insType->fieldIndex("priceForDay")).data().toDouble();


    double cost = priceForDay * countDay;
    qDebug() << cost;
    ui->costEdit->setText(QString::number(cost, 'f', 2));
}

void InsuranceDealCreate::init()
{
    ui->setupUi(this);
    insType = _model->relationModel(1);

    ui->createdAtEdit->setDate(QDate::currentDate());
    ui->validToEdit->setDate(QDate::currentDate().addMonths(6));

    ui->clientEdit->setModel(clients);    
    ui->clientEdit->setModelColumn(1);

    ui->insTypeEdit->setModel(insType);    
    ui->insTypeEdit->setModelColumn(1);

    connect(this, SIGNAL(rejected()), _model, SLOT(revert()));
}

void InsuranceDealCreate::on_buttonBox_accepted()
{
    QSqlQuery query;
    query.prepare("INSERT INTO InsuranceDeal (InsTypeID, CID, createdAt, validTo, cost)\n"
                     " VALUE (:insTypeID, :cid, :createdAt, :validTo, :cost)");
    query.bindValue(":insTypeID", insType->record(ui->insTypeEdit->currentIndex())
                    .value("insTypeID"));
    query.bindValue(":cid", clients->record(ui->clientEdit->currentIndex())
                    .value("CID"));
    query.bindValue(":createdAt", ui->createdAtEdit->date());
    query.bindValue(":validTo", ui->validToEdit->date());
    query.bindValue(":cost", ui->costEdit->text().toDouble());


    if(!query.exec()) {
        QSqlError error = query.lastError();

        qDebug() << query.lastQuery();

        QMessageBox::warning(this, "Ошибка при добавлении сделки",
                             QString("Внимание, сделка не была сохранена из за возникновения ошибки: %1").arg(error.text()));

        return;
    }
    _model->select();
    accept();
}
