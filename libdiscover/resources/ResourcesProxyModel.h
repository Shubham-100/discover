/***************************************************************************
 *   Copyright © 2010 Jonathan Thomas <echidnaman@kubuntu.org>             *
 *   Copyright © 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef RESOURCESPROXYMODEL_H
#define RESOURCESPROXYMODEL_H

#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QString>
#include <QStringList>

#include <Category/Category.h>

#include "discovercommon_export.h"
#include "AbstractResource.h"

namespace QApt {
    class Backend;
}

class Application;

class DISCOVERCOMMON_EXPORT ResourcesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* sourceModel READ sourceModel CONSTANT)
    Q_PROPERTY(bool shouldShowTechnical READ shouldShowTechnical WRITE setShouldShowTechnical NOTIFY showTechnicalChanged)
    Q_PROPERTY(Category* filteredCategory READ filteredCategory WRITE setFiltersFromCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString originFilter READ originFilter WRITE setOriginFilter)
    Q_PROPERTY(bool isShowingTechnical READ shouldShowTechnical WRITE setShouldShowTechnical NOTIFY showTechnicalChanged)
    Q_PROPERTY(bool isSortingByRelevancy READ sortingByRelevancy WRITE setSortByRelevancy)
    Q_PROPERTY(AbstractResource::State stateFilter READ stateFilter WRITE setStateFilter NOTIFY stateFilterChanged)
    Q_PROPERTY(QString mimeTypeFilter READ mimeTypeFilter WRITE setMimeTypeFilter)
    Q_PROPERTY(QString search READ lastSearch WRITE setSearch NOTIFY searchChanged)
    Q_PROPERTY(QString extends READ extends WRITE setExtends)
    Q_PROPERTY(QVariantList subcategories READ subcategories NOTIFY subcategoriesChanged)
public:
    explicit ResourcesProxyModel(QObject *parent=nullptr);

    QHash<int, QByteArray> roleNames() const override;

    void setSearch(const QString &text);
    QString lastSearch() const;
    void setOriginFilter(const QString &origin);
    QString originFilter() const;
    void setFiltersFromCategory(Category *category);
    void setShouldShowTechnical(bool show);
    bool shouldShowTechnical() const;
    void setSortByRelevancy(bool sort);
    bool sortingByRelevancy() const;
    void setStateFilter(AbstractResource::State s);
    AbstractResource::State stateFilter() const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    Category* filteredCategory() const;
    
    QString mimeTypeFilter() const;
    void setMimeTypeFilter(const QString& mime);

    QString extends() const;
    void setExtends(const QString &extends);

    QVariantList subcategories() const;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private Q_SLOTS:
    void refreshSearch();

private:
    void fetchSubcategories();
    void setSourceModel(QAbstractItemModel *sourceModel) override;

    QString m_lastSearch;
    QList<AbstractResource*> m_searchResults;

    typedef QPair<FilterType, QString> FilterPair;
    QHash<QByteArray, QVariant> m_roleFilters;

    bool m_sortByRelevancy;
    bool m_filterBySearch;
    Category* m_filteredCategory;
    AbstractResource::State m_stateFilter;
    QString m_filteredMimeType;
    QString m_extends;
    QVariantList m_subcategories;

Q_SIGNALS:
    void categoryChanged();
    void stateFilterChanged();
    void showTechnicalChanged();
    void searchChanged(const QString &search);
    void subcategoriesChanged(const QVariantList &subcategories);
};

#endif
