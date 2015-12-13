#ifndef MULTIPLEFILTERPROXYMODEL_H
#define MULTIPLEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>

class MultipleFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    enum PatternType {fixed, wildcard, regexp};
    QMap<int, QPair<PatternType, QString>> _patterns;

public:
    MultipleFilterProxyModel(QObject *parent);

    void setFilterWildcard(int column, const QString &pattern);
    void setFilterFixedString(int column, const QString &pattern);
    void setFilterRegExp(int column, const QString &pattern);

protected:
    void setFilterData(int column, PatternType type, const QString &patter);
    virtual bool fillterAcceptsCeil(const QModelIndex &index,
                                    const QPair<PatternType, QString> &pattern) const;
    virtual bool checkFixedString(const QString &data, const QString &pattern) const;
    virtual bool checkWildcard(const QString &data, const QString &pattern) const;
    virtual bool checkRegExp(const QString &data, const QString &pattern) const;

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // MULTIPLEFILTERPROXYMODEL_H
