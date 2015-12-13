#include "insurancedealview.h"
#include "ui_insurancedealview.h"
#include <QDataWidgetMapper>

InsuranceDealView::InsuranceDealView(QSqlRelationalTableModel *model, const QModelIndex &index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsuranceDealView)
{
    ui->setupUi(this);

    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);

    mapper->addMapping(ui->firstNameView, model->fieldIndex("firstName"));
    mapper->addMapping(ui->lastNameView, model->fieldIndex("lastName"));
    mapper->addMapping(ui->createdAtView, model->fieldIndex("createdAt"));
    mapper->addMapping(ui->validToView, model->fieldIndex("validTo"));
    mapper->addMapping(ui->costView, model->fieldIndex("cost"));

    mapper->setCurrentModelIndex(index);
}

InsuranceDealView::~InsuranceDealView()
{
    delete ui;
}
