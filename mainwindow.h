#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"
#include <QMainWindow>
#include <QSqlRelationalTableModel>

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

private:
    QSqlRelationalTableModel *createModel(const QString &tableName, std::initializer_list<QString> headers = {},
                                          std::initializer_list<std::tuple<int, QString, QString, QString> > relations = {});


private slots:
    void on_settingsAction_triggered();

    void on_clientsView_doubleClicked(const QModelIndex &index);

    void on_addClientAction_triggered();

    void on_insuranceTypeView_doubleClicked(const QModelIndex &index);

    void on_addInsuranceType_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
