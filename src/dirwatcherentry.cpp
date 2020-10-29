#include "dirwatcherentry.h"

using namespace vfFiles;

DirWatcherEntry::DirWatcherEntry(QObject *parent) : QObject(parent)
{
}

bool DirWatcherEntry::create(VfCpp::veinmoduleentity *entity, const QString componentName, const QString path, const QStringList nameFilters, QDir::Filters filters)
{
    bool ok = false;
    // once only
    if(m_fileWatcher.directories().isEmpty()) {
        if(m_fileWatcher.addPath(path)) {
            // create vein component
            m_veinComponent = entity->createComponent(componentName, QStringList(), true);

            // keep filters & glue
            m_nameFilters = nameFilters;
            m_filters = filters;
            connect(&m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &DirWatcherEntry::onDirectoryChanged);

            // do initial populate
            onDirectoryChanged(path);

            ok = true;
        }
    }
    if(!ok) {
        qWarning("Cannot watch %s", qPrintable(path));
    }
    return ok;
}

void DirWatcherEntry::onDirectoryChanged(const QString &path)
{
    QDir fileList(path);
    fileList.setFilter(m_filters);
    fileList.setNameFilters(m_nameFilters);
    if(m_veinComponent.value() != fileList.entryList()) {
        m_veinComponent.setValue(fileList.entryList());
    }
}
