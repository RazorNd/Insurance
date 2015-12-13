#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"
#include "multiplefilterproxymodel.h"
#include <functional>
#include <QFunctionPointer>
#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    SettingsDialog *_settingDialog;
    QSqlRelationalTableModel *_clients;
    QSqlRelationalTableModel *_insuranceType;
    QSqlRelationalTableModel *_insuranceDeal;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_settingsAction_triggered();

    void on_clientsView_doubleClicked(const QModelIndex &index);

    void on_addClientAction_triggered();

    void on_insuranceTypeView_doubleClicked(const QModelIndex &index);

    void on_addInsuranceType_triggered();

    void on_addInsuranceDeal_triggered();

    void on_InsuranceDealView_doubleClicked(const QModelIndex &index);

private:

    QSortFilterProxyModel *createClientFilter(QSqlRelationalTableModel *clients);
    QSortFilterProxyModel *createInsuranceTypeFilter(QSqlRelationalTableModel *insType);
    std::function<void (QString)> filterSlotsFactory(MultipleFilterProxyModel *model, int columnNumber) const;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
