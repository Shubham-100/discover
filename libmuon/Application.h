/***************************************************************************
 *   Copyright © 2010-2011 Jonathan Thomas <echidnaman@kubuntu.org>        *
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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QStringList>

#include <KUrl>
#include <KService>

#include <LibQApt/Package>

#include "libmuonprivate_export.h"
#include "resources/AbstractResource.h"

class KJob;
namespace QApt {
    class Backend;
}

class MUONPRIVATE_EXPORT Application : public AbstractResource
{
Q_OBJECT
// Q_PROPERTY(QString mimetypes READ mimetypes CONSTANT)
// Q_PROPERTY(QString menuPath READ menuPath CONSTANT)
// Q_PROPERTY(int usageCount READ usageCount CONSTANT)
public:
    friend class TransactionListener;

    explicit Application(const QString &fileName, QApt::Backend *backend);
    explicit Application(QApt::Package *package, QApt::Backend *backend);
    ~Application();

    QString name();
    QString untranslatedName();
    QString comment();
    QApt::Package *package();
    QString icon() const;
    QString mimetypes() const;
    QString menuPath();
    QString categories();
    QString license();
    QUrl screenshotUrl() { return screenshotUrl(QApt::Screenshot); }
    QUrl screenshotUrl(QApt::ScreenshotType type);
    QUrl thumbnailUrl();
    QApt::PackageList addons();
    virtual QList< PackageState > addonsInformation();
    bool isValid() const;
    bool isTechnical() const;
    int usageCount();
    QString packageName() const;

    QByteArray getField(const QByteArray &field) const;
    QHash<QByteArray, QByteArray> desktopContents();

    //QApt::Package forwarding
    QUrl homepage() const;
    QString longDescription() const;
    QString installedVersion() const;
    QString availableVersion() const;
    QString sizeDescription();
    QString origin() const;

    void clearPackage();
    QVector<KService::Ptr> executables() const;
    
    /** Used to trigger the stateChanged signal from the ApplicationBackend */
    void emitStateChanged();
    
    void invokeApplication() const;
    
    bool canExecute() const;
    QString section();
    int popularityContest() const;
    
    virtual State state();
    virtual void fetchScreenshots();

private:
    void populateZeitgeistInfo();
    QVector<QPair<QString, QString> > locateApplication(const QString &_relPath, const QString &menuId) const;

    QString m_fileName;
    QHash<QByteArray, QByteArray> m_data;
    QApt::Backend *m_backend;
    QApt::Package *m_package;
    QByteArray m_packageName;

    bool m_isValid;
    bool m_isTechnical;
    bool m_isExtrasApp;
    int m_usageCount;
};

#endif
