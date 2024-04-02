#ifndef MOUNTWATCHERENTRY_H
#define MOUNTWATCHERENTRY_H

#include "mountwatcherentrybase.h"
#include <vf-cpp-entity.h>
#include <vf-cpp-compproxy.h>

namespace vfFiles {

class MountWatcherEntry : public QObject
{
    Q_OBJECT
public:
    explicit MountWatcherEntry(QObject *parent = nullptr);
    bool create(VfCpp::VfCppEntity* entity,
                const QString componentName,
                const QString procFileMount,
                const QString mountBasePath);

public slots:
    void onMountsChanged(const QStringList mounts);

private:
    MountWatcherEntryBase m_mountWatcher;
    VfCpp::VeinCompProxy<QStringList> m_veinComponent;
};

}// namespace

#endif // MOUNTWATCHERENTRY_H
