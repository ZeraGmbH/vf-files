#ifndef MOUNTWATCHERENTRYBASE_H
#define MOUNTWATCHERENTRYBASE_H

#include <QObject>

namespace vfFiles {

class MountWatcherEntryBase : public QObject
{
    Q_OBJECT
public:
    explicit MountWatcherEntryBase(QObject *parent = nullptr);
    bool create(const QString procFileMount,
                const QString mountBasePath);
signals:
    void sigMountsChanged(const QStringList mounts);
};

}

#endif // MOUNTWATCHERENTRYBASE_H
