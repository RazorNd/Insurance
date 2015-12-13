#ifndef CUSTOMCOMPARATORPROXYFILTERMODEL_H
#define CUSTOMCOMPARATORPROXYFILTERMODEL_H

#include "multiplefilterproxymodel.h"
#include <QObject>
#include <QMap>
#include <functional>

class CustomComparatorProxyFilterModel : public MultipleFilterProxyModel
{
    Q_OBJECT
public:
    typedef std::function<bool (const QVariant&)> CustomComparator;
private:
    QMap<int, CustomComparator> _customFilter;
public:    
    CustomComparatorProxyFilterModel(QObject *parent);

    virtual void addCustomComparatorFilter(int column, const CustomComparator &comp);
    virtual void removeCustomComparatorFilter(int column);

    template <typename Type>
    CustomComparator static filterBetween(const Type &a, const Type &b)
    {
        return [a, b] (const QVariant &data) -> bool {
            if(!data.canConvert<Type>()) {
                return false;
            }
            Type value = data.value<Type>();
            return a <= value && value <= b;
        };
    }

    template <typename Type>
    CustomComparator static filterMore(const Type &a)
    {
        return [a](const QVariant &data) -> bool {
            if(!data.canConvert<Type>()) {
                return false;
            }
            Type value = data.value<Type>();
            return a < value;
        };
    }

    template <typename Type>
    CustomComparator static filterLess(const Type &a)
    {
        return [a](const QVariant &data) -> bool {
            if(!data.canConvert<Type>()) {
                return false;
            }
            Type value = data.value<Type>();
            return a < value;
        };
    }

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // CUSTOMCOMPARATORPROXYFILTERMODEL_H
