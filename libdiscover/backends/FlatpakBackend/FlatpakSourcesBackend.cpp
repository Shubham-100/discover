/***************************************************************************
 *   Copyright © 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>       *
 *   Copyright © 2017 Jan Grulich <jgrulich@redhat.com>                    *
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

#include "FlatpakSourcesBackend.h"

#include <QDebug>

#include <glib.h>

class FlatpakSourceItem : public QStandardItem
{
public:
    FlatpakSourceItem(const QString &text) : QStandardItem(text) { }
    void setFlatpakInstallation(FlatpakInstallation *installation) { m_installation = installation; }
    FlatpakInstallation *flatpakInstallation() const { return m_installation; }

private:
    FlatpakInstallation *m_installation;
};

FlatpakSourcesBackend::FlatpakSourcesBackend(FlatpakInstallation *systemInstallation, FlatpakInstallation *userInstallation, QObject* parent)
    : AbstractSourcesBackend(parent)
    , m_sources(new QStandardItemModel(this))
{
    QHash<int, QByteArray> roles = m_sources->roleNames();
    roles.insert(Qt::CheckStateRole, "checked");
    roles.insert(Qt::UserRole, "flatpakInstallation");
    m_sources->setItemRoleNames(roles);

    if (!listRepositories(systemInstallation)) {
        qWarning() << "Failed to list repositories from system installation";
    }

    if (!listRepositories(userInstallation)) {
        qWarning() << "Failed to list repositories from user installation";
    }
}

QAbstractItemModel* FlatpakSourcesBackend::sources()
{
    return m_sources;
}

bool FlatpakSourcesBackend::addSource(const QString &id)
{
    Q_UNUSED(id);
    return false;
}

bool FlatpakSourcesBackend::removeSource(const QString &id)
{
    QList<QStandardItem*> items = m_sources->findItems(id);
    if (items.count() == 1) {
        FlatpakSourceItem *sourceItem = static_cast<FlatpakSourceItem*>(items.first());
        g_autoptr(GCancellable) cancellable = g_cancellable_new();
        if (flatpak_installation_remove_remote(sourceItem->flatpakInstallation(), id.toStdString().c_str(), cancellable, nullptr)) {
            m_sources->removeRow(sourceItem->row());
            return true;
        } else {
            qWarning() << "Failed to remove " << id << " remote repository";
            return false;
        }
    } else {
        qWarning() << "couldn't find " << id  << items;
        return false;
    }

    return false;
}

QList<QAction*> FlatpakSourcesBackend::actions() const
{
    return {};
}

bool FlatpakSourcesBackend::listRepositories(FlatpakInstallation* installation)
{
    Q_ASSERT(installation);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GPtrArray) remotes = flatpak_installation_list_remotes(installation, cancellable, nullptr);

    if (!remotes) {
        return false;
    }

    for (uint i = 0; i < remotes->len; i++) {
        FlatpakRemote *remote = FLATPAK_REMOTE(g_ptr_array_index(remotes, i));

        const QString id = QString::fromUtf8(flatpak_remote_get_name(remote));
        const QString title = QString::fromUtf8(flatpak_remote_get_title(remote));

        FlatpakSourceItem *it = new FlatpakSourceItem(id);
        it->setCheckState(flatpak_remote_get_disabled(remote) ? Qt::Unchecked : Qt::Checked);
        it->setData(title.isEmpty() ? id : title, Qt::ToolTipRole);
        it->setData(name(), AbstractSourcesBackend::SectionRole);
        it->setFlatpakInstallation(installation);

        m_sources->appendRow(it);
    }

    return true;
}