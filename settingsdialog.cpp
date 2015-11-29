#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QMessageBox>
#include <QSqlError>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);    
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::checkConnectionDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "testConnection");
    db.setHostName(ui->dbHostEdit->text());
    db.setDatabaseName(ui->dbNameEdit->text());
    db.setUserName(ui->dbUsernameEdit->text());
    db.setPassword(ui->dbPasswordEdit->text());
    if(db.open() && db.isOpen()) {
        QMessageBox::information(this, "Упешное подключение", "Подключение к БД было успешно");
    } else {
        QMessageBox::warning(this, "Ошибка подключениея", QString("Ошибка подключения к БД: %1")
                             .arg(db.lastError().text()));
    }
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase("testConnection");
}
