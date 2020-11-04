#include "dirwatcherentry.h"

using namespace vfFiles;

DirWatcherEntry::DirWatcherEntry(QObject *parent) : QObject(parent)
{
}

bool DirWatcherEntry::create(VfCpp::veinmoduleentity *entity,
                             const QString componentName,
                             const QString path,
                             const QStringList nameFilters,
                             QDir::Filters filters,
                             bool fullPathForResults)
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
            m_fullPathForResults = fullPathForResults;
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
    QStringList newList(fileList.entryList());
    if(m_fullPathForResults) {
        for(auto& subDir : newList) {
            if(!subDir.startsWith(path)) {
                QDir tmpPath(path);
                if(tmpPath.cd(subDir)) {
                    subDir = tmpPath.path();
                }
            }
        }
    }
    if(m_veinComponent.value() != newList) {
        m_veinComponent.setValue(newList);
    }
}
