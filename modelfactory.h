#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QObject>
#include <QSqlRelationalTableModel>

class ModelFactory : public QObject
{
    Q_OBJECT
    QString _tableName;
    std::initializer_list<QString> _headers;
    std::initializer_list<std::tuple<int, QString, QString, QString>> _relations;
public:
    explicit ModelFactory(QObject *parent = 0);

    static QSqlRelationalTableModel *createModel(QObject *parentForModel, const QString &tableName,
                                                 std::initializer_list<QString> headers = {},
                                                 std::initializer_list<std::tuple<int, QString, QString, QString>> relations = {});
    QSqlRelationalTableModel *operator() (const QString tableModel = QString(),
                                          const std::initializer_list<QString> &headers = {},
                                          const std::initializer_list<std::tuple<int, QString, QString, QString>> &relations = {}) const;
    QString tableName() const;
    void setTableName(const QString &tableName);
    std::initializer_list<QString> headers() const;
    void setHeaders(const std::initializer_list<QString> &headers);
    std::initializer_list<std::tuple<int, QString, QString, QString> > relations() const;
    void setRelations(const std::initializer_list<std::tuple<int, QString, QString, QString> > &relations);
};

#endif // MODELFACTORY_H
