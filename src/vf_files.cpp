#include "vf_files.h"
#include "dirwatcherentry.h"

using namespace vfFiles;

vf_files::vf_files(QObject *parent, int id) : QObject(parent),
    m_entity(new VfCpp::veinmoduleentity(id)),
    m_isInitalized(false)
{
}

bool vf_files::initOnce()
{
    // create our entity once
    if(!m_isInitalized) {
        m_isInitalized = true;
        m_entity->initModule();
        m_entity->createComponent("EntityName", "_Files", true);
    }
    return true;
}

bool vf_files::addDirToWatch(const QString dir,
                             const QString componentName,
                             const QStringList nameFilters,
                             QDir::Filters filters,
                             bool fullPathForResults)
{
    bool ok = false;
    if(m_isInitalized && !m_mapWatchDirs.contains(componentName)) { // prerequisites ok?
        // try to add another watcher component
        DirWatcherEntry* dirWatcherEntry = new DirWatcherEntry(this);
        if(dirWatcherEntry->create(m_entity, componentName, dir, nameFilters, filters, fullPathForResults)) {
            m_mapWatchDirs[componentName] = dirWatcherEntry;
            ok = true;
        }
        else {
            delete dirWatcherEntry;
        }
    }
    return ok;
}

VfCpp::veinmoduleentity *vf_files::getVeinEntity() const
{
    return m_entity;
}
