#include "customcomparatorproxyfiltermodel.h"

CustomComparatorProxyFilterModel::CustomComparatorProxyFilterModel(QObject *parent):
    MultipleFilterProxyModel(parent)
{
}

void CustomComparatorProxyFilterModel::addCustomComparatorFilter(int column, const CustomComparatorProxyFilterModel::CustomComparator &comp)
{
    _customFilter.insert(column, comp);
    filterChanged();
}

void CustomComparatorProxyFilterModel::removeCustomComparatorFilter(int column)
{
    _customFilter.remove(column);
    filterChanged();
}

bool CustomComparatorProxyFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!MultipleFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
        return false;
    }
    for(auto it = _customFilter.begin(); it != _customFilter.end(); it++) {
        if(!it.value()(sourceModel()->index(source_row, it.key(), source_parent)
                      .data(filterRole()))) {
            return false;
        }
    }
    return true;
}
