#ifndef CLIENTEDIT_H
#define CLIENTEDIT_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>

namespace Ui {
class ClientEdit;
}

class ClientEdit : public QDialog
{
    Q_OBJECT
    QDataWidgetMapper *_mapper;
    QSqlRelationalTableModel *_model;
    QSqlRelationalTableModel *_dealModel;
    int CID;
public:
    explicit ClientEdit(QSqlRelationalTableModel *model, const QModelIndex &index, bool insert = false, QWidget *parent = 0);
    ~ClientEdit();

    void setFilters(bool filterDateEnabled = false);
private slots:
    void on_filterApply_clicked();

    void on_filterClear_clicked();

    void on_buttonBox_accepted();

    void on_createNewDealButton_clicked();

private:
    Ui::ClientEdit *ui;
};

#endif // CLIENTEDIT_H
