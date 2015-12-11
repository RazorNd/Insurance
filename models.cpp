#include "models.h"

QSqlRelationalTableModel *createClienModel(QObject *paretnt)
{
    return ModelFactory::createModel(paretnt, "Clients", {"id", "Имя", "Фамилия", "Паспорт"}, {});
}

QSqlRelationalTableModel *createTypeModel(QObject *parent)
{
    return ModelFactory::createModel(parent, "InsuranceType", {"id", "вид страхования"}, {});
}

QSqlRelationalTableModel *createDealModel(QObject *parent)
{
    return ModelFactory::createModel(parent, "InsuranceDeal",
                                     {
                                        "id", "вид страхования", "имя клиента", "фамилия клиента",
                                        "действует с", "дейчтвительна до", "Стоимость"
                                     },
                                     {
                                         std::make_tuple(1, QString("InsuranceType"), QString("InsTypeID"), QString("name")),
                                         std::make_tuple(2, QString("Clients"), QString("CID"), QString("firstName, lastName"))
                                     });
}
