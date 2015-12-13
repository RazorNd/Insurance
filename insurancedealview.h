#ifndef INSURANCEDEALVIEW_H
#define INSURANCEDEALVIEW_H

#include <QDialog>
#include <QSqlRelationalTableModel>

namespace Ui {
class InsuranceDealView;
}

class InsuranceDealView : public QDialog
{
    Q_OBJECT

public:
    explicit InsuranceDealView(QSqlRelationalTableModel *model, const QModelIndex &index, QWidget *parent = 0);
    ~InsuranceDealView();

private:
    Ui::InsuranceDealView *ui;
};

#endif // INSURANCEDEALVIEW_H
