#ifndef INSURANCEDEALEDIT_H
#define INSURANCEDEALEDIT_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include "models.h"

namespace Ui {
class InsuranceDealEdit;
}

class InsuranceDealCreate : public QDialog
{
    Q_OBJECT    
    QSqlRelationalTableModel *_model;    
    QSqlQueryModel *clients;
    QSqlTableModel *insType;
public:
    explicit InsuranceDealCreate(QSqlRelationalTableModel *model, QWidget *parent = 0);
    explicit InsuranceDealCreate(QSqlRelationalTableModel *model, int CID, QWidget *parent = 0);
    ~InsuranceDealCreate();

private slots:
    void calculateCost();

    void on_buttonBox_accepted();

private:
    void init();
    Ui::InsuranceDealEdit *ui;
};

#endif // INSURANCEDEALEDIT_H
