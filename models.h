#ifndef MODELS
#define MODELS

#include "modelfactory.h"
#include <functional>
#include <tuple>

QSqlRelationalTableModel *createClienModel(QObject *parent);
QSqlRelationalTableModel *createTypeModel(QObject *parent);
QSqlRelationalTableModel *createDealModel(QObject *parent);


#endif // MODELS

