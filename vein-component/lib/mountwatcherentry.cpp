#include "mountwatcherentry.h"

using namespace vfFiles;

extern void appendErrorMsg(QString& errorMsg, const QString appendError);


MountWatcherEntry::MountWatcherEntry(QObject *parent) : QObject(parent)
{
}

bool MountWatcherEntry::create(VfCpp::VfCppEntity *entity,
                               const QString componentName,
                               const QString procFileMount,
                               const QString mountBasePath)
{
    connect(&m_mountWatcher, &MountWatcherEntryBase::sigMountsChanged,
            this, &MountWatcherEntry::onMountsChanged);
    bool ok = m_mountWatcher.create(procFileMount, mountBasePath);
    if(ok)
        m_veinComponent = entity->createComponent(componentName, QStringList(), true);
    else
        disconnect(&m_mountWatcher, 0, this, 0);
    return ok;
}

void MountWatcherEntry::onMountsChanged(const QStringList mounts)
{
    m_veinComponent.setValue(mounts);
}
