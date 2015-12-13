#include "multiplefilterproxymodel.h"

MultipleFilterProxyModel::MultipleFilterProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
}

void MultipleFilterProxyModel::setFilterWildcard(int column, const QString &pattern)
{
    setFilterData(column, wildcard, pattern);
}

void MultipleFilterProxyModel::setFilterFixedString(int column, const QString &pattern)
{
    setFilterData(column, fixed, pattern);
}

void MultipleFilterProxyModel::setFilterRegExp(int column, const QString &pattern)
{
    setFilterData(column, regexp, pattern);
}

void MultipleFilterProxyModel::setFilterData(int column, MultipleFilterProxyModel::PatternType type, const QString &pattern)
{
    if(column < 0 || column > columnCount()) {
        return;
    }

    if(pattern.isEmpty()) {
        _patterns.remove(column);        
    } else {
        _patterns.insert(column, {type, pattern});
    }
    filterChanged();
}

bool MultipleFilterProxyModel::fillterAcceptsCeil(const QModelIndex &index,
                                                  const QPair<MultipleFilterProxyModel::PatternType, QString> &pattern) const
{
    const QString &data = index.data(filterRole()).toString();
    switch(pattern.first) {
    case fixed:
        return checkFixedString(data, pattern.second);
    case wildcard:
        return checkWildcard(data, pattern.second);
    case regexp:
        return checkRegExp(data, pattern.second);
    }
    return false;
}

bool MultipleFilterProxyModel::checkFixedString(const QString &data, const QString &pattern) const
{
    return !data.compare(pattern, filterCaseSensitivity());
}

bool MultipleFilterProxyModel::checkWildcard(const QString &data, const QString &pattern) const
{
    QRegExp rx(pattern, filterCaseSensitivity(), QRegExp::Wildcard);
    return !rx.indexIn(data);
}

bool MultipleFilterProxyModel::checkRegExp(const QString &data, const QString &pattern) const
{
    QRegExp rx(pattern, filterCaseSensitivity());
    return !rx.indexIn(data);
}

bool MultipleFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

    for(auto it = _patterns.begin(); it != _patterns.end(); it++) {
        int column = it.key();
        if(!fillterAcceptsCeil(sourceModel()->index(source_row, column, source_parent), it.value())) {
            return false;
        }
    }

    return true;
}
