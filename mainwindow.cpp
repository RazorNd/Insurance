#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientedit.h"
#include "insurancetypeedit.h"
#include "insurancedealcreate.h"
#include "insurancedealview.h"
#include "models.h"
#include "customcomparatorproxyfiltermodel.h"
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _settingDialog(new SettingsDialog(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Insuarance");
    db.setUserName("root");
    db.setPassword("root");

    if(!db.open("root", "root")) {
        qDebug() << db.lastError().text();
    }

    ui->clientsView->setModel(createClientFilter(_clients = createClienModel(this)));
    ui->insuranceTypeView->setModel(createInsuranceTypeFilter(_insuranceType = createTypeModel(this)));
    ui->InsuranceDealView->setModel(createInsuranceDealFilter(_insuranceDeal = createDealModel(this)));
    ui->InsuranceDealView->hideColumn(_insuranceDeal->fieldIndex("param"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settingsAction_triggered()
{
    int result = _settingDialog->exec();
    if(result == QDialog::Accepted) {
        //TODO: accept setting
    }
}

void MainWindow::on_clientsView_doubleClicked(const QModelIndex &index)
{
    ClientEdit edit(_clients, index, false, this);
    edit.exec();
}

void MainWindow::on_addClientAction_triggered()
{
    int newRow = _clients->rowCount();
    _clients->insertRow(newRow);
    ClientEdit edit(_clients, _clients->index(newRow, 0), true, this);
    if(edit.exec() != QDialog::Accepted) {
        _clients->revertAll();
    }
}

void MainWindow::on_insuranceTypeView_doubleClicked(const QModelIndex &index)
{
    InsuranceTypeEdit edit(_insuranceType, index, false, this);
    edit.exec();
}

void MainWindow::on_addInsuranceType_triggered()
{
    int newRow = _insuranceType->rowCount();
    _insuranceType->insertRow(newRow);    
    InsuranceTypeEdit edit(_insuranceType, _insuranceType->index(newRow, 0), true, this);
    if(edit.exec() != QDialog::Accepted) {
        _insuranceType->revertAll();
    }
}

void MainWindow::on_addInsuranceDeal_triggered()
{
    InsuranceDealCreate edit(_insuranceDeal, this);
    edit.exec();
}

void MainWindow::on_InsuranceDealView_doubleClicked(const QModelIndex &index)
{
    InsuranceDealView view(_insuranceDeal, index, this);
    view.exec();
}

QSortFilterProxyModel *MainWindow::createClientFilter(QSqlRelationalTableModel *clients)
{
    MultipleFilterProxyModel *clientFilterModel = new MultipleFilterProxyModel(this);
    clientFilterModel->setSourceModel(clients);

    connect(ui->clientFirstNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("firstName")));
    connect(ui->clientLastNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("lastName")));
    connect(ui->clientPassportFilter, &QLineEdit::textChanged,
            filterSlotsFactory(clientFilterModel, clients->fieldIndex("passport")));

    createMenuAction(ui->clientFilterGroupBox, "Скрыть", QList<std::function <void()>>()
                     << std::bind(&QGroupBox::hide, ui->clientFilterGroupBox)
                     << [this](){
        ui->clientFirstNameFilter->clear();
        ui->clientLastNameFilter->clear();
        ui->clientPassportFilter->clear();
    });
    setActionTriggeredSlots(ui->findClientAction, QList<std::function <void()>>()
                            << std::bind(&QGroupBox::show, ui->clientFilterGroupBox)
                            << changeTabIndexSlot(0));

    ui->clientFilterGroupBox->hide();


    return clientFilterModel;
}

QSortFilterProxyModel *MainWindow::createInsuranceTypeFilter(QSqlRelationalTableModel *insType)
{
    CustomComparatorProxyFilterModel *insTypeFilterModel = new CustomComparatorProxyFilterModel(this);
    insTypeFilterModel->setSourceModel(insType);

    connect(ui->insTypeNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(insTypeFilterModel, insType->fieldIndex("name")));    
    auto slotFilterChanged =
            [&, insType, insTypeFilterModel](){
        insTypeFilterModel->addCustomComparatorFilter(insType->fieldIndex("priceForDay"),
                                                      CustomComparatorProxyFilterModel::filterBetween(ui->priceForDayFIlterMore->value(),
                                                                                                      ui->priceForDayFIlterLess->value()));
    };

    connect(ui->priceForDayFIlterMore, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            slotFilterChanged);
    connect(ui->priceForDayFIlterLess, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            slotFilterChanged);
    connect(ui->priceForDayFIlterMore, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](double min) -> void {
        ui->priceForDayFIlterLess->setMinimum(min);
    });
    connect(ui->priceForDayFIlterLess, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](double max) -> void {
        ui->priceForDayFIlterMore->setMaximum(max);
    });

    auto clearFilterSlot = [this](){
        ui->insTypeNameFilter->clear();
        ui->priceForDayFIlterMore->setValue(0.0);
        ui->priceForDayFIlterLess->setValue(1000000.0);
    };

    connect(ui->insTypeClearFilterButton, &QPushButton::clicked, clearFilterSlot);

    createMenuAction(ui->insTypeFilterGroupBox, "Скрыть", QList<std::function <void()>>()
                     << std::bind(&QGroupBox::hide, ui->insTypeFilterGroupBox)
                     << clearFilterSlot);
    setActionTriggeredSlots(ui->findInsuranceType, QList<std::function <void()>>()
                            << std::bind(&QGroupBox::show, ui->insTypeFilterGroupBox)
                            << changeTabIndexSlot(1));

    ui->insTypeFilterGroupBox->hide();

    return insTypeFilterModel;
}

