#include "modelfactory.h"

QString ModelFactory::tableName() const
{
    return _tableName;
}

void ModelFactory::setTableName(const QString &tableName)
{
    _tableName = tableName;
}

std::initializer_list<QString> ModelFactory::headers() const
{
    return _headers;
}

void ModelFactory::setHeaders(const std::initializer_list<QString> &headers)
{
    _headers = headers;
}

std::initializer_list<std::tuple<int, QString, QString, QString> > ModelFactory::relations() const
{
    return _relations;
}

void ModelFactory::setRelations(const std::initializer_list<std::tuple<int, QString, QString, QString> > &relations)
{
    _relations = relations;
}

ModelFactory::ModelFactory(QObject *parent) : QObject(parent)
{

}

QSqlRelationalTableModel *ModelFactory::createModel(QObject *parentForModel, const QString &tableName, std::initializer_list<QString> headers, std::initializer_list<std::tuple<int, QString, QString, QString> > relations)
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(parentForModel);
    model->setTable(tableName);

    for(const std::tuple<int, QString, QString, QString> &relation : relations) {
        int column;
        QString table, index, display;
        std::tie(column, table, index, display) = relation;
        model->setRelation(column, QSqlRelation(table, index, display));
    }

    if(headers.size()) {
        auto headerIterator = headers.begin();
        for(int i = 0; headerIterator != headers.end(); headerIterator++, i++) {
            model->setHeaderData(i, Qt::Horizontal, *headerIterator);
        }
    }
    model->select();
    return model;
}

QSqlRelationalTableModel *ModelFactory::operator()(const QString tableModel, const std::initializer_list<QString> &headers, const std::initializer_list<std::tuple<int, QString, QString, QString> > &relations) const
{
    return ModelFactory::createModel(parent(),
                                     this->tableName().size()   ? this->tableName() : tableModel,
                                     this->headers().size()     ? this->headers()   : headers,
                                     this->relations().size()   ? this->relations() : relations);
}

