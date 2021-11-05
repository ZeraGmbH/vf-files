#ifndef DIRWATCHERENTRY_H
#define DIRWATCHERENTRY_H

#include <vf-cpp-entity.h>
#include <vf-cpp-compproxy.h>
#include <QObject>
#include <QDir>
#include <QFileSystemWatcher>

namespace vfFiles {

class DirWatcherEntry : public QObject
{
    Q_OBJECT
public:
    explicit DirWatcherEntry(QObject *parent = nullptr);
    bool create(VfCpp::veinmoduleentity* entity,
                const QString componentName,
                const QString path,
                const QStringList nameFilters,
                QDir::Filters filters,
                bool fullPathForResults);

private:
    void onDirectoryChanged(const QString &path);

    VfCpp::VeinCompProxy<QStringList> m_veinComponent;
    QFileSystemWatcher m_fileWatcher;
    QDir::Filters m_filters;
    QStringList m_nameFilters;
    bool m_fullPathForResults  = false;
};

}// namespace

#endif // DIRWATCHERENTRY_H