QSortFilterProxyModel *MainWindow::createInsuranceDealFilter(QSqlRelationalTableModel *insDeal)
{
    CustomComparatorProxyFilterModel *insDealFilterModel = new CustomComparatorProxyFilterModel(this);
    insDealFilterModel->setSourceModel(insDeal);

    ui->insDealTypeFilter->setModel(insDeal->relationModel(insDeal->fieldIndex("name")));
    ui->insDealTypeFilter->setModelColumn(1);

    connect(ui->insDealClientFirstNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(insDealFilterModel, insDeal->fieldIndex("firstName")));
    connect(ui->insDealClientLastNameFilter, &QLineEdit::textChanged,
            filterSlotsFactory(insDealFilterModel, insDeal->fieldIndex("lastName")));
    connect(ui->insDealTypeFilter, &QComboBox::currentTextChanged,
            filterSlotsFactory(insDealFilterModel, insDeal->fieldIndex("name")));

    connect(ui->insDealDateMore, &QDateEdit::dateChanged, [insDealFilterModel, insDeal](QDate date) {
        insDealFilterModel->addCustomComparatorFilter(insDeal->fieldIndex("validTo"),
                                                      CustomComparatorProxyFilterModel::filterLess(date));
    });

    connect(ui->insDealDateLess, &QDateEdit::dateChanged, [insDealFilterModel, insDeal](QDate date) {
        insDealFilterModel->addCustomComparatorFilter(insDeal->fieldIndex("createdAt"),
                                                      CustomComparatorProxyFilterModel::filterMore(date));
    });

    auto costFilterChanged = [this, insDeal, insDealFilterModel]() {
        insDealFilterModel->addCustomComparatorFilter(insDeal->fieldIndex("cost"),
                                                      CustomComparatorProxyFilterModel::filterBetween(ui->insDealCostMore->value(),
                                                                                                      ui->insDealCostLess->value()));
    };

    connect(ui->insDealCostMore, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            costFilterChanged);
    connect(ui->insDealCostLess, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            costFilterChanged);

    auto clearFiltersSlot = [this]() {
        ui->insDealDateMore->setDate(QDate::currentDate());
        ui->insDealDateLess->setDate(QDate::currentDate().addYears(1));

        ui->insDealCostMore->setValue(0.0);
        ui->insDealCostLess->setValue(1000000000.0);

        ui->insDealClientFirstNameFilter->clear();
        ui->insDealClientLastNameFilter->clear();
        ui->insDealTypeFilter->clearEditText();
    };

    connect(ui->insDealClearFilrteButton, &QPushButton::clicked, clearFiltersSlot);

    clearFiltersSlot();

    ui->insDealFilterGroupBox->hide();

    createMenuAction(ui->insDealFilterGroupBox, "Скрыть", QList<std::function <void()>>()
                     << std::bind(&QGroupBox::hide, ui->insDealFilterGroupBox)
                     << clearFiltersSlot);
    setActionTriggeredSlots(ui->findInsuranceDeal, QList<std::function <void()>>()
                            << std::bind(&QGroupBox::show, ui->insDealFilterGroupBox)
                            << changeTabIndexSlot(2));

    return insDealFilterModel;
}

std::function<void (QString)> MainWindow::filterSlotsFactory(MultipleFilterProxyModel *model, int columnNumber) const
{
    return [columnNumber, model] (QString filterPattern) -> void {
        model->setFilterWildcard(columnNumber, filterPattern);
    };
}

void MainWindow::createMenuAction(QWidget *widget, const QString &text, QList<std::function<void ()>> trigeredSlots)
{
    QAction *action = new QAction(text, widget);
    widget->addAction(action);
    setActionTriggeredSlots(action, trigeredSlots);
}

void MainWindow::setActionTriggeredSlots(QAction *action, QList<std::function<void ()> > trigeredSlots)
{
    for(const auto &slot : trigeredSlots) {
        connect(action, &QAction::triggered, slot);
    }
}

std::function<void ()> MainWindow::changeTabIndexSlot(int index)
{
    return [this, index]() {
        ui->tabWidget->setCurrentIndex(index);
    };
}
