/***************************************************************************
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

#ifndef KNSRESOURCE_H
#define KNSRESOURCE_H

#include <resources/AbstractResource.h>
#include <kns3/entry.h>
#include <attica/content.h>

#include "discovercommon_export.h"

class KNSBackend;
class DISCOVERCOMMON_EXPORT KNSResource : public AbstractResource
{
Q_OBJECT
public:
    explicit KNSResource(const Attica::Content& c, QString  category, QString  icon, KNSBackend* parent);
    virtual ~KNSResource();

    void setStatus(KNS3::Entry::Status status);

    virtual AbstractResource::State state() override;
    virtual QString icon() const override;
    virtual QString comment() override;
    virtual QString name() override;
    virtual QString packageName() const override;
    virtual QStringList categories() override;
    virtual QUrl homepage() override;
    virtual QUrl thumbnailUrl() override;
    virtual QUrl screenshotUrl() override;
    virtual QString license() override;
    virtual QString longDescription() override;
    virtual QList<PackageState> addonsInformation() override { return QList<PackageState>(); }
    virtual QString availableVersion() const override;
    virtual QString installedVersion() const override;
    virtual QString origin() const override;
    virtual QString section() override;
    virtual void fetchScreenshots() override;
    virtual int size() override;
    virtual void fetchChangelog() override;

    const Attica::Content& content();
    void setEntry(const KNS3::Entry& entry);
    KNS3::Entry* entry() const;

private:
    KNS3::Entry::Status m_status;
    const Attica::Content m_content;
    const QString m_category;
    const QString m_icon;
    KNS3::Entry* m_entry;
};

#endif // KNSRESOURCE_H