#ifndef INSURANCETYPEEDIT_H
#define INSURANCETYPEEDIT_H

#include <QDialog>
#include <QSqlRelationalTableModel>


namespace Ui {
class InsuranceTypeEdit;
}

class InsuranceTypeEdit : public QDialog
{
    Q_OBJECT

public:
    explicit InsuranceTypeEdit(QSqlRelationalTableModel *model, const QModelIndex &index, bool insert, QWidget *parent = 0);
    ~InsuranceTypeEdit();

private:
    Ui::InsuranceTypeEdit *ui;
};

#endif // INSURANCETYPEEDIT_H
