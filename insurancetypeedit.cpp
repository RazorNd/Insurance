#include "insurancetypeedit.h"
#include "ui_insurancetypeedit.h"
#include <QDataWidgetMapper>

InsuranceTypeEdit::InsuranceTypeEdit(QSqlRelationalTableModel *model, const QModelIndex &index, bool insert, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsuranceTypeEdit)
{
    ui->setupUi(this);
    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    connect(this, SIGNAL(accepted()), mapper, SLOT(submit()));

    mapper->setModel(model);

    mapper->addMapping(ui->insTypeNameEdit, model->fieldIndex("name"));
    mapper->addMapping(ui->insTypePriceForDayEdit, model->fieldIndex("priceForDay"));

    mapper->setCurrentModelIndex(index);
    if(insert) {
        setWindowTitle("Создание вида страхования");
    } else {
        setWindowTitle("Правка вида страхования");
    }
}

InsuranceTypeEdit::~InsuranceTypeEdit()
{
    delete ui;
}
